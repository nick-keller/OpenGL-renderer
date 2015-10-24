#include "Camera.h"



Camera::Camera(vec3 pPosition, vec3 pLookAt, vec3 pUp) :
	m_position(pPosition), m_lookAt(normalize(pLookAt)), m_up(normalize(pUp)), m_height(PLAYER_HEIGHT + PLAYER_EYES), m_crouched(false)
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

void Camera::rotate(float h, float v)
{
	vec3 left = cross(m_up, m_lookAt);

	m_lookAt = rotateZ(m_lookAt, h);
	m_lookAt = glm::rotate(m_lookAt, v, left);

	updateViewMatrix();
}

void Camera::moveForward(float length)
{
	vec2 forward = normalize(vec2(m_lookAt.x, m_lookAt.y));

	m_position.x += forward.x * length;
	m_position.y += forward.y * length;

	updateViewMatrix();
}

void Camera::moveLeft(float length)
{
	vec3 left = cross(m_up, m_lookAt);

	m_position.x += left.x * length;
	m_position.y += left.y * length;

	updateViewMatrix();
}

void Camera::move(vec3 delta)
{
	m_position += delta;
	updateViewMatrix();
}

void Camera::update(float delta)
{
	if (m_crouched && m_height > PLAYER_CROUCH + PLAYER_EYES) {
		m_height = max(PLAYER_CROUCH + PLAYER_EYES, m_height - delta / 200.f);
	}

	if (!m_crouched && m_height < PLAYER_HEIGHT + PLAYER_EYES) {
		m_height = min(PLAYER_HEIGHT + PLAYER_EYES, m_height + delta / 200.f);
	}
}

void Camera::setCrouched(bool crouched)
{
	m_crouched = crouched;
}

AABB Camera::getBoundingBox()
{
	return{
		m_position - vec3(PLAYER_HALF_WIDTH, PLAYER_HALF_WIDTH, 0),
		m_position + vec3(PLAYER_HALF_WIDTH, PLAYER_HALF_WIDTH, m_height - PLAYER_EYES)
	};
}

void Camera::updateViewMatrix()
{
	m_viewMatrix = lookAt(m_position + vec3(0, 0, m_height), m_position + vec3(0, 0, m_height)  + m_lookAt, m_up);
}
