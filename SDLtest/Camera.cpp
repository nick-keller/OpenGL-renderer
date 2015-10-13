#include "Camera.h"



Camera::Camera(vec3 pPosition, vec3 pLookAt, vec3 pUp) :
	m_position(pPosition), m_lookAt(pLookAt), m_up(pUp)
{
	updateViewMatrix();
}


Camera::~Camera()
{
}

mat4 Camera::getViewMatrix() const
{
	return m_viewMatrix;
}

void Camera::rotate(float pAngle, vec3 pAxis)
{
	m_viewMatrix = glm::rotate(m_viewMatrix, pAngle, pAxis);
}

void Camera::updateViewMatrix()
{
	m_viewMatrix = lookAt(m_position, m_position + m_lookAt, m_up);
}
