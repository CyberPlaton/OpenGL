#pragma once


#include<vector>
#include<string>

#include<GL/glew.h>
#include<glm/glm.hpp>


// Assimp
#include<assimp/Importer.hpp>
#include<assimp/scene.h>
#include<assimp/postprocess.h>

#include"ShaderProgram.h"
#include"Texture2D.h"



GLint TextureFromFile(const char* path, std::string directory, bool genMipMaps = true);

struct Vertex3 {
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 TexCoords;
};


struct Texture {
	GLuint ID;
	std::string Type;
	std::string Path;
};


struct Material {

	glm::vec3 ambient;
	std::string diffuse;
	std::string specular;
	float shininess;

};


class AssimpMesh {
public:
	AssimpMesh(std::vector<Vertex3> vertices, std::vector<GLuint> indices, std::vector<Texture> textures);

	void Draw(ShaderProgram& shader); // TODO: Move shader to functions: set and get for rendering.

	std::vector<Vertex3> m_Vertices;
	std::vector<GLuint> m_Indices;
	std::vector<Texture> m_Textures;

private:
	GLuint VAO, VBO, EBO;


private:

	void _setup();
};






class AssimpModel{
public:
	AssimpModel(const std::string& path) { _loadModel(path); }

	// Setting shader dynamically for drawing may help
	// try out different shader for optimizing FPS...
	void Draw(ShaderProgram& shader);

private:
	std::vector<AssimpMesh> m_Meshes;
	std::string m_Directory;


private:

	void _loadModel(std::string path);
	


	void _processNode(aiNode* node, const aiScene* scene);
	AssimpMesh _processMesh(aiMesh* mesh, const aiScene* scene);
	std::vector<Texture> _loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);

};

