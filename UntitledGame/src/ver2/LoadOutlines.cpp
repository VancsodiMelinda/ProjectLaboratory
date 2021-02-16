#include "LoadOutlines.h"

LoadOutlines::LoadOutlines(LoadAssets& assets_, ProgramContainer programContainer_) :
	assets(assets_)
{
	InstrumentationTimer timer("Load outlines");

	programContainer = programContainer_;
	checkProgram(programContainer);
}

void LoadOutlines::checkProgram(ProgramContainer programContainer_)
{
	if (programContainer_.type == ProgramType::outline)
		std::cout << "OK: Your are using the correct program for the ourline." << std::endl;
	else
		std::cout << "ERROR: Your are using the incorrect program for the ourline." << std::endl;
}

void LoadOutlines::configOutline(ObjectContainer& object)
{
	//	configure vertex attributes
	glBindVertexArray(object.vao);
	glBindBuffer(GL_ARRAY_BUFFER, object.vbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, object.ibo);

	GLuint positionAttribIndex = glGetAttribLocation(programContainer.ID, "in_vertexPosition");

	glEnableVertexAttribArray(positionAttribIndex);

	GLintptr vOffset = 0 * sizeof(GL_FLOAT);
	int stride3f = 3 * sizeof(GL_FLOAT);
	glVertexAttribPointer(positionAttribIndex, 3, GL_FLOAT, GL_FALSE, stride3f, (GLvoid*)vOffset);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	// get uniform locations
	uniformLocations.MVPloc = glGetUniformLocation(programContainer.ID, "MVP");
}

void LoadOutlines::renderOutline(ObjectContainer& object, Kamera& kamera)
{
	glUseProgram(programContainer.ID);

	// update MVP
	float scale = 1.1f;
	glm::mat4 scaledModelMatrix = glm::scale(object.modelMatrix, glm::vec3(scale, scale, scale));
	glm::mat4 MVP = kamera.cameraContainer.projectionMatrix * kamera.cameraContainer.viewMatrix * scaledModelMatrix;

	// upload uniforms
	glUniformMatrix4fv(uniformLocations.MVPloc, 1, GL_FALSE, glm::value_ptr(MVP));

	// draw
	glBindVertexArray(object.vao);
	glDrawElements(GL_TRIANGLES, object.data.indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

void LoadOutlines::config()
{
	InstrumentationTimer timer("Config outlines");

	for (int i = 0; i < NUMBER_OF_OBJECTS; i++)
	{
		configOutline(assets.models[i]);
	}
}

void LoadOutlines::render(Kamera& kamera)
{
	InstrumentationTimer timer("Render outlines");


	for (int i = 0; i < NUMBER_OF_OBJECTS; i++)
	{
		renderOutline(assets.models[i], kamera);
	}
}
