#include "Camera.h"



Camera::Camera(glm::vec3 position, glm::vec3 lookAt, glm::vec3 up) :
	m_position(position), m_lookAt(lookAt), m_up(up)
{
	updateViewMatrix();
}


Camera::~Camera()
{
}

glm::mat4 Camera::getViewMatrix() const
{
	return m_viewMatrix;
}

void Camera::updateViewMatrix()
{
	m_viewMatrix = glm::lookAt(m_position, m_position + m_lookAt, m_up);
}
