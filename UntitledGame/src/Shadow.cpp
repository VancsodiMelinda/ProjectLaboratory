#include "Shadow.h"


Shadow::Shadow(GLuint shaderID_, GLuint vao_, GLuint vbo_, GLuint ibo_, objectData data_, glm::mat4 modelMatrix_,
	glm::vec3 lightPos_, int shadowWidth, int shadowHeight)
{
	shaderID = shaderID_;
	vao = vao_;
	vbo = vbo_;
	ibo = ibo_;
	data = data_;
	modelMatrix = modelMatrix_;
	lightPos = lightPos_;
	SHADOW_WIDTH = shadowWidth;
	SHADOW_HEIGHT = shadowHeight;
}

Shadow::Shadow()
{
	SHADOW_WIDTH = 1024;
	SHADOW_HEIGHT = 1024;
	createTexture();	// shadowMap
	createFBO();		// fbo
}

void Shadow::initialize()
{
	createMVP();
	//createTexture();
	//createFBO();
	configVertexAttributes();
	getUniformLocations();
}

void Shadow::createMVP()
{
	// create view matrix
	glm::mat4 lightViewMatrix = glm::lookAt(lightPos, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

	// create projection matrix
	float nearPlane = 0.1f;
	float farPlane = 20.0f;
	//glm::mat4 lightProjMatrix = glm::ortho(-2.0f, 2.0f, -1.5f, 1.5f, nearPlane, farPlane);
	glm::mat4 lightProjMatrix = glm::ortho(-4.0f, 4.0f, -3.0f, 3.0f, nearPlane, farPlane);

	// create MVP
	MVP = lightProjMatrix * lightViewMatrix * modelMatrix;
}


void Shadow::createTexture()
{
	// create texture, serves as the depth attachment fot the fbo
	//GLuint shadowMap;
	glActiveTexture(GL_TEXTURE1);
	glGenTextures(1, &shadowMap);
	glBindTexture(GL_TEXTURE_2D, shadowMap);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_INT, NULL);  // allocate memory
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);	// oversampling
	glBindTexture(GL_TEXTURE_2D, 0);
}

void Shadow::createFBO()
{
	// create framebuffer object
	//GLuint fbo;
	glGenFramebuffers(1, &fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);
	std::cout << "fbo: " << fbo << std::endl;

	// attach shadowMap to the framebuffer
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, shadowMap, 0);  // attach texture to the framebuffer
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
}

void Shadow::configVertexAttributes()
{
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);

	// get attribute indexes from the relevant shader
	GLuint positionAttribIndex = glGetAttribLocation(shaderID, "in_vertexPosition");	// layout (location = 0) in vec3 in_vertexPosition;
	//GLuint textureAttribIndex = glGetAttribLocation(shaderID, "in_textureCoords");		// layout (location = 1) in vec2 in_textureCoords;
	//GLuint normalAttribIndex = glGetAttribLocation(shaderID, "in_normalVec");			// layout (location = 2) in vec3 in_normalVec;

	// enable vertex attribute array
	glEnableVertexAttribArray(positionAttribIndex);
	//glEnableVertexAttribArray(textureAttribIndex);
	//glEnableVertexAttribArray(normalAttribIndex);

	// calculate offsets
	GLintptr vOffset = 0 * sizeof(GL_FLOAT);
	//GLintptr tOffset = data.vertices.size() * sizeof(GL_FLOAT);
	//GLintptr nOffset = (data.vertices.size() + data.uvs.size()) * sizeof(GL_FLOAT);
	int stride3f = 3 * sizeof(GL_FLOAT);
	//int stride2f = 2 * sizeof(GL_FLOAT);

	// setup the pointers
	glVertexAttribPointer(positionAttribIndex, 3, GL_FLOAT, GL_FALSE, stride3f, (GLvoid*)vOffset);
	//glVertexAttribPointer(textureAttribIndex, 2, GL_FLOAT, GL_FALSE, stride2f, (GLvoid*)tOffset);
	//glVertexAttribPointer(normalAttribIndex, 3, GL_FLOAT, GL_FALSE, stride3f, (GLvoid*)nOffset);

	glBindVertexArray(0);  // unbind VAO
	glBindBuffer(GL_ARRAY_BUFFER, 0);  // unbind VBO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);  // unbind IBO

	//std::cout << "4. In Shadow Class vertex attributes have been configured." << std::endl;
	//std::cout << "positionAttribIndex: " << positionAttribIndex << std::endl;
	//std::cout << "textureAttribIndex: " << textureAttribIndex << std::endl;
	//std::cout << "normalAttribIndex: " << normalAttribIndex << std::endl;
}

void Shadow::getUniformLocations()  // need to do this only once
{
	int MVPloc = glGetUniformLocation(shaderID, "MVP");

	// full up uniforms struct
	uniLocs.MVPloc = MVPloc;

	//std::cout << "6. In Shadow Class got uniform locations." << std::endl;
	//std::cout << "MVPloc: " << MVPloc << std::endl;
}

void Shadow::render()
{
	glUseProgram(shaderID);
	uploadUniforms();

	glBindVertexArray(vao);
	glDrawElements(GL_TRIANGLES, data.indices.size(), GL_UNSIGNED_INT, 0);
}

void Shadow::uploadUniforms()  // in every frame
{
	// upload uniform variables
	glUniformMatrix4fv(uniLocs.MVPloc, 1, GL_FALSE, glm::value_ptr(MVP));
}


/*
Shadow::Shadow(std::string objectFileName_, GLuint shaderID_, glm::vec3 translate_, glm::vec3 scale_, float rotateAngle_, std::string rotateAxis_, 
				int windowWidth, int windowHeight, glm::vec3 lightPos_)  // parameterized constructor
{
	objectFileName = objectFileName_;
	shaderID = shaderID_;
	createModelMatrix(translate_, scale_, rotateAngle_, rotateAxis_);
	WINDOW_WIDTH = windowWidth;
	WINDOW_HEIGHT = windowHeight;
	lightPos = lightPos_;
}


Shadow::Shadow(GLuint shaderID_, GLuint vao_, GLuint vbo_, GLuint ibo_, glm::mat4 modelMatrix_, int windowWidth, int windowHeight, glm::vec3 lightPos_)  // parameterized constructor
{
	shaderID = shaderID_;
	vao = vao_;
	vbo = vbo_;
	ibo = ibo_;
	modelMatrix = modelMatrix_;
	WINDOW_WIDTH = windowWidth;
	WINDOW_HEIGHT = windowHeight;
	lightPos = lightPos_;
}



void Shadow::createModelMatrix(glm::vec3 translate, glm::vec3 scale, float rotateAngle, std::string rotateAxis)
{
	modelMatrix = glm::mat4(1.0f);

	modelMatrix = glm::translate(modelMatrix, translate);

	modelMatrix = glm::scale(modelMatrix, scale);

	if ((rotateAxis == "x") || (rotateAxis == "X"))
	{
		modelMatrix = glm::rotate(modelMatrix, glm::radians(rotateAngle), glm::vec3(1.0f, 0.0f, 0.0f));
	}
	else if ((rotateAxis == "y") || (rotateAxis == "Y"))
	{
		modelMatrix = glm::rotate(modelMatrix, glm::radians(rotateAngle), glm::vec3(0.0f, 1.0f, 0.0f));
	}
	else if ((rotateAxis == "z") || (rotateAxis == "Z"))
	{
		modelMatrix = glm::rotate(modelMatrix, glm::radians(rotateAngle), glm::vec3(0.0f, 0.0f, 1.0f));
	}
	else
	{
		std::cout << "The rotation angle is not correct!" << std::endl;
	}

}


void Shadow::initialize()
{
	loadObjectData();
	createVAOandVBOs();
	fillVBOs();
	configVertexAttributes();
	createMVP();  // even if render method doesn't get called, the object still has an initial MVP matrix, maybe its not neccesary
	getUniformLocations();
}



//void Shadow::initialize()
//{
	//configVertexAttributes();
	//createMVP();  // even if render method doesn't get called, the object still has an initial MVP matrix, maybe its not neccesary
	//getUniformLocations();
//}


void Shadow::loadObjectData()
{
	ObjLoader objLoader;
	data = objLoader.advancedObjLoader(objectFileName);

	//std::cout << "1. In Shadow Class .obj file has been loaded succesfully: " << objectFileName << std::endl;
}


void Shadow::createVAOandVBOs()
{
	glGenVertexArrays(1, &vao);  // create VAO
	glBindVertexArray(vao);  // bind VAO

	glGenBuffers(1, &vbo);  // create VBO
	glGenBuffers(1, &ibo);  // create IBO

	//std::cout << "2. In Shadow Class VAO and VBO has been created." << std::endl;
	//std::cout << "VAO: " << vao << std::endl;
	//std::cout << "VBO: " << vbo << std::endl;
	//std::cout << "IBO: " << ibo << std::endl;
}

void Shadow::fillVBOs()
{
	glBindBuffer(GL_ARRAY_BUFFER, vbo);  // bind VBO
	glBufferData(GL_ARRAY_BUFFER, (data.vertices.size() + data.uvs.size() + data.normals.size()) * sizeof(GL_FLOAT), 0, GL_STATIC_DRAW);  // reserve space
	glBufferSubData(GL_ARRAY_BUFFER, 0, data.vertices.size() * sizeof(GL_FLOAT), &data.vertices[0]);															// VERTEX COORDINATES
	glBufferSubData(GL_ARRAY_BUFFER, data.vertices.size() * sizeof(GL_FLOAT), data.uvs.size() * sizeof(GL_FLOAT), &data.uvs[0]);								// TEXTURE COORDINATES
	glBufferSubData(GL_ARRAY_BUFFER, (data.vertices.size() + data.uvs.size()) * sizeof(GL_FLOAT), data.normals.size() * sizeof(GL_FLOAT), &data.normals[0]);	// NORMAL COORDINATES
	glBindBuffer(GL_ARRAY_BUFFER, 0);  // unbind VBO

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);  // bind IBO
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, data.indices.size() * sizeof(GLuint), &data.indices[0], GL_STATIC_DRAW);  // INDICES
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);  // unbind IBO

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);

	//std::cout << "3. In Shadow Class VBOs have been filled with object data." << std::endl;
}

void Shadow::configVertexAttributes()
{
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);

	// get attribute indexes from the relevant shader
	GLuint positionAttribIndex = glGetAttribLocation(shaderID, "in_vertexPosition");	// layout (location = 0) in vec3 in_vertexPosition;
	//GLuint textureAttribIndex = glGetAttribLocation(shaderID, "in_textureCoords");		// layout (location = 1) in vec2 in_textureCoords;
	//GLuint normalAttribIndex = glGetAttribLocation(shaderID, "in_normalVec");			// layout (location = 2) in vec3 in_normalVec;

	// enable vertex attribute array
	glEnableVertexAttribArray(positionAttribIndex);
	//glEnableVertexAttribArray(textureAttribIndex);
	//glEnableVertexAttribArray(normalAttribIndex);

	// calculate offsets
	GLintptr vOffset = 0 * sizeof(GL_FLOAT);
	//GLintptr tOffset = data.vertices.size() * sizeof(GL_FLOAT);
	//GLintptr nOffset = (data.vertices.size() + data.uvs.size()) * sizeof(GL_FLOAT);
	int stride3f = 3 * sizeof(GL_FLOAT);
	//int stride2f = 2 * sizeof(GL_FLOAT);

	// setup the pointers
	glVertexAttribPointer(positionAttribIndex, 3, GL_FLOAT, GL_FALSE, stride3f, (GLvoid*)vOffset);
	//glVertexAttribPointer(textureAttribIndex, 2, GL_FLOAT, GL_FALSE, stride2f, (GLvoid*)tOffset);
	//glVertexAttribPointer(normalAttribIndex, 3, GL_FLOAT, GL_FALSE, stride3f, (GLvoid*)nOffset);

	glBindVertexArray(0);  // unbind VAO
	glBindBuffer(GL_ARRAY_BUFFER, 0);  // unbind VBO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);  // unbind IBO

	//std::cout << "4. In Shadow Class vertex attributes have been configured." << std::endl;
	//std::cout << "positionAttribIndex: " << positionAttribIndex << std::endl;
	//std::cout << "textureAttribIndex: " << textureAttribIndex << std::endl;
	//std::cout << "normalAttribIndex: " << normalAttribIndex << std::endl;
}


void Shadow::createMVP()  // only once
{
	// create view matrix
	glm::mat4 lightViewMatrix = glm::lookAt(lightPos, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

	// create projection matrix
	float nearPlane = 0.1f;
	float farPlane = 20.0f;
	glm::mat4 lightProjMatrix = glm::ortho(-2.0f, 2.0f, -1.5f, 1.5f, nearPlane, farPlane);

	lightSpaceMatrix = lightProjMatrix * lightViewMatrix;

	// create MVP
	MVP = lightProjMatrix * lightViewMatrix * modelMatrix;

	//std::cout << "5. In Shadow Class MVP has been created." << std::endl;
}

void Shadow::getUniformLocations()  // need to do this only once
{
	int MVPloc = glGetUniformLocation(shaderID, "MVP");

	// full up uniforms struct
	uniLocs.MVPloc = MVPloc;

	//std::cout << "6. In Shadow Class got uniform locations." << std::endl;
	//std::cout << "MVPloc: " << MVPloc << std::endl;
}


//void Shadow::updateMVP()  // every frame
//{
	//glm::mat4 viewMatrix = camera.CreateViewMatrix();  // update in every frame (WASD and mouse)
	//glm::mat4 projectionMatrix = glm::perspective(glm::radians(camera.fov), (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT, 0.1f, 100.0f);  // update in every frame (zoom)
	////glm::mat4 projectionMatrix = glm::ortho(-2.0f, 2.0f, -1.5f, 1.5f, 0.1f, 100.0f);

	//MVP = projectionMatrix * viewMatrix * modelMatrix;
//}


void Shadow::uploadUniforms()  // in every frame
{
	// upload uniform variables
	glUniformMatrix4fv(uniLocs.MVPloc, 1, GL_FALSE, glm::value_ptr(MVP));
}

void Shadow::render()
{
	// modify the value of uniform variables (glUniformMatrix4fv/glUniform3fv) after calling glUseProgram
	glUseProgram(shaderID);
	uploadUniforms();
	glBindVertexArray(vao);
	glDrawElements(GL_TRIANGLES, data.indices.size(), GL_UNSIGNED_INT, 0);
	//glDrawElements(GL_LINES, data.indices.size(), GL_UNSIGNED_INT, 0);
}

*/