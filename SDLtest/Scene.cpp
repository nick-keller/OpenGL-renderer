#include "Scene.h"



Scene::Scene(double ratio, double angle, double near, double far) :
	m_ratio(ratio), m_angle(angle), m_near(near), m_far(far), m_shader("vert.glsl", "frag.glsl")
{
	updateProjectionMatrix();
	createAxis();
}


Scene::~Scene()
{
}

void Scene::render(const Camera& camera)
{
	m_shader.use();
	m_shader.updateViewMatrix(camera.getViewMatrix());
	m_mesh.render();
	m_axis.drawLines();
	m_shader.stop();
}

void Scene::updateProjectionMatrix()
{
	m_projectionMatrix = glm::perspective(m_angle, m_ratio, m_near, m_far);

	m_shader.use();
	m_shader.updateProjectionMatrix(m_projectionMatrix);
	m_shader.stop();
}

void Scene::createAxis()
{
	float vertices[] = {
		0,0,0, 1,0,0,
		0,0,0, 0,1,0,
		0,0,0, 0,0,1
	};

	float colors[] = {
		1,0,0, 1,0,0,
		0,1,0, 0,1,0,
		0,0,1, 0,0,1
	};

	m_axis.storeVertices(std::vector<glm::vec3>((glm::vec3*) vertices, (glm::vec3*) vertices + 6));
	m_axis.storeColors(std::vector<glm::vec3>((glm::vec3*) colors, (glm::vec3*) colors + 6));
}
