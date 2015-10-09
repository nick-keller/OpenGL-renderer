#pragma once

#include <GL/glew.h>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include "VAO.h"
#include "Texture.h"

class Mesh
{
public:
	Mesh(std::string filePath, std::string textureName);
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

private:
	void LoadFromFile(std::string& filePath);
	void setData(std::vector <glm::vec3>& vertices, std::vector <glm::vec3>& normals, std::vector <glm::vec2>& uvs, std::vector <glm::uvec3>& faces);
	std::vector <glm::vec3> computeNormalsVertices(std::vector <glm::vec3>& vertices, std::vector <glm::vec3>& normals, int factor = 1);
	std::vector <glm::vec3> computeTangents(std::vector <glm::vec3>& vertices, std::vector <glm::vec3>& normals, std::vector<glm::vec2>& uvs, std::vector <glm::uvec3>& faces);

private:
	VAO m_data;
	VAO m_normals;
	VAO m_tangents;
	Texture m_diffuse;
	Texture m_specular;
	Texture m_normal;
	Texture m_bump;
};

