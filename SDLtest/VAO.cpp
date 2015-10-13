#include "VAO.h"



VAO::VAO() :
	m_facesCount(0), m_verticesCount(0)
{
	glGenVertexArrays(1, &m_id);
}


VAO::~VAO()
{
	// Free all VBOs
	for (map<Prop, VBO*>::iterator it = m_vbos.begin(); it != m_vbos.end(); ++it) {
		delete m_vbos[it->first];
	}

	glDeleteVertexArrays(1, &m_id);
}

GLuint VAO::getId()
{
	return m_id;
}

GLuint VAO::getFacesCount()
{
	return m_facesCount;
}

void VAO::store(Prop pAttribute, const GLvoid * pData, int pVerticesLength, GLint pPerVertex, GLenum pType, GLenum pUsage, GLenum pTarget)
{
	bind();

	// Delete any previous VBO
	if (m_vbos.count(pAttribute)) {
		delete m_vbos[pAttribute];
	}

	// Create VBO and store data
	m_vbos[pAttribute] = new VBO(pTarget);
	m_vbos[pAttribute]->setData(pData, pVerticesLength * pPerVertex * sizeOf(pType), pUsage);

	// Define attributes
	if (pTarget == GL_ARRAY_BUFFER) {
		defineAttribute(pAttribute, pPerVertex, pType);
		enableAttribute(pAttribute);

		m_vbos[pAttribute]->unbind();

		if (m_vbos.count(FACES)) {
			m_vbos[FACES]->bind();
		}
	}

	unbind();
}

void VAO::storeVertices(vector<vec3> pVertices, GLenum pUsage)
{
	m_verticesCount = pVertices.size();
	store(VERTICES, &pVertices[0].x, pVertices.size(), 3, GL_FLOAT, pUsage);
}

void VAO::storeColors(vector<vec3> pColors, GLenum pUsage)
{
	store(COLORS, &pColors[0].x, pColors.size(), 3, GL_FLOAT, pUsage);
}

void VAO::storeNormals(vector<vec3> pNormals, GLenum pUsage)
{
	store(NORMALS, &pNormals[0].x, pNormals.size(), 3, GL_FLOAT, pUsage);
}

void VAO::storeTangents(vector<vec3> pTangents, GLenum pUsage)
{
	store(TANGENTS, &pTangents[0].x, pTangents.size(), 3, GL_FLOAT, pUsage);
}

void VAO::storeUvs(vector<vec2> pUVs, GLenum pUsage)
{
	store(UV, &pUVs[0].x, pUVs.size(), 2, GL_FLOAT, pUsage);
}

void VAO::storeFaces(vector<uvec3> pFaces, GLenum pUsage)
{
	m_facesCount = pFaces.size();
	store(FACES, &pFaces[0].x, pFaces.size(), 3, GL_UNSIGNED_INT, pUsage, GL_ELEMENT_ARRAY_BUFFER);
}

void VAO::bind()
{
	glBindVertexArray(m_id);
}

void VAO::unbind()
{
	glBindVertexArray(0);
}

void VAO::drawTriangles()
{
	glDrawElements(GL_TRIANGLES, m_facesCount * 3, GL_UNSIGNED_INT, (void*)0);
}

void VAO::drawLines()
{
	bind();
	glDrawArrays(GL_LINES, 0, m_verticesCount);
	unbind();
}

void VAO::defineAttribute(Prop pAttribute, GLint pPerVertex, GLenum pType)
{
	glVertexAttribPointer(pAttribute, pPerVertex, pType, GL_FALSE, 0, 0);
}

void VAO::enableAttribute(Prop pAttribute)
{
	glEnableVertexAttribArray(pAttribute);
}

void VAO::disableAttribute(Prop pAttribute)
{
	glDisableVertexAttribArray(pAttribute);
}

size_t VAO::sizeOf(GLenum pType)
{
	if (pType == GL_FLOAT) {
		return sizeof(GLfloat);
	}
	if (pType == GL_UNSIGNED_INT) {
		return sizeof(GLuint);
	}
}
