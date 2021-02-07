
#include"Camera.h"

const float DEF_FOV = 45.0f;

glm::mat4 Camera::GetViewMatrix()const {
	
	return glm::lookAt(m_Position, m_Target, m_Up);
}

Camera::Camera() :  m_Up(glm::vec3(0.0f, 1.0f, 0.0f)),
					m_Target(glm::vec3(0.0f, 0.0f, 0.0f)),
					m_Position(glm::vec3(0.0f, 0.0f, 0.0f)),
					m_Yaw(glm::pi<float>()),
					m_Pitch(0),
					WORLD_UP(0.0f, 1.0f, 0.0f),
					m_FOV(DEF_FOV),
					m_Right(0.0f, 0.0f, 0.0f),
					m_LookTo(0.0f, 0.0f, 0.0f){
}

OrbitCamera::OrbitCamera() : m_Radius(10.0f) {

}

void OrbitCamera::Rotate(float yawDegree, float pitchDegree) {

	m_Yaw = glm::radians(yawDegree);
	m_Pitch = glm::radians(pitchDegree);

	m_Pitch = glm::clamp(m_Pitch, -glm::pi<float>() / 2.0f + 0.1f, glm::pi<float>() / 2.0f - 0.1f);

	
	_updateCameraVectors();
}

void OrbitCamera::SetLookAt(const glm::vec3& target) {

	m_Target = target;
}


void OrbitCamera::SetRadius(float r) {

	m_Radius = glm::clamp(r, 2.0f, 80.0f);
}


void OrbitCamera::_updateCameraVectors() {

	// Spherical coordinates to cartesian coordinates.
	m_Position.x = m_Target.x + m_Radius * cosf(m_Pitch) * sinf(m_Yaw);
	m_Position.y = m_Target.y + m_Radius * sinf(m_Pitch);
	m_Position.z = m_Target.z + m_Radius * cosf(m_Pitch) * cosf(m_Yaw);
}









FPSCamera::FPSCamera(glm::vec3 pos, float yaw, float pitch) {

	m_Position = pos;
	m_Yaw = yaw;
	m_Pitch = pitch;
}


void FPSCamera::Rotate(float yawDegree, float pitchDegree) {

	m_Yaw += glm::radians(yawDegree);
	m_Pitch += glm::radians(pitchDegree);

	m_Pitch = glm::clamp(m_Pitch, -glm::pi<float>() / 2.0f + 0.1f, glm::pi<float>() / 2.0f - 0.1f);
	_updateCameraVectors();
}

void FPSCamera::SetPosition(const glm::vec3& pos) {

	m_Position = pos;
}

void FPSCamera::Move(const glm::vec3& offSetPos) {

	m_Position += offSetPos;
	_updateCameraVectors();
}

void  FPSCamera::_updateCameraVectors() {

	// Spherical to cartesian coords. system
	glm::vec3 look;
	look.x = cosf(m_Pitch) * sinf(m_Yaw);
	look.y= sinf(m_Pitch);
	look.z = cosf(m_Pitch) * cosf(m_Yaw);

	m_LookTo = glm::normalize(look);

	// Re-calculate the right and up vector for camera.
	m_Right = glm::normalize(glm::cross(m_LookTo, WORLD_UP));
	m_Up = glm::normalize(glm::cross(m_Right, m_LookTo));

	m_Target = m_Position + m_LookTo;

}


