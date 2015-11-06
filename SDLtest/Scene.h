#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <map>
#include <vector>
#include "Camera.h"
#include "Mesh.h"
#include "ShaderProgram.h"
#include "Entity.h"
#include "CubeMap.h"

using namespace std;
using namespace glm;

typedef map<string, ShaderProgram*> ShaderList;
typedef map<string, Mesh*> MeshList;

class Scene
{
public:
	Scene(double pRatio, double pAngle = 70, double pNear = 0.1, double pFar = 100);
	~Scene();

	void render(const Camera& pCamera);
	void fastRender(mat4 pViewMatrix);
	void drawSky(mat4 pViewMatrix);
	void drawSky(const Camera& pCamera);
	void drawNormals();
	void drawAxis();

	void addEntity(string pName, mat4 pModelMatrix = mat4());
	vector<Entity*>* getEntities();
	mat4 getProjectionMatrix();

private:
	void updateProjectionMatrix();
	void createAxis();
	void loadShaders();
	void loadShader(string pName);
	void loadMeshs();

	void drawMeshs(const Camera& pCamera);

private:
	mat4 m_projectionMatrix;
	double m_angle;
	double m_ratio;
	double m_near;
	double m_far;

	VAO m_axis;
	ShaderList m_shaders;
	MeshList m_meshs;
	vector<Entity*> m_entities;
	map<ShaderProgram*, vector<Mesh*> > m_shaderMeshs;
	map<Mesh*, vector<Entity*> > m_meshEntities;

	CubeMap m_skyboxMap;
	Mesh m_skybox;
};

