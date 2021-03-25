#include "LoadShadows.h"

/*
LoadShadows::LoadShadows(LoadLights& lights_, LoadAssets& assets_, LoadPrograms& programs_) :
	lights(lights_),
	assets(assets_),
	programs(programs_)
{
	InstrumentationTimer timer("Load shadows");

	// get programs
	dirShadowProgramContainer = programs.programs[2];
	pointShadowProgramContainer = programs.programs[3];

	loadDirShadows();
	loadPointShadows();
}
*/

LoadShadows::LoadShadows(LoadLights& lights_, const std::vector<ObjectContainer>& models_, LoadPrograms& programs_) :
	lights(lights_),
	models(models_),
	programs(programs_)
{
	//InstrumentationTimer timer("Load shadows");

	// get programs
	dirShadowProgramContainer = programs.programs[2];
	pointShadowProgramContainer = programs.programs[3];

	loadDirShadows();
	loadPointShadows();
}

void LoadShadows::loadDirShadows()
{
	//InstrumentationTimer timer("Load dir shadows");

	std::cout << "Load directional shadows..." << std::endl;
	for (int i = 0; i < lights.dirLights_.size(); i++)
	{
		std::cout << "OK: Created directional shadow." << std::endl;
		CreateDirShadow shadow;
		//dirShadows[i] = shadow.dirShadowContainer;
		dirShadows_.push_back(shadow.dirShadowContainer);
	}
	std::cout << std::endl;
}

void LoadShadows::loadPointShadows()
{
	//InstrumentationTimer timer("Load point shadows");

	std::cout << "Load point shadows..." << std::endl;
	for (int i = 0; i < lights.pointLights_.size(); i++)
	{
		std::cout << "OK: Created point shadow." << std::endl;
		CreatePointShadow shadow;
		//pointShadows[i] = shadow.pointShadowContainer;
		pointShadows_.push_back(shadow.pointShadowContainer);
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

void LoadShadows::configPointShadow(ObjectContainer& object, GLuint programID)
{
	glBindVertexArray(object.vao);
	glBindBuffer(GL_ARRAY_BUFFER, object.vbo);

	GLuint positionAttribIndex = glGetAttribLocation(programID, "vertexPos");
	glEnableVertexAttribArray(positionAttribIndex);

	GLintptr vOffset = 0 * sizeof(GL_FLOAT);
	int stride3f = 3 * sizeof(GL_FLOAT);
	glVertexAttribPointer(positionAttribIndex, 3, GL_FLOAT, GL_FALSE, stride3f, (GLvoid*)vOffset);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);


	uniformLocations_.modelMatrixLoc = glGetUniformLocation(programID, "modelMatrix");
	uniformLocations_.VPsLoc = glGetUniformLocation(programID, "VPs");
	uniformLocations_.lightPosLoc = glGetUniformLocation(programID, "lightPos");
	uniformLocations_.farPlaneLoc = glGetUniformLocation(programID, "farPlane");
}

void LoadShadows::renderPointShadow(PointLightContainer light, ObjectContainer& object, GLuint programID)
{
	glUseProgram(programID);

	std::vector<glm::mat4> VPs;

	// create projection matrix
	float aspect = (float)POINT_SHADOW_WIDTH_ / (float)POINT_SHADOW_HEIGHT_;
	float near = 1.0f;
	float farPlane = 100.0f;
	glm::mat4 projectionMatrix = glm::perspective(glm::radians(90.0f), aspect, near, farPlane);


	// create view matrices
	glm::mat4 viewMatrix1 = glm::lookAt(light.position, light.position + glm::vec3(1.0, 0.0, 0.0), glm::vec3(0.0, -1.0, 0.0));	// right
	glm::mat4 viewMatrix2 = glm::lookAt(light.position, light.position + glm::vec3(-1.0, 0.0, 0.0), glm::vec3(0.0, -1.0, 0.0));	// left
	glm::mat4 viewMatrix3 = glm::lookAt(light.position, light.position + glm::vec3(0.0, 1.0, 0.0), glm::vec3(0.0, 0.0, 1.0));		// top
	glm::mat4 viewMatrix4 = glm::lookAt(light.position, light.position + glm::vec3(0.0, -1.0, 0.0), glm::vec3(0.0, 0.0, -1.0));	// bottom
	glm::mat4 viewMatrix5 = glm::lookAt(light.position, light.position + glm::vec3(0.0, 0.0, 1.0), glm::vec3(0.0, -1.0, 0.0));	// near
	glm::mat4 viewMatrix6 = glm::lookAt(light.position, light.position + glm::vec3(0.0, 0.0, -1.0), glm::vec3(0.0, -1.0, 0.0));	// far

	// create MVPs
	//std::vector<glm::mat4> VPs;
	VPs.push_back(projectionMatrix * viewMatrix1);
	VPs.push_back(projectionMatrix * viewMatrix2);
	VPs.push_back(projectionMatrix * viewMatrix3);
	VPs.push_back(projectionMatrix * viewMatrix4);
	VPs.push_back(projectionMatrix * viewMatrix5);
	VPs.push_back(projectionMatrix * viewMatrix6);

	glUniformMatrix4fv(uniformLocations_.modelMatrixLoc, 1, GL_FALSE, glm::value_ptr(object.modelMatrix));
	glUniformMatrix4fv(uniformLocations_.VPsLoc, 6, GL_FALSE, glm::value_ptr(VPs[0]));
	glUniform3fv(uniformLocations_.lightPosLoc, 1, glm::value_ptr(light.position));
	glUniform1f(uniformLocations_.farPlaneLoc, farPlane);

	glBindVertexArray(object.vao);
	glDrawElements(GL_TRIANGLES, object.data.indices.size(), GL_UNSIGNED_INT, 0);
}


void LoadShadows::config()
{
	//InstrumentationTimer timer("Config shadows");

	if (dirShadowProgramContainer.type == ProgramType::directionalShadow)
	{
		//InstrumentationTimer timer("Config dir shadows");

		for (int i = 0; i < models.size(); i++)  // new
		{
			configDirShadow(models[i], dirShadowProgramContainer.ID);
		}
		std::cout << "OK: Directional shadows are configured using the correct program." << std::endl;
	}
	else
	{
		std::cout << "ERROR: You are using the incorrect program for configuring directional shadows!" << std::endl;
	}

	if (pointShadowProgramContainer.type == ProgramType::omnidirectionalShadow)
	{
		//InstrumentationTimer timer("Config point shadows");

		for (int i = 0; i < models.size(); i++)  // new
		{
			configPointShadow(models[i], pointShadowProgramContainer.ID);
		}
		std::cout << "OK: Omnidirectional shadows are configured using the correct program." << std::endl;
	}
	else
	{
		std::cout << "ERROR: You are using the incorrect program for configuring omnidirectional shadows!" << std::endl;
	}
}

void LoadShadows::render()
{
	//glViewport(0, 0, DIR_SHADOW_WIDTH, DIR_SHADOW_HEIGHT);
	//glEnable(GL_DEPTH_TEST);
	//InstrumentationTimer timer("Render shadows");

	for (int i = 0; i < lights.dirLights_.size(); i++)
	{
		//InstrumentationTimer timer("Render dir shadows");

		glBindFramebuffer(GL_FRAMEBUFFER, dirShadows_[i].fbo);
		glClear(GL_DEPTH_BUFFER_BIT);

		for (int j = 0; j < models.size(); j++)  // new
		{
			renderDirShadow(lights.dirLights_[i], models[j], dirShadowProgramContainer.ID);
		}

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	for (int i = 0; i < lights.pointLights_.size(); i++)
	{
		//InstrumentationTimer timer("Render point shadows");

		glBindFramebuffer(GL_FRAMEBUFFER, pointShadows_[i].fbo);
		glClear(GL_DEPTH_BUFFER_BIT);

		for (int j = 0; j < models.size(); j++)  // new
		{
			renderPointShadow(lights.pointLights_[i], models[j], pointShadowProgramContainer.ID);
		}

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
}
