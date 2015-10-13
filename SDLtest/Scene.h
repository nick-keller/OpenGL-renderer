#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <map>
#include <vector>
#include "Camera.h"
#include "Mesh.h"
#include "ShaderProgram.h"
#include "Entity.h"

using namespace std;
using namespace glm;

typedef map<string, ShaderProgram*> ShaderList;
typedef map<string, Mesh*> MeshList;

class Scene
{
public:
	Scene(double pRatio, double pAngle = 70, double pNear = 1, double pFar = 100);
	~Scene();

	void render(const Camera& pCamera);

private:
	void updateProjectionMatrix();
	void createAxis();
	void loadShaders();
	void loadShader(string pName);
	void loadMeshs();

	void drawMeshs(const Camera& pCamera);
	void drawAxis();
	void drawNormals();

private:
	mat4 m_projectionMatrix;
	double m_angle;
	double m_ratio;
	double m_near;
	double m_far;

	VAO m_axis;
	ShaderList m_shaders;
	MeshList m_meshs;
	map<ShaderProgram*, vector<Mesh*> > m_shaderMeshs;
	map<Mesh*, vector<Entity*> > m_meshEntities;
};

