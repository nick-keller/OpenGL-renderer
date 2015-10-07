#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Mesh.h"

class Entity
{
public:
	Entity(Mesh* mesh);
	~Entity();

	glm::mat4 getModelMatrix() const;

private:
	Mesh* m_mesh;
	glm::mat4 m_modelMatrix;
};

