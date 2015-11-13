#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Mesh.h"

using namespace glm;

class Entity
{
public:
	Entity(Mesh* pMesh, mat4 pModelMatrix);
	~Entity();

	mat4 getModelMatrix() const;
	AABB getBoundingBox();
	AABB getRawBoundingBox() { return m_mesh->getRawBoundingBox(); };
	Mesh* getMesh();

	//--  Mutators -- 
		string	getName() { return m_name; }
		void	setName(string pName) { m_name = pName; }
		vec3	getCoordinates() { return m_coordinates; }
		void	setCoordinates(vec3 pCoordinates) { m_coordinates = pCoordinates; }
		vec3	getScale() { return m_scale; }
		void	setScale(vec3 pScale) { m_scale = pScale; }
	// --\ Mutators --

private:
	Mesh* m_mesh;
	mat4 m_modelMatrix;
	string m_name;
	vec3	m_coordinates;
	vec3	m_scale;
};

