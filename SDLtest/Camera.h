#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

using namespace glm;

class Camera
{
public:
	Camera(vec3 pPosition = vec3(0, 0, 0), vec3 pLookAt = vec3(0, 1, 0), vec3 pUp = vec3(0, 0, 1));
	~Camera();

	glm::mat4 getViewMatrix() const;

	void rotate(float pAngle, vec3 pAxis);

private:
	void updateViewMatrix();

private:
	mat4 m_viewMatrix;
	vec3 m_position;
	vec3 m_lookAt;
	vec3 m_up;
};

