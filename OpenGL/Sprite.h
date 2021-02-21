#pragma once

#include<GL/glew.h>
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>


#include"ColorConsole.h"
#include"ShaderProgram.h"
#include<stb_image/stb_image.h>


class Texture2D {
public:

	Texture2D();
	~Texture2D();


	bool LoadTexture(const std::string& fileName, bool genMipMaps = true);
	void Bind(GLuint texUint = 0);
	void Unbind(GLuint texUint = 0);
	GLuint GetSlot() const { return m_TextureHandle; }

private:

	GLuint m_TextureHandle;

private:

};


class Sprite {
public:
	Sprite(std::string shader, std::string texture);
	~Sprite();

	void Draw();

	void SetPosition(glm::vec2 pos) { m_Position = pos; }
private:
	glm::vec2 m_Position;
	glm::vec3 m_Color;
	float m_Rotation = 0.0f;
	glm::vec2 m_Size = glm::vec2(1.0f);

	Texture2D* m_SpriteTexture = nullptr;

	ShaderProgram* m_SpriteShader = nullptr;
	GLuint m_QuadVAO;
private:

};
