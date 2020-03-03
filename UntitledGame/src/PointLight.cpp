#include "PointLight.h"

/*
PointLight::PointLight(PointLightStruct params_, Data& object_, GLuint shaderID_, glm::mat4 modelMatrix_, Camera& camera_)
	: object(object_),
	camera(camera_)
{
	params = params_;

	shaderID = shaderID_;  // light shader
	modelMatrix = modelMatrix_;

	MVP = glm::mat4(0.0f);
	objectUniforms = { 0, 0, 0, 0, 0 };  // uniform variable locations for standard shader
	lightUniforms = { 0, 0 };  // uniform variable locations for light shader
}



void PointLight::initialize()
{
	configVertexAttributes();	// only once
	getLightUniformLocations();			// only once
}

void PointLight::configVertexAttributes()
{
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
}

void PointLight::getLightUniformLocations()
{
	int MVPloc = glGetUniformLocation(shaderID, "MVP");
	int colorLoc = glGetUniformLocation(shaderID, "lightColor");

	lightUniforms.MVPloc = MVPloc;
	lightUniforms.colorLoc = colorLoc;
}

///////////////////////////////////////////////////////////////////////////////////////////////

void PointLight::render(Camera& camera_)
{
	camera = camera_;
	glUseProgram(shaderID);
	updateMVP();
	uploadLightUniforms();
	glBindVertexArray(object.vao);
	glDrawElements(GL_TRIANGLES, object.data.indices.size(), GL_UNSIGNED_INT, 0);
}

void PointLight::updateMVP()
{
	glm::mat4 viewMatrix = camera.CreateViewMatrix();  // update in every frame (WASD and mouse)
	glm::mat4 projectionMatrix = glm::perspective(glm::radians(camera.fov), (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT, 0.1f, 100.0f);  // update in every frame (zoom)

	MVP = projectionMatrix * viewMatrix * modelMatrix;
}

void PointLight::uploadLightUniforms()
{
	glUniformMatrix4fv(lightUniforms.MVPloc, 1, GL_FALSE, glm::value_ptr(MVP));
	glUniform3fv(lightUniforms.colorLoc, 1, glm::value_ptr(color));
}


////////////////////////////////////////////////////////////////////////////////////////////////////

void PointLight::getObjectUniformLocations(GLuint objectShader)
{
	objectUniforms.positionLoc = glGetUniformLocation(objectShader, "pointLight.position");
	//objectUniforms.colorLoc = colorLoc;
	objectUniforms.ambientStrengthLoc = glGetUniformLocation(objectShader, "pointLight.ambientStrength");
	objectUniforms.diffuseStrengthLoc = glGetUniformLocation(objectShader, "pointLight.diffuseStrength");
	objectUniforms.specularStrengthLoc = glGetUniformLocation(objectShader, "pointLight.specularStrength");
	objectUniforms.constantLoc = glGetUniformLocation(objectShader, "pointLight.constant");
	objectUniforms.linearLoc = glGetUniformLocation(objectShader, "pointLight.linear");
	objectUniforms.quadraticLoc = glGetUniformLocation(objectShader, "pointLight.quadratic");
}

void PointLight::uploadObjectUniforms()
{
	glUniform3fv(objectUniforms.positionLoc, 1, glm::value_ptr(params.position));
	//glUniform3fv(objectUniforms.colorLoc, 1, glm::value_ptr(color));

	glUniform1f(objectUniforms.ambientStrengthLoc, params.ambientStrength);
	glUniform1f(objectUniforms.diffuseStrengthLoc, params.diffuseStrength);
	glUniform1f(objectUniforms.specularStrengthLoc, params.specularStrength);
}

*/