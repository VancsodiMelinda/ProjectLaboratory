#include "LoadSkyboxes.h"

LoadSkyboxes::LoadSkyboxes()
{
	std::cout << "Loading skyboxes..." << std::endl;

	CreateSkybox sky1;
	skyboxes[0] = sky1.skyboxContainer;

	std::cout << std::endl;

	CreateSkybox sky2("interstellar", "png");
	skyboxes[1] = sky2.skyboxContainer;

	std::cout << std::endl;
}

void LoadSkyboxes::config(ObjectContainer& object, ProgramContainer programContainer)
{
	if (programContainer.type == ProgramType::skybox)
	{
		std::cout << "OK: Skybox is configured using the correct program." << std::endl;
	}
	else
	{
		std::cout << "ERROR: You are using the incorrect program for configuring the skybox!" << std::endl;
	}

	//// CONFIGURE VERTEX ATTRIBUTES
	glBindVertexArray(object.vao);
	glBindBuffer(GL_ARRAY_BUFFER, object.vbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, object.ibo);

	GLuint positionAttribIndex = glGetAttribLocation(programContainer.ID, "vertexPosition");

	glEnableVertexAttribArray(positionAttribIndex);

	int stride3f = 3 * sizeof(GL_FLOAT);
	glVertexAttribPointer(positionAttribIndex, 3, GL_FLOAT, GL_FALSE, stride3f, 0);

	glBindVertexArray(0);

	//// GET UNIFORM LOACTIONS
	uniformLocations.MVPloc = glGetUniformLocation(programContainer.ID, "MVP");
	uniformLocations.skyboxLoc = glGetUniformLocation(programContainer.ID, "skybox");
}

void LoadSkyboxes::render(ObjectContainer& object, ProgramContainer programContainer, Kamera& kamera)
{
	glUseProgram(programContainer.ID);

	glm::mat4 MVP = kamera.cameraContainer.projectionMatrix * glm::mat4(glm::mat3(kamera.cameraContainer.viewMatrix));

	glUniformMatrix4fv(uniformLocations.MVPloc, 1, GL_FALSE, glm::value_ptr(MVP));
	glUniform1i(uniformLocations.skyboxLoc, 0);

	glBindVertexArray(object.vao);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, skyboxes[1].ID);

	glDrawElements(GL_TRIANGLES, object.data.indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}