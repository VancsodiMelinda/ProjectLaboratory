#include "LoadAssets.h"

LoadAssets::LoadAssets()
{
	std::cout << "Loading textures..." << std::endl;
	loadDiffuseMaps();		// load all diffuse maps
	loadSpecularMaps();		// load all spcular maps
	std::cout << std::endl << "Loading models..." << std::endl;
	loadObjects();			// load all models and assign texture maps
	std::cout << std::endl;
}

void LoadAssets::loadDiffuseMaps()
{
	CreateTexture tex1(TextureType::diffuseMap);  // load default diffuse map
	diffuseMaps[0] = tex1.textureContainer;

	CreateTexture tex2("resources/diffuse maps/color grid.png", TextureType::diffuseMap);
	diffuseMaps[1] = tex2.textureContainer;

	CreateTexture tex3("resources/diffuse maps/uv grid.png", TextureType::diffuseMap);
	diffuseMaps[2] = tex3.textureContainer;
}

void LoadAssets::loadSpecularMaps()
{
	CreateTexture tex1(TextureType::specularMap);  // load default spacular map
	specularMaps[0] = tex1.textureContainer;

	CreateTexture tex2("resources/specular maps/black.png", TextureType::specularMap);
	specularMaps[1] = tex2.textureContainer;
}

void LoadAssets::loadObjects()
{
	CreateModel cube("resources/models/cube.obj", glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), 0.0f, "y");
	cube.objectContainer.material.diffuseMap = diffuseMaps[0].ID;
	cube.objectContainer.material.specularMap = specularMaps[1].ID;
	cube.objectContainer.material.shininess = 0.6f * 128.0f;
	models[0] = cube.objectContainer;

	CreateModel sphere("resources/models/sphere.obj", glm::vec3(-3.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), 0.0f, "y");
	sphere.objectContainer.material.diffuseMap = diffuseMaps[1].ID;
	sphere.objectContainer.material.specularMap = specularMaps[0].ID;
	sphere.objectContainer.material.shininess = 0.6f * 128.0f;
	models[1] = sphere.objectContainer;

	CreateModel suzanne("resources/models/suzanne.obj", glm::vec3(3.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), 0.0f, "y");
	suzanne.objectContainer.material.diffuseMap = diffuseMaps[2].ID;
	suzanne.objectContainer.material.specularMap = specularMaps[0].ID;
	suzanne.objectContainer.material.shininess = 0.6f * 128.0f;
	models[2] = suzanne.objectContainer;

	CreateModel plane("resources/models/plane.obj", glm::vec3(0.0f, -1.5f, 0.0f), glm::vec3(8.0f, 8.0f, 8.0f), 0.0f, "y");
	plane.objectContainer.material.diffuseMap = diffuseMaps[2].ID;
	plane.objectContainer.material.specularMap = specularMaps[0].ID;
	plane.objectContainer.material.shininess = 0.6f * 128.0f;
	models[3] = plane.objectContainer;
}

void LoadAssets::configAsset(ObjectContainer& object, GLuint programID)
{
	//// CONFIGURE VERTEX ATTRIBUTES
	glBindVertexArray(object.vao);
	glBindBuffer(GL_ARRAY_BUFFER, object.vbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, object.ibo);

	GLuint positionAttribIndex = glGetAttribLocation(programID, "in_vertexPosition");
	GLuint textureAttribIndex = glGetAttribLocation(programID, "in_textureCoords");
	GLuint normalAttribIndex = glGetAttribLocation(programID, "in_normalVec");

	glEnableVertexAttribArray(positionAttribIndex);
	glEnableVertexAttribArray(textureAttribIndex);
	glEnableVertexAttribArray(normalAttribIndex);

	GLintptr vOffset = 0 * sizeof(GL_FLOAT);
	GLintptr tOffset = object.data.vertices.size() * sizeof(GL_FLOAT);
	GLintptr nOffset = (object.data.vertices.size() + object.data.uvs.size()) * sizeof(GL_FLOAT);
	int stride3f = 3 * sizeof(GL_FLOAT);
	int stride2f = 2 * sizeof(GL_FLOAT);

	glVertexAttribPointer(positionAttribIndex, 3, GL_FLOAT, GL_FALSE, stride3f, (GLvoid*)vOffset);
	glVertexAttribPointer(textureAttribIndex, 2, GL_FLOAT, GL_FALSE, stride2f, (GLvoid*)tOffset);
	glVertexAttribPointer(normalAttribIndex, 3, GL_FLOAT, GL_FALSE, stride3f, (GLvoid*)nOffset);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	//// GET UNIFORM LOCATIONS
	// vertex shader
	defaultUniformLocs.MVPloc = glGetUniformLocation(programID, "MVP");
	defaultUniformLocs.modelMatrixLoc = glGetUniformLocation(programID, "modelMatrix");
	// fragment shader
	defaultUniformLocs.diffuseMapLoc = glGetUniformLocation(programID, "material.diffuseMap");
	defaultUniformLocs.specularMapLoc = glGetUniformLocation(programID, "material.specularMap");
	defaultUniformLocs.shininessLoc = glGetUniformLocation(programID, "material.shininess");
}

void LoadAssets::renderAsset(ObjectContainer& object, GLuint programID, Kamera& kamera)
{
	//glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glUseProgram(programID);

	glm::mat4 MVP = updateMVP(object.modelMatrix, kamera.cameraContainer.viewMatrix, kamera.cameraContainer.projectionMatrix);

	// upload uniforms
	glUniformMatrix4fv(defaultUniformLocs.MVPloc, 1, GL_FALSE, glm::value_ptr(MVP));
	glUniformMatrix4fv(defaultUniformLocs.modelMatrixLoc, 1, GL_FALSE, glm::value_ptr(object.modelMatrix));
	glUniform1i(defaultUniformLocs.diffuseMapLoc, 0);
	glUniform1i(defaultUniformLocs.specularMapLoc, 1);
	glUniform1f(defaultUniformLocs.shininessLoc, object.material.shininess);

	// bind textures
	glBindVertexArray(object.vao);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, object.material.diffuseMap);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, object.material.specularMap);

	glDrawElements(GL_TRIANGLES, object.data.indices.size(), GL_UNSIGNED_INT, 0);
}

glm::mat4 LoadAssets::updateMVP(glm::mat4 M, glm::mat4 V, glm::mat4 P)
{
	return P * V * M;
}

void LoadAssets::config(GLuint programID)
{
	for (int i = 0; i < sizeof(models) / sizeof(models[0]); i++)
	{
		configAsset(models[i], programID);
	}
}

void LoadAssets::render(GLuint programID, Kamera& kamera)
{
	for (int i = 0; i < sizeof(models) / sizeof(models[0]); i++)
	{
		renderAsset(models[i], programID, kamera);
	}
}
