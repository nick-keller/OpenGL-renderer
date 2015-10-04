#include "Mesh.h"



Mesh::Mesh()
{
	std::vector<glm::vec3> vertices;
	vertices.push_back(glm::vec3(0, 1, 0));
	vertices.push_back(glm::vec3(0, 0, 0));
	vertices.push_back(glm::vec3(1, 0, 0));

	vertices.push_back(glm::vec3(0.5, 1, -0.5));
	vertices.push_back(glm::vec3(0.5, 0, -0.5));
	vertices.push_back(glm::vec3(0.5, 0, .5));

	std::vector<glm::vec3> colors;
	colors.push_back(glm::vec3(1., 0., 0.));
	colors.push_back(glm::vec3(0., 1., 0.));
	colors.push_back(glm::vec3(0., 0., 1.));

	colors.push_back(glm::vec3(1., 1., 0.));
	colors.push_back(glm::vec3(0., 1., 1.));
	colors.push_back(glm::vec3(1., 0., 1.));

	std::vector<glm::uvec3> faces;
	faces.push_back(glm::uvec3(3, 4, 5));
	faces.push_back(glm::uvec3(0, 1, 2));

	m_data.storeVertices(vertices);
	m_data.storeFaces(faces);
	m_data.storeColors(colors);
}


Mesh::~Mesh()
{
}

GLuint Mesh::getVaoId()
{
	return m_data.getId();
}

VAO Mesh::getVao()
{
	return m_data;
}

void Mesh::render()
{
	m_data.drawTriangles();
}
