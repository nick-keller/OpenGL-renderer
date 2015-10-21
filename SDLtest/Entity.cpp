#include "Entity.h"


Entity::Entity(Mesh* pMesh, mat4 pModelMatrix) :
	m_mesh( pMesh ), m_modelMatrix(pModelMatrix)
{
}


Entity::~Entity()
{
}

mat4 Entity::getModelMatrix() const
{
	return m_modelMatrix;
}
