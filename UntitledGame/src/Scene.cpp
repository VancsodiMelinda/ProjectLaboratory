#include "Scene.h"


Scene::Scene(GLuint shaderID_, GLuint vao_, GLuint vbo_, GLuint ibo_, objectData data_, glm::mat4 modelMatrix_,
	glm::vec3 lightColor_, glm::vec3 lightPos_, glm::mat4 lightSpaceMatrix_, Camera camera_, int windowWidth, int windowHeight, GLuint textureID_, GLuint shadowMap_)
{
	shaderID = shaderID_;
	vao = vao_;
	vbo = vbo_;
	ibo = ibo_;
	data = data_;
	modelMatrix = modelMatrix_;
	lightColor = lightColor_;
	lightPos = lightPos_;
	lightSpaceMatrix = lightSpaceMatrix_;
	camera = camera_;
	WINDOW_WIDTH = windowWidth;
	WINDOW_HEIGHT = windowHeight;
	textureID = textureID_;
	shadowMap = shadowMap_;
}


void Scene::initialize()
{
	configVertexAttributes();
	getUniformLocations();
}


void Scene::configVertexAttributes()
{
	glBindVertexArray(vao);  // bind VAO
	glBindBuffer(GL_ARRAY_BUFFER, vbo);  // bind VBO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);  // bind IBO

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
	GLintptr tOffset = data.vertices.size() * sizeof(GL_FLOAT);
	GLintptr nOffset = (data.vertices.size() + data.uvs.size()) * sizeof(GL_FLOAT);
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


void Scene::getUniformLocations()
{
	// GET UNIFORM LOCATION
	// vertex shader
	int MVPloc = glGetUniformLocation(shaderID, "MVP");
	int modelMatrixLoc = glGetUniformLocation(shaderID, "modelMatrix");
	int lightSpaceMatrixLoc = glGetUniformLocation(shaderID, "lightSpaceMatrix");

	// fragment shader
	int lightColorLoc = glGetUniformLocation(shaderID, "lightColor");
	int lightPosLoc = glGetUniformLocation(shaderID, "lightPos");
	int cameraPosLoc = glGetUniformLocation(shaderID, "cameraPos");

	int texLoc = glGetUniformLocation(shaderID, "tex");
	int shadowMapLoc = glGetUniformLocation(shaderID, "shadowMap");

	int ambientLoc = glGetUniformLocation(shaderID, "material.ambient");		// new
	int diffuseLoc = glGetUniformLocation(shaderID, "material.diffuse");		// new
	int specularLoc = glGetUniformLocation(shaderID, "material.specular");		// new
	int shininessLoc = glGetUniformLocation(shaderID, "material.shininess");	// new

	int ambientStrengthLoc = glGetUniformLocation(shaderID, "light.ambientStrength");	// new
	int diffuseStrengthLoc = glGetUniformLocation(shaderID, "light.diffuseStrength");	// new
	int specularStrengthLoc = glGetUniformLocation(shaderID, "light.specularStrength");	// new

	// SAVE UNIFORM LOCATION INTO STRUCT
	uniLocs.MVPloc = MVPloc;
	uniLocs.modelMatrixLoc = modelMatrixLoc;
	uniLocs.lightSpaceMatrixLoc = lightSpaceMatrixLoc;
	uniLocs.lightColorLoc = lightColorLoc;
	uniLocs.lightPosLoc = lightPosLoc;
	uniLocs.cameraPosLoc = cameraPosLoc;
	uniLocs.texLoc = texLoc;
	uniLocs.shadowMapLoc = shadowMapLoc;
	uniLocs.ambientLoc = ambientLoc;		// new
	uniLocs.diffuseLoc = diffuseLoc;		// new
	uniLocs.specularLoc = specularLoc;		// new
	uniLocs.shininessLoc = shininessLoc;	// new
	uniLocs.ambientStrengthLoc = ambientStrengthLoc;	// new
	uniLocs.diffuseStrengthLoc = diffuseStrengthLoc;	// new
	uniLocs.specularStrengthLoc = specularStrengthLoc;	// new
}

////////////////////////////////////////////////////////////////////////////

void Scene::render(Camera camera_)
{
	camera = camera_;
	glUseProgram(shaderID);
	updateMVP();
	uploadUniforms();

	glBindVertexArray(vao);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureID);  // bind texture
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, shadowMap);  // bind texture

	glDrawElements(GL_TRIANGLES, data.indices.size(), GL_UNSIGNED_INT, 0);
}

void Scene::updateMVP()  // every frame
{
	glm::mat4 viewMatrix = camera.CreateViewMatrix();  // update in every frame (WASD and mouse)
	glm::mat4 projectionMatrix = glm::perspective(glm::radians(camera.fov), (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT, 0.1f, 100.0f);  // update in every frame (zoom)
	//glm::mat4 projectionMatrix = glm::ortho(-2.0f, 2.0f, -1.5f, 1.5f, 0.1f, 100.0f);

	MVP = projectionMatrix * viewMatrix * modelMatrix;
}

void Scene::uploadUniforms()
{
	// vertex shader
	glUniformMatrix4fv(uniLocs.MVPloc, 1, GL_FALSE, glm::value_ptr(MVP));
	glUniformMatrix4fv(uniLocs.modelMatrixLoc, 1, GL_FALSE, glm::value_ptr(modelMatrix));
	glUniformMatrix4fv(uniLocs.lightSpaceMatrixLoc, 1, GL_FALSE, value_ptr(lightSpaceMatrix));

	// fragment shader
	glUniform3fv(uniLocs.lightColorLoc, 1, glm::value_ptr(lightColor));
	glUniform3fv(uniLocs.lightPosLoc, 1, glm::value_ptr(lightPos));
	glUniform3fv(uniLocs.cameraPosLoc, 1, glm::value_ptr(camera.cameraPosition));

	glUniform1i(uniLocs.texLoc, 0);
	glUniform1i(uniLocs.shadowMapLoc, 1);

	glm::vec3 a = glm::vec3(0.0215f, 0.1745f, 0.0215f);		// new
	glm::vec3 d = glm::vec3(0.07568f, 0.61424f, 0.07568f);	// new
	glm::vec3 s = glm::vec3(0.633f, 0.727811f, 0.633f);		// new
	float shine = 0.6f * 128.0f;		// new

	glUniform3fv(uniLocs.ambientLoc, 1, glm::value_ptr(a));		// new
	glUniform3fv(uniLocs.diffuseLoc, 1, glm::value_ptr(d));		// new
	glUniform3fv(uniLocs.specularLoc, 1, glm::value_ptr(s));	// new
	glUniform1f(uniLocs.shininessLoc, shine);		// new

	glUniform1f(uniLocs.ambientStrengthLoc, 0.2f);		// new
	glUniform1f(uniLocs.diffuseStrengthLoc, 0.5f);		// new
	glUniform1f(uniLocs.specularStrengthLoc, 1.0f);		// new
}


