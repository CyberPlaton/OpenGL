#pragma once

#include"Mesh.h"
#include"ShaderProgram.h"
#include<glm/glm.hpp>
#include<glm/gtx/transform.hpp>

class Light{
public:

	void SetShader(const std::string& vsname, const std::string& fsname);
	void Use();

	// Special method for drawing light source.
	// We use a standard vertex, fragment shader.
	void Draw(glm::mat4 projection, glm::mat4 view, glm::vec3 viewPos);

	glm::vec3 GetColor()const { return m_Color; }
	glm::vec3 GetPosition()const { return m_Position; }
	glm::vec3 GetScale()const { return m_Scale; }
	float GetBrightness()const { return m_Brightness; }

	void SetPosition(glm::vec3 pos);
	void SetColor(glm::vec3 color);
	void SetScale(glm::vec3 scale);
	void SetBrightness(float b);


	ShaderProgram* GetLightShader() const { return m_LightShader; }

private:

	ShaderProgram* m_DefaultShader = nullptr;

	ShaderProgram* m_LightShader = nullptr;
	Mesh* m_LightMesh = nullptr;

	glm::vec3 m_Position;
	glm::vec3 m_Scale;
	glm::vec3 m_Color;

	float m_Brightness = 1.0f;


protected:
	Light() {
		m_LightMesh = new Mesh();
		m_LightMesh->LoadOBJ("Sphere.obj");

		m_LightShader = new ShaderProgram();
		m_DefaultShader = new ShaderProgram();
		m_DefaultShader->LoadShaders("basic.vert", "basic.frag");
	}

};


class DirectionalLight : public Light {
public:
	DirectionalLight(){}

};


class PointLight : public Light {
public:
	PointLight(){}

};


class SpotLight : public Light {
public:
	SpotLight(){}

};
