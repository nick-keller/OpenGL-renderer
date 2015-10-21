#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Mesh.h"

using namespace glm;

class Entity
{
public:
	Entity(Mesh* pMesh, mat4 pModelMatrix);
	~Entity();

	mat4 getModelMatrix() const;

private:
	Mesh* m_mesh;
	mat4 m_modelMatrix;
};

