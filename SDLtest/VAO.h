#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>
#include <map>
#include "VBO.h"

class VAO
{
public:

	enum Prop {VERTICES, NORMALS, UV, COLORS, FACES};

	VAO();
	~VAO();

	GLuint getId();
	GLuint getFacesCount();

	void storeVertices(std::vector<glm::vec3> vertices, GLenum usage = GL_STATIC_DRAW);
	void storeColors(std::vector<glm::vec3> colors, GLenum usage = GL_STATIC_DRAW);
	void storeFaces(std::vector<glm::uvec3> faces, GLenum usage = GL_STATIC_DRAW);

	void bind();
	void unbind();

	void drawTriangles();
	void drawLines();

	void defineAttribute(Prop attribute, GLint perVertex, GLenum type);
	void enableAttribute(Prop attribute);
	void disableAttribute(Prop attribute);

private:
	void store(Prop attribute, const GLvoid * data, int verticesLength, GLint perVertex, GLenum type, GLenum usage = GL_STATIC_DRAW, GLenum target = GL_ARRAY_BUFFER);
	std::size_t sizeOf(GLenum type);

private:
	GLuint m_id;
	GLuint m_verticesCount;
	GLuint m_facesCount;
	std::map<Prop, VBO*> m_vbos;
};

