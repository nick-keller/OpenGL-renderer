#include "Scene.h"



Scene::Scene(double ratio, double angle, double near, double far) :
	m_ratio(ratio), m_angle(angle), m_near(near), m_far(far)
{
}


Scene::~Scene()
{
}

void Scene::render(const Camera& camera)
{
	m_mesh.render();
}

void Scene::updateProjectionMatrix()
{
	m_projectionMatrix = glm::perspective(m_angle, m_ratio, m_near, m_far);
}
