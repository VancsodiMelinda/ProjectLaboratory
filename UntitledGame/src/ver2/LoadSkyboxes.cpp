#include "LoadSkyboxes.h"

LoadSkyboxes::LoadSkyboxes()
{
	InstrumentationTimer timer("Load skyboxes");

	std::cout << "Loading skyboxes..." << std::endl;

	loadSkyboxModel();

	CreateSkybox sky1;
	skyboxes[0] = sky1.skyboxContainer;
	std::cout << std::endl;

	CreateSkybox sky2("space", "png");
	skyboxes[1] = sky2.skyboxContainer;
	std::cout << std::endl;
}

void LoadSkyboxes::loadSkyboxModel()
{
	CreateModel cube("resources/models/cube.obj");
	model = cube.objectContainer;
}

void LoadSkyboxes::config(ProgramContainer programContainer)
{
	InstrumentationTimer timer("Config skyboxes");

	if (programContainer.type == ProgramType::skybox)
	{
		std::cout << "OK: Skybox is configured using the correct program." << std::endl;
	}
	else
	{
		std::cout << "ERROR: You are using the incorrect program for configuring the skybox!" << std::endl;
	}

	//// CONFIGURE VERTEX ATTRIBUTES
	glBindVertexArray(model.vao);
	glBindBuffer(GL_ARRAY_BUFFER, model.vbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, model.ibo);

	GLuint positionAttribIndex = glGetAttribLocation(programContainer.ID, "vertexPosition");

	glEnableVertexAttribArray(positionAttribIndex);

	int stride3f = 3 * sizeof(GL_FLOAT);
	glVertexAttribPointer(positionAttribIndex, 3, GL_FLOAT, GL_FALSE, stride3f, 0);

	glBindVertexArray(0);

	//// GET UNIFORM LOACTIONS
	uniformLocations.MVPloc = glGetUniformLocation(programContainer.ID, "MVP");
	uniformLocations.skyboxLoc = glGetUniformLocation(programContainer.ID, "skybox");
}

void LoadSkyboxes::render(ProgramContainer programContainer, Kamera& kamera)
{
	InstrumentationTimer timer("Render skybox");


	glUseProgram(programContainer.ID);

	glm::mat4 MVP = kamera.cameraContainer.projectionMatrix * glm::mat4(glm::mat3(kamera.cameraContainer.viewMatrix));

	glUniformMatrix4fv(uniformLocations.MVPloc, 1, GL_FALSE, glm::value_ptr(MVP));
	glUniform1i(uniformLocations.skyboxLoc, 0);

	glBindVertexArray(model.vao);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, skyboxes[1].ID);

	glDrawElements(GL_TRIANGLES, model.data.indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}
