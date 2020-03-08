#include "Scene2.h"

Scene2::Scene2(Data& object_, glm::mat4 modelMatrix_, GLuint shaderID_, Camera& camera_,
	GLuint diffuseMap_, GLuint specularMap_, GLuint shadowMap_, PointLight& pointLight_) :
	object(object_),
	camera(camera_),
	pointLight(pointLight_)
{
	modelMatrix = modelMatrix_;
	shaderID = shaderID_;

	diffuseMap = diffuseMap_;
	specularMap = specularMap_;
	shadowMap = shadowMap_;

	uniforms = { 0 };
}


void Scene2::initialize()
{
	configVertexAttributes();
	getUniformLocations();
}

void Scene2::configVertexAttributes()
{
	glBindVertexArray(object.vao);  // bind VAO
	glBindBuffer(GL_ARRAY_BUFFER, object.vbo);  // bind VBO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, object.ibo);  // bind IBO

	// get attribute indexes from the relevant shader
	GLuint positionAttribIndex = glGetAttribLocation(shaderID, "in_vertexPosition");	// layout (location = 0) in vec3 in_vertexPosition;
	GLuint textureAttribIndex = glGetAttribLocation(shaderID, "in_textureCoords");		// layout (location = 1) in vec2 in_textureCoords;
	GLuint normalAttribIndex = glGetAttribLocation(shaderID, "in_normalVec");			// layout (location = 2) in vec3 in_normalVec;

	// enable vertex attribute array - uses currently bound VAO
	glEnableVertexAttribArray(positionAttribIndex);
	glEnableVertexAttribArray(textureAttribIndex);
	glEnableVertexAttribArray(normalAttribIndex);

	// calculate offsets
	GLintptr vOffset = 0 * sizeof(GL_FLOAT);
	GLintptr tOffset = object.data.vertices.size() * sizeof(GL_FLOAT);
	GLintptr nOffset = (object.data.vertices.size() + object.data.uvs.size()) * sizeof(GL_FLOAT);
	int stride3f = 3 * sizeof(GL_FLOAT);
	int stride2f = 2 * sizeof(GL_FLOAT);

	// setup the pointers - needs the currently bound VBO
	glVertexAttribPointer(positionAttribIndex, 3, GL_FLOAT, GL_FALSE, stride3f, (GLvoid*)vOffset);
	glVertexAttribPointer(textureAttribIndex, 2, GL_FLOAT, GL_FALSE, stride2f, (GLvoid*)tOffset);
	glVertexAttribPointer(normalAttribIndex, 3, GL_FLOAT, GL_FALSE, stride3f, (GLvoid*)nOffset);

	glBindVertexArray(0);  // unbind VAO
	glBindBuffer(GL_ARRAY_BUFFER, 0);  // unbind VBO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);  // unbind IBO
}

void Scene2::getUniformLocations()
{
	// VERTEX SHADER
	uniforms.MVPloc = glGetUniformLocation(shaderID, "MVP");
	uniforms.modelMatrixLoc = glGetUniformLocation(shaderID, "modelMatrix");
	uniforms.lightSpaceMatrixLoc = glGetUniformLocation(shaderID, "lightSpaceMatrix");

	// FRAGMENT SHADER
	uniforms.cameraPosLoc = glGetUniformLocation(shaderID, "cameraPos");

	// material
	uniforms.diffuseMapLoc = glGetUniformLocation(shaderID, "material.diffuseMap");
	uniforms.specularMapLoc = glGetUniformLocation(shaderID, "material.specularMap");
	uniforms.shininessLoc = glGetUniformLocation(shaderID, "material.shininess");

	// shadow
	uniforms.shadowMapLoc = glGetUniformLocation(shaderID, "shadowMap");

	pointLight.getObjectUniformLocations(shaderID);
}

void Scene2::render(Camera& camera_)
{
	camera = camera_;
	glUseProgram(shaderID);
	updateMVP();
	uploadUniforms();

	glBindVertexArray(object.vao);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, diffuseMap);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, specularMap);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, shadowMap);

	glDrawElements(GL_TRIANGLES, object.data.indices.size(), GL_UNSIGNED_INT, 0);
}


void Scene2::updateMVP()
{
	glm::mat4 viewMatrix = camera.CreateViewMatrix();  // update in every frame (WASD and mouse)
	glm::mat4 projectionMatrix = glm::perspective(glm::radians(camera.fov), (float)windowWidth / (float)windowHeight, 0.1f, 100.0f);  // update in every frame (zoom)
	//glm::mat4 projectionMatrix = glm::ortho(-2.0f, 2.0f, -1.5f, 1.5f, 0.1f, 100.0f);

	MVP = projectionMatrix * viewMatrix * modelMatrix;
}


void Scene2::uploadUniforms()
{
	// vertex shader
	glUniformMatrix4fv(uniforms.MVPloc, 1, GL_FALSE, glm::value_ptr(MVP));
	glUniformMatrix4fv(uniforms.modelMatrixLoc, 1, GL_FALSE, glm::value_ptr(modelMatrix));
	//glUniformMatrix4fv(uniforms.lightSpaceMatrixLoc, 1, GL_FALSE, value_ptr(lightSpaceMatrix));
	glUniformMatrix4fv(uniforms.lightSpaceMatrixLoc, 1, GL_FALSE, value_ptr(pointLight.calculateLightSpaceMatrix(modelMatrix)));

	// fragment shader
	glUniform3fv(uniforms.cameraPosLoc, 1, glm::value_ptr(camera.cameraPosition));

	glUniform1i(uniforms.diffuseMapLoc, 0);
	glUniform1i(uniforms.specularMapLoc, 1);
	float shine = 0.6f * 128.0f;
	glUniform1f(uniforms.shininessLoc, shine);

	glUniform1i(uniforms.shadowMapLoc, 2);

	pointLight.uploadObjectUniforms();
}
