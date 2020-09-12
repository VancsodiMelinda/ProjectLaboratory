#include "LoadShadows.h"

LoadShadows::LoadShadows(LoadLights& lights_, LoadAssets& assets_, LoadPrograms& programs_) :
	lights(lights_),
	assets(assets_),
	programs(programs_)
{
	// get programs
	dirShadowProgramContainer = programs.programs[2];

	std::cout << "Load directional shadows..." << std::endl;
	int numberOfDirShadows = sizeof(lights.dirLights) / sizeof(lights.dirLights[0]);
	loadDirShadows(numberOfDirShadows);
}

void LoadShadows::loadDirShadows(int numberOfDirShadows)
{
	for (int i = 0; i < numberOfDirShadows; i++)
	{
		std::cout << "OK: Created directional shadow." << std::endl;
		CreateDirShadow shadow;
		dirShadows[i] = shadow.dirShadowContainer;
	}
	std::cout << std::endl;
}

void LoadShadows::configDirShadow(ObjectContainer& object, GLuint programID)
{
	//// CONFIGURE VERTEX ATTRIBUTES
	glBindVertexArray(object.vao);
	glBindBuffer(GL_ARRAY_BUFFER, object.vbo);

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

	//// GET UNIFORM LOACTIONS
	uniformLocations.MVPloc = glGetUniformLocation(programID, "MVP");
}

void LoadShadows::renderDirShadow(DirLightContainer dirLight, ObjectContainer& object, GLuint programID)
{
	glUseProgram(programID);
	
	glm::mat4 MVP = dirLight.lightSpaceMatrix * object.modelMatrix;
	glUniformMatrix4fv(uniformLocations.MVPloc, 1, GL_FALSE, glm::value_ptr(MVP));

	glBindVertexArray(object.vao);
	glDrawElements(GL_TRIANGLES, object.data.indices.size(), GL_UNSIGNED_INT, 0);
}


void LoadShadows::config()
{
	if (dirShadowProgramContainer.type == ProgramType::directionalShadow)
	{
		for (int i = 0; i < sizeof(assets.models) / sizeof(assets.models[0]); i++)
		{
			configDirShadow(assets.models[i], dirShadowProgramContainer.ID);
		}
		std::cout << "OK: Directional shadows are configured using the correct program." << std::endl;
	}
	else
	{
		std::cout << "ERROR: You are using the incorrect program for configuring directional shadows!" << std::endl;
	}
}

void LoadShadows::render()
{
	glViewport(0, 0, DIR_SHADOW_WIDTH, DIR_SHADOW_HEIGHT);

	for (int i = 0; i < sizeof(lights.dirLights) / sizeof(lights.dirLights[0]); i++)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, dirShadows[i].fbo);
		glClear(GL_DEPTH_BUFFER_BIT);
		glEnable(GL_DEPTH_TEST);

		for (int j = 0; j < sizeof(assets.models) / sizeof(assets.models[0]); j++)
		{
			renderDirShadow(lights.dirLights[i], assets.models[j], dirShadowProgramContainer.ID);
		}

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
}
