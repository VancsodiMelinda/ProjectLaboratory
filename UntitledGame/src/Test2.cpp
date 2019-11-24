#include "Test2.h"


Test2::Test2(GLuint shaderID_, GLuint vao_, GLuint vbo_, GLuint ibo_, objectData data_,
	glm::mat4 modelMatrix_, glm::mat4 viewMatrix_, glm::mat4 projectionMatrix_)
{
	shaderID = shaderID_;
	vao = vao_;
	vbo = vbo_;
	ibo = ibo_;
	data = data_;
	modelMatrix = modelMatrix_;
	viewMatrix = viewMatrix_;
	projectionMatrix = projectionMatrix_;
	MVP = projectionMatrix_ * viewMatrix_ * modelMatrix_;

	/*
	camera = camera_;
	textureID = textureID_;
	WINDOW_WIDTH = windowWidth;
	WINDOW_HEIGHT = windowHeight;
	lightColor = lightColor_;
	lightPos = lightPos_;
	*/
}

void Test2::initialize()
{
	configVertexAttributes();
	getUniformLocations();
}

void Test2::createMVP()  // only once
{
	// create view matrix
	glm::mat4 viewMatrix = camera.CreateViewMatrix();  // create viewMatrix with default parameters

	// create projection matrix
	glm::mat4 projectionMatrix = glm::perspective(glm::radians(camera.fov), (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT, 0.1f, 100.0f);  // PERSPECTIVE PROJECTION
	//glm::mat4 projectionMatrix = glm::ortho(-2.0f, 2.0f, -1.5f, 1.5f, 0.1f, 100.0f);

	// create MVP
	MVP = projectionMatrix * viewMatrix * modelMatrix;

	//std::cout << "5. In Object Class MVP has been created." << std::endl;
}

void Test2::configVertexAttributes()
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

	std::cout << "_____________________________________________" << std::endl;
	std::cout << "4. In TEST2 Class vertex attributes have been configured." << std::endl;
	std::cout << "positionAttribIndex: " << positionAttribIndex << std::endl;
	std::cout << "textureAttribIndex: " << textureAttribIndex << std::endl;
	std::cout << "normalAttribIndex: " << normalAttribIndex << std::endl;
	std::cout << "_____________________________________________" << std::endl;
}

void Test2::getUniformLocations()
{
	int MVPloc = glGetUniformLocation(shaderID, "MVP");
	int modelMatrixLoc = glGetUniformLocation(shaderID, "modelMatrix");

	int lightColorLoc = glGetUniformLocation(shaderID, "lightColor");
	int lightPosLoc = glGetUniformLocation(shaderID, "lightPos");

	int cameraPosLoc = glGetUniformLocation(shaderID, "cameraPos");

	uniLocs.MVPloc = MVPloc;
	uniLocs.modelMatrixLoc = modelMatrixLoc;
	uniLocs.lightColorLoc = lightColorLoc;
	uniLocs.lightPosLoc = lightPosLoc;
	uniLocs.cameraPosLoc = cameraPosLoc;

	std::cout << "________________________________________________" << std::endl;
	std::cout << "6. In TEST2 Class got uniform locations." << std::endl;
	std::cout << "MVPloc: " << MVPloc << std::endl;
	std::cout << "modelMatrixLoc: " << modelMatrixLoc << std::endl;
	std::cout << "lightColorLoc: " << lightColorLoc << std::endl;
	std::cout << "lightPosLoc: " << lightPosLoc << std::endl;
	std::cout << "cameraPosLoc: " << cameraPosLoc << std::endl;
	std::cout << "________________________________________________" << std::endl;
}

void Test2::uploadUniforms()
{
	glm::vec3 lightColor = glm::vec3(1.0f, 1.0f, 1.0f);
	glm::vec3 lightPos = glm::vec3(3.0f, 1.0f, -3.0f);
	glm::vec3 cameraPosition = glm::vec3(0.0f, 0.0f, 4.0f);
	glUniformMatrix4fv(uniLocs.MVPloc, 1, GL_FALSE, glm::value_ptr(MVP));
	glUniformMatrix4fv(uniLocs.modelMatrixLoc, 1, GL_FALSE, glm::value_ptr(modelMatrix));
	glUniform3fv(uniLocs.lightColorLoc, 1, glm::value_ptr(lightColor));
	glUniform3fv(uniLocs.lightPosLoc, 1, glm::value_ptr(lightPos));
	glUniform3fv(uniLocs.cameraPosLoc, 1, glm::value_ptr(cameraPosition));
}

void Test2::render()
{
	glUseProgram(shaderID);
	uploadUniforms();
	glBindVertexArray(vao);
	glBindTexture(GL_TEXTURE_2D, 1);  // bind texture
	glDrawElements(GL_TRIANGLES, data.indices.size(), GL_UNSIGNED_INT, 0);
}

