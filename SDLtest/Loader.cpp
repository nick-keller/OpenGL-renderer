#include "Loader.h"

Loader::Loader(Scene* pScene)
{
	m_scene = pScene;
}

void Loader::load() {

	loadShaders	("Shaders.txt");
	loadMeshes	("MeshLoader.txt");
	loadMap		("Map.txt");
	m_scene->init();
}

void Loader::loadTextures(string & pFilePath) {

}

void Loader::loadMap(string pFilePath) {
	ifstream file(pFilePath.c_str());
	string line, id;
	string entityLabel;
	string entityName;
	string placement = "", refObjectName = "";
	vec3 entityScale, entityCoordinates, entityOffset;

	while (getline(file, line)) {
		if (line.empty()) {
			continue;
		}
		stringstream data(line);

		data >> id;

		if (id == "LABEL") {
			data >> entityLabel;
		}
		if (id == "NAME") {
			data >> entityName;
		}
		if (id == "SCALE") {
			data >> entityScale.x >> entityScale.y >> entityScale.z;
		}
		if (id == "COOR") {
			data >> entityCoordinates.x >> entityCoordinates.y >> entityCoordinates.z;
		}
		if (id == "OFFSET") {
			data >> entityOffset.x >> entityOffset.y >> entityOffset.z;
		}
		if (id == "PLACE") {
			data >> placement >> refObjectName;
		}
		if (id == "END") {

			if (!placement.empty()) {
				// Then we have a relative position for the entity.
				placeObject(entityCoordinates, entityLabel, placement, refObjectName, entityOffset);
			}
			auto entity =m_scene->addEntityAndRetrieve(entityLabel, translate(scale(mat4(), entityScale), entityCoordinates));
			entity->setName(entityName);
			entity->setScale(entityScale);
			entity->setCoordinates(entityCoordinates);
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


void Loader::placeObject(vec3& coordinates,string label, string placement, string refObject, vec3& offset) {

	auto entities = m_scene->getEntities();
	vec3 refObjectPos, refObjectScale;
	AABB refObjectBox;
	vec3 naturalOffset;
	for (int i(0); i < entities->size(); ++i) {
		if (entities->at(i)->getName() == refObject) {
			refObjectPos = entities->at(i)->getCoordinates();
			refObjectBox = entities->at(i)->getBoundingBox();
			refObjectScale = (entities->at(i)->getScale() - 1);
			break;
		}
	}

	if (refObjectPos.null) return;

	naturalOffset = vec3(1/ (refObjectPos.x+1), 
							1/ (refObjectPos.y +1),
							1 / (refObjectPos.z+1));

	coordinates	   = refObjectPos;
	coordinates.x *= (naturalOffset.x * refObjectBox.size.x);
	coordinates.y *= (naturalOffset.y * refObjectBox.size.y);
	coordinates.z *= (naturalOffset.z * refObjectBox.size.z);


	/**
	We add "half cubes" to have the correct new position
	*/
	if (placement == "BEFORE") { // z+
		coordinates.y += ((1 / sum(0.5, refObjectPos.y))   * refObjectBox.size.y);
	}
	else if (placement == "BEHIND") { // z-
		coordinates.y -= ((1 / sum(0.5, refObjectPos.y))   * refObjectBox.size.y);
	}
	else if (placement == "LEFT") { // x-
		coordinates.x -= ((1 / sum(0.5, refObjectPos.x))   *refObjectBox.size.x) ;
	}
	else if (placement == "RIGHT") { // x+
		coordinates.x += ((1 / sum(0.5, refObjectPos.x))   * refObjectBox.size.x) ;
	}
	else if (placement == "UNDER") { // y-
		coordinates.z -= ((1 / sum(0.5, refObjectPos.z))   * refObjectBox.size.z);
	}
	else if (placement == "ABOVE") { // y+
		coordinates.z += ( (1 / sum(0.5, refObjectPos.z)) * refObjectBox.size.z );
	}

	//offset -= 0.5;

	coordinates += offset;
}

double Loader::sum(double val, double n) {
	double ret = val;
	for (int i(1); i <= n; ++i) {
		ret += val;
	}
	return ret;
}