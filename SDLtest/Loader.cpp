#include "Loader.h"

Loader::Loader(Scene* pScene)
{
	m_scene = pScene;
}

void Loader::load() {

	loadShaders	("Shaders.txt");
	loadMeshes	("MeshLoader.txt");
	
	m_scene->init();

	loadMap("Map.txt");
}

void Loader::loadTextures(string & pFilePath) {

}

void Loader::loadMap(string pFilePath) {
	ifstream file(pFilePath.c_str());
	string line, id;
	string entityLabel;
	string entityName;
	string placement = "", refObjectName = "";
	vec3 entityScale, entityCoordinates, entityOffset, rotationVector;
	float rotationAngle = 0.0f;
	Entity *entity;

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
		if (id == "ROT_A") {
			data >> rotationAngle;
		}
		if (id == "ROT_V") {
			data >> rotationVector.x >> rotationVector.y >> rotationVector.z;
		}
		if (id == "END") {

			if (!placement.empty()) {
				// Then we have a relative position for the entity.
				placeObject(entityCoordinates, entityLabel, placement, refObjectName, entityOffset);
			}
			if (rotationAngle != 0.0f) { // Object is rotated
				entity = m_scene->addEntityAndRetrieve(entityLabel, rotate(translate(scale(mat4(), entityScale), entityCoordinates), rotationAngle, rotationVector));
			}
			else {
				entity = m_scene->addEntityAndRetrieve(entityLabel, translate(scale(mat4(), entityScale), entityCoordinates));
			}
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
			refObjectBox = entities->at(i)->getRawBoundingBox();
			refObjectScale = (entities->at(i)->getScale() - 1);
			break;
		}
	}

	if (refObjectPos.null) return;

	naturalOffset = vec3   (1 / (refObjectPos.x + 1), 
							1 / (refObjectPos.y + 1),
							1 / (refObjectPos.z + 1)
							);

	coordinates = refObjectPos;

	/**
	We add "half pos" to have the correct new position
	*/
	if (placement == "BEFORE") { // y+
		if (coordinates.y == 0) {
			coordinates.y *= (naturalOffset.y * refObjectBox.size.y);
		}
		else {
			coordinates.y += refObjectBox.size.y * 2;
		}
		coordinates.y += ((1 / sum(0.5, refObjectPos.y))   * refObjectBox.size.y);
	}
	else if (placement == "BEHIND") { // y-
		if (coordinates.y == 0) {
			coordinates.y *= (naturalOffset.y * refObjectBox.size.y);
		}
		else {
			coordinates.y += refObjectBox.size.y * 2;
		}
		coordinates.y -= ((1 / sum(0.5, refObjectPos.y))   * refObjectBox.size.y);
	}
	else if (placement == "LEFT") { // x-
		if (coordinates.x == 0) {
			coordinates.x *= (naturalOffset.x * refObjectBox.size.x);
		}
		else {
			coordinates.x += refObjectBox.size.x * 2;
		}
		coordinates.x -= ((1 / sum(0.5, refObjectPos.x))   *refObjectBox.size.x) ;
	}
	else if (placement == "RIGHT") { // x+
		if (coordinates.x == 0) {
			coordinates.x *= (naturalOffset.x * refObjectBox.size.x);
		}
		else {
			coordinates.x += refObjectBox.size.x * 2;
		}
		coordinates.x += ((1 / sum(0.5, refObjectPos.x))   * refObjectBox.size.x) ;
	}
	else if (placement == "UNDER") { // z-
		if (coordinates.z == 0) {
			coordinates.z *= (naturalOffset.z * refObjectBox.size.z);
		}
		else {
			coordinates.z += refObjectBox.size.z * 2;
		}
		coordinates.z -= ((1 / sum(0.5, refObjectPos.z))   * refObjectBox.size.z);
	}
	else if (placement == "ABOVE") { // z+
		if (coordinates.z == 0) {
			coordinates.z *= (naturalOffset.z * refObjectBox.size.z);
		}
		else {
			coordinates.z += refObjectBox.size.z * 2;
		}
		coordinates.z += ( (1 / sum(0.5, refObjectPos.z)) * refObjectBox.size.z );
	}

	coordinates.x = (int)coordinates.x;
	coordinates.y = (int)coordinates.y;
	coordinates.z = (int)coordinates.z;
	coordinates += offset;

}

double Loader::sum(double val, double n) {
	double ret = val;
	for (int i(1); i <= n; ++i) {
		ret += val;
	}
	return ret;
}