#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Camera.h"
#include "Mesh.h"

class Scene
{
public:
	Scene(double ratio, double angle = 70, double near = 1, double far = 100);
	~Scene();

	void render(const Camera& camera);

private:
	void updateProjectionMatrix();

private:
	glm::mat4 m_projectionMatrix;
	double m_angle;
	double m_ratio;
	double m_near;
	double m_far;
	Mesh m_mesh;
};

