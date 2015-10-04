#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Mesh.h"

class Entity
{
public:
	Entity();
	~Entity();

private:
	Mesh* m_mesh;
	glm::mat4 m_modelMatrix;
};

