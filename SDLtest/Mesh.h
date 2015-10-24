#pragma once

#include <GL/glew.h>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include "VAO.h"
#include "Texture.h"
#include <float.h>

using namespace std;
using namespace glm;

struct AABB {
	vec3 position;
	vec3 size;
} typedef AABB;

class Mesh
{
public:
	Mesh(string pFilePath, string pTextureName = "");
	~Mesh();

	void bind();
	void unbind();
	void draw();

	void bindNormals();
	void unbindNormals();
	void drawNormals();

	void bindTangents();
	void unbindTangents();
	void drawTangents();

	AABB getBoundingBox();
	AABB getRawBoundingBox();

private:
	void LoadFromFile(string& filePath);
	void setData(vector <vec3>& pVertices, vector <vec3>& pNormals, vector <vec2>& pUVs, vector <uvec3>& pFaces);
	vector <vec3> computeNormalsVertices(vector <vec3>& pVertices, vector <vec3>& pNormals, int pFactor = 1);
	vector <vec3> computeTangents(vector <vec3>& pVertices, vector <vec3>& pNormals, vector<vec2>& pUVs, vector <uvec3>& pFaces);
	void updateBoundingBox(vec3 v);

private:
	VAO m_data;
	VAO m_normals;
	VAO m_tangents;
	Texture m_diffuse;
	Texture m_specular;
	Texture m_normal;
	Texture m_bump;
	AABB m_boundingBox;
};

