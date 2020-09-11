#include "Render.h"

Render::Render(LoadAssets& assets_, LoadPrograms& programs_, Kamera& kamera_, LoadLights& lights_) :
	assets(assets_), programs(programs_), kamera(kamera_), lights(lights_)
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

	glDrawElements(GL_TRIANGLES, object.data.indices.size(), GL_UNSIGNED_INT, 0);
}

void Render::uploadUniforms(ObjectContainer& object)
{
	glm::mat4 MVP = updateMVP(object.modelMatrix, kamera.cameraContainer.viewMatrix, kamera.cameraContainer.projectionMatrix);

	// vertex shader
	glUniformMatrix4fv(uniformLocations.MVPloc, 1, GL_FALSE, glm::value_ptr(MVP));
	glUniformMatrix4fv(uniformLocations.modelMatrixLoc, 1, GL_FALSE, glm::value_ptr(object.modelMatrix));

	// fragment shader
	glUniform1i(uniformLocations.diffuseMapLoc, 0);
	glUniform1i(uniformLocations.specularMapLoc, 1);
	glUniform1f(uniformLocations.shininessLoc, object.material.shininess);
}

glm::mat4 Render::updateMVP(glm::mat4 M, glm::mat4 V, glm::mat4 P)
{
	return P * V * M;
}

void Render::config()
{
	for (int i = 0; i < sizeof(assets.models) / sizeof(assets.models[0]); i++)
	{
		configAsset(assets.models[i]);
	}
}

void Render::render(Kamera& kamera_)
{
	kamera = kamera_;
	for (int i = 0; i < sizeof(assets.models) / sizeof(assets.models[0]); i++)
	{
		renderAsset(assets.models[i]);
	}
}
