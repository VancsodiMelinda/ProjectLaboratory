#include "Render.h"

Render::Render(LoadAssets& assets_, LoadPrograms& programs_, Kamera& kamera_, LoadLights& lights_, LoadShadows& shadows_, SkyboxContainer skybox_) :
	assets(assets_),
	programs(programs_),
	kamera(kamera_),
	lights(lights_),
	shadows(shadows_),
	skybox(skybox_)
{
	programID = programs.programs[0].ID;
}

void Render::configAsset(ObjectContainer& object)
{
	configVertexAttributes(object);
	getUniformLocations();
}

void Render::configVertexAttributes(ObjectContainer& object)
{
	glBindVertexArray(object.vao);
	glBindBuffer(GL_ARRAY_BUFFER, object.vbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, object.ibo);

	GLuint positionAttribIndex = glGetAttribLocation(programID, "in_vertexPosition");
	GLuint textureAttribIndex = glGetAttribLocation(programID, "in_textureCoords");
	GLuint normalAttribIndex = glGetAttribLocation(programID, "in_normalVec");
	GLuint tangentAttribIndex = glGetAttribLocation(programID, "in_tangent"); //
	GLuint bitangentAttribIndex = glGetAttribLocation(programID, "in_bitangent"); //

	glEnableVertexAttribArray(positionAttribIndex);
	glEnableVertexAttribArray(textureAttribIndex);
	glEnableVertexAttribArray(normalAttribIndex);
	glEnableVertexAttribArray(tangentAttribIndex);  //
	glEnableVertexAttribArray(bitangentAttribIndex);  //

	GLintptr vOffset = 0 * sizeof(GL_FLOAT);
	GLintptr tOffset = object.data.vertices.size() * sizeof(GL_FLOAT);
	GLintptr nOffset = (object.data.vertices.size() + object.data.uvs.size()) * sizeof(GL_FLOAT);
	GLintptr tanOffset = (object.data.vertices.size() + object.data.uvs.size() + object.data.normals.size()) * sizeof(GL_FLOAT);  //
	GLintptr bitOffset = (object.data.vertices.size() + object.data.uvs.size() + object.data.normals.size() + object.data.tangents.size()) * sizeof(GL_FLOAT);  //
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

void Render::getUniformLocations()
{
	// vertex shader
	uniformLocations.MVPloc = glGetUniformLocation(programID, "MVP");
	uniformLocations.modelMatrixLoc = glGetUniformLocation(programID, "modelMatrix");

	// fragment shader
	uniformLocations.diffuseMapLoc = glGetUniformLocation(programID, "material.diffuseMap");
	uniformLocations.specularMapLoc = glGetUniformLocation(programID, "material.specularMap");
	uniformLocations.shininessLoc = glGetUniformLocation(programID, "material.shininess");
	uniformLocations.normalMapLoc = glGetUniformLocation(programID, "material.normalMap");
	uniformLocations.hasNormalMapLoc = glGetUniformLocation(programID, "hasNormalMap");

	uniformLocations.cameraPosLoc = glGetUniformLocation(programID, "camera.position");
	uniformLocations.cameraFarPlaneLoc = glGetUniformLocation(programID, "camera.farPlane");

	uniformLocations.skyboxLoc = glGetUniformLocation(programID, "skybox");

	//for (int i = 0; i < sizeof(lights.dirLights) / sizeof(lights.dirLights[0]); i++)
	for (int i = 0; i < NUMBER_OF_DIR_LIGHTS; i++)
	{
		uniformLocations.dirLightLocs[i][0] = glGetUniformLocation(programID, ("dirLightArray[" + std::to_string(i) + "].direction").c_str());
		uniformLocations.dirLightLocs[i][1] = glGetUniformLocation(programID, ("dirLightArray[" + std::to_string(i) + "].color").c_str());

		uniformLocations.dirLightLocs[i][2] = glGetUniformLocation(programID, ("dirLightArray[" + std::to_string(i) + "].ambientStrength").c_str());
		uniformLocations.dirLightLocs[i][3] = glGetUniformLocation(programID, ("dirLightArray[" + std::to_string(i) + "].diffuseStrength").c_str());
		uniformLocations.dirLightLocs[i][4] = glGetUniformLocation(programID, ("dirLightArray[" + std::to_string(i) + "].specularStrength").c_str());

		uniformLocations.dirLightLocs[i][5] = glGetUniformLocation(programID, ("dirLightArray[" + std::to_string(i) + "].shadowMap").c_str());
		uniformLocations.dirLightLocs[i][6] = glGetUniformLocation(programID, ("dirLightArray[" + std::to_string(i) + "].lightSpaceMatrix").c_str());
	}

	for (int i = 0; i < NUMBER_OF_POINT_LIGHTS; i++)
	{
		uniformLocations.pointLightLocs[i][0] = glGetUniformLocation(programID, ("pointLightArray[" + std::to_string(i) + "].position").c_str());
		uniformLocations.pointLightLocs[i][1] = glGetUniformLocation(programID, ("pointLightArray[" + std::to_string(i) + "].color").c_str());

		uniformLocations.pointLightLocs[i][2] = glGetUniformLocation(programID, ("pointLightArray[" + std::to_string(i) + "].ambientStrength").c_str());
		uniformLocations.pointLightLocs[i][3] = glGetUniformLocation(programID, ("pointLightArray[" + std::to_string(i) + "].diffuseStrength").c_str());
		uniformLocations.pointLightLocs[i][4] = glGetUniformLocation(programID, ("pointLightArray[" + std::to_string(i) + "].specularStrength").c_str());

		uniformLocations.pointLightLocs[i][5] = glGetUniformLocation(programID, ("pointLightArray[" + std::to_string(i) + "].constant").c_str());
		uniformLocations.pointLightLocs[i][6] = glGetUniformLocation(programID, ("pointLightArray[" + std::to_string(i) + "].linear").c_str());
		uniformLocations.pointLightLocs[i][7] = glGetUniformLocation(programID, ("pointLightArray[" + std::to_string(i) + "].quadratic").c_str());

		uniformLocations.pointLightLocs[i][8] = glGetUniformLocation(programID, ("pointLightArray[" + std::to_string(i) + "].shadowBox").c_str());
	}
}


void Render::renderAsset(ObjectContainer& object)
{
	glUseProgram(programID);

	uploadUniforms(object);

	// bind textures
	glBindVertexArray(object.vao);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, object.material.diffuseMap);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, object.material.specularMap);

	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, object.material.normalMap);

	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_CUBE_MAP, skybox.ID);

	//for (int i = 0; i < sizeof(shadows.dirShadows) / sizeof(shadows.dirShadows[0]); i++)
	for (int i = 4, j = 0; i < (NUMBER_OF_DIR_LIGHTS + 4); i++, j++)
	{
		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D, shadows.dirShadows[j].shadowMap);
	}

	
	for (int i = (NUMBER_OF_DIR_LIGHTS + 4), j = 0; i < (NUMBER_OF_DIR_LIGHTS + NUMBER_OF_POINT_LIGHTS + 4); i++, j++)
	{
		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_CUBE_MAP, shadows.pointShadows[j].shadowCube);
	}
	

	glDrawElements(GL_TRIANGLES, object.data.indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

void Render::uploadUniforms(ObjectContainer& object)
{
	glm::mat4 MVP = updateMVP(object.modelMatrix, kamera.cameraContainer.viewMatrix, kamera.cameraContainer.projectionMatrix);

	// vertex shader
	glUniformMatrix4fv(uniformLocations.MVPloc, 1, GL_FALSE, glm::value_ptr(MVP));
	glUniformMatrix4fv(uniformLocations.modelMatrixLoc, 1, GL_FALSE, glm::value_ptr(object.modelMatrix));

	// fragment shader
	glUniform1i(uniformLocations.diffuseMapLoc, 0);	// tex
	glUniform1i(uniformLocations.specularMapLoc, 1);	// tex
	glUniform1i(uniformLocations.normalMapLoc, 2);	// tex
	glUniform1f(uniformLocations.shininessLoc, object.material.shininess);

	if (object.material.normalMap == 0)
		glUniform1i(uniformLocations.hasNormalMapLoc, 0);	// false, object doesn't have normal map
	else
		glUniform1i(uniformLocations.hasNormalMapLoc, 1);	// true, object has normal map

	glUniform3fv(uniformLocations.cameraPosLoc, 1, glm::value_ptr(kamera.cameraContainer.cameraPosition));
	glUniform1f(uniformLocations.cameraFarPlaneLoc, kamera.cameraContainer.farPlane);

	glUniform1i(uniformLocations.skyboxLoc, 3);	// tex

	//for (int i = 0; i < sizeof(lights.dirLights) / sizeof(lights.dirLights[0]); i++)
	for (int i = 0; i < NUMBER_OF_DIR_LIGHTS; i++)
	{
		glm::vec3 direction = glm::vec3(0.0f) - lights.dirLights[i].position;
		glUniform3fv(uniformLocations.dirLightLocs[i][0], 1, glm::value_ptr(direction));
		glUniform3fv(uniformLocations.dirLightLocs[i][1], 1, glm::value_ptr(lights.dirLights[i].color));

		glUniform1f(uniformLocations.dirLightLocs[i][2], lights.dirLights[i].ambientStrength);
		glUniform1f(uniformLocations.dirLightLocs[i][3], lights.dirLights[i].diffuseStrength);
		glUniform1f(uniformLocations.dirLightLocs[i][4], lights.dirLights[i].specularStrength);

		glUniform1i(uniformLocations.dirLightLocs[i][5], 4 + i);
		glUniformMatrix4fv(uniformLocations.dirLightLocs[i][6], 1, GL_FALSE, glm::value_ptr(lights.dirLights[i].lightSpaceMatrix));
	}

	for (int i = 0; i < NUMBER_OF_POINT_LIGHTS; i++)
	{
		glUniform3fv(uniformLocations.pointLightLocs[i][0], 1, glm::value_ptr(lights.pointLights[i].position));
		glUniform3fv(uniformLocations.pointLightLocs[i][1], 1, glm::value_ptr(lights.pointLights[i].color));

		glUniform1f(uniformLocations.pointLightLocs[i][2], lights.pointLights[i].ambientStrength);
		glUniform1f(uniformLocations.pointLightLocs[i][3], lights.pointLights[i].diffuseStrength);
		glUniform1f(uniformLocations.pointLightLocs[i][4], lights.pointLights[i].specularStrength);

		glUniform1f(uniformLocations.pointLightLocs[i][5], lights.pointLights[i].constant);
		glUniform1f(uniformLocations.pointLightLocs[i][6], lights.pointLights[i].linear);
		glUniform1f(uniformLocations.pointLightLocs[i][7], lights.pointLights[i].quadratic);

		glUniform1i(uniformLocations.pointLightLocs[i][8], 4 + NUMBER_OF_DIR_LIGHTS + i);
	}
}

glm::mat4 Render::updateMVP(glm::mat4 M, glm::mat4 V, glm::mat4 P)
{
	return P * V * M;
}

void Render::configAssets()
{
	//for (int i = 0; i < sizeof(assets.models) / sizeof(assets.models[0]); i++)
	for (int i = 0; i < NUMBER_OF_OBJECTS; i++)
	{
		configAsset(assets.models[i]);
	}

	std::cout << "OK: Assets are configured." << std::endl;
}

void Render::renderAssets(Kamera& kamera_)
{
	kamera = kamera_;
	//glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
	//glBindFramebuffer(GL_FRAMEBUFFER, 0);
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//glEnable(GL_DEPTH_TEST);

	//for (int i = 0; i < sizeof(assets.models) / sizeof(assets.models[0]); i++)
	for (int i = 0; i < NUMBER_OF_OBJECTS; i++)
	{
		renderAsset(assets.models[i]);
	}
}

