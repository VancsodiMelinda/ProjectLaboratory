#include "Object.h"

/*
Object::Object()
{
	objectFileName = "resources/NewSuzanne.obj";
}
*/

Object::Object(std::string objectFileName_, GLuint shaderID_, glm::vec3 translate_, glm::vec3 scale_, float rotateAngle_, std::string rotateAxis_,
				Camera camera_, GLuint textureID_, int windowWidth, int windowHeight, glm::vec3 lightColor_, glm::vec3 lightPos_)  // parameterized constructor
{
	objectFileName = objectFileName_;
	shaderID = shaderID_;
	createModelMatrix(translate_, scale_, rotateAngle_, rotateAxis_);
	camera = camera_;
	textureID = textureID_;
	WINDOW_WIDTH = windowWidth;
	WINDOW_HEIGHT = windowHeight;
	lightColor = lightColor_;
	lightPos = lightPos_;
}


void Object::createModelMatrix(glm::vec3 translate, glm::vec3 scale, float rotateAngle, std::string rotateAxis)
{
	modelMatrix = glm::mat4(1.0f);  // 4x4 identity matrix
	
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

void Object::initialize()
{
	loadObjectData();
	createVAOandVBOs();
	fillVBOs();
	configVertexAttributes();
	createMVP();  // even if render method doesn't get called, the object still has an initial MVP matrix, maybe its not neccesary
	getUniformLocations();
	//glUseProgram(shaderID);
	//uploadUniforms();
}

void Object::loadObjectData()
{
	ObjLoader objLoader;
	data = objLoader.advancedObjLoader(objectFileName);

	//std::cout << "1. In Object Class .obj file has been loaded succesfully: " << objectFileName << std::endl;
}

void Object::createVAOandVBOs()
{
	glGenVertexArrays(1, &vao);  // create VAO
	glBindVertexArray(vao);  // bind VAO

	glGenBuffers(1, &vbo);  // create VBO
	glGenBuffers(1, &ibo);  // create IBO

	//std::cout << "2. In Object Class VAO and VBO has been created." << std::endl;
	//std::cout << "VAO: " << vao << std::endl;
	//std::cout << "VBO: " << vbo << std::endl;
	//std::cout << "IBO: " << ibo << std::endl;
}

void Object::fillVBOs()
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

	//std::cout << "3. In Object Class VBOs have been filled with object data." << std::endl;
}

void Object::configVertexAttributes()
{
	// get attribute indexes from the relevant shader
	GLuint positionAttribIndex = glGetAttribLocation(shaderID, "in_vertexPosition");	// layout (location = 0) in vec3 in_vertexPosition;
	GLuint textureAttribIndex = glGetAttribLocation(shaderID, "in_textureCoords");		// layout (location = 1) in vec2 in_textureCoords;
	GLuint normalAttribIndex = glGetAttribLocation(shaderID, "in_normalVec");			// layout (location = 2) in vec3 in_normalVec;

	// enable vertex attribute array
	glEnableVertexAttribArray(positionAttribIndex);
	glEnableVertexAttribArray(textureAttribIndex);
	glEnableVertexAttribArray(normalAttribIndex);

	// calculate offsets
	GLintptr vOffset = 0 * sizeof(GL_FLOAT);
	GLintptr tOffset = data.vertices.size() * sizeof(GL_FLOAT);
	GLintptr nOffset = (data.vertices.size() + data.uvs.size()) * sizeof(GL_FLOAT);
	int stride3f = 3 * sizeof(GL_FLOAT);
	int stride2f = 2 * sizeof(GL_FLOAT);

	// setup the pointers
	glVertexAttribPointer(positionAttribIndex, 3, GL_FLOAT, GL_FALSE, stride3f, (GLvoid*)vOffset);
	glVertexAttribPointer(textureAttribIndex, 2, GL_FLOAT, GL_FALSE, stride2f, (GLvoid*)tOffset);
	glVertexAttribPointer(normalAttribIndex, 3, GL_FLOAT, GL_FALSE, stride3f, (GLvoid*)nOffset);

	glBindVertexArray(0);  // unbind VAO

	//std::cout << "4. In Object Class vertex attributes have been configured." << std::endl;
	//std::cout << "positionAttribIndex: " << positionAttribIndex << std::endl;
	//std::cout << "textureAttribIndex: " << textureAttribIndex << std::endl;
	//std::cout << "normalAttribIndex: " << normalAttribIndex << std::endl;
}

void Object::createMVP()  // only once
{
	// create view matrix
	glm::mat4 viewMatrix = camera.CreateViewMatrix();  // create viewMatrix with default parameters

	// create projection matrix
	glm::mat4 projectionMatrix = glm::perspective(glm::radians(camera.fov), (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT, 0.1f, 100.0f);  // PERSPECTIVE PROJECTION

	// create MVP
	MVP = projectionMatrix * viewMatrix * modelMatrix;

	//std::cout << "5. In Object Class MVP has been created." << std::endl;
}

void Object::getUniformLocations()  // need to do this only once
{
	int MVPloc = glGetUniformLocation(shaderID, "MVP");
	int modelMatrixLoc = glGetUniformLocation(shaderID, "modelMatrix");

	int lightColorLoc = glGetUniformLocation(shaderID, "lightColor");
	int lightPosLoc = glGetUniformLocation(shaderID, "lightPos");
	
	int cameraPosLoc = glGetUniformLocation(shaderID, "cameraPos");

	// full up uniforms struct
	uniLocs.modelMatrixLoc = modelMatrixLoc;
	uniLocs.MVPloc = MVPloc;
	uniLocs.lightColorLoc = lightColorLoc;
	uniLocs.lightPosLoc = lightPosLoc;
	uniLocs.cameraPosLoc = cameraPosLoc;

	//std::cout << "6. In Object Class got uniform locations." << std::endl;
	//std::cout << "MVPloc: " << MVPloc << std::endl;
	//std::cout << "modelMatrixLoc: " << modelMatrixLoc << std::endl;
	//std::cout << "lightColorLoc: " << lightColorLoc << std::endl;
	//std::cout << "lightPosLoc: " << lightPosLoc << std::endl;
}

void Object::updateMVP()  // every frame
{
	glm::mat4 viewMatrix = camera.CreateViewMatrix();  // update in every frame (WASD and mouse)
	glm::mat4 projectionMatrix = glm::perspective(glm::radians(camera.fov), (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT, 0.1f, 100.0f);  // update in every frame (zoom)

	MVP = projectionMatrix * viewMatrix * modelMatrix;
}

void Object::uploadUniforms()  // in every frame
{	
	// upload uniform variables
	glUniformMatrix4fv(uniLocs.MVPloc, 1, GL_FALSE, glm::value_ptr(MVP));
	glUniformMatrix4fv(uniLocs.modelMatrixLoc, 1, GL_FALSE, glm::value_ptr(modelMatrix));
	glUniform3fv(uniLocs.lightColorLoc, 1, glm::value_ptr(lightColor));
	glUniform3fv(uniLocs.lightPosLoc, 1, glm::value_ptr(lightPos));
	glUniform3fv(uniLocs.cameraPosLoc, 1, glm::value_ptr(camera.cameraPosition));
}

/*
void Object::updateUniforms()  // uniforms that get uploaded in every frame
{
	// upload uniform variables
	glUniformMatrix4fv(uniLocs.MVPloc, 1, GL_FALSE, glm::value_ptr(MVP));
	//glUniformMatrix4fv(uniLocs.modelMatrixLoc, 1, GL_FALSE, glm::value_ptr(modelMatrix));
}
*/

void Object::render(Camera camera_)
{
	camera = camera_;  // update camera
	glUseProgram(shaderID);
	updateMVP();
	uploadUniforms();
	//updateUniforms();
	glBindVertexArray(vao);
	glBindTexture(GL_TEXTURE_2D, textureID);  // bind texture
	glDrawElements(GL_TRIANGLES, data.indices.size(), GL_UNSIGNED_INT, 0);
}

