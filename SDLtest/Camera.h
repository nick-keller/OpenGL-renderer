#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include "Mesh.h"

#define PLAYER_HEIGHT 2.f
#define PLAYER_WIDTH .35f
#define PLAYER_HALF_WIDTH PLAYER_WIDTH / 2.f
#define PLAYER_EYES -.3f
#define PLAYER_CROUCH 1.2f

using namespace glm;

class Camera
{
public:
	Camera(vec3 pPosition = vec3(0, 0, 0), vec3 pLookAt = vec3(0, 1, 0), vec3 pUp = vec3(0, 0, 1));
	~Camera();

	glm::mat4 getViewMatrix() const;
	vec3 getEyePos();

	void rotate(float h, float v);
	void moveForward(float length);
	void moveLeft(float length);
	void move(vec3 delta);
	void update(float delta);
	void setCrouched(bool crouched);
	mat4 getReflexted(float z);

	AABB getBoundingBox();

private:
	void updateViewMatrix();

private:
	mat4 m_viewMatrix;
	vec3 m_position;
	vec3 m_lookAt;
	vec3 m_up;
	float m_height;
	bool m_crouched;
};

