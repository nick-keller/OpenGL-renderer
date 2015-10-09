#include "Entity.h"



Entity::Entity(Mesh* mesh) :
	m_mesh(mesh)
{
	//m_modelMatrix = glm::rotate(m_modelMatrix, 70.f, glm::vec3(1, 2, 0));
	m_modelMatrix = glm::scale(m_modelMatrix, glm::vec3(5, 5, 5));
	//m_modelMatrix = glm::scale(m_modelMatrix, glm::vec3(8, 8, 8));
}


Entity::~Entity()
{
}

glm::mat4 Entity::getModelMatrix() const
{
	return m_modelMatrix;
}
