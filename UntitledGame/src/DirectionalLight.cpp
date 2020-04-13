#include "DirectionalLight.h"


/*
DirectionalLight::DirectionalLight(DirLightParams params_, Data& object_, GLuint shaderID_, Camera& camera_)
	: object(object_),
	camera(camera_)
{
	params = params_;

	shaderID = shaderID_;		// light shader
	modelMatrix = glm::mat4(0.0f);

	MVP = glm::mat4(0.0f);
	objectUniforms = { 0 };		// uniform variable locations for object shader
	lightUniforms = { 0 };		// uniform variable locations for light shader
}

glm::mat4 DirectionalLight::calculateLightSpaceMatrix(glm::mat4 actModelMatrix)
{
	glm::mat4 lightViewMatrix = glm::lookAt(params.position, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

	float nearPlane = 0.1f;
	float farPlane = 20.0f;

	glm::mat4 lightProjMatrix = glm::ortho(-4.0f, 4.0f, -3.0f, 3.0f, nearPlane, farPlane);

	// create MVP
	glm::mat4 lightSpaceMatrix = lightProjMatrix * lightViewMatrix * actModelMatrix;

	return lightSpaceMatrix;
}


void DirectionalLight::initialize()
{
	configVertexAttributes();		// only once
	getLightUniformLocations();		// only once
}


void DirectionalLight::configVertexAttributes()
{
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

void DirectionalLight::getLightUniformLocations()
{
	int MVPloc = glGetUniformLocation(shaderID, "MVP");
	int colorLoc = glGetUniformLocation(shaderID, "lightColor");

	lightUniforms.MVPloc = MVPloc;
	lightUniforms.colorLoc = colorLoc;
}

///////////////////////////////////////////////////////////////////////////////////////////////

void DirectionalLight::render(Camera& camera_)  // render light source object
{
	camera = camera_;
	glUseProgram(shaderID);
	updateMVP();
	uploadLightUniforms();
	glBindVertexArray(object.vao);
	glDrawElements(GL_TRIANGLES, object.data.indices.size(), GL_UNSIGNED_INT, 0);
}

void DirectionalLight::updateMVP()
{
	glm::mat4 viewMatrix = camera.CreateViewMatrix();  // update in every frame (WASD and mouse)
	//glm::mat4 projectionMatrix = glm::perspective(glm::radians(camera.fov), (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT, 0.1f, 100.0f);  // update in every frame (zoom)
	glm::mat4 projectionMatrix = glm::perspective(glm::radians(camera.fov), (float)windowWidth / (float)windowHeight, 0.1f, 100.0f);  // update in every frame (zoom)
	modelMatrix = glm::mat4(1.0f);
	//modelMatrix = glm::translate(modelMatrix, params.position);
	modelMatrix = createModelMatrix(params.position, params.scale, params.angle, params.axes);

	MVP = projectionMatrix * viewMatrix * modelMatrix;
}

void DirectionalLight::uploadLightUniforms()
{
	glUniformMatrix4fv(lightUniforms.MVPloc, 1, GL_FALSE, glm::value_ptr(MVP));
	glUniform3fv(lightUniforms.colorLoc, 1, glm::value_ptr(params.color));
}


glm::mat4 DirectionalLight::createModelMatrix(glm::vec3 translate, glm::vec3 scale, float rotateAngle, std::string rotateAxis)
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

////////////////////////////////////////////////////////////////////////////////////////////////////

void DirectionalLight::getObjectUniformLocations(GLuint objectShader)
{
	objectUniforms.positionLoc = glGetUniformLocation(objectShader, "dirLight.position");
	objectUniforms.colorLoc = glGetUniformLocation(objectShader, "dirLight.color");

	objectUniforms.ambientStrengthLoc = glGetUniformLocation(objectShader, "dirLight.ambientStrength");
	objectUniforms.diffuseStrengthLoc = glGetUniformLocation(objectShader, "dirLight.diffuseStrength");
	objectUniforms.specularStrengthLoc = glGetUniformLocation(objectShader, "dirLight.specularStrength");
}

void DirectionalLight::uploadObjectUniforms()
{
	glUniform3fv(objectUniforms.positionLoc, 1, glm::value_ptr(params.position));
	glUniform3fv(objectUniforms.colorLoc, 1, glm::value_ptr(params.color));

	glUniform1f(objectUniforms.ambientStrengthLoc, params.ambientStrength);
	glUniform1f(objectUniforms.diffuseStrengthLoc, params.diffuseStrength);
	glUniform1f(objectUniforms.specularStrengthLoc, params.specularStrength);
}

void DirectionalLight::changeParams()
{
	ImGui::Begin("Point light params");
	ImGui::Text("Press 'P' to let go of mouse.");

	ImGui::SliderFloat("ambientStrength", &params.ambientStrength, 0.0f, 1.0f);
	ImGui::SliderFloat("diffuseStrength", &params.diffuseStrength, 0.0f, 1.0f);
	ImGui::SliderFloat("specularStrength", &params.specularStrength, 0.0f, 1.0f);

	ImGui::SliderFloat("x position", &params.position.x, -5.0f, 5.0f);

	ImGui::End();
}

*/


DirectionalLight::DirectionalLight(DirLightParams params_, Data& object_, GLuint shaderID_, Camera& camera_) : LightBase(object_, shaderID_, camera_)
{
	std::cout << "DirectionalLight ctor" << std::endl;

	params = params_;
}



glm::mat4 DirectionalLight::calculateLightSpaceMatrix(glm::mat4 actModelMatrix)
{
	//std::cout << "DirectionalLight::calculateLightSpaceMatrix()" << std::endl;

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


void DirectionalLight::updateMVP()
{
	//std::cout << "DirectionalLight::updateMVP()" << std::endl;

	glm::mat4 viewMatrix = LightBase::camera.CreateViewMatrix();  // update in every frame (WASD and mouse)
	//glm::mat4 projectionMatrix = glm::perspective(glm::radians(camera.fov), (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT, 0.1f, 100.0f);  // update in every frame (zoom)
	glm::mat4 projectionMatrix = glm::perspective(glm::radians(LightBase::camera.fov), (float)windowWidth / (float)windowHeight, 0.1f, 100.0f);  // update in every frame (zoom)
	modelMatrix = glm::mat4(1.0f);
	//modelMatrix = glm::translate(modelMatrix, params.position);
	modelMatrix = createModelMatrix(params.position, params.scale, params.angle, params.axes);

	LightBase::MVP = projectionMatrix * viewMatrix * modelMatrix;
}

void DirectionalLight::uploadLightUniforms()
{
	//std::cout << "DirectionalLight::uploadLightUniforms()" << std::endl;

	glUniformMatrix4fv(lightUniforms.MVPloc, 1, GL_FALSE, glm::value_ptr(MVP));
	glUniform3fv(lightUniforms.colorLoc, 1, glm::value_ptr(params.color));
}

void DirectionalLight::getObjectUniformLocations(GLuint objectShader)
{
	//std::cout << "DirectionalLight::getObjectUniformLocations()" << std::endl;

	objectUniforms.positionLoc = glGetUniformLocation(objectShader, "dirLight.direction");
	objectUniforms.colorLoc = glGetUniformLocation(objectShader, "dirLight.color");

	objectUniforms.ambientStrengthLoc = glGetUniformLocation(objectShader, "dirLight.ambientStrength");
	objectUniforms.diffuseStrengthLoc = glGetUniformLocation(objectShader, "dirLight.diffuseStrength");
	objectUniforms.specularStrengthLoc = glGetUniformLocation(objectShader, "dirLight.specularStrength");
}

void DirectionalLight::uploadObjectUniforms()
{
	//std::cout << "DirectionalLight::uploadObjectUniforms()" << std::endl;
	glm::vec3 direction = glm::vec3(0.0f) - params.position;

	glUniform3fv(objectUniforms.positionLoc, 1, glm::value_ptr(direction));
	glUniform3fv(objectUniforms.colorLoc, 1, glm::value_ptr(params.color));

	glUniform1f(objectUniforms.ambientStrengthLoc, params.ambientStrength);
	glUniform1f(objectUniforms.diffuseStrengthLoc, params.diffuseStrength);
	glUniform1f(objectUniforms.specularStrengthLoc, params.specularStrength);
}

void DirectionalLight::changeParams()
{
	//std::cout << "DirectionalLight::changeParams()" << std::endl;

	ImGui::Begin("Directional light params");
	ImGui::Text("Press 'P' to let go of mouse.");

	ImGui::SliderFloat("ambientStrength", &params.ambientStrength, 0.0f, 1.0f);
	ImGui::SliderFloat("diffuseStrength", &params.diffuseStrength, 0.0f, 1.0f);
	ImGui::SliderFloat("specularStrength", &params.specularStrength, 0.0f, 1.0f);

	ImGui::SliderFloat("x position", &params.position.x, -5.0f, 5.0f);
	ImGui::SliderFloat("y position", &params.position.y, -5.0f, 5.0f);
	ImGui::SliderFloat("z position", &params.position.z, -5.0f, 5.0f);

	ImGui::End();
}