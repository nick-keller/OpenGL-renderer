#include "Entity.h"


Entity::Entity(Mesh* pMesh, mat4 pModelMatrix) :
	m_mesh( pMesh ), m_modelMatrix(pModelMatrix)
{
}


Entity::~Entity()
{
}

mat4 Entity::getModelMatrix() const
{
	return m_modelMatrix;
}

AABB Entity::getBoundingBox()
{
	AABB meshBB = m_mesh->getRawBoundingBox();

	AABB boundingBox = {
		vec3(FLT_MAX, FLT_MAX, FLT_MAX),
		vec3(FLT_MIN, FLT_MIN, FLT_MIN)
	};

	vec3 vertices[] = {
		vec3(meshBB.position.x, meshBB.position.y, meshBB.position.z),
		vec3(meshBB.position.x, meshBB.position.y, meshBB.size.z),
		vec3(meshBB.position.x, meshBB.size.y,     meshBB.position.z),
		vec3(meshBB.position.x, meshBB.size.y,     meshBB.size.z),
		vec3(meshBB.size.x,     meshBB.position.y, meshBB.position.z),
		vec3(meshBB.size.x,     meshBB.position.y, meshBB.size.z),
		vec3(meshBB.size.x,     meshBB.size.y,     meshBB.position.z),
		vec3(meshBB.size.x,     meshBB.size.y,     meshBB.size.z)
	};

	for (int i(0); i < 8; ++i) {
		vec3 v(m_modelMatrix * vec4(vertices[i].x, vertices[i].y, vertices[i].z, 1));

		boundingBox.position.x = min(boundingBox.position.x, v.x);
		boundingBox.position.y = min(boundingBox.position.y, v.y);
		boundingBox.position.z = min(boundingBox.position.z, v.z);

		boundingBox.size.x = max(boundingBox.size.x, v.x);
		boundingBox.size.y = max(boundingBox.size.y, v.y);
		boundingBox.size.z = max(boundingBox.size.z, v.z);
	}

	return boundingBox;
}
