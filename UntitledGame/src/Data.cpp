#include "Data.h"

Data::Data(std::string objectFileName_)
{
	objectFileName = objectFileName_;
}


void Data::initialize()
{
	loadObjectData();		// data
	createVAOandVBOs();		// vao, vbo, ibo
	fillVBOs();				// vbo, ibo
}

void Data::loadObjectData()
{
	ObjLoader objLoader;
	data = objLoader.advancedObjLoader(objectFileName);
}

void Data::createVAOandVBOs()
{
	glGenVertexArrays(1, &vao);  // create VAO
	glBindVertexArray(vao);  // bind VAO

	glGenBuffers(1, &vbo);  // create VBO
	glGenBuffers(1, &ibo);  // create IBO
}

void Data::fillVBOs()
{
	glBindBuffer(GL_ARRAY_BUFFER, vbo);  // bind VBO
	glBufferData(GL_ARRAY_BUFFER, (data.vertices.size() + data.uvs.size() + data.normals.size()) * sizeof(GL_FLOAT), 0, GL_STATIC_DRAW);						// reserve space
	glBufferSubData(GL_ARRAY_BUFFER, 0, data.vertices.size() * sizeof(GL_FLOAT), &data.vertices[0]);															// VERTEX COORDINATES
	glBufferSubData(GL_ARRAY_BUFFER, data.vertices.size() * sizeof(GL_FLOAT), data.uvs.size() * sizeof(GL_FLOAT), &data.uvs[0]);								// TEXTURE COORDINATES
	glBufferSubData(GL_ARRAY_BUFFER, (data.vertices.size() + data.uvs.size()) * sizeof(GL_FLOAT), data.normals.size() * sizeof(GL_FLOAT), &data.normals[0]);	// NORMAL COORDINATES
	glBindBuffer(GL_ARRAY_BUFFER, 0);  // unbind VBO

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);  // bind IBO
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, data.indices.size() * sizeof(GLuint), &data.indices[0], GL_STATIC_DRAW);  // INDICES
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);  // unbind IBO

	glBindVertexArray(0);  // unbind VAO
}
