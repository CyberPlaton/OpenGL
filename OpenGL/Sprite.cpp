#include"Sprite.h"

Texture2D::Texture2D() : m_TextureHandle(0) {

}

Texture2D::~Texture2D() {


}


bool Texture2D::LoadTexture(const std::string& fileName, bool genMipMaps) {

	int w, h, cmp; // cmp = rgb or rgba etc.

	// Load image.
	unsigned char* imgData = stbi_load(fileName.c_str(), &w, &h, &cmp, STBI_rgb_alpha);

	if (imgData != NULL) {

			// Invrt image orientation.
			int byteWidth = w * 4;
		unsigned char* top = NULL;
		unsigned char* bottom = NULL;
		unsigned char temp = NULL;
		int halfHeight = h / 2;
		for (int row = 0; row < halfHeight; row++) {
			top = imgData + row * byteWidth;
			bottom = imgData + (h - row - 1) * byteWidth;
			for (int col = 0; col < byteWidth; col++) {
				temp = *top;
				*top = *bottom;
				*bottom = temp;
				top++;
				bottom++;
			}
		}


		// Get information from image.
		m_Size = glm::vec2(w, h); // Size aka Dimensions.
		m_FilePath = fileName; // Save path for resource manager.

		// Create texture object.
		glGenTextures(1, &m_TextureHandle);


		glBindTexture(GL_TEXTURE_2D, m_TextureHandle);


		// Apply options for texture.
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT); // Up axis for OpenGL texels
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); // Down axis.

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); // Linear texture filtering.
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

		return true;
	}
	else {

		using namespace std;
		cout << color(colors::RED);
		cout << "Error Loading image: " << fileName << white << endl;
		return false;
	}
}

void Texture2D::Bind(GLuint texUint) {


	glActiveTexture(GL_TEXTURE0 + texUint); // For binding texture to "texUint" particular index.

	glBindTexture(GL_TEXTURE_2D, m_TextureHandle);
}

void Texture2D::Unbind(GLuint texUint) {

	glActiveTexture(GL_TEXTURE0 + texUint); // Bind to textures index a 0 texture --> unbind.
	glBindTexture(GL_TEXTURE_2D, 0);
}


Sprite::Sprite(std::string shader, std::string texture) {

	m_SpriteShader = new ShaderProgram();
	m_SpriteShader->LoadShaders((shader + ".vert").c_str(), (shader + ".frag").c_str());

	m_SpriteTexture = new Texture2D();
	m_SpriteTexture->LoadTexture(texture);


	Pos_Color_Tex v1, v2, v3, v4;
	v1.m_Position = glm::vec3(0.5f, 0.5f, 0.0f);
	v2.m_Position = glm::vec3(0.5f, -0.5f, 0.0f);
	v3.m_Position = glm::vec3(-0.5f, -0.5f, 0.0f);
	v4.m_Position = glm::vec3(-0.5f, 0.5f, 0.0f);

	v1.m_Color = glm::vec3(0.0f, 1.0f, 0.0f);
	v2.m_Color = glm::vec3(0.0f, 1.0f, 0.0f);
	v3.m_Color = glm::vec3(0.0f, 1.0f, 0.0f);
	v4.m_Color = glm::vec3(0.0f, 1.0f, 0.0f);

	v1.m_TexCoords = glm::vec2(1.0f, 1.0f);
	v2.m_TexCoords = glm::vec2(1.0f, 0.0f);
	v3.m_TexCoords = glm::vec2(0.0f, 0.0f);
	v4.m_TexCoords = glm::vec2(0.0f, 1.0f);

	m_CommonVertices.push_back(v1);
	m_CommonVertices.push_back(v2);
	m_CommonVertices.push_back(v3);
	m_CommonVertices.push_back(v4);

	/*
	float vertices[] = {
		// positions          // colors           // texture coords
		 0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // top right
		 0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // bottom right
		-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // bottom left
		-0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f  // top left 
	};
	*/

	unsigned int indices[] = {
		0, 1, 3, // first triangle
		1, 2, 3  // second triangle
	};

	//unsigned int VBO, EBO; // Todo: move to memory protocol.
	glGenVertexArrays(1, &m_VAO);
	glGenBuffers(1, &m_VBO);
	glGenBuffers(1, &m_EBO);

	glBindVertexArray(m_VAO);

	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBufferData(GL_ARRAY_BUFFER, m_CommonVertices.size()* sizeof(Pos_Color_Tex), &m_CommonVertices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);


	// Vertex positions.
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Pos_Color_Tex), (GLvoid*)0);

	// Vertex colors.
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Pos_Color_Tex), (GLvoid*)offsetof(Pos_Color_Tex, m_Color));

	// Vertex texture coords.
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Pos_Color_Tex), (GLvoid*)offsetof(Pos_Color_Tex, m_TexCoords));

	/*
	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	// texture coord attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);
	*/


	glBindVertexArray(0); // Unbind.
}


Sprite::~Sprite() {

}


void Sprite::Draw(){

	m_SpriteShader->Use();

	m_SpriteTexture->Bind(); // Equals to setting uniform sampler...


	glm::mat4 model = glm::mat4(1);
	model = glm::translate(glm::mat4(), glm::vec3(m_Position, 1.0f)) * glm::rotate(model, m_Rotation, glm::vec3(0.0f, 0.0f, 1.0f)) *glm::scale(glm::mat4(), glm::vec3(m_Scale, m_Scale, 0.0f));

	m_SpriteShader->SetUniform("view", glm::mat4(1.0f)); // id
	m_SpriteShader->SetUniform("projection", glm::mat4(1.0f)); // id
	m_SpriteShader->SetUniform("model", model);
	m_SpriteShader->SetUniform("color", glm::vec4(m_Color, m_Brightness));
	
	//glBlendFunc(GL_SRC_COLOR, GL_SRC_COLOR);


	glBindVertexArray(m_VAO);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0); // TODO: Move to batch renderer.

	glBindVertexArray(0);


	m_SpriteTexture->Unbind();
}
