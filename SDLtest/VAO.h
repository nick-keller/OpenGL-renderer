#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>
#include <map>
#include "VBO.h"

using namespace std;
using namespace glm;

class VAO
{
public:

	enum Prop {VERTICES, NORMALS, UV, COLORS, TANGENTS, FACES};

	VAO();
	~VAO();

	GLuint getId();
	GLuint getFacesCount();

	void storeVertices(vector<vec3> pVertices, GLenum pUsage = GL_STATIC_DRAW);
	void storeColors(vector<vec3> pColors, GLenum pUsage = GL_STATIC_DRAW);
	void storeNormals(vector<vec3> pNormals, GLenum pUsage = GL_STATIC_DRAW);
	void storeTangents(vector<vec3> pTangents, GLenum pUsage = GL_STATIC_DRAW);
	void storeUvs(vector<vec2> pUVs, GLenum pUsage = GL_STATIC_DRAW);
	void storeFaces(vector<uvec3> pFaces, GLenum pUsage = GL_STATIC_DRAW);

	void bind();
	void unbind();

	void drawTriangles();
	void drawLines();

	void defineAttribute(Prop pAttribute, GLint pPerVertex, GLenum pType);
	void enableAttribute(Prop pAttribute);
	void disableAttribute(Prop pAttribute);

private:
	void store(Prop pAttribute, const GLvoid * pData, int pVerticesLength, GLint pPerVertex, GLenum pType, GLenum pUsage = GL_STATIC_DRAW, GLenum pTarget = GL_ARRAY_BUFFER);
	size_t sizeOf(GLenum pType);

private:
	GLuint m_id;
	GLuint m_verticesCount;
	GLuint m_facesCount;
	map<Prop, VBO*> m_vbos;
};

