#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Camera
{
public:
	Camera(glm::vec3 position = glm::vec3(0, 0, 0), glm::vec3 lookAt = glm::vec3(0, 1, 0), glm::vec3 up = glm::vec3(0, 0, 1));
	~Camera();

	glm::mat4 getViewMatrix() const;

	void rotate(float angle, glm::vec3 axis);

private:
	void updateViewMatrix();

private:
	glm::mat4 m_viewMatrix;
	glm::vec3 m_position;
	glm::vec3 m_lookAt;
	glm::vec3 m_up;
};

