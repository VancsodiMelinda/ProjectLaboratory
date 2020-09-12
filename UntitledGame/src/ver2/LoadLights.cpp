#include "LoadLights.h"

LoadLights::LoadLights()
{
	std::cout << "Loading directional lights..." << std::endl;
	loadDirectionalLights();
	std::cout << std::endl;
}

void LoadLights::loadDirectionalLights()
{
	CreateDirLight light1;	// creates default directional light
	dirLights[0] = light1.dirLightContainer;

	CreateDirLight light2;
	light2.dirLightContainer.position = glm::vec3(-5.0f, 3.0f, 0.0f);
	light2.dirLightContainer.color = glm::vec3(1.0f, 1.0f, 1.0f);
	light2.dirLightContainer.ambientStrength = 0.3f;
	light2.dirLightContainer.diffuseStrength = 0.8f;
	light2.dirLightContainer.specularStrength = 1.0f;
	light2.dirLightContainer.lightSpaceMatrix = light2.createLightSpaceMatrix();
	dirLights[1] = light2.dirLightContainer;
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

void LoadLights::renderLight(DirLightContainer& dirLight, ObjectContainer& object, GLuint programID, Kamera& kamera)
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
}

void LoadLights::config(ObjectContainer& object, ProgramContainer programContainer)
{
	if (programContainer.type == ProgramType::light)
	{
		configLight(object, programContainer.ID);
		std::cout << "OK: Lights are configured using the correct program." << std::endl;
	}
	else
	{
		std::cout << "ERROR: You are using the incorrect program for configuring the lights!" << std::endl;
	}

	//for (int i = 0; i < sizeof(dirLights) / sizeof(dirLights[0]); i++)
	//{
		//configLight(object, programID);
	//}

	//for (int i = 0; i < sizeof(pointLights) / sizeof(pointLights[0]); i++) {}

	//for (int i = 0; i < sizeof(spotLights) / sizeof(spotLights[0]); i++) {}
}

void LoadLights::render(ObjectContainer& object, ProgramContainer programContainer, Kamera& kamera)
{
	for (int i = 0; i < sizeof(dirLights) / sizeof(dirLights[0]); i++)
	{
		renderLight(dirLights[i], object, programContainer.ID, kamera);
	}
}

