#include "VAO.h"



VAO::VAO() :
	m_facesCount(0), m_verticesCount(0)
{
	glGenVertexArrays(1, &m_id);
}


VAO::~VAO()
{
	// Free all VBOs
	for (std::map<Prop, VBO*>::iterator it = m_vbos.begin(); it != m_vbos.end(); ++it) {
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

void VAO::store(Prop attribute, const GLvoid * data, int verticesLength, GLint perVertex, GLenum type, GLenum usage, GLenum target)
{
	bind();

	// Delete any previous VBO
	if (m_vbos.count(attribute)) {
		delete m_vbos[attribute];
	}

	// Create VBO and store data
	m_vbos[attribute] = new VBO(target);
	m_vbos[attribute]->setData(data, verticesLength * perVertex * sizeOf(type), usage);

	// Define attributes
	if (target == GL_ARRAY_BUFFER) {
		defineAttribute(attribute, perVertex, type);
		enableAttribute(attribute);

		m_vbos[attribute]->unbind();

		if (m_vbos.count(FACES)) {
			m_vbos[FACES]->bind();
		}
	}

	unbind();
}

void VAO::storeVertices(std::vector<glm::vec3> vertices, GLenum usage)
{
	m_verticesCount = vertices.size();
	store(VERTICES, &vertices[0].x, vertices.size(), 3, GL_FLOAT, usage);
}

void VAO::storeColors(std::vector<glm::vec3> colors, GLenum usage)
{
	store(COLORS, &colors[0].x, colors.size(), 3, GL_FLOAT, usage);
}

void VAO::storeNormals(std::vector<glm::vec3> normals, GLenum usage)
{
	store(NORMALS, &normals[0].x, normals.size(), 3, GL_FLOAT, usage);
}

void VAO::storeTangents(std::vector<glm::vec3> tangents, GLenum usage)
{
	store(TANGENTS, &tangents[0].x, tangents.size(), 3, GL_FLOAT, usage);
}

void VAO::storeUvs(std::vector<glm::vec2> uvs, GLenum usage)
{
	store(UV, &uvs[0].x, uvs.size(), 2, GL_FLOAT, usage);
}

void VAO::storeFaces(std::vector<glm::uvec3> faces, GLenum usage)
{
	m_facesCount = faces.size();
	store(FACES, &faces[0].x, faces.size(), 3, GL_UNSIGNED_INT, usage, GL_ELEMENT_ARRAY_BUFFER);
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

void VAO::defineAttribute(Prop attribute, GLint perVertex, GLenum type)
{
	glVertexAttribPointer(attribute, perVertex, type, GL_FALSE, 0, 0);
}

void VAO::enableAttribute(Prop attribute)
{
	glEnableVertexAttribArray(attribute);
}

void VAO::disableAttribute(Prop attribute)
{
	glDisableVertexAttribArray(attribute);
}

std::size_t VAO::sizeOf(GLenum type)
{
	if (type == GL_FLOAT) {
		return sizeof(GLfloat);
	}
	if (type == GL_UNSIGNED_INT) {
		return sizeof(GLuint);
	}
}
