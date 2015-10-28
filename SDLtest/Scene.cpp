#include "Scene.h"



Scene::Scene(double pRatio, double pAngle, double pNear, double pFar) :
	m_ratio(pRatio), m_angle(pAngle), m_near(pNear), m_far(pFar), m_skyboxMap("sky"), m_skybox("cube.obj")
{
	loadShaders();
	loadMeshs();

	updateProjectionMatrix();
	createAxis();
}


Scene::~Scene()
{
	for (ShaderList::iterator it = m_shaders.begin(); it != m_shaders.end(); ++it) {
		delete it->second;
	}

	for (MeshList::iterator it = m_meshs.begin(); it != m_meshs.end(); ++it) {
		delete it->second;
	}
}

void Scene::render(const Camera& pCamera)
{
	drawMeshs(pCamera);
}

void Scene::updateProjectionMatrix()
{
	m_projectionMatrix = perspective(m_angle, m_ratio, m_near, m_far);

	for (ShaderList::iterator it = m_shaders.begin(); it != m_shaders.end(); ++it) {
		ShaderProgram* shader = it->second;

		shader->use();
		shader->updateProjectionMatrix(m_projectionMatrix);
		shader->stop();
	}
}

void Scene::createAxis()
{
	float pVertices[] = {
		0,0,0, 1,0,0,
		0,0,0, 0,1,0,
		0,0,0, 0,0,1
	};

	float pColors[] = {
		1,0,0, 1,0,0,
		0,1,0, 0,1,0,
		0,0,1, 0,0,1
	};

	m_axis.storeVertices(vector<vec3>((vec3*) pVertices, (vec3*) pVertices + 6));
	m_axis.storeColors(vector<vec3>((vec3*) pColors, (vec3*) pColors + 6));
}

void Scene::loadShaders()
{
	loadShader("simpleColor");
	loadShader("simpleShadow");
	loadShader("simpleTextured");
	loadShader("forceColor");
	loadShader("skybox");
	loadShader("deferredGeometry");
}

void Scene::loadShader(std::string pName)
{
	m_shaders[pName] = new ShaderProgram(pName + ".vert", pName + ".frag");
}

void Scene::loadMeshs()
{
	//m_meshs["sphere"] = new Mesh("sphere.obj", "rock");
	//m_shaderMeshs[m_shaders["simpleTextured"]].push_back(m_meshs["sphere"]);

	m_meshs["cube"] = new Mesh("cube.obj", "stone");
	m_shaderMeshs[m_shaders["deferredGeometry"]].push_back(m_meshs["cube"]);

	//m_meshs["dragon"] = new Mesh("dragon.obj");
	//m_shaderMeshs[m_shaders["simpleShadow"]].push_back(m_meshs["dragon"]);
}


void Scene::drawSky(const Camera& pCamera)
{
	glDepthMask(GL_FALSE);
	glDisable(GL_CULL_FACE);

	m_shaders["skybox"]->use();
	m_shaders["skybox"]->updateViewMatrix(glm::mat4(glm::mat3(pCamera.getViewMatrix())));
	m_skybox.bind();
	m_skyboxMap.bind();
	m_skybox.draw();
	m_skybox.unbind();
	m_shaders["skybox"]->stop();

	glEnable(GL_CULL_FACE);
	glDepthMask(GL_TRUE);
}

void Scene::drawMeshs(const Camera& pCamera)
{
	for (ShaderList::iterator it = m_shaders.begin(); it != m_shaders.end(); ++it) {
		ShaderProgram* shader = it->second;

		// Use shader and update view matrix
		shader->use();
		shader->updateViewMatrix(pCamera.getViewMatrix());

		for (int i(0); i < m_shaderMeshs[shader].size(); ++i) {
			Mesh* mesh = m_shaderMeshs[shader][i];

			// Bind mesh
			mesh->bind();

			for (int j(0); j < m_meshEntities[mesh].size(); ++j) {
				Entity* entity = m_meshEntities[mesh][j];

				// Update model matrix and draw mesh
				shader->updateModelMatrix(entity->getModelMatrix());
				mesh->draw();
			}

			mesh->unbind();
		}

		shader->stop();
	}
}

void Scene::drawAxis()
{
	m_shaders["simpleColor"]->use();
	m_shaders["simpleColor"]->updateModelMatrix(glm::mat4());
	m_axis.drawLines();
	m_shaders["simpleColor"]->stop();
}

void Scene::drawNormals()
{
	m_shaders["forceColor"]->use();

	for (MeshList::iterator it = m_meshs.begin(); it != m_meshs.end(); ++it) {
		Mesh* mesh = it->second;

		for (int j(0); j < m_meshEntities[mesh].size(); ++j) {
			Entity* entity = m_meshEntities[mesh][j];

			m_shaders["forceColor"]->updateModelMatrix(entity->getModelMatrix());

			// Normals
			m_shaders["forceColor"]->updateUniform("color", glm::vec3(0, 1, 0));

			mesh->bindNormals();
			mesh->drawNormals();
			mesh->unbindNormals();

			// Tangents
			m_shaders["forceColor"]->updateUniform("color", glm::vec3(1, 0, 0));

			mesh->bindTangents();
			mesh->drawTangents();
			mesh->unbindTangents();
		}

	}

	m_shaders["forceColor"]->stop();
}

void Scene::addEntity(string pName, mat4 pModelMatrix)
{
	Entity* entity = new Entity(m_meshs[pName], pModelMatrix);

	m_meshEntities[m_meshs[pName]].push_back(entity);
	m_entities.push_back(entity);
}

vector<Entity*>* Scene::getEntities()
{
	return &m_entities;
}

mat4 Scene::getProjectionMatrix()
{
	return m_projectionMatrix;
}
