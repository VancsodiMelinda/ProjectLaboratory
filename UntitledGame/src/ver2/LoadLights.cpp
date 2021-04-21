#include "LoadLights.h"

LoadLights::LoadLights()
{
	//InstrumentationTimer timer("Load lights");

	std::cout << "Loading light models..." << std::endl;
	loadLightModels();
	std::cout << std::endl;

	std::cout << "Loading directional lights..." << std::endl;
	loadDirectionalLights();
	std::cout << std::endl;

	std::cout << "Loading point lights..." << std::endl;
	loadPointLights();
	std::cout << std::endl;

	std::cout << "Loading spot lights..." << std::endl;
	loadSpotLights();
	std::cout << std::endl;
}

LoadLights::LoadLights(std::string scene)
{
	if (scene == "demo")
	{
		std::cout << "Loading light models..." << std::endl;
		loadLightModels();
		std::cout << std::endl;

		std::cout << "Demo lights are being loaded." << std::endl;
		// DIRECTIONAL LIGHTS
		{
			CreateDirLight light1;
			light1.dirLightContainer.position = glm::vec3(1.0f, 20.0f, 1.0f);
			light1.dirLightContainer.target = glm::vec3(0.0f, 0.0f, 0.0f);
			light1.dirLightContainer.color = glm::vec3(1.0f, 1.0f, 1.0f);
			light1.dirLightContainer.lightSpaceMatrix = light1.createLightSpaceMatrix();
			dirLights_.push_back(light1.dirLightContainer);
		}

		// POINT LIGHTS
		{
			CreatePointLight light1;
			light1.pointLightContainer.position = glm::vec3(-2.0f, 1.5f, 0.0f);
			light1.pointLightContainer.color = glm::vec3(1.0f, 1.0f, 1.0f);  // blue
			light1.pointLightContainer.ambientStrength = 0.0f;
			pointLights_.push_back(light1.pointLightContainer);

			CreatePointLight light2;
			light2.pointLightContainer.position = glm::vec3(0.0f, 1.5f, -2.0f);
			light2.pointLightContainer.color = glm::vec3(1.0f, 1.0f, 1.0f);  // red
			light2.pointLightContainer.ambientStrength = 0.0f;
			pointLights_.push_back(light2.pointLightContainer);

			CreatePointLight light3;
			light3.pointLightContainer.position = glm::vec3(2.0f, 1.5f, 0.0f);
			light3.pointLightContainer.color = glm::vec3(1.0f, 1.0f, 1.0f);  // green
			light3.pointLightContainer.ambientStrength = 0.0f;
			pointLights_.push_back(light3.pointLightContainer);

			CreatePointLight light4;
			light4.pointLightContainer.position = glm::vec3(0.0f, 1.5f, 2.0f);
			light4.pointLightContainer.color = glm::vec3(1.0f, 1.0f, 1.0f);  // yellow
			light4.pointLightContainer.ambientStrength = 0.0f;
			pointLights_.push_back(light4.pointLightContainer);

			CreatePointLight light5;
			light5.pointLightContainer.position = glm::vec3(0.0f, 4.0f, 0.0f);
			light5.pointLightContainer.color = glm::vec3(1.0f, 1.0f, 1.0f);
			light5.pointLightContainer.ambientStrength = 0.0f;
			pointLights_.push_back(light5.pointLightContainer);
		}

		// SPOT LIGHTS
		{
			// little nightmare árnyékhoz
			CreateSpotLight light1;
			light1.spotLightContainer.position = glm::vec3(0.0f, 1.5f, 12.0f);
			light1.spotLightContainer.target = glm::vec3(0.0f, 1.5f, 15.0f);
			light1.spotLightContainer.color = glm::vec3(1.0f, 1.0f, 1.0f);
			light1.spotLightContainer.ambientStrength = 0.1f;
			light1.spotLightContainer.cutOffAngle = 15.0f;
			light1.spotLightContainer.lightSpaceMatrix = light1.createLightSpaceMatrix();
			spotLights.push_back(light1.spotLightContainer);

			// projektív textúrához
			CreateTexture tex1("resources/projective/logo1.png", TextureType::projectiveMap);
			//CreateTexture tex1(TextureType::projectiveMap);
			CreateSpotLight light2;
			light2.spotLightContainer.position = glm::vec3(-1.05f, 2.0f, -15.0f);
			light2.spotLightContainer.target = glm::vec3(-1.05f, 2.0f, -18.0f);
			light2.spotLightContainer.color = glm::vec3(1.0f, 1.0f, 1.0f);
			light2.spotLightContainer.ambientStrength = 0.0f;
			light2.spotLightContainer.cutOffAngle = 45.0f;
			light2.spotLightContainer.hasProjective = true;
			light2.spotLightContainer.projectiveMapID = tex1.textureContainer.ID;
			light2.spotLightContainer.lightSpaceMatrix = light2.createLightSpaceMatrix();
			spotLights.push_back(light2.spotLightContainer);

			// projektív textúrához
			CreateTexture tex2("resources/projective/logo2.png", TextureType::projectiveMap);
			//CreateTexture tex1(TextureType::projectiveMap);
			CreateSpotLight light3;
			light3.spotLightContainer.position = glm::vec3(0.0f, 2.0f, -15.0f);
			light3.spotLightContainer.target = glm::vec3(0.0f, 2.0f, -18.0f);
			light3.spotLightContainer.color = glm::vec3(1.0f, 1.0f, 1.0f);
			light3.spotLightContainer.ambientStrength = 0.0f;
			light3.spotLightContainer.cutOffAngle = 45.0f;
			light3.spotLightContainer.hasProjective = true;
			light3.spotLightContainer.projectiveMapID = tex2.textureContainer.ID;
			light3.spotLightContainer.lightSpaceMatrix = light3.createLightSpaceMatrix();
			spotLights.push_back(light3.spotLightContainer);

			// projektív textúrához
			CreateTexture tex3("resources/projective/logo3.png", TextureType::projectiveMap);
			//CreateTexture tex1(TextureType::projectiveMap);
			CreateSpotLight light4;
			light4.spotLightContainer.position = glm::vec3(0.90f, 2.0f, -15.0f);
			light4.spotLightContainer.target = glm::vec3(0.90f, 2.0f, -18.0f);
			light4.spotLightContainer.color = glm::vec3(1.0f, 1.0f, 1.0f);
			light4.spotLightContainer.ambientStrength = 0.0f;
			light4.spotLightContainer.cutOffAngle = 45.0f;
			light4.spotLightContainer.hasProjective = true;
			light4.spotLightContainer.projectiveMapID = tex3.textureContainer.ID;
			light4.spotLightContainer.lightSpaceMatrix = light4.createLightSpaceMatrix();
			spotLights.push_back(light4.spotLightContainer);
		}
	}

	if (scene == "room")
	{
		std::cout << "Loading light models..." << std::endl;
		loadLightModels();
		std::cout << std::endl;

		std::cout << "Demo lights are being loaded." << std::endl;
		// DIRECTIONAL LIGHTS
		{
			CreateDirLight light1;
			light1.dirLightContainer.position = glm::vec3(-3.0f, 20.0f, -10.0f);
			light1.dirLightContainer.target = glm::vec3(3.0f, 0.0f, 10.0f);
			light1.dirLightContainer.color = glm::vec3(1.0f, 1.0f, 1.0f);
			light1.dirLightContainer.lightSpaceMatrix = light1.createLightSpaceMatrix();
			dirLights_.push_back(light1.dirLightContainer);
		}

		// POINT LIGHTS
		{
			CreatePointLight light1;
			light1.pointLightContainer.position = glm::vec3(-3.0f, 3.0f, 5.0f);
			light1.pointLightContainer.color = glm::vec3(1.0f, 1.0f, 1.0f);
			light1.pointLightContainer.ambientStrength = 0.1f;
			pointLights_.push_back(light1.pointLightContainer);

			CreatePointLight light2;
			light2.pointLightContainer.position = glm::vec3(-3.0f, 3.0f, -3.0f);
			light2.pointLightContainer.color = glm::vec3(1.0f, 1.0f, 1.0f);
			light2.pointLightContainer.ambientStrength = 0.1f;
			pointLights_.push_back(light2.pointLightContainer);
		}

		// SPOT LIGHTS
		{
			CreateSpotLight light1;
			light1.spotLightContainer.position = glm::vec3(0.0f, 20.0f, 3.0f);
			light1.spotLightContainer.target = glm::vec3(0.0f, 1.5f, 5.0f);
			light1.spotLightContainer.color = glm::vec3(0.0f, 0.0f, 1.0f);
			light1.spotLightContainer.ambientStrength = 0.1f;
			light1.spotLightContainer.cutOffAngle = 15.0f;
			light1.spotLightContainer.lightSpaceMatrix = light1.createLightSpaceMatrix();
			spotLights.push_back(light1.spotLightContainer);
		}
	}
	if (scene == "space")
	{
		std::cout << "Loading light models..." << std::endl;
		loadLightModels();
		std::cout << std::endl;

		// DIRECTIONAL LIGHTS
		{
			CreateDirLight light1;
			light1.dirLightContainer.position = glm::vec3(-3.0f, 20.0f, -10.0f);
			light1.dirLightContainer.target = glm::vec3(3.0f, 0.0f, 10.0f);
			light1.dirLightContainer.color = glm::vec3(1.0f, 1.0f, 1.0f);
			light1.dirLightContainer.lightSpaceMatrix = light1.createLightSpaceMatrix();
			dirLights_.push_back(light1.dirLightContainer);
		}

		// POINT LIGHT
		{
			CreatePointLight light1;
			light1.pointLightContainer.position = glm::vec3(0.0f, 2.0f, 0.0f);
			light1.pointLightContainer.color = glm::vec3(1.0f, 1.0f, 1.0f);
			light1.pointLightContainer.ambientStrength = 0.0f;
			pointLights_.push_back(light1.pointLightContainer);
		}

		// SPOT LIGHTS
		{
			CreateSpotLight light1;
			light1.spotLightContainer.position = glm::vec3(0.0f, 20.0f, 3.0f);
			light1.spotLightContainer.target = glm::vec3(0.0f, 1.5f, 5.0f);
			light1.spotLightContainer.color = glm::vec3(1.0f, 1.0f, 1.0f);
			light1.spotLightContainer.ambientStrength = 0.1f;
			light1.spotLightContainer.cutOffAngle = 15.0f;
			light1.spotLightContainer.lightSpaceMatrix = light1.createLightSpaceMatrix();
			spotLights.push_back(light1.spotLightContainer);
		}
	}
}

void LoadLights::loadDirectionalLights()
{
	//InstrumentationTimer timer("Load dir lights");
	
	CreateDirLight light1;
	light1.dirLightContainer.position = glm::vec3(0.0f, 1.0f, 12.0f);
	light1.dirLightContainer.target = glm::vec3(0.0f, 1.0f, 15.0f);
	light1.dirLightContainer.color = glm::vec3(1.0f, 1.0f, 1.0f);
	light1.dirLightContainer.ambientStrength = 0.1f;
	light1.dirLightContainer.diffuseStrength = 0.5f;
	light1.dirLightContainer.specularStrength = 0.2f;
	light1.dirLightContainer.lightSpaceMatrix = light1.createLightSpaceMatrix();
	dirLights_.push_back(light1.dirLightContainer);
	
	/*
	CreateDirLight light2;
	light2.dirLightContainer.position = glm::vec3(-2.0f, 1.0f, -2.0f);
	light2.dirLightContainer.target = glm::vec3(0.0f, 0.0f, 0.0f);
	light2.dirLightContainer.color = glm::vec3(1.0f, 0.0f, 0.0f);  // red
	light2.dirLightContainer.ambientStrength = 0.1f;
	light2.dirLightContainer.diffuseStrength = 0.8f;
	light2.dirLightContainer.specularStrength = 1.0f;
	light2.dirLightContainer.lightSpaceMatrix = light2.createLightSpaceMatrix();
	dirLights_.push_back(light2.dirLightContainer);

	CreateDirLight light3;
	light3.dirLightContainer.position = glm::vec3(2.0f, 1.0f, -2.0f);
	light3.dirLightContainer.target = glm::vec3(0.0f, 0.0f, 0.0f);
	light3.dirLightContainer.color = glm::vec3(0.0f, 1.0f, 0.0f);  // green
	light3.dirLightContainer.ambientStrength = 0.0f;
	light3.dirLightContainer.diffuseStrength = 0.8f;
	light3.dirLightContainer.specularStrength = 1.0f;
	light3.dirLightContainer.lightSpaceMatrix = light3.createLightSpaceMatrix();
	dirLights_.push_back(light3.dirLightContainer);
	*/
}

void LoadLights::loadPointLights()
{
	//InstrumentationTimer timer("Load point lights");
	
	CreatePointLight light1;
	light1.pointLightContainer.position = glm::vec3(-2.0f, 1.0f, 2.0f);
	light1.pointLightContainer.color  = glm::vec3(0.0f, 0.0f, 1.0f);  // blue
	pointLights_.push_back(light1.pointLightContainer);

	CreatePointLight light2;
	light2.pointLightContainer.position = glm::vec3(-2.0f, 1.0f, -2.0f);
	light2.pointLightContainer.color = glm::vec3(1.0f, 0.0f, 0.0f);  // red
	pointLights_.push_back(light2.pointLightContainer);

	CreatePointLight light3;
	light3.pointLightContainer.position = glm::vec3(2.0f, 1.0f, -2.0f);
	light3.pointLightContainer.color = glm::vec3(0.0f, 1.0f, 0.0f);  // green
	pointLights_.push_back(light3.pointLightContainer);

	CreatePointLight light4;
	light4.pointLightContainer.position = glm::vec3(2.0f, 1.0f, 2.0f);
	light4.pointLightContainer.color = glm::vec3(1.0f, 1.0f, 1.0f);  // white
	pointLights_.push_back(light4.pointLightContainer);
}

void LoadLights::loadSpotLights()
{
	CreateSpotLight light1;
	light1.spotLightContainer.position = glm::vec3(0.0f, 1.5f, 10.0f);
	light1.spotLightContainer.target = glm::vec3(0.0f, 1.5f, 15.0f);
	light1.spotLightContainer.color = glm::vec3(1.0f, 1.0f, 1.0f);
	light1.spotLightContainer.lightSpaceMatrix = light1.createLightSpaceMatrix();
	spotLights.push_back(light1.spotLightContainer);

	/*
	CreateSpotLight light2;
	light2.spotLightContainer.position = glm::vec3(-4.0f, 1.3f, 2.0f);
	light2.spotLightContainer.target = glm::vec3(0.0f, 1.0f, -1.0f);
	light2.spotLightContainer.color = glm::vec3(1.0f, 0.5f, 0.5f);
	light2.spotLightContainer.cutOffAngle = 20.0f;
	light2.spotLightContainer.lightSpaceMatrix = light2.createLightSpaceMatrix();
	spotLights.push_back(light2.spotLightContainer);
	*/
}

void LoadLights::loadLightModels()
{
	//InstrumentationTimer timer("Load light models");

	// model for directional light
	CreateModel sphere("resources/models/sphere.obj");
	models[0] = sphere.objectContainer;

	// model for point light
	CreateModel lightbulb("resources/models/lightbulb.obj");
	models[1] = lightbulb.objectContainer;

	// model for spot light
	CreateModel flashlight("resources/models/flashlight.obj");
	models[2] = flashlight.objectContainer;
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
	uniformLocations.MVPloc = glGetUniformLocation(programID, "MVP");
	uniformLocations.colorLoc = glGetUniformLocation(programID, "lightColor");
	uniformLocations.IDloc = glGetUniformLocation(programID, "ID");
}

void LoadLights::renderDirLight(DirLightContainer& dirLight, ObjectContainer& object, GLuint programID, Kamera& kamera)
{
	glUseProgram(programID);

	// create/update MVP
	glm::mat4 modelMatrix = glm::mat4(1.0);
	modelMatrix = glm::translate(modelMatrix, dirLight.position);
	modelMatrix = glm::scale(modelMatrix, glm::vec3(0.2f, 0.2f, 0.2f));
	glm::mat4 MVP = kamera.cameraContainer.projectionMatrix * kamera.cameraContainer.viewMatrix * modelMatrix;

	// upload uniforms
	glUniformMatrix4fv(uniformLocations.MVPloc, 1, GL_FALSE, glm::value_ptr(MVP));
	glUniform3fv(uniformLocations.colorLoc, 1, glm::value_ptr(dirLight.color));
	glUniform1i(uniformLocations.IDloc, dirLight.ID);  //

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
	modelMatrix = glm::scale(modelMatrix, glm::vec3(0.2f, 0.2f, 0.2f));
	glm::mat4 MVP = kamera.cameraContainer.projectionMatrix * kamera.cameraContainer.viewMatrix * modelMatrix;

	// upload uniforms
	glUniformMatrix4fv(uniformLocations.MVPloc, 1, GL_FALSE, glm::value_ptr(MVP));
	glUniform3fv(uniformLocations.colorLoc, 1, glm::value_ptr(pointLight.color));
	glUniform1i(uniformLocations.IDloc, pointLight.ID);  //

	// draw
	glBindVertexArray(object.vao);
	glDrawElements(GL_TRIANGLES, object.data.indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

void LoadLights::renderSpotLight(SpotLightContainer& spotLight, ObjectContainer& object, GLuint programID, Kamera& kamera)
{
	glUseProgram(programID);

	// NEW: update lightspace matrix
	spotLight.lightSpaceMatrix = updateLightViewMatrix(spotLight);

	// calculate angles
	glm::vec3 dir = spotLight.target - spotLight.position;
	glm::vec2 dir_zy = glm::normalize(glm::vec2(dir.z, dir.y));
	glm::vec2 dir_xz = glm::normalize(glm::vec2(dir.x, dir.z));

	float angle_x = glm::angle(dir_zy, glm::vec2(0.0f, 1.0f));
	float angle_y = glm::angle(glm::vec2(0.0f, 1.0f), dir_xz);

	// correct angles
	//if (dir_zy.y < 0.0f)
		//angle_x = glm::radians(360.0f) - angle_x;

	if (dir_xz.x < 0.0f)
		angle_y = glm::radians(360.0f) - angle_y;

	// create/update MVP
	glm::mat4 modelMatrix = glm::mat4(1.0);
	modelMatrix = glm::translate(modelMatrix, spotLight.position);
	modelMatrix = glm::scale(modelMatrix, glm::vec3(0.2f, 0.2f, 0.2f));
	modelMatrix = glm::rotate(modelMatrix, angle_y, glm::vec3(0.0f, 1.0f, 0.0f));
	modelMatrix = glm::rotate(modelMatrix, angle_x, glm::vec3(1.0f, 0.0f, 0.0f));
	glm::mat4 MVP = kamera.cameraContainer.projectionMatrix * kamera.cameraContainer.viewMatrix * modelMatrix;

	// upload uniforms
	glUniformMatrix4fv(uniformLocations.MVPloc, 1, GL_FALSE, glm::value_ptr(MVP));
	glUniform3fv(uniformLocations.colorLoc, 1, glm::value_ptr(spotLight.color));
	glUniform1i(uniformLocations.IDloc, spotLight.ID);  //

	// draw
	glBindVertexArray(object.vao);
	glDrawElements(GL_TRIANGLES, object.data.indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

glm::mat4 LoadLights::updateLightViewMatrix(SpotLightContainer currentSpotLight)
{
	glm::mat4 lightViewMatrix = glm::lookAt(currentSpotLight.position, currentSpotLight.target, glm::vec3(0.0f, 1.0f, 0.0f));

	// create projection matrix (perspective)
	float aspect = (float)POINT_SHADOW_WIDTH_ / (float)POINT_SHADOW_HEIGHT_;
	float nearPlane = 1.0f;
	float farPlane = 20.0f;
	glm::mat4 lightProjMatrix = glm::perspective(glm::radians(90.0f), aspect, nearPlane, farPlane);

	if (!currentSpotLight.hasProjective)
		lightProjMatrix = glm::ortho(-4.0f, 4.0f, -3.0f, 3.0f, nearPlane, farPlane);  // projection matrix (orthogonal)

	glm::mat4 lightSpaceMatrix = lightProjMatrix * lightViewMatrix;

	return lightSpaceMatrix;
}

void LoadLights::config(ProgramContainer programContainer)
{
	//InstrumentationTimer timer("Config point lights");

	if (programContainer.type == ProgramType::light)
	{
		//configLight(dirLightObject, programContainer.ID);
		//configLight(pointLightObject, programContainer.ID);
		configLight(models[0], programContainer.ID);
		configLight(models[1], programContainer.ID);
		configLight(models[2], programContainer.ID);
		std::cout << "OK: Lights are configured using the correct program." << std::endl;
	}
	else
	{
		std::cout << "ERROR: You are using the incorrect program for configuring the lights!" << std::endl;
	}
}

void LoadLights::render(ProgramContainer programContainer, Kamera& kamera)
{
	//InstrumentationTimer timer("Render lights");

	// render directional lights
	for (int i = 0; i < dirLights_.size(); i++)
	{
		//InstrumentationTimer timer("Render dir lights");

		renderDirLight(dirLights_[i], models[0], programContainer.ID, kamera);
	}
	
	// render point lights
	for (int i = 0; i < pointLights_.size(); i++)
	{
		//InstrumentationTimer timer("Render point lights");

		renderPointLight(pointLights_[i], models[1], programContainer.ID, kamera);
	}

	for (int i = 0; i < spotLights.size(); i++)
	{
		renderSpotLight(spotLights[i], models[2], programContainer.ID, kamera);
	}
}

void LoadLights::renderDynamic(ProgramContainer programContainer, Kamera& kamera, std::vector<DirLightContainer>& dir, std::vector<PointLightContainer>& point, std::vector<SpotLightContainer>& spot)
{
	// update lights
	dirLights_ = dir;
	pointLights_ = point;
	spotLights = spot;

	// render directional lights
	for (int i = 0; i < dirLights_.size(); i++)
	{
		//InstrumentationTimer timer("Render dir lights");

		renderDirLight(dirLights_[i], models[0], programContainer.ID, kamera);
	}

	// render point lights
	for (int i = 0; i < pointLights_.size(); i++)
	{
		//InstrumentationTimer timer("Render point lights");

		renderPointLight(pointLights_[i], models[1], programContainer.ID, kamera);
	}

	for (int i = 0; i < spotLights.size(); i++)
	{
		renderSpotLight(spotLights[i], models[2], programContainer.ID, kamera);
	}
}

