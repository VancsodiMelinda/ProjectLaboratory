#include "LoadAssets.h"

LoadAssets::LoadAssets()
{
	loadDiffuseMaps();		// load all diffuse maps
	loadSpecularMaps();		// load all spcular maps
	loadObjects();			// load all models and assign texture maps
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

	CreateTexture tex2("resources/spacular maps/black.png", TextureType::specularMap);
	specularMaps[1] = tex2.textureContainer;
}

void LoadAssets::loadObjects()
{
	CreateModel cube("resources/RubiksCube.obj", glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), 0.0f, "x");
	cube.objectContainer.material.diffuseMap = diffuseMaps[0].ID;
	cube.objectContainer.material.specularMap = specularMaps[0].ID;
	cube.objectContainer.material.shininess = 0.0f;
	models[0] = cube.objectContainer;
}

void LoadAssets::config(ObjectContainer& object, GLuint programID)
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
	defaultUniformLocs.modelMatrixLoc = glGetUniformLocation(programID, "modelMatrix");
	defaultUniformLocs.diffuseMapLoc = glGetUniformLocation(programID, "material.diffuseMap");
	defaultUniformLocs.specularMapLoc = glGetUniformLocation(programID, "material.specularMap");
	defaultUniformLocs.shininessLoc = glGetUniformLocation(programID, "material.shininess");
}

void LoadAssets::render(ObjectContainer& object, GLuint programID)
{
	glUseProgram(programID);

	// upload uniforms
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
