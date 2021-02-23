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



	glm::vec2 GetSize()const { return m_Size; }
	std::string GetPath()const { return m_FilePath; }

private:

	GLuint m_TextureHandle;

	std::string m_FilePath;

	glm::vec2 m_Size = glm::vec2(0.0f);

private:

};


class Sprite {
public:
	Sprite(std::string shader, std::string texture);
	~Sprite();

	void Draw();

	void SetPosition(glm::vec2 pos) { m_Position = pos; }
	void SetScale(float s) { m_Scale = s; }
	void SetColor(glm::vec3 c) { m_Color = c; }
	void SetBrightness(float b) { if (b <= 1.0f && b >= 0.0f) m_Brightness = b; }
	void SetRotation(float a) { m_Rotation = a; }

public:
	glm::vec2 m_Position;
	glm::vec3 m_Color = glm::vec3(1.0f); // Default white color.
	float m_Brightness = 1.0f; // Valid value 0.0f-1.0f, where 1.0f is full brightness.
	float m_Scale = 1.0f;
	float m_Rotation = 0.0f; // Default, no rotation.

	Texture2D* m_SpriteTexture = nullptr;

	ShaderProgram* m_SpriteShader = nullptr;
	GLuint m_QuadVAO;

private:

};
