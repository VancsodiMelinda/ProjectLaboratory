#include "LoadLights.h"

LoadLights::LoadLights()
{
	std::cout << "Loading light models..." << std::endl;
	loadLightModels();
	std::cout << std::endl;

	std::cout << "Loading directional lights..." << std::endl;
	loadDirectionalLights();
	std::cout << std::endl;

	std::cout << "Loading point lights..." << std::endl;
	loadPointLights();
	std::cout << std::endl;
}

void LoadLights::loadDirectionalLights()
{
	CreateDirLight light1;	// creates default directional light
	light1.dirLightContainer.position = glm::vec3(0.0f, 3.0f, -5.0f);
	light1.dirLightContainer.color = glm::vec3(1.0f, 1.0f, 1.0f);
	light1.dirLightContainer.ambientStrength = 0.2f;
	light1.dirLightContainer.diffuseStrength = 0.8f;
	light1.dirLightContainer.specularStrength = 1.0f;
	light1.dirLightContainer.lightSpaceMatrix = light1.createLightSpaceMatrix();
	dirLights[0] = light1.dirLightContainer;

	CreateDirLight light2;
	light2.dirLightContainer.position = glm::vec3(-5.0f, 3.0f, 0.0f);
	light2.dirLightContainer.color = glm::vec3(1.0f, 0.0f, 0.0f);
	light2.dirLightContainer.ambientStrength = 0.0f;
	light2.dirLightContainer.diffuseStrength = 0.8f;
	light2.dirLightContainer.specularStrength = 1.0f;
	light2.dirLightContainer.lightSpaceMatrix = light2.createLightSpaceMatrix();
	dirLights[1] = light2.dirLightContainer;

	CreateDirLight light3;
	light3.dirLightContainer.position = glm::vec3(0.0f, 3.0f, 5.0f);
	light3.dirLightContainer.color = glm::vec3(0.0f, 1.0f, 0.0f);
	light3.dirLightContainer.ambientStrength = 0.0f;
	light3.dirLightContainer.diffuseStrength = 0.8f;
	light3.dirLightContainer.specularStrength = 1.0f;
	light3.dirLightContainer.lightSpaceMatrix = light3.createLightSpaceMatrix();
	dirLights[2] = light3.dirLightContainer;
	
	CreateDirLight light4;
	light4.dirLightContainer.position = glm::vec3(5.0f, 3.0f, 0.0f);
	light4.dirLightContainer.color = glm::vec3(0.0f, 0.0f, 1.0f);
	light4.dirLightContainer.ambientStrength = 0.0f;
	light4.dirLightContainer.diffuseStrength = 0.8f;
	light4.dirLightContainer.specularStrength = 1.0f;
	light4.dirLightContainer.lightSpaceMatrix = light4.createLightSpaceMatrix();
	dirLights[3] = light4.dirLightContainer;
}

void LoadLights::loadPointLights()
{
	CreatePointLight light1;
	pointLights[0] = light1.pointLightContainer;

	CreatePointLight light2;
	light2.pointLightContainer.position = glm::vec3(3.0f, 3.0f, 3.0f);
	light2.pointLightContainer.color = glm::vec3(0.0f, 1.0f, 1.0f);
	light2.pointLightContainer.ambientStrength = 0.3f;
	light2.pointLightContainer.diffuseStrength = 0.9f;
	light2.pointLightContainer.specularStrength = 0.8f;
	light2.pointLightContainer.constant = 1.0f;
	light2.pointLightContainer.linear = 0.045f;
	light2.pointLightContainer.quadratic = 0.0075f;
	light2.pointLightContainer.lightSpaceMatrix = light2.createLightSpaceMatrix();
	pointLights[1] = light2.pointLightContainer;
}

void LoadLights::loadLightModels()
{
	CreateModel cube("resources/models/cube.obj");
	models[0] = cube.objectContainer;

	CreateModel sphere("resources/models/sphere.obj");
	models[1] = sphere.objectContainer;
}

void LoadLights::configLight(ObjectContainer& object, GLuint programID)
{
	//// CONFIGURE VERTEX ATTRIBUTES
	glBindVertexArray(object.vao);  // bind VAO
	glBindBuffer(GL_ARRAY_BUFFER, object.vbo);  // bind VBO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, object.ibo);  // bind IBO

	// get attribute indexes from the relevant shader
	GLuint positionAttribIndex = glGetAttribLocation(programID, "in_vertexPosition");	// layout (location = 0) in vec3 in_vertexPosition;

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

	//// GET UNIFORM LOACTIONS
	uniformLocations.MVPloc = glGetUniformLocation(programID, "MVP");;
	uniformLocations.colorLoc = glGetUniformLocation(programID, "lightColor");
}

void LoadLights::renderDirLight(DirLightContainer& dirLight, ObjectContainer& object, GLuint programID, Kamera& kamera)
{
	glUseProgram(programID);

	// create/update MVP
	glm::mat4 modelMatrix = glm::mat4(1.0);
	modelMatrix = glm::translate(modelMatrix, dirLight.position);
	modelMatrix = glm::scale(modelMatrix, glm::vec3(0.3f, 0.3f, 0.3f));
	glm::mat4 MVP = kamera.cameraContainer.projectionMatrix * kamera.cameraContainer.viewMatrix * modelMatrix;

	// upload uniforms
	glUniformMatrix4fv(uniformLocations.MVPloc, 1, GL_FALSE, glm::value_ptr(MVP));
	glUniform3fv(uniformLocations.colorLoc, 1, glm::value_ptr(dirLight.color));

	// draw
	glBindVertexArray(object.vao);
	glDrawElements(GL_TRIANGLES, object.data.indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

void LoadLights::renderPointLight(PointLightContainer& pointLight, ObjectContainer& object, GLuint programID, Kamera& kamera)
{
	glUseProgram(programID);

	// create/update MVP
	glm::mat4 modelMatrix = glm::mat4(1.0);
	modelMatrix = glm::translate(modelMatrix, pointLight.position);
	modelMatrix = glm::scale(modelMatrix, glm::vec3(0.3f, 0.3f, 0.3f));
	glm::mat4 MVP = kamera.cameraContainer.projectionMatrix * kamera.cameraContainer.viewMatrix * modelMatrix;

	// upload uniforms
	glUniformMatrix4fv(uniformLocations.MVPloc, 1, GL_FALSE, glm::value_ptr(MVP));
	glUniform3fv(uniformLocations.colorLoc, 1, glm::value_ptr(pointLight.color));

	// draw
	glBindVertexArray(object.vao);
	glDrawElements(GL_TRIANGLES, object.data.indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

void LoadLights::config(ProgramContainer programContainer)
{
	if (programContainer.type == ProgramType::light)
	{
		//configLight(dirLightObject, programContainer.ID);
		//configLight(pointLightObject, programContainer.ID);
		configLight(models[0], programContainer.ID);
		configLight(models[1], programContainer.ID);
		std::cout << "OK: Lights are configured using the correct program." << std::endl;
	}
	else
	{
		std::cout << "ERROR: You are using the incorrect program for configuring the lights!" << std::endl;
	}
}

void LoadLights::render(ProgramContainer programContainer, Kamera& kamera)
{
	// render directional lights
	
	for (int i = 0; i < NUMBER_OF_DIR_LIGHTS; i++)
	{
		//renderDirLight(dirLights[i], dirLightObject, programContainer.ID, kamera);
		renderDirLight(dirLights[i], models[0], programContainer.ID, kamera);
	}
	
	// render point lights
	for (int i = 0; i < NUMBER_OF_POINT_LIGHTS; i++)
	{
		//renderPointLight(pointLights[i], pointLightObject, programContainer.ID, kamera);
		renderPointLight(pointLights[i], models[1], programContainer.ID, kamera);
	}
}

