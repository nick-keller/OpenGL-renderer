#include "Mesh.h"



Mesh::Mesh(string pFilePath, string pTextureName) :
	m_diffuse(pTextureName + "_diffuse.jpg", Texture::DIFFUSE),
	m_specular(pTextureName + "_specular.jpg", Texture::SPECULAR),
	m_normal(pTextureName + "_normal.jpg", Texture::NORMAL),
	m_bump(pTextureName + "_bump.jpg", Texture::BUMP)
{
	m_boundingBox = {
		vec3(FLT_MAX, FLT_MAX, FLT_MAX),
		vec3(FLT_MIN, FLT_MIN, FLT_MIN)
	};

	LoadFromFile(pFilePath);
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
	m_bump.bind();
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

AABB Mesh::getBoundingBox()
{
	return {
		m_boundingBox.position,
		m_boundingBox.size - m_boundingBox.position
	};
}

AABB Mesh::getRawBoundingBox()
{
	return m_boundingBox;
}

void Mesh::LoadFromFile(string & pFilePath)
{
	ifstream file(pFilePath.c_str());
	string line, id;

	vector <vec3> objVertices;
	vector <vec2> objUvs;
	vector <vec3> objNormals;
	vector <uvec3> objFaces;

	vector <vec3> finalVertices;
	vector <vec2> finalUvs;
	vector <vec3> finalNormals;
	vector <uvec3> finalFaces;

	while (getline(file, line)) {
		if (line.empty()) {
			continue;
		}

		stringstream data(line);

		data >> id;

		if (id == "v") {
			vec3 v;
			data >> v.x >> v.y >> v.z;
			objVertices.push_back(v);
			updateBoundingBox(v);
		}

		if (id == "vn") {
			vec3 vn;
			data >> vn.x >> vn.y >> vn.z;
			objNormals.push_back(vn);
		}

		if (id == "vt") {
			vec2 vt;
			data >> vt.x >> vt.y;
			objUvs.push_back(vt);
		}

		if (id == "f") {
			string faceInfo;

			for (int i(0); i < 3; ++i) {
				uvec3 f;
				data >> faceInfo;

				size_t first = faceInfo.find("/");
				size_t second = faceInfo.find("/", first + 1);

				// Vertex
				f.x = stoi(faceInfo.substr(0, first)) - 1;

				// UV
				f.y = second - first == 1 ? 
					-1 : 
					stoi(faceInfo.substr(first + 1, second - first)) - 1;

				// Normal
				f.z = faceInfo.size() - second == 1 ?
					-1 :
					stoi(faceInfo.substr(second + 1)) - 1;
					
				objFaces.push_back(f);
			}
		}
	}

	file.close();

	for (int i(0); i < objFaces.size(); ++i) {
		uvec3 face = objFaces[i];
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
			finalFaces.push_back(uvec3(index, 0, 0));
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

void Mesh::setData(vector<vec3>& pVertices, vector<vec3>& pNormals, vector<vec2>& pUVs, vector<uvec3>& pFaces)
{
	m_data.storeVertices(pVertices);

	if (pNormals.size()) {
		m_data.storeNormals(pNormals);
		m_normals.storeVertices(computeNormalsVertices(pVertices, pNormals, 7));

		if (pUVs.size()) {
			vector<vec3> tangents = computeTangents(pVertices, pNormals, pUVs, pFaces);
			m_data.storeTangents(tangents);
			m_tangents.storeVertices(computeNormalsVertices(pVertices, tangents, 7));
		}
	}

	if (pUVs.size()) {
		m_data.storeUvs(pUVs);
	}

	m_data.storeFaces(pFaces);
}

vector<vec3> Mesh::computeNormalsVertices(vector<vec3>& pVertices, vector<vec3>& pNormals, int pFactor)
{
	vector <vec3> normalsVertices;

	for (int i(0); i < pVertices.size(); ++i) {
		normalsVertices.push_back(pVertices[i]);
		normalsVertices.push_back(pVertices[i] + glm::normalize(pNormals[i]) / (float) pFactor);
	}

	return normalsVertices;
}

vector<vec3> Mesh::computeTangents(vector<vec3>& pVertices, vector<vec3>& pNormals, vector<vec2>& pUVs, vector<uvec3>& pFaces)
{
	vector <vec3> tangents(pVertices.size());

	for (int i(0); i < pFaces.size(); ++i) {
		vec3 edge1(pVertices[pFaces[i].y] - pVertices[pFaces[i].x]);
		vec3 edge2(pVertices[pFaces[i].z] - pVertices[pFaces[i].x]);

		vec2 delta1(pUVs[pFaces[i].y] - pUVs[pFaces[i].x]);
		vec2 delta2(pUVs[pFaces[i].z] - pUVs[pFaces[i].x]);

		float f = 1.f / (delta1.x * delta2.y - delta2.x * delta1.y);

		vec3 tangent;

		tangent.x = f * ((edge1.x * delta2.y) + (edge2.x * -delta1.y));
		tangent.y = f * ((edge1.y * delta2.y) + (edge2.y * -delta1.y));
		tangent.z = f * ((edge1.z * delta2.y) + (edge2.z * -delta1.y));

		tangents[pFaces[i].x] += tangent;
		tangents[pFaces[i].y] += tangent;
		tangents[pFaces[i].z] += tangent;
	}

	return tangents;
}

void Mesh::updateBoundingBox(vec3 v)
{
	m_boundingBox.position.x = min(m_boundingBox.position.x, v.x);
	m_boundingBox.position.y = min(m_boundingBox.position.y, v.y);
	m_boundingBox.position.z = min(m_boundingBox.position.z, v.z);

	m_boundingBox.size.x = max(m_boundingBox.size.x, v.x);
	m_boundingBox.size.y = max(m_boundingBox.size.y, v.y);
	m_boundingBox.size.z = max(m_boundingBox.size.z, v.z);
}
