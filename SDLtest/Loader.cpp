#include "Loader.h"

Loader::Loader(Scene* pScene)
{
	m_scene = pScene;
}

void Loader::load() {
	//loadShaders("Shaders.txt");
	loadMeshes("MeshLoader.txt");
	loadMap("Map.txt");
}

void Loader::loadTextures(string & pFilePath) {

}

void Loader::loadMap(string pFilePath) {
	ifstream file(pFilePath.c_str());
	string line, id;
	string entityLabel;
	vec3 entityScale, entityCoordinates;

	while (getline(file, line)) {
		if (line.empty()) {
			continue;
		}
		stringstream data(line);

		data >> id;

		if (id == "LABEL") {
			data >> entityLabel;
		}
		if (id == "SCALE") {
			data >> entityScale.x >> entityScale.y >> entityScale.z;
		}
		if (id == "COOR") {
			data >> entityCoordinates.x >> entityCoordinates.y >> entityCoordinates.z;
		}
		if (id == "END") {
			m_scene->addEntity(entityLabel, translate(scale(mat4(), entityScale), entityCoordinates));
		}

	}
	file.close();
}

void Loader::loadMeshes(string const& pFilePath) {

	ifstream file(pFilePath.c_str());
	string line, id;
	string meshLabel, meshFile, shaderType, texture;

	while (getline(file, line)) {
		if (line.empty()) {
			continue;
		}
		stringstream data(line);

		data >> id;

		if (id == "LABEL") {
			data >> meshLabel;
		}
		if (id == "FILE") {
			data >> meshFile;
		}
		if (id == "SHADER") {
			data >> shaderType;
		}
		if (id == "TEXTURE") {
			data >> texture;
		}
		if (id == "END") {
			m_scene->addMesh(meshLabel, meshFile, shaderType, texture);

		}

	}
	file.close();

}

void Loader::loadShaders(string pFilePath) {

	ifstream file(pFilePath.c_str());
	string line, id;
	string shaderLabel;

	while (getline(file, line)) {
		if (line.empty()) {
			continue;
		}
		stringstream data(line);

		data >> id;

		if (id == "LABEL") {
			data >> shaderLabel;
			m_scene->loadShader(shaderLabel);
		}
	}
	file.close();
}
Loader::~Loader(void)
{
}
