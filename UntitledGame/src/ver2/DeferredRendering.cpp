#include "DeferredRendering.h"

DeferredRendering::DeferredRendering(Kamera& kamera_) : kamera(kamera_)
{
}

void DeferredRendering::createFboAndAttachments()
{
	// G-buffer - framebuffer object (fbo)
	glGenFramebuffers(1, &geomPassData.gBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, geomPassData.gBuffer);

	// position
	glGenTextures(1, &geomPassData.gPosition);
	glBindTexture(GL_TEXTURE_2D, geomPassData.gPosition);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, WINDOW_WIDTH, WINDOW_HEIGHT, 0, GL_RGBA, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, geomPassData.gPosition, 0);  // color attachment #1 of fbo

	// normal
	glGenTextures(1, &geomPassData.gNormal);
	glBindTexture(GL_TEXTURE_2D, geomPassData.gNormal);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, WINDOW_WIDTH, WINDOW_HEIGHT, 0, GL_RGBA, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, geomPassData.gNormal, 0);  // color attachment #2 of fbo

	// color & specular
	glGenTextures(1, &geomPassData.gAlbedoSpecular);
	glBindTexture(GL_TEXTURE_2D, geomPassData.gAlbedoSpecular);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, WINDOW_WIDTH, WINDOW_HEIGHT, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, geomPassData.gAlbedoSpecular, 0);  // color attachment #3 of fbo

	GLenum attachments[3] = { GL_COLOR_ATTACHMENT0 , GL_COLOR_ATTACHMENT1 , GL_COLOR_ATTACHMENT2 };
	glDrawBuffers(3, attachments);

	// RBO as depth buffer
	GLuint rbo;
	glGenRenderbuffers(1, &rbo);
	glBindRenderbuffer(GL_RENDERBUFFER, rbo);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, WINDOW_WIDTH, WINDOW_HEIGHT);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rbo);

	// debug and unbind fbo
	debugFramebuffer(GL_FRAMEBUFFER);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void DeferredRendering::initGeometryPass()
{
	// load model
	//std::string pathStr = "resources/assimp/test/test.obj";
	//std::string pathStr = "resources/assimp/plane/plane.obj";
	std::string pathStr = "resources/assimp/earth/Earth.obj";
	SceneLoader assets(&pathStr[0]);
	geomPassData.models = assets.models;

	// load program
	std::string vertexShaderFileName = "src/shaders/DR_GeometryPass.vert";
	std::string fragmentShaderFileName = "src/shaders/DR_GeometryPass.frag";
	CreateProgram program(vertexShaderFileName, fragmentShaderFileName);
	geomPassData.programID = program.programContainer.ID;

	// config vertex attribs for every model
	for (int i = 0; i < geomPassData.models.size(); i++)
	{
		glBindVertexArray(geomPassData.models[i].vao);
		glBindBuffer(GL_ARRAY_BUFFER, geomPassData.models[i].vbo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, geomPassData.models[i].ibo);

		GLint positionAttribIndex = glGetAttribLocation(geomPassData.programID, "vs_position");
		GLint textureAttribIndex = glGetAttribLocation(geomPassData.programID, "vs_uv");
		GLint normalAttribIndex = glGetAttribLocation(geomPassData.programID, "vs_normal");
		GLint tangentAttribIndex = glGetAttribLocation(geomPassData.programID, "vs_tangent");  //
		GLint bitangentAttribIndex = glGetAttribLocation(geomPassData.programID, "vs_bitangent");  //

		glEnableVertexAttribArray(positionAttribIndex);
		glEnableVertexAttribArray(textureAttribIndex);
		glEnableVertexAttribArray(normalAttribIndex);
		glEnableVertexAttribArray(tangentAttribIndex);  //
		glEnableVertexAttribArray(bitangentAttribIndex);  //

		GLintptr vOffset = 0 * sizeof(GL_FLOAT);
		GLintptr tOffset = geomPassData.models[i].data.vertices.size() * sizeof(GL_FLOAT);
		GLintptr nOffset = (geomPassData.models[i].data.vertices.size() + geomPassData.models[i].data.uvs.size()) * sizeof(GL_FLOAT);
		GLintptr tanOffset = (geomPassData.models[i].data.vertices.size() + geomPassData.models[i].data.uvs.size() +
			geomPassData.models[i].data.normals.size()) * sizeof(GL_FLOAT);  //
		GLintptr bitOffset = (geomPassData.models[i].data.vertices.size() + geomPassData.models[i].data.uvs.size() +
			geomPassData.models[i].data.normals.size() + geomPassData.models[i].data.tangents.size()) * sizeof(GL_FLOAT);  //
		int stride3f = 3 * sizeof(GL_FLOAT);
		int stride2f = 2 * sizeof(GL_FLOAT);

		glVertexAttribPointer(positionAttribIndex, 3, GL_FLOAT, GL_FALSE, stride3f, (GLvoid*)vOffset);
		glVertexAttribPointer(textureAttribIndex, 2, GL_FLOAT, GL_FALSE, stride2f, (GLvoid*)tOffset);
		glVertexAttribPointer(normalAttribIndex, 3, GL_FLOAT, GL_FALSE, stride3f, (GLvoid*)nOffset);
		glVertexAttribPointer(tangentAttribIndex, 3, GL_FLOAT, GL_FALSE, stride3f, (GLvoid*)tanOffset);  //
		glVertexAttribPointer(bitangentAttribIndex, 3, GL_FLOAT, GL_FALSE, stride3f, (GLvoid*)bitOffset);  //

		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}
}

void DeferredRendering::renderGeometryPass()
{
	// specify clear color
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	// use spec. framebuffer, clear
	glBindFramebuffer(GL_FRAMEBUFFER, geomPassData.gBuffer);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glUseProgram(geomPassData.programID);

	// upload uniforms
	glUniform1i(glGetUniformLocation(geomPassData.programID, "gPosition"), 0);
	glUniform1i(glGetUniformLocation(geomPassData.programID, "gNormal"), 1);
	glUniform1i(glGetUniformLocation(geomPassData.programID, "gAlbedoSpecular"), 2);
	glUniform1i(glGetUniformLocation(geomPassData.programID, "diffuseMap"), 3);
	glUniform1i(glGetUniformLocation(geomPassData.programID, "normalMap"), 4);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, geomPassData.gPosition);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, geomPassData.gNormal);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, geomPassData.gAlbedoSpecular);

	/*
	// fix camera
	geomPassData.cameraPos = glm::vec3(4.0f, 4.0f, 10.0f);
	glm::mat4 P = glm::perspective(glm::radians(45.0f), (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT, 0.1f, 100.0f);
	glm::mat4 V = glm::lookAt(geomPassData.cameraPos, glm::vec3(0.0f, -2.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	glm::mat4 M = glm::mat4(1.0f);
	*/

	
	// moving camera
	glm::mat4 M = glm::mat4(1.0f);
	glm::mat4 V = kamera.cameraContainer.viewMatrix;
	glm::mat4 P = kamera.cameraContainer.projectionMatrix;
	geomPassData.cameraPos = kamera.cameraContainer.cameraPosition;
	

	std::vector<glm::vec3> modelPositions;
	modelPositions.push_back(glm::vec3(0.0f, 0.0f, 0.0f));
	modelPositions.push_back(glm::vec3(-4.0f, 0.0f, 0.0f));
	modelPositions.push_back(glm::vec3(4.0f, 0.0f, 0.0f));

	modelPositions.push_back(glm::vec3(0.0f, 0.0f, -4.0f));
	modelPositions.push_back(glm::vec3(-4.0f, 0.0f, -4.0f));
	modelPositions.push_back(glm::vec3(4.0f, 0.0f, -4.0f));

	modelPositions.push_back(glm::vec3(0.0f, 0.0f, 4.0f));
	modelPositions.push_back(glm::vec3(-4.0f, 0.0f, 4.0f));
	modelPositions.push_back(glm::vec3(4.0f, 0.0f, 4.0f));

	//glm::mat4 MVP = P * V * M;

	//glUniformMatrix4fv(glGetUniformLocation(geomPassData.programID, "MVP"), 1, GL_FALSE, glm::value_ptr(MVP));
	//glUniformMatrix4fv(glGetUniformLocation(geomPassData.programID, "M"), 1, GL_FALSE, glm::value_ptr(M));

	for (int i = 0; i < geomPassData.models.size(); i++)
	{
		glBindVertexArray(geomPassData.models[i].vao);

		glActiveTexture(GL_TEXTURE3);
		glBindTexture(GL_TEXTURE_2D, geomPassData.models[i].material.diffuseMap);
		glActiveTexture(GL_TEXTURE4);
		glBindTexture(GL_TEXTURE_2D, geomPassData.models[i].material.normalMap);

		for (int j = 0; j < modelPositions.size(); j++)
		{
			M = glm::mat4(1.0f);
			M = glm::translate(M, modelPositions[j]);
			glm::mat4 MVP = P * V * M;
			glUniformMatrix4fv(glGetUniformLocation(geomPassData.programID, "MVP"), 1, GL_FALSE, glm::value_ptr(MVP));
			glUniformMatrix4fv(glGetUniformLocation(geomPassData.programID, "M"), 1, GL_FALSE, glm::value_ptr(M));
			glDrawElements(GL_TRIANGLES, geomPassData.models[i].data.indices.size(), GL_UNSIGNED_INT, 0);
		}

		//glDrawElements(GL_TRIANGLES, geomPassData.models[i].data.indices.size(), GL_UNSIGNED_INT, 0);

		glBindVertexArray(0);
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void DeferredRendering::initLightingPass()
{
	lightPassData.numberOfLights = 33;
	/*
	for (int i = 0; i < lightPassData.numberOfLights; i++)
	{
		// generate random position
		float x = ((rand() % 100) / 100.0f) * 20.0f - 10.0f;
		float y = ((rand() % 100) / 100.0f) * 10.0f - 5.0f;
		lightPassData.lightPositions.push_back(glm::vec3(x, 0.1f, y));
		lightPassData.lightPositions.push_back(glm::vec3(0.0f, 0.1f, 0.0f));

		// generate random color
		float r = ((rand() % 100) / 200.0f) + 0.5f;
		float g = ((rand() % 100) / 200.0f) + 0.5f;
		float b = ((rand() % 100) / 200.0f) + 0.5f;
		//lightPassData.lightColors.push_back(glm::vec3(r, g, b));
		lightPassData.lightColors.push_back(glm::vec3(1.0f, 0.0f, 0.0f));
	}
	*/

	// positions
	lightPassData.lightPositions.push_back(glm::vec3(-4.0f, 0.0f, -6.0f));
	lightPassData.lightPositions.push_back(glm::vec3(0.0f, 0.0f, -6.0f));
	lightPassData.lightPositions.push_back(glm::vec3(4.0f, 0.0f, -6.0f));

	lightPassData.lightPositions.push_back(glm::vec3(-6.0f, 0.0f, -4.0f));
	lightPassData.lightPositions.push_back(glm::vec3(-2.0f, 0.0f, -4.0f));
	lightPassData.lightPositions.push_back(glm::vec3(2.0f, 0.0f, -4.0f));
	lightPassData.lightPositions.push_back(glm::vec3(6.0f, 0.0f, -4.0f));

	lightPassData.lightPositions.push_back(glm::vec3(-4.0f, 0.0f, -2.0f));
	lightPassData.lightPositions.push_back(glm::vec3(0.0f, 0.0f, -2.0f));
	lightPassData.lightPositions.push_back(glm::vec3(4.0f, 0.0f, -2.0f));

	lightPassData.lightPositions.push_back(glm::vec3(-6.0f, 0.0f, 0.0f));
	lightPassData.lightPositions.push_back(glm::vec3(-2.0f, 0.0f, 0.0f));
	lightPassData.lightPositions.push_back(glm::vec3(2.0f, 0.0f, 0.0f));
	lightPassData.lightPositions.push_back(glm::vec3(6.0f, 0.0f, 0.0f));

	lightPassData.lightPositions.push_back(glm::vec3(-4.0f, 0.0f, 2.0f));
	lightPassData.lightPositions.push_back(glm::vec3(0.0f, 0.0f, 2.0f));
	lightPassData.lightPositions.push_back(glm::vec3(4.0f, 0.0f, 2.0f));
	
	lightPassData.lightPositions.push_back(glm::vec3(-6.0f, 0.0f, 4.0f));
	lightPassData.lightPositions.push_back(glm::vec3(-2.0f, 0.0f, 4.0f));
	lightPassData.lightPositions.push_back(glm::vec3(2.0f, 0.0f, 4.0f));
	lightPassData.lightPositions.push_back(glm::vec3(6.0f, 0.0f, 4.0f));

	lightPassData.lightPositions.push_back(glm::vec3(-4.0f, 0.0f, 6.0f));
	lightPassData.lightPositions.push_back(glm::vec3(0.0f, 0.0f, 6.0f));
	lightPassData.lightPositions.push_back(glm::vec3(4.0f, 0.0f, 6.0f));


	lightPassData.lightPositions.push_back(glm::vec3(0.0f, 1.5f, 0.0f));
	lightPassData.lightPositions.push_back(glm::vec3(-4.0f, 1.5f, 0.0f));
	lightPassData.lightPositions.push_back(glm::vec3(4.0f, 1.5f, 0.0f));
	lightPassData.lightPositions.push_back(glm::vec3(0.0f, 1.5f, -4.0f));
	lightPassData.lightPositions.push_back(glm::vec3(-4.0f, 1.5f, -4.0f));
	lightPassData.lightPositions.push_back(glm::vec3(4.0f, 1.5f, -4.0f));
	lightPassData.lightPositions.push_back(glm::vec3(0.0f, 1.5f, 4.0f));
	lightPassData.lightPositions.push_back(glm::vec3(-4.0f, 1.5f, 4.0f));
	lightPassData.lightPositions.push_back(glm::vec3(4.0f, 1.5f, 4.0f));

	// colors
	/*
	glm::vec3 Red = glm::vec3(255.0f / 255.0f, 0.0f / 255.0f, 0.0f / 255.0f);
	glm::vec3 Orange = glm::vec3(255.0f / 255.0f, 165.0f / 255.0f, 0.0f / 255.0f);
	glm::vec3 Yellow = glm::vec3(255.0f / 255.0f, 255.0f / 255.0f, 0.0f / 255.0f);
	glm::vec3 Green = glm::vec3(0.0f / 255.0f, 128.0f / 255.0f, 0.0f / 255.0f);
	glm::vec3 Blue = glm::vec3(0.0f / 255.0f, 0.0f / 255.0f, 255.0f / 255.0f);
	glm::vec3 Indigo = glm::vec3(75.0f / 255.0f, 0.0f / 255.0f, 130.0f / 255.0f);
	glm::vec3 Violet = glm::vec3(238.0f / 255.0f, 130.0f / 255.0f, 238.0f / 255.0f);

	
	lightPassData.lightColors.push_back(Red);
	lightPassData.lightColors.push_back(Red);

	lightPassData.lightColors.push_back(Orange);
	lightPassData.lightColors.push_back(Orange);
	lightPassData.lightColors.push_back(Orange);

	lightPassData.lightColors.push_back(Yellow);
	lightPassData.lightColors.push_back(Yellow);

	lightPassData.lightColors.push_back(Indigo);
	lightPassData.lightColors.push_back(Orange);
	lightPassData.lightColors.push_back(Yellow);

	lightPassData.lightColors.push_back(Violet);
	lightPassData.lightColors.push_back(Green);
	*/

	glm::vec3 Red = glm::vec3(255.0f, 0.0f, 0.0f) / 255.0f;
	glm::vec3 Orange = glm::vec3(255.0f, 165.0f, 0.0f) / 255.0f;
	glm::vec3 Yellow = glm::vec3(255.0f, 255.0f, 0.0f) / 255.0f;
	glm::vec3 Green = glm::vec3(0.0f, 128.0f, 0.0f) / 255.0f;
	glm::vec3 Blue = glm::vec3(0.0f, 0.0f, 255.0f) / 255.0f;
	glm::vec3 Indigo = glm::vec3(75.0f, 0.0f, 130.0f) / 255.0f;

	glm::vec3 MountainMeadow = glm::vec3(37.0f, 179.0f, 150.0f) / 255.0f;  // green
	glm::vec3 MiddleBlue = glm::vec3(112.0f, 206.0f, 208.0f) / 255.0f;  // blue
	glm::vec3 HotPink = glm::vec3(255.0f, 105.0f, 180.0f) / 255.0f;

	glm::vec3 c1 = glm::vec3(255.0f, 0.0f, 166.0f) / 255.0f;
	glm::vec3 c2 = glm::vec3(255.0f, 73.0f, 83.0f) / 255.0f;
	glm::vec3 c3 = glm::vec3(255.0f, 152.0f, 0.0f) / 255.0f;
	glm::vec3 c4 = glm::vec3(187.0f, 212.0f, 0.0f) / 255.0f;
	glm::vec3 c5 = glm::vec3(0.0f, 255.0f, 133.0f) / 255.0f;

	std::vector<glm::vec3> colorSelection = { c1, c2, c3, c4, c5, Blue, HotPink };

	for (int i = 0; i < lightPassData.numberOfLights; i++)
	{
		int rndIndex = round(rand() % colorSelection.size());
		lightPassData.lightColors.push_back(colorSelection[rndIndex]);
		//lightPassData.lightColors.push_back(MountainMeadow);

		/*
		float r = 0.0f;
		float g = (lightPassData.lightPositions[i].x + 6.0f) / 12.0f;
		float b = (lightPassData.lightPositions[i].z + 6.0f) / 12.0f;
		lightPassData.lightColors.push_back(glm::vec3(r, g, b));
		*/
	}

	// load program
	std::string vertexShaderFileName = "src/shaders/DR_LightingPass.vert";
	std::string fragmentShaderFileName = "src/shaders/DR_LightingPass.frag";
	CreateProgram program(vertexShaderFileName, fragmentShaderFileName);
	lightPassData.programID = program.programContainer.ID;

	// load model
	createQuad();
}

void DeferredRendering::renderLightingPass()
{
	// specify clear color
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	// use default framebuffer, clear
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glUseProgram(lightPassData.programID);

	// upload uniforms
	glUniform1i(glGetUniformLocation(lightPassData.programID, "gPosition"), 0);
	glUniform1i(glGetUniformLocation(lightPassData.programID, "gNormal"), 1);
	glUniform1i(glGetUniformLocation(lightPassData.programID, "gAlbedoSpecular"), 2);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, geomPassData.gPosition);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, geomPassData.gNormal);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, geomPassData.gAlbedoSpecular);

	glUniform3fv(glGetUniformLocation(lightPassData.programID, "cameraPos"), 1, glm::value_ptr(geomPassData.cameraPos));

	for (int i = 0; i < lightPassData.numberOfLights; i++)
	{
		glUniform3fv(glGetUniformLocation(lightPassData.programID, ("lights[" + std::to_string(i) + "].position").c_str()), 1,
			glm::value_ptr(lightPassData.lightPositions[i]));
		glUniform3fv(glGetUniformLocation(lightPassData.programID, ("lights[" + std::to_string(i) + "].color").c_str()), 1,
			glm::value_ptr(lightPassData.lightColors[i]));

		glUniform1f(glGetUniformLocation(lightPassData.programID, ("lights[" + std::to_string(i) + "].constant").c_str()), 1.0f);
		glUniform1f(glGetUniformLocation(lightPassData.programID, ("lights[" + std::to_string(i) + "].linear").c_str()), 0.35f);
		glUniform1f(glGetUniformLocation(lightPassData.programID, ("lights[" + std::to_string(i) + "].quadratic").c_str()), 0.44f);
	}

	// render quad
	glBindVertexArray(lightPassData.quad.vao);
	glDrawElements(GL_TRIANGLES, lightPassData.quad.data.indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

void DeferredRendering::initLightModel()
{
	// load model
	std::string pathStr = "resources/assimp/lightbulb/lightbulb.obj";
	SceneLoader assets(&pathStr[0]);
	lightModelData.model = assets.models[0];

	// load program
	std::string vertexShaderFileName = "src/shaders/Light.vert";
	std::string fragmentShaderFileName = "src/shaders/Light.frag";
	CreateProgram program(vertexShaderFileName, fragmentShaderFileName);
	lightModelData.programID = program.programContainer.ID;

	// config vertex attrib
	glBindVertexArray(lightModelData.model.vao);
	glBindBuffer(GL_ARRAY_BUFFER, lightModelData.model.vbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, lightModelData.model.ibo);

	GLuint positionAttribIndex = glGetAttribLocation(lightModelData.programID, "in_vertexPosition");
	glEnableVertexAttribArray(positionAttribIndex);

	GLintptr vOffset = 0 * sizeof(GL_FLOAT);
	int stride3f = 3 * sizeof(GL_FLOAT);
	glVertexAttribPointer(positionAttribIndex, 3, GL_FLOAT, GL_FALSE, stride3f, (GLvoid*)vOffset);

	glBindVertexArray(0);
}

void DeferredRendering::renderLightModel()
{
	// use default framebuffer, clear depth (so the quad doesn't hide the lightsources
	glBindFramebuffer(GL_READ_FRAMEBUFFER, geomPassData.gBuffer);  // source fbo
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);  // destination fbo
	glBlitFramebuffer(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, GL_DEPTH_BUFFER_BIT, GL_NEAREST);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	glUseProgram(lightModelData.programID);

	// moving camera
	glm::mat4 M = glm::mat4(1.0f);
	glm::mat4 V = kamera.cameraContainer.viewMatrix;
	glm::mat4 P = kamera.cameraContainer.projectionMatrix;

	glBindVertexArray(lightModelData.model.vao);

	for (int i = 0; i < lightPassData.lightPositions.size(); i++)
	{
		M = glm::mat4(1.0f);
		M = glm::translate(M, lightPassData.lightPositions[i]);
		M = glm::scale(M, glm::vec3(0.1f));
		glm::mat4 MVP = P * V * M;
		glUniformMatrix4fv(glGetUniformLocation(lightModelData.programID, "MVP"), 1, GL_FALSE, glm::value_ptr(MVP));
		glUniform3fv(glGetUniformLocation(lightModelData.programID, "lightColor"), 1, glm::value_ptr(lightPassData.lightColors[i]));
		glDrawElements(GL_TRIANGLES, lightModelData.model.data.indices.size(), GL_UNSIGNED_INT, 0);
	}

	glBindVertexArray(0);
}

void DeferredRendering::createQuad()
{
	// generate quad data
	ObjectContainer object;
	ObjectData data;

	data.vertices = { -1.0f,  1.0f, 0.0f,
					  -1.0f, -1.0f, 0.0f,
					   1.0f, -1.0f, 0.0f,
					   1.0f,  1.0f, 0.0f };

	data.uvs = { 0.0f, 1.0f,
				 0.0f, 0.0f,
				 1.0f, 0.0f,
				 1.0f, 1.0f };

	data.indices = { 0, 1, 2,
					 0, 2, 3 };

	object.data = data;

	// create VAO, VBO, IBO
	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	object.vao = vao;

	GLuint vbo;
	glGenBuffers(1, &vbo);
	object.vbo = vbo;

	GLuint ibo;
	glGenBuffers(1, &ibo);
	object.ibo = ibo;

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, (data.vertices.size() + data.uvs.size()) * sizeof(GL_FLOAT), 0, GL_STATIC_DRAW);  // reserve space
	glBufferSubData(GL_ARRAY_BUFFER, 0, data.vertices.size() * sizeof(GL_FLOAT), &data.vertices[0]);  // VERTEX COORDINATES
	glBufferSubData(GL_ARRAY_BUFFER, data.vertices.size() * sizeof(GL_FLOAT), data.uvs.size() * sizeof(GL_FLOAT), &data.uvs[0]);  // TEXTURE COORDINATES
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, data.indices.size() * sizeof(GLuint), &data.indices[0], GL_STATIC_DRAW);  // INDICES

	glBindVertexArray(0);

	// config vertex attributes
	glBindVertexArray(vao);  // bind VAO
	glBindBuffer(GL_ARRAY_BUFFER, vbo);  // bind VBO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);  // bind IBO

	GLuint positionAttribIndex = glGetAttribLocation(lightPassData.programID, "vs_position");
	GLuint textureAttribIndex = glGetAttribLocation(lightPassData.programID, "vs_uv");

	glEnableVertexAttribArray(positionAttribIndex);
	glEnableVertexAttribArray(textureAttribIndex);

	GLintptr vOffset = 0 * sizeof(GL_FLOAT);
	GLintptr tOffset = data.vertices.size() * sizeof(GL_FLOAT);
	int stride3f = 3 * sizeof(GL_FLOAT);
	int stride2f = 2 * sizeof(GL_FLOAT);

	glVertexAttribPointer(positionAttribIndex, 3, GL_FLOAT, GL_FALSE, stride3f, (GLvoid*)vOffset);
	glVertexAttribPointer(textureAttribIndex, 2, GL_FLOAT, GL_FALSE, stride2f, (GLvoid*)tOffset);

	// unbind
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	lightPassData.quad = object;
}
