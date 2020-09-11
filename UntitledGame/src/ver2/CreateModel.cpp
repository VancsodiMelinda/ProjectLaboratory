#include "CreateModel.h"

// load default model
CreateModel::CreateModel()
{
	objectContainer.name = "resources/models/default model";
	loadObjectData(objectContainer.name);		// data
	createVAOandVBOs();		// vao, vbo, ibo
	fillVBOs();				// vbo, ibo
	objectContainer.modelMatrix = glm::mat4();	// identity matrix
}


CreateModel::CreateModel(std::string objectFileName)
{
	objectContainer.name = objectFileName;
	loadObjectData(objectFileName);		// data
	createVAOandVBOs();		// vao, vbo, ibo
	fillVBOs();				// vbo, ibo
	objectContainer.modelMatrix = glm::mat4();	// identity matrix
}

CreateModel::CreateModel(std::string objectFileName, glm::vec3 translate, glm::vec3 scale, float rotate, std::string rotateAxis)
{
	objectContainer.name = objectFileName;
	loadObjectData(objectFileName);		// data
	createVAOandVBOs();		// vao, vbo, ibo
	fillVBOs();				// vbo, ibo
	objectContainer.modelMatrix = createModelMatrix(translate, scale, rotate, rotateAxis);
}

void CreateModel::loadObjectData(std::string objectFileName)
{
	ObjReader objectLoader;
	objectContainer.data = objectLoader.readObjFile(objectFileName);
}

void CreateModel::createVAOandVBOs()
{
	glGenVertexArrays(1, &objectContainer.vao);  // create VAO
	glBindVertexArray(objectContainer.vao);  // bind VAO

	glGenBuffers(1, &objectContainer.vbo);  // create VBO
	glGenBuffers(1, &objectContainer.ibo);  // create IBO
}

void CreateModel::fillVBOs()
{
	glBindBuffer(GL_ARRAY_BUFFER, objectContainer.vbo);  // bind VBO
	glBufferData(GL_ARRAY_BUFFER, (objectContainer.data.vertices.size() + objectContainer.data.uvs.size() + objectContainer.data.normals.size()) * sizeof(GL_FLOAT), 0, GL_STATIC_DRAW);						// reserve space
	glBufferSubData(GL_ARRAY_BUFFER, 0, objectContainer.data.vertices.size() * sizeof(GL_FLOAT), &objectContainer.data.vertices[0]);															// VERTEX COORDINATES
	glBufferSubData(GL_ARRAY_BUFFER, objectContainer.data.vertices.size() * sizeof(GL_FLOAT), objectContainer.data.uvs.size() * sizeof(GL_FLOAT), &objectContainer.data.uvs[0]);								// TEXTURE COORDINATES
	glBufferSubData(GL_ARRAY_BUFFER, (objectContainer.data.vertices.size() + objectContainer.data.uvs.size()) * sizeof(GL_FLOAT), objectContainer.data.normals.size() * sizeof(GL_FLOAT), &objectContainer.data.normals[0]);	// NORMAL COORDINATES
	glBindBuffer(GL_ARRAY_BUFFER, 0);  // unbind VBO

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, objectContainer.ibo);  // bind IBO
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, objectContainer.data.indices.size() * sizeof(GLuint), &objectContainer.data.indices[0], GL_STATIC_DRAW);  // INDICES
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);  // unbind IBO

	glBindVertexArray(0);  // unbind VAO
}

glm::mat4 CreateModel::createModelMatrix(glm::vec3 translate, glm::vec3 scale, float rotateAngle, std::string rotateAxis)
{
	glm::mat4 modelMatrix = glm::mat4(1.0f);

	modelMatrix = glm::translate(modelMatrix, translate);

	modelMatrix = glm::scale(modelMatrix, scale);

	if ((rotateAxis == "x") || (rotateAxis == "X"))
	{
		modelMatrix = glm::rotate(modelMatrix, glm::radians(rotateAngle), glm::vec3(1.0f, 0.0f, 0.0f));
	}
	else if ((rotateAxis == "y") || (rotateAxis == "Y"))
	{
		modelMatrix = glm::rotate(modelMatrix, glm::radians(rotateAngle), glm::vec3(0.0f, 1.0f, 0.0f));
	}
	else if ((rotateAxis == "z") || (rotateAxis == "Z"))
	{
		modelMatrix = glm::rotate(modelMatrix, glm::radians(rotateAngle), glm::vec3(0.0f, 0.0f, 1.0f));
	}
	else
	{
		std::cout << "The rotation angle is not correct!" << std::endl;
	}

	return modelMatrix;
}
