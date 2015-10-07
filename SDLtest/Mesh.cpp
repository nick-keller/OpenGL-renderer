#include "Mesh.h"



Mesh::Mesh(std::string filePath, std::string textureName) :
	m_diffuse(textureName + "_diffuse.jpg", Texture::DIFFUSE), 
	m_specular(textureName + "_specular.jpg", Texture::SPECULAR), 
	m_normal(textureName + "_normal.jpg", Texture::NORMAL)
{
	LoadFromFile(filePath);
}


Mesh::~Mesh()
{
}

void Mesh::bind()
{
	m_data.bind();
	m_diffuse.bind();
	m_specular.bind();
	m_normal.bind();
}

void Mesh::unbind()
{
	m_data.unbind();
}

void Mesh::draw()
{
	m_data.drawTriangles();
}

void Mesh::bindNormals()
{
	m_normals.bind();
}

void Mesh::unbindNormals()
{
	m_normals.unbind();
}

void Mesh::drawNormals()
{
	m_normals.drawLines();
}

void Mesh::bindTangents()
{
	m_tangents.bind();
}

void Mesh::unbindTangents()
{
	m_tangents.unbind();
}

void Mesh::drawTangents()
{
	m_tangents.drawLines();
}

void Mesh::LoadFromFile(std::string & filePath)
{
	std::ifstream file(filePath.c_str());
	std::string line, id;

	std::vector <glm::vec3> objVertices;
	std::vector <glm::vec2> objUvs;
	std::vector <glm::vec3> objNormals;
	std::vector <glm::uvec3> objFaces;

	std::vector <glm::vec3> finalVertices;
	std::vector <glm::vec2> finalUvs;
	std::vector <glm::vec3> finalNormals;
	std::vector <glm::uvec3> finalFaces;

	while (std::getline(file, line)) {
		if (line.empty()) {
			continue;
		}

		std::stringstream data(line);

		data >> id;

		if (id == "v") {
			glm::vec3 v;
			data >> v.x >> v.y >> v.z;
			objVertices.push_back(v);
		}

		if (id == "vn") {
			glm::vec3 vn;
			data >> vn.x >> vn.y >> vn.z;
			objNormals.push_back(vn);
		}

		if (id == "vt") {
			glm::vec2 vt;
			data >> vt.x >> vt.y;
			objUvs.push_back(vt);
		}

		if (id == "f") {
			std::string faceInfo;

			for (int i(0); i < 3; ++i) {
				glm::uvec3 f;
				data >> faceInfo;

				std::size_t first = faceInfo.find("/");
				std::size_t second = faceInfo.find("/", first + 1);

				// Vertex
				f.x = std::stoi(faceInfo.substr(0, first)) - 1;

				// UV
				f.y = second - first == 1 ? 
					-1 : 
					std::stoi(faceInfo.substr(first + 1, second - first)) - 1;

				// Normal
				f.z = faceInfo.size() - second == 1 ?
					-1 :
					std::stoi(faceInfo.substr(second + 1)) - 1;
					

				objFaces.push_back(f);
			}
		}
	}

	file.close();

	for (int i(0); i < objFaces.size(); ++i) {
		glm::uvec3 face = objFaces[i];
		GLuint index = finalVertices.size();

		// Check if vertice already exists
		for (int j(0); j < finalVertices.size(); ++j) {
			if (objVertices[face.x] != finalVertices[j]) {
				continue;
			}

			if (face.y != -1 && objUvs[face.y] != finalUvs[j]) {
				continue;
			}

			if (face.z != -1 && objNormals[face.z] != finalNormals[j]) {
				continue;
			}

			index = j;
			break;
		}

		// If not create it
		if (index == finalVertices.size()) {
			// Vertex
			finalVertices.push_back(objVertices[face.x]);

			// UV
			if (face.y != -1) {
				finalUvs.push_back(objUvs[face.y]);
			}

			// Normal
			if (face.z != -1) {
				finalNormals.push_back(objNormals[face.z]);
			}
		}

		// Save face
		if (i % 3 == 0) {
			finalFaces.push_back(glm::uvec3(index, 0, 0));
		}
		else if (i % 3 == 1) {
			finalFaces[i / 3].y = index;
		}
		else {
			finalFaces[i / 3].z = index;
		}
	}

	setData(finalVertices, finalNormals, finalUvs, finalFaces);
}

void Mesh::setData(std::vector<glm::vec3>& vertices, std::vector<glm::vec3>& normals, std::vector<glm::vec2>& uvs, std::vector<glm::uvec3>& faces)
{
	m_data.storeVertices(vertices);

	if (normals.size()) {
		m_data.storeNormals(normals);
		std::vector<glm::vec3> tangents = computeTangents(vertices, normals, uvs, faces);
		m_data.storeTangents(tangents);
		m_tangents.storeVertices(computeNormalsVertices(vertices, tangents, 7));
		m_normals.storeVertices(computeNormalsVertices(vertices, normals, 7));
	}

	if (uvs.size()) {
		m_data.storeUvs(uvs);
	}

	m_data.storeFaces(faces);
}

std::vector<glm::vec3> Mesh::computeNormalsVertices(std::vector<glm::vec3>& vertices, std::vector<glm::vec3>& normals, int factor)
{
	std::vector <glm::vec3> normalsVertices;

	for (int i(0); i < vertices.size(); ++i) {
		normalsVertices.push_back(vertices[i]);
		normalsVertices.push_back(vertices[i] + glm::normalize(normals[i]) / (float) factor);
	}

	return normalsVertices;
}

std::vector<glm::vec3> Mesh::computeTangents(std::vector<glm::vec3>& vertices, std::vector<glm::vec3>& normals, std::vector<glm::vec2>& uvs, std::vector<glm::uvec3>& faces)
{
	std::vector <glm::vec3> tangents(vertices.size());

	for (int i(0); i < faces.size(); ++i) {
		glm::vec3 edge1(vertices[faces[i].y] - vertices[faces[i].x]);
		glm::vec3 edge2(vertices[faces[i].z] - vertices[faces[i].x]);

		glm::vec2 delta1(uvs[faces[i].y] - uvs[faces[i].x]);
		glm::vec2 delta2(uvs[faces[i].z] - uvs[faces[i].x]);

		float f = 1.f / (delta1.x * delta2.y - delta2.x * delta1.y);

		glm::vec3 tangent;

		tangent.x = f * ((edge1.x * delta2.y) + (edge2.x * -delta1.y));
		tangent.y = f * ((edge1.y * delta2.y) + (edge2.y * -delta1.y));
		tangent.z = f * ((edge1.z * delta2.y) + (edge2.z * -delta1.y));

		tangents[faces[i].x] += tangent;
		tangents[faces[i].y] += tangent;
		tangents[faces[i].z] += tangent;
	}

	return tangents;
}
