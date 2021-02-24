#pragma once

#include<GL/glew.h>
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtx/quaternion.hpp>



struct Pos_Color_Tex {
	glm::vec3 m_Position;
	glm::vec3 m_Color;
	glm::vec2 m_TexCoords;
};