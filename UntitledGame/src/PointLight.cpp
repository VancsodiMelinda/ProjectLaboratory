#include "PointLight.h"


PointLight::PointLight(PointLightParams params_, Data& object_, GLuint shaderID_, glm::mat4 modelMatrix_, Camera& camera_)
	: object(object_),
	camera(camera_)
{
	params = params_;

	shaderID = shaderID_;		// light shader
	modelMatrix = modelMatrix_;

	MVP = glm::mat4(0.0f);
	objectUniforms = { 0 };		// uniform variable locations for object shader
	lightUniforms = { 0 };		// uniform variable locations for light shader
}

glm::mat4 PointLight::calculateLightSpaceMatrix(glm::mat4 actModelMatrix)
{
	//lookAt(eye, center, up)
	// eye = position of lightsource
	// center = the point where the lightsource "looks", should be the scene center
	// up = "camera up"
	glm::mat4 lightViewMatrix = glm::lookAt(params.position, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

	float nearPlane = 0.1f;
	float farPlane = 20.0f;

	glm::mat4 lightProjMatrix = glm::ortho(-4.0f, 4.0f, -3.0f, 3.0f, nearPlane, farPlane);

	// create MVP
	glm::mat4 lightSpaceMatrix = lightProjMatrix * lightViewMatrix * actModelMatrix;

	return lightSpaceMatrix;
}


void PointLight::initialize()
{
	configVertexAttributes();		// only once
	getLightUniformLocations();		// only once
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

void PointLight::render(Camera& camera_)  // render light source object
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
	//glm::mat4 projectionMatrix = glm::perspective(glm::radians(camera.fov), (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT, 0.1f, 100.0f);  // update in every frame (zoom)
	glm::mat4 projectionMatrix = glm::perspective(glm::radians(camera.fov), (float)windowWidth / (float)windowHeight, 0.1f, 100.0f);  // update in every frame (zoom)

	MVP = projectionMatrix * viewMatrix * modelMatrix;
}

void PointLight::uploadLightUniforms()
{
	glUniformMatrix4fv(lightUniforms.MVPloc, 1, GL_FALSE, glm::value_ptr(MVP));
	glUniform3fv(lightUniforms.colorLoc, 1, glm::value_ptr(params.color));
}


////////////////////////////////////////////////////////////////////////////////////////////////////

void PointLight::getObjectUniformLocations(GLuint objectShader)
{
	objectUniforms.positionLoc = glGetUniformLocation(objectShader, "pointLight.position");
	objectUniforms.colorLoc = glGetUniformLocation(objectShader, "pointLight.color");

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
	glUniform3fv(objectUniforms.colorLoc, 1, glm::value_ptr(params.color));

	glUniform1f(objectUniforms.ambientStrengthLoc, params.ambientStrength);
	glUniform1f(objectUniforms.diffuseStrengthLoc, params.diffuseStrength);
	glUniform1f(objectUniforms.specularStrengthLoc, params.specularStrength);

	glUniform1f(objectUniforms.constantLoc, params.constant);
	glUniform1f(objectUniforms.linearLoc, params.linear);
	glUniform1f(objectUniforms.quadraticLoc, params.quadratic);
}

