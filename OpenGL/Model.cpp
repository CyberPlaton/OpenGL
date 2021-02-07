#include "Model.h"


AssimpMesh::AssimpMesh(std::vector<Vertex3> vertices, std::vector<GLuint> indices, std::vector<Texture> textures) {

	m_Vertices = vertices;
	m_Indices = indices;
	m_Textures = textures;

	_setup();
}



void AssimpMesh::Draw(ShaderProgram& shader) {

	/*
	For shader:

	we assume that in shader we have uniforms like:

	uniform sampler2D texture_diffuse1;
	uniform sampler2D texture_diffuse2;
	uniform sampler2D texture_diffuse3;
	uniform sampler2D texture_specular1;
	uniform sampler2D texture_specular2;

	So if a mesh has more than one diffuse or specular texture ,
	we enumerate them accordingly.
	*/

	using namespace std;

	GLuint diffuseNr = 1;
	GLuint specularNr = 1;

	for (GLuint i = 0; i < m_Textures.size(); i++) {

		glActiveTexture(GL_TEXTURE0 + i); // Activate current texture.


		// Retrieve Textures number..
		stringstream ss;
		string num, name;

		name = m_Textures[i].Type;

		if (name == "texture_diffuse") {

			ss << diffuseNr++; // Save GLuint in stream
		}
		else if (name == "texture_specular") {

			ss << specularNr;
		}

		num = ss.str();



		// Set uniform for shader..
		// Should actually be done in texture class... TODO:

		/*
		We save material.texture_diffuse_n
		in Material struct of shader:

				struct Material{
					vec3 ambient;
					sampler2D diffuseMap;
					vec3 specular;
					float shininess;
				};

		thus we can have more options for each of the materials...

		Before we done it in the mainloop like:

				lightMesh.GetLightShader()->SetUniform("material.ambient", glm::vec3(0.1, 0.1, 0.1));
				lightMesh.GetLightShader()->SetUniformSampler("material.diffuseMap", 0); // Diffusemap to slot 0. Setting slot needed for setting multuple textures...
				lightMesh.GetLightShader()->SetUniform("material.specular", glm::vec3(1.0f, 1.0f, 1.0f));
				lightMesh.GetLightShader()->SetUniform("material.shininess", 128.0f);

		where "material.diffuseMap", 0
		is what we set for shader, means the texture was in slot 0 and had a predefined name...
		*/
		string material = "material." + name + num;
		//glUniform1f(glGetUniformLocation(shader.GetProgram(), material.c_str()), i); // Set texture to appropriate location in shader uniform 
		//shader.SetUniformSampler("material.diffuseMap", 0);
		
		// Set diffuse and specualar.
		shader.SetUniformSampler(material.c_str(), i);

		// Set other standard uniforms needed for shader.
		shader.SetUniform("material.ambient", glm::vec3(0.1, 0.1, 0.1));
		shader.SetUniform("material.specular", glm::vec3(1.0f, 1.0f, 1.0f)); // For specular we can set our texture?
		shader.SetUniform("material.shininess", 32.0f);


		glBindTexture(GL_TEXTURE_2D, m_Textures[i].ID); // Bind Texture for usage.
	}

	glActiveTexture(GL_TEXTURE0); // Deactivate..


	// Draw the mesh.
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, m_Indices.size(), GL_UNSIGNED_INT, 0); // Draw indexes.
	glBindVertexArray(0); // Unbind...
}


void AssimpMesh::_setup() {


	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);


	// Send verticess to vao and vbo.
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, m_Vertices.size() * sizeof(Vertex3), &m_Vertices[0], GL_STATIC_DRAW);


	// Send indices to ebo.
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_Indices.size() * sizeof(GLuint), &m_Indices[0], GL_STATIC_DRAW);


	// Vertex Positions
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex3),(GLvoid*)0);


	// Vertex Normals
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex3), (GLvoid*)offsetof(Vertex3, Normal));

	// Vertex Texture Coords
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex3), (GLvoid*)offsetof(Vertex3, TexCoords));



	glBindVertexArray(0); // Unbind.
}


void AssimpModel::Draw(ShaderProgram& shader) {

	for (GLuint i = 0; i < m_Meshes.size(); i++) {
		m_Meshes[i].Draw(shader);
	}
}

void AssimpModel::_loadModel(std::string path) {

	using namespace std;

	Assimp::Importer import;
	//const aiScene* scene = import.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);
	const aiScene* scene = import.ReadFile(path, aiProcess_Triangulate);


	if (!scene) {
		cout << color(colors::RED);
		cout << "Error loading \"" << path << "\". Error String: \n" << import.GetErrorString() << white << endl;
		return;
	}

	m_Directory = path.substr(0, path.find_last_of('/'));


	cout << color(colors::GREEN);
	cout << "Processing \"" << path << "\".\n";
	cout << "Dir: \"" << m_Directory << "\".\n" << white;


	_processNode(scene->mRootNode, scene);
}

void AssimpModel::_processNode(aiNode* node, const aiScene* scene) {

	using namespace std;

	// Recursively process all nodes meshes..
	for (GLuint i = 0; i < node->mNumMeshes; i++) {


		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];


		cout << color(colors::CYAN);
		cout << "Processing aiMesh \"" << mesh->mName.C_Str() << "\".\n" << white;

		m_Meshes.push_back(_processMesh(mesh, scene));
	}

	// Same for each nodes child..
	for (GLuint i = 0; i < node->mNumChildren; i++) {

		_processNode(node->mChildren[i], scene);
	}
}

AssimpMesh AssimpModel::_processMesh(aiMesh* mesh, const aiScene* scene) {

	using namespace std;

	vector<Vertex3> vertices;
	vector<GLuint> indices;
	vector<Texture> textures;



	for (GLuint i = 0; i < mesh->mNumVertices; i++) {

		Vertex3 vertex;

		// Process vertex pos, normals and tex coords.

		// vertex pos.
		glm::vec3 vector;
		aiVector3D aiVector = mesh->mVertices[i]; // Preparing for pragma.

		vector.x = aiVector.x;
		vector.y = aiVector.y;
		vector.z = aiVector.z;

		vertex.Position = vector;


		// Normals.
		aiVector = mesh->mNormals[i];
		vector.x = aiVector.x;
		vector.y = aiVector.y;
		vector.z = aiVector.z;

		vertex.Normal = vector;


		// Tex cooords, if any..
		if (mesh->mTextureCoords[0]) {

			glm::vec2 vec;
			aiVector3D aiVec; aiVec = mesh->mTextureCoords[0][i];

			vec.x = aiVec.x;
			vec.y = aiVec.y;

			vertex.TexCoords = vec;
		}
		else {
			vertex.TexCoords = glm::vec2(0.0f);
		}


		vertices.push_back(vertex);
	}



	// Process indices..
	for (GLuint i = 0; i < mesh->mNumFaces; i++) {

		aiFace face = mesh->mFaces[i]; // Get next face...
		for (GLuint j = 0; j < face.mNumIndices; j++) {

			indices.push_back(face.mIndices[j]); // Store each faces index. As we triangulated, we only have faces like  e.g. "1/2/3 1/2/1 4/1/2"
		}

	}



	// Process materials...
	if (mesh->mMaterialIndex >= 0) {

		aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

		// Diffuse.
		vector<Texture> diffuseMaps = _loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
		textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end()); // Store whole diffuseMaps vec in textures.


		// Specular.
		vector<Texture> specularMaps = _loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
		textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
	}


	return AssimpMesh(vertices, indices, textures);
}

std::vector<Texture> AssimpModel::_loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName){

	
	using namespace std;

	// TODO:
	// Check whether a texture was already loaded, and if
	// just reuse it and if not then load it...


	vector<Texture> textures;
	for (GLuint i = 0; i < mat->GetTextureCount(type); i++) { // Cycle through count of textures of needed type.


		aiString str;

		mat->GetTexture(type, i, &str);


		Texture texture;

		// Store information about texture.
		texture.ID = TextureFromFile(str.C_Str(), m_Directory);


		if (texture.ID == 0) {

			cout << color(colors::RED);
			cout << "Error Loading Texture \"" << str.C_Str() << "\" :\n" << white;
			cout << "Dir: \""<< m_Directory <<"\" .\n" << white;

		}
		cout << color(colors::GREEN);
		cout << "Texture \"" << str.C_Str() << "\" Loaded!\n" << white;


		texture.Type = typeName;
		texture.Path = str.C_Str();


		textures.push_back(texture);
	}

	return textures;
}



#define STB_IMAGE_IMPLEMENTATION // Needed for library.
#include<stb_image/stb_image.h>

GLint TextureFromFile(const char* path, std::string directory, bool genMipMaps){


	int w, h, cmp; // cmp = rgb or rgba etc.
	GLuint textureID;

	// Load image.
	unsigned char* imgData = stbi_load(path, &w, &h, &cmp, STBI_rgb_alpha);

	if (imgData != NULL) {

		// Create texture object.
		glGenTextures(1, &textureID);


		glBindTexture(GL_TEXTURE_2D, textureID);


		// Apply options for texture.
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // Up axis for OpenGL texels
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); // Down axis.

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR); // Linear texture filtering.
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		// Map bits from image to opengl texture.
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, imgData);

		// Generate mip maps.
		if (genMipMaps) {
			glGenerateMipmap(GL_TEXTURE_2D);
		}

		// Free memory of image.
		stbi_image_free(imgData);

		// Free binding for image = unbind. 
		glBindTexture(GL_TEXTURE_2D, 0);

		return textureID;
	}
	else {
		return 0;
	}
}
