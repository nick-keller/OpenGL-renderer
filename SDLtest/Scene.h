#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <map>
#include <vector>
#include "Camera.h"
#include "Mesh.h"
#include "ShaderProgram.h"
#include "Entity.h"

typedef std::map<std::string, ShaderProgram*> ShaderList;
typedef std::map<std::string, Mesh*> MeshList;

class Scene
{
public:
	Scene(double ratio, double angle = 70, double near = 1, double far = 100);
	~Scene();

	void render(const Camera& camera);

private:
	void updateProjectionMatrix();
	void createAxis();
	void loadShaders();
	void loadShader(std::string name);
	void loadMeshs();

	void drawMeshs(const Camera& camera);
	void drawAxis();
	void drawNormals();

private:
	glm::mat4 m_projectionMatrix;
	double m_angle;
	double m_ratio;
	double m_near;
	double m_far;

	VAO m_axis;
	ShaderList m_shaders;
	MeshList m_meshs;
	std::map<ShaderProgram*, std::vector<Mesh*> > m_shaderMeshs;
	std::map<Mesh*, std::vector<Entity*> > m_meshEntities;
};

