#include "Entity.h"



Entity::Entity(Mesh* mesh) :
	m_mesh(mesh)
{
	m_modelMatrix = glm::scale(m_modelMatrix, glm::vec3(7, 7, 7));
	m_modelMatrix = glm::rotate(m_modelMatrix, 90.f, glm::vec3(1, 0, 0));
}


Entity::~Entity()
{
}

glm::mat4 Entity::getModelMatrix() const
{
	return m_modelMatrix;
}
