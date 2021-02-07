#pragma once


#include<vector>
#include<string>

#include<GL/glew.h>
#include<glm/glm.hpp>

#include"ColorConsole.h"

struct Vertex {

	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 texCoords;
};

class Mesh{
public:
	Mesh();
	~Mesh();


	bool LoadOBJ(const std::string& fileName);
	void DrawMesh();


	//void SetTexture(const std::string& fileName);
	//Texture2D* GetTexture();


private:

	bool m_IsLoaded = false;
	
	std::vector<Vertex> m_Vertices;

	GLuint m_VBO, m_VAO; // Vertexarray objet and vertexbuffer object.

	//Texture2D* m_MeshTexture = nullptr;

private:

	void _initBuffers();
};

