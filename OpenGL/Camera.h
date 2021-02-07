#pragma once

#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/constants.hpp>


class Camera {
public:

	glm::mat4 GetViewMatrix()const;

	virtual void Rotate(float yawDegree, float pitchDegree) {};

	virtual void SetPosition(const glm::vec3& pos) {};
	virtual void Move(const glm::vec3& offSetPos) {};

	const glm::vec3& GetLookTo() const { return m_LookTo; } // = Forward vector.
	const glm::vec3& GetRight() const { return m_Right; }
	const glm::vec3& GetUp() const { return m_Up; }
	const glm::vec3& GetPos()const { return m_Position; }

	float GetFieldOfView(){ return m_FOV; };
	void SetFieldOfView(float fovDegree) { m_FOV = fovDegree; }

protected:

	Camera();


	const glm::vec3 WORLD_UP;

	glm::vec3 m_LookTo;
	glm::vec3 m_Right;

	glm::vec3 m_Position;
	glm::vec3 m_Target;
	glm::vec3 m_Up;


	// Euler angles in radians.
	float m_Yaw;
	float m_Pitch;

	float m_FOV; // In Degrees.
};



class OrbitCamera : public Camera{
public:
	OrbitCamera();

	void Rotate(float yawDegree, float pitchDegree) override;

	void SetLookAt(const glm::vec3& target);

	void SetRadius(float r);

private:

	float m_Radius;

private:

	void _updateCameraVectors();
};


class FPSCamera : public Camera {
public:
	FPSCamera(glm::vec3 pos = glm::vec3(.0f, .0f, .0f), float yaw = glm::pi<float>(), float pitch = .0f);


	void Rotate(float yawDegree, float pitchDegree) override;
	virtual void SetPosition(const glm::vec3& pos) override;
	virtual void Move(const glm::vec3& offSetPos) override;



private:

private:

	void _updateCameraVectors();
};