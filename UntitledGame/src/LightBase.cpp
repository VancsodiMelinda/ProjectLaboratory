#include "LightBase.h"



LightBase::LightBase(Data& object_, GLuint shaderID_, Camera& camera_):
	object(object_),
	camera(camera_)
{
	std::cout << "LightBase ctor" << std::endl;

	shaderID = shaderID_;
	modelMatrix = glm::mat4(0.0f);
	MVP = glm::mat4(0.0f);
	lightUniforms = { 0 };
}

glm::mat4 LightBase::calculateLightSpaceMatrix(glm::mat4 actModelMatrix)  // virtual
{
	return glm::mat4();
}

void LightBase::initialize()
{
	std::cout << "LightBase::initialize" << std::endl;
	configVertexAttributes();		// only once
	getLightUniformLocations();		// only once
}

void LightBase::configVertexAttributes()
{
	std::cout << "LightBase::configVertexAttributes" << std::endl;

	glBindVertexArray(object.vao);  // bind VAO
	glBindBuffer(GL_ARRAY_BUFFER, object.vbo);  // bind VBO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, object.ibo);  // bind IBO

	// get attribute indexes from the relevant shader
	GLuint positionAttribIndex = glGetAttribLocation(shaderID, "in_vertexPosition");	// layout (location = 0) in vec3 in_vertexPosition;

	// enable vertex attribute array
	glEnableVertexAttribArray(positionAttribIndex);

	// calculate offsets
	GLintptr vOffset = 0 * sizeof(GL_FLOAT);
	int stride3f = 3 * sizeof(GL_FLOAT);

	// setup the pointers
	glVertexAttribPointer(positionAttribIndex, 3, GL_FLOAT, GL_FALSE, stride3f, (GLvoid*)vOffset);

	glBindVertexArray(0);  // unbind VAO
	glBindBuffer(GL_ARRAY_BUFFER, 0);  // unbind VBO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);  // unbind IBO
}


void LightBase::getLightUniformLocations()
{
	std::cout << "LightBase::getLightUniformLocations" << std::endl;

	int MVPloc = glGetUniformLocation(shaderID, "MVP");
	int colorLoc = glGetUniformLocation(shaderID, "lightColor");

	lightUniforms.MVPloc = MVPloc;
	lightUniforms.colorLoc = colorLoc;
}

void LightBase::render(Camera& camera_)
{
	//std::cout << "LightBase::render" << std::endl;

	camera = camera_;
	glUseProgram(shaderID);
	updateMVP();
	uploadLightUniforms();
	glBindVertexArray(object.vao);
	glDrawElements(GL_TRIANGLES, object.data.indices.size(), GL_UNSIGNED_INT, 0);
}

void LightBase::updateMVP(){}  // virtual

void LightBase::uploadLightUniforms(){}  // virtual

glm::mat4 LightBase::createModelMatrix(glm::vec3 translate, glm::vec3 scale, float rotateAngle, std::string rotateAxis)
{
	//std::cout << "LightBase::createModelMatrix" << std::endl;

	glm::mat4 modelMatrix_ = glm::mat4(1.0f);

	modelMatrix_ = glm::translate(modelMatrix_, translate);

	modelMatrix_ = glm::scale(modelMatrix_, scale);

	if ((rotateAxis == "x") || (rotateAxis == "X"))
	{
		modelMatrix_ = glm::rotate(modelMatrix_, glm::radians(rotateAngle), glm::vec3(1.0f, 0.0f, 0.0f));
	}
	else if ((rotateAxis == "y") || (rotateAxis == "Y"))
	{
		modelMatrix_ = glm::rotate(modelMatrix_, glm::radians(rotateAngle), glm::vec3(0.0f, 1.0f, 0.0f));
	}
	else if ((rotateAxis == "z") || (rotateAxis == "Z"))
	{
		modelMatrix_ = glm::rotate(modelMatrix_, glm::radians(rotateAngle), glm::vec3(0.0f, 0.0f, 1.0f));
	}
	else
	{
		std::cout << "The rotation angle is not correct!" << std::endl;
	}

	return modelMatrix_;
}

void LightBase::getObjectUniformLocations(GLuint objectShader){}  // virtual

void LightBase::uploadObjectUniforms(){}  // virtual

void LightBase::changeParams(){}  // virtual
