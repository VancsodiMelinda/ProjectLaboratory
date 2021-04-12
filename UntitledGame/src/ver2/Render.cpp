#include "Render.h"

/*
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
*/

Render::Render(std::vector<ObjectContainer>& models_, LoadPrograms& programs_, Kamera& kamera_, LoadLights& lights_, LoadShadows& shadows_, SkyboxContainer skybox_) :
	models(models_),
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
	//InstrumentationTimer timer("Config assets");

	configVertexAttributes(object);
	getUniformLocations();
}

void Render::configVertexAttributes(ObjectContainer& object)
{
	//InstrumentationTimer timer("Config vertex attribs");

	glBindVertexArray(object.vao);
	glBindBuffer(GL_ARRAY_BUFFER, object.vbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, object.ibo);

	GLint positionAttribIndex = glGetAttribLocation(programID, "in_vertexPosition");
	GLint textureAttribIndex = glGetAttribLocation(programID, "in_textureCoords");
	GLint normalAttribIndex = glGetAttribLocation(programID, "in_normalVec");
	GLint tangentAttribIndex = glGetAttribLocation(programID, "in_tangent"); //
	GLint bitangentAttribIndex = glGetAttribLocation(programID, "in_bitangent"); //

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
	InstrumentationTimer timer("Get uniform locs");

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

	uniformLocations.IDloc = glGetUniformLocation(programID, "ID");

	//for (int i = 0; i < sizeof(lights.dirLights) / sizeof(lights.dirLights[0]); i++)
	for (int i = 0; i < lights.dirLights_.size(); i++)
	{
		/*
		uniformLocations.dirLightLocs_[i][0] = glGetUniformLocation(programID, ("dirLightArray[" + std::to_string(i) + "].direction").c_str());
		uniformLocations.dirLightLocs_[i][1] = glGetUniformLocation(programID, ("dirLightArray[" + std::to_string(i) + "].color").c_str());

		uniformLocations.dirLightLocs_[i][2] = glGetUniformLocation(programID, ("dirLightArray[" + std::to_string(i) + "].ambientStrength").c_str());
		uniformLocations.dirLightLocs_[i][3] = glGetUniformLocation(programID, ("dirLightArray[" + std::to_string(i) + "].diffuseStrength").c_str());
		uniformLocations.dirLightLocs_[i][4] = glGetUniformLocation(programID, ("dirLightArray[" + std::to_string(i) + "].specularStrength").c_str());

		uniformLocations.dirLightLocs_[i][5] = glGetUniformLocation(programID, ("dirLightArray[" + std::to_string(i) + "].shadowMap").c_str());
		uniformLocations.dirLightLocs_[i][6] = glGetUniformLocation(programID, ("dirLightArray[" + std::to_string(i) + "].lightSpaceMatrix").c_str());
		*/

		std::vector<int> temp;
		temp.push_back(glGetUniformLocation(programID, ("dirLightArray[" + std::to_string(i) + "].direction").c_str()));
		temp.push_back(glGetUniformLocation(programID, ("dirLightArray[" + std::to_string(i) + "].color").c_str()));

		temp.push_back(glGetUniformLocation(programID, ("dirLightArray[" + std::to_string(i) + "].ambientStrength").c_str()));
		temp.push_back(glGetUniformLocation(programID, ("dirLightArray[" + std::to_string(i) + "].diffuseStrength").c_str()));
		temp.push_back(glGetUniformLocation(programID, ("dirLightArray[" + std::to_string(i) + "].specularStrength").c_str()));

		temp.push_back(glGetUniformLocation(programID, ("dirLightArray[" + std::to_string(i) + "].shadowMap").c_str()));
		temp.push_back(glGetUniformLocation(programID, ("dirLightArray[" + std::to_string(i) + "].lightSpaceMatrix").c_str()));

		uniformLocations.dirLightLocs_.push_back(temp);
	}

	for (int i = 0; i < lights.pointLights_.size(); i++)
	{
		/*
		uniformLocations.pointLightLocs_[i][0] = glGetUniformLocation(programID, ("pointLightArray[" + std::to_string(i) + "].position").c_str());
		uniformLocations.pointLightLocs_[i][1] = glGetUniformLocation(programID, ("pointLightArray[" + std::to_string(i) + "].color").c_str());

		uniformLocations.pointLightLocs_[i][2] = glGetUniformLocation(programID, ("pointLightArray[" + std::to_string(i) + "].ambientStrength").c_str());
		uniformLocations.pointLightLocs_[i][3] = glGetUniformLocation(programID, ("pointLightArray[" + std::to_string(i) + "].diffuseStrength").c_str());
		uniformLocations.pointLightLocs_[i][4] = glGetUniformLocation(programID, ("pointLightArray[" + std::to_string(i) + "].specularStrength").c_str());

		uniformLocations.pointLightLocs_[i][5] = glGetUniformLocation(programID, ("pointLightArray[" + std::to_string(i) + "].constant").c_str());
		uniformLocations.pointLightLocs_[i][6] = glGetUniformLocation(programID, ("pointLightArray[" + std::to_string(i) + "].linear").c_str());
		uniformLocations.pointLightLocs_[i][7] = glGetUniformLocation(programID, ("pointLightArray[" + std::to_string(i) + "].quadratic").c_str());

		uniformLocations.pointLightLocs_[i][8] = glGetUniformLocation(programID, ("pointLightArray[" + std::to_string(i) + "].shadowBox").c_str());
		*/

		std::vector<int> temp;
		temp.push_back(glGetUniformLocation(programID, ("pointLightArray[" + std::to_string(i) + "].position").c_str()));
		temp.push_back(glGetUniformLocation(programID, ("pointLightArray[" + std::to_string(i) + "].color").c_str()));

		temp.push_back(glGetUniformLocation(programID, ("pointLightArray[" + std::to_string(i) + "].ambientStrength").c_str()));
		temp.push_back(glGetUniformLocation(programID, ("pointLightArray[" + std::to_string(i) + "].diffuseStrength").c_str()));
		temp.push_back(glGetUniformLocation(programID, ("pointLightArray[" + std::to_string(i) + "].specularStrength").c_str()));

		temp.push_back(glGetUniformLocation(programID, ("pointLightArray[" + std::to_string(i) + "].constant").c_str()));
		temp.push_back(glGetUniformLocation(programID, ("pointLightArray[" + std::to_string(i) + "].linear").c_str()));
		temp.push_back(glGetUniformLocation(programID, ("pointLightArray[" + std::to_string(i) + "].quadratic").c_str()));

		temp.push_back(glGetUniformLocation(programID, ("pointLightArray[" + std::to_string(i) + "].shadowBox").c_str()));

		uniformLocations.pointLightLocs_.push_back(temp);
	}

	for (int i = 0; i < lights.spotLights.size(); i++)
	{
		std::vector<int> temp;
		temp.push_back(glGetUniformLocation(programID, ("spotLightArray[" + std::to_string(i) + "].position").c_str()));
		temp.push_back(glGetUniformLocation(programID, ("spotLightArray[" + std::to_string(i) + "].direction").c_str()));
		temp.push_back(glGetUniformLocation(programID, ("spotLightArray[" + std::to_string(i) + "].color").c_str()));

		temp.push_back(glGetUniformLocation(programID, ("spotLightArray[" + std::to_string(i) + "].cutOffCos").c_str()));

		temp.push_back(glGetUniformLocation(programID, ("spotLightArray[" + std::to_string(i) + "].ambientStrength").c_str()));
		temp.push_back(glGetUniformLocation(programID, ("spotLightArray[" + std::to_string(i) + "].diffuseStrength").c_str()));
		temp.push_back(glGetUniformLocation(programID, ("spotLightArray[" + std::to_string(i) + "].specularStrength").c_str()));

		temp.push_back(glGetUniformLocation(programID, ("spotLightArray[" + std::to_string(i) + "].constant").c_str()));
		temp.push_back(glGetUniformLocation(programID, ("spotLightArray[" + std::to_string(i) + "].linear").c_str()));
		temp.push_back(glGetUniformLocation(programID, ("spotLightArray[" + std::to_string(i) + "].quadratic").c_str()));

		temp.push_back(glGetUniformLocation(programID, ("spotLightArray[" + std::to_string(i) + "].shadowMap").c_str()));
		temp.push_back(glGetUniformLocation(programID, ("spotLightArray[" + std::to_string(i) + "].lightSpaceMatrix").c_str()));

		uniformLocations.spotLightLocs.push_back(temp);
	}
}


void Render::renderAsset(ObjectContainer& object)
{
	int k = 0;
	glUseProgram(programID);

	uploadUniforms(object);

	// bind textures
	glBindVertexArray(object.vao);

	//glActiveTexture(GL_TEXTURE0);
	glActiveTexture(GL_TEXTURE0); k++;
	glBindTexture(GL_TEXTURE_2D, object.material.diffuseMap);

	//glActiveTexture(GL_TEXTURE1);
	glActiveTexture(GL_TEXTURE0 + k); k++;
	glBindTexture(GL_TEXTURE_2D, object.material.specularMap);

	//glActiveTexture(GL_TEXTURE2);
	glActiveTexture(GL_TEXTURE0 + k); k++;
	glBindTexture(GL_TEXTURE_2D, object.material.normalMap);

	//glActiveTexture(GL_TEXTURE3);
	glActiveTexture(GL_TEXTURE0 + k); k++;
	glBindTexture(GL_TEXTURE_CUBE_MAP, skybox.ID);

	
	for (int i = 4, j = 0; i < (lights.dirLights_.size() + 4); i++, j++)
	{
		//glActiveTexture(GL_TEXTURE0 + i);
		glActiveTexture(GL_TEXTURE0 + k); k++;
		glBindTexture(GL_TEXTURE_2D, shadows.dirShadows_[j].shadowMap);
	}

	for (int i = (lights.dirLights_.size() + 4), j = 0; i < (lights.dirLights_.size() + lights.pointLights_.size() + 4); i++, j++)
	{
		//glActiveTexture(GL_TEXTURE0 + i);
		glActiveTexture(GL_TEXTURE0 + k); k++;
		glBindTexture(GL_TEXTURE_CUBE_MAP, shadows.pointShadows_[j].shadowCube);
	}
	
	for (int i = 0; i < lights.spotLights.size(); i++)
	{
		glActiveTexture(GL_TEXTURE0 + k); k++;
		glBindTexture(GL_TEXTURE_2D, shadows.spotShadows[i].shadowMap);
	}

	glDrawElements(GL_TRIANGLES, object.data.indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

void Render::uploadUniforms(ObjectContainer& object)
{
	int c = 0;
	glm::mat4 MVP = updateMVP(object.modelMatrix, kamera.cameraContainer.viewMatrix, kamera.cameraContainer.projectionMatrix);

	// vertex shader
	glUniformMatrix4fv(uniformLocations.MVPloc, 1, GL_FALSE, glm::value_ptr(MVP));
	glUniformMatrix4fv(uniformLocations.modelMatrixLoc, 1, GL_FALSE, glm::value_ptr(object.modelMatrix));

	// fragment shader
	//glUniform1i(uniformLocations.diffuseMapLoc, 0);	// tex
	//glUniform1i(uniformLocations.specularMapLoc, 1);	// tex
	//glUniform1i(uniformLocations.normalMapLoc, 2);	// tex
	glUniform1i(uniformLocations.diffuseMapLoc, c);	// tex
	glUniform1i(uniformLocations.specularMapLoc, ++c);	// tex
	glUniform1i(uniformLocations.normalMapLoc, ++c);	// tex
	glUniform1f(uniformLocations.shininessLoc, object.material.shininess);

	if (object.material.normalMap == 0)
		glUniform1i(uniformLocations.hasNormalMapLoc, 0);	// false, object doesn't have normal map
	else
		glUniform1i(uniformLocations.hasNormalMapLoc, 1);	// true, object has normal map

	glUniform3fv(uniformLocations.cameraPosLoc, 1, glm::value_ptr(kamera.cameraContainer.cameraPosition));
	glUniform1f(uniformLocations.cameraFarPlaneLoc, kamera.cameraContainer.farPlane);

	//glUniform1i(uniformLocations.skyboxLoc, 3);	// tex
	glUniform1i(uniformLocations.skyboxLoc, ++c);	// tex

	glUniform1i(uniformLocations.IDloc, object.ID);

	//for (int i = 0; i < sizeof(lights.dirLights) / sizeof(lights.dirLights[0]); i++)
	for (int i = 0; i < lights.dirLights_.size(); i++)
	{
		glm::vec3 direction = lights.dirLights_[i].target - lights.dirLights_[i].position;
		glUniform3fv(uniformLocations.dirLightLocs_[i][0], 1, glm::value_ptr(direction));
		glUniform3fv(uniformLocations.dirLightLocs_[i][1], 1, glm::value_ptr(lights.dirLights_[i].color));

		glUniform1f(uniformLocations.dirLightLocs_[i][2], lights.dirLights_[i].ambientStrength);
		glUniform1f(uniformLocations.dirLightLocs_[i][3], lights.dirLights_[i].diffuseStrength);
		glUniform1f(uniformLocations.dirLightLocs_[i][4], lights.dirLights_[i].specularStrength);

		//glUniform1i(uniformLocations.dirLightLocs_[i][5], 4 + i);  // tex
		glUniform1i(uniformLocations.dirLightLocs_[i][5], ++c);  // tex
		glUniformMatrix4fv(uniformLocations.dirLightLocs_[i][6], 1, GL_FALSE, glm::value_ptr(lights.dirLights_[i].lightSpaceMatrix));
	}

	for (int i = 0; i < lights.pointLights_.size(); i++)
	{
		glUniform3fv(uniformLocations.pointLightLocs_[i][0], 1, glm::value_ptr(lights.pointLights_[i].position));
		glUniform3fv(uniformLocations.pointLightLocs_[i][1], 1, glm::value_ptr(lights.pointLights_[i].color));

		glUniform1f(uniformLocations.pointLightLocs_[i][2], lights.pointLights_[i].ambientStrength);
		glUniform1f(uniformLocations.pointLightLocs_[i][3], lights.pointLights_[i].diffuseStrength);
		glUniform1f(uniformLocations.pointLightLocs_[i][4], lights.pointLights_[i].specularStrength);

		glUniform1f(uniformLocations.pointLightLocs_[i][5], lights.pointLights_[i].constant);
		glUniform1f(uniformLocations.pointLightLocs_[i][6], lights.pointLights_[i].linear);
		glUniform1f(uniformLocations.pointLightLocs_[i][7], lights.pointLights_[i].quadratic);

		//glUniform1i(uniformLocations.pointLightLocs_[i][8], 4 + lights.dirLights_.size() + i);  // tex
		glUniform1i(uniformLocations.pointLightLocs_[i][8], ++c);  // tex
	}

	for (int i = 0; i < lights.spotLights.size(); i++)
	{
		glm::vec3 direction = lights.spotLights[i].target - lights.spotLights[i].position;
		glUniform3fv(uniformLocations.spotLightLocs[i][0], 1, glm::value_ptr(lights.spotLights[i].position));
		glUniform3fv(uniformLocations.spotLightLocs[i][1], 1, glm::value_ptr(direction));
		glUniform3fv(uniformLocations.spotLightLocs[i][2], 1, glm::value_ptr(lights.spotLights[i].color));

		float cutOffCos = glm::cos(glm::radians(lights.spotLights[i].cutOffAngle));
		glUniform1f(uniformLocations.spotLightLocs[i][3], cutOffCos);

		glUniform1f(uniformLocations.spotLightLocs[i][4], lights.spotLights[i].ambientStrength);
		glUniform1f(uniformLocations.spotLightLocs[i][5], lights.spotLights[i].diffuseStrength);
		glUniform1f(uniformLocations.spotLightLocs[i][6], lights.spotLights[i].specularStrength);

		glUniform1f(uniformLocations.spotLightLocs[i][7], lights.spotLights[i].constant);
		glUniform1f(uniformLocations.spotLightLocs[i][8], lights.spotLights[i].linear);
		glUniform1f(uniformLocations.spotLightLocs[i][9], lights.spotLights[i].quadratic);

		glUniform1i(uniformLocations.spotLightLocs[i][10], ++c);  // tex
		glUniformMatrix4fv(uniformLocations.spotLightLocs[i][11], 1, GL_FALSE, glm::value_ptr(lights.spotLights[i].lightSpaceMatrix));
	}
}

glm::mat4 Render::updateMVP(glm::mat4 M, glm::mat4 V, glm::mat4 P)
{
	return P * V * M;
}

void Render::generateIDs()
{
	int ID = 0;

	/*
	for (int i = 0; i < NUMBER_OF_OBJECTS; i++)
	{
		assets.models[i].ID = ID;
		ID++;
	}
	*/

	for (int i = 0; i < models.size(); i++)  // new
	{
		std::cout << "model name: " << models[i].name << std::endl;
		models[i].ID = ID;
		std::cout << "model ID: " << models[i].ID << std::endl;
		ID++;
	}

	for (int i = 0; i < lights.dirLights_.size(); i++)
	{

	}

	for (int i = 0; i < lights.pointLights_.size(); i++)
	{

	}
}

void Render::configAssets()
{
	//for (int i = 0; i < sizeof(assets.models) / sizeof(assets.models[0]); i++)
	/*
	for (int i = 0; i < NUMBER_OF_OBJECTS; i++)
	{
		configAsset(assets.models[i]);
	}
	*/

	for (int i = 0; i < models.size(); i++)  // new
	{
		configAsset(models[i]);
	}

	generateIDs();

	std::cout << "OK: Assets are configured and IDs are generated." << std::endl;
}

void Render::renderAssets(Kamera& kamera_)
{
	//InstrumentationTimer timer("Render models");

	kamera = kamera_;
	//glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
	//glBindFramebuffer(GL_FRAMEBUFFER, 0);
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//glEnable(GL_DEPTH_TEST);

	//for (int i = 0; i < sizeof(assets.models) / sizeof(assets.models[0]); i++)
	/*
	for (int i = 0; i < NUMBER_OF_OBJECTS; i++)
	{
		renderAsset(assets.models[i]);
	}
	*/

	for (int i = 0; i < models.size(); i++)  // new
	{
		renderAsset(models[i]);
	}
}

void Render::changeParams(int selectedID)
{
	/*
	ImGui::Begin("rotate selected object");
	float angle = 0.0f;
	ImGui::SliderFloat("angle", &angle, 0.0f, 1.0f);
	ImGui::End();
	*/
	
	
	if (selectedID < models.size())
	{
		// draw gui
		ImGui::Begin("rotate selected");

		float angle = 0.0f;
		ImGui::SliderAngle("angle", &angle, 0.0f, 360.0f);

		// change model matrix
		glm::mat4 modelMatrix = models[selectedID].modelMatrix;
		modelMatrix = glm::rotate(modelMatrix, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));  // zero rotation
		modelMatrix = glm::rotate(modelMatrix, glm::radians(angle), glm::vec3(0.0f, 1.0f, 0.0f));
		models[selectedID].modelMatrix = modelMatrix;

		ImGui::End();
	}
	
}

