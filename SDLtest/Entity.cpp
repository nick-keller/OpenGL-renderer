#include "Entity.h"


Entity::Entity(Mesh* pMesh) :
	m_mesh( pMesh )
{
	//m_modelMatrix = glm::rotate(m_modelMatrix, 70.f, glm::vec3(1, 2, 0));
	m_modelMatrix = scale(m_modelMatrix, vec3(5, 5, 5));
	//m_modelMatrix = glm::scale(m_modelMatrix, glm::vec3(8, 8, 8));
}


Entity::~Entity()
{
}

mat4 Entity::getModelMatrix() const
{
	return m_modelMatrix;
}
