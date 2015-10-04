#include "VAO.h"



VAO::VAO() :
	m_facesCount(0)
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

void VAO::store(Prop attribute, const GLvoid * data, int verticesLength, GLint perVertex, GLenum type, GLenum usage, GLenum target,  GLboolean normalized)
{
	bind();

	// Compute buffer size
	GLsizeiptr size = verticesLength * perVertex * sizeOf(type);

	// Delete any previous VBO
	if (m_vbos.count(attribute)) {
		delete m_vbos[attribute];
	}

	// Create a VBO to store the data in
	m_vbos[attribute] = new VBO(target);
	m_vbos[attribute]->setData(data, size, usage);

	// Define attribute exept for vertices indices wich are not attributes (GL_ELEMENT_ARRAY_BUFFER)
	if (target == GL_ARRAY_BUFFER) {
		defineAttribute(attribute, perVertex, type, normalized);
	}

	// Unbind everything
	m_vbos[attribute]->unbind();
	unbind();
}

void VAO::storeVertices(std::vector<glm::vec3> vertices)
{
	store(VERTICES, &vertices[0].x, vertices.size(), 3, GL_FLOAT);
}

void VAO::storeFaces(std::vector<glm::uvec3> faces)
{
	m_facesCount = faces.size();
	store(FACES, &faces[0].x, faces.size(), 3, GL_UNSIGNED_INT, GL_STATIC_DRAW, GL_ELEMENT_ARRAY_BUFFER);
}

void VAO::bind(bool enableAttributes)
{
	glBindVertexArray(m_id);

	if (enableAttributes) {

		// Bind VBO wich contains vertices indices
		if (m_vbos.count(FACES)) {
			m_vbos[FACES]->bind();
		}

		// Enable each attribute
		for (std::map<Prop, VBO*>::iterator it = m_vbos.begin(); it != m_vbos.end(); ++it) {
			if (it->first != FACES) {
				enableAttribute(it->first);
			}
		}
	}
}

void VAO::unbind(bool disableAttributes)
{
	if (disableAttributes) {

		// Disable each attribute
		for (std::map<Prop, VBO*>::iterator it = m_vbos.begin(); it != m_vbos.end(); ++it) {
			if (it->first != FACES) {
				disableAttribute(it->first);
			}
		}

		// Unind VBO wich contains vertices indices
		if (m_vbos.count(FACES)) {
			m_vbos[FACES]->bind();
		}
	}

	glBindVertexArray(0);
}

void VAO::defineAttribute(Prop attribute, GLint perVertex, GLenum type, GLboolean normalized)
{
	glVertexAttribPointer(attribute, perVertex, type, normalized, 0, 0);
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
