#include"Light.h"


void Light::Draw(glm::mat4 projection, glm::mat4 view, glm::vec3 viewPos) {

	glm::mat4 model = glm::translate(glm::mat4(), m_Position) * glm::scale(glm::mat4(), m_Scale);

	m_DefaultShader->Use();

	m_DefaultShader->SetUniform("model", model);
	m_DefaultShader->SetUniform("view", view);
	m_DefaultShader->SetUniform("projection", projection);
	m_DefaultShader->SetUniform("lightColor", m_Color); // Send color defs. to shader.
	m_DefaultShader->SetUniform("lightPos", m_Position); // Send color defs. to shader.
	m_DefaultShader->SetUniform("viewPos", viewPos); // Send color defs. to shader.

	m_LightMesh->DrawMesh();
}

void Light::SetShader(const std::string& vsname, const std::string& fsname) {

	m_LightShader->LoadShaders(vsname.c_str(), fsname.c_str());
}

void Light::Use() {

	m_LightShader->Use();
}


void Light::SetPosition(glm::vec3 pos) {

	m_Position = pos;
}

void Light::SetColor(glm::vec3 color) {

	m_Color = color;
}

void Light::SetScale(glm::vec3 scale){
	m_Scale = scale;
}

void Light::SetBrightness(float b) {

	m_Brightness = b;
}
