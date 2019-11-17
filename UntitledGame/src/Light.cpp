#include "Light.h"

Light::Light(std::string objectFileName_, GLuint shaderID_, glm::vec3 translate_, glm::vec3 scale_, float rotateAngle_, std::string rotateAxis_,
	Camera camera_, int windowWidth, int windowHeight, glm::vec3 lightColor_)
{
	objectFileName = objectFileName_;
	shaderID = shaderID_;
	createModelMatrix(translate_, scale_, rotateAngle_, rotateAxis_);
	camera = camera_;
	WINDOW_WIDTH = windowWidth;
	WINDOW_HEIGHT = windowHeight;
	lightColor = lightColor_;
	lightPos = translate_;
}

void Light::createModelMatrix(glm::vec3 translate, glm::vec3 scale, float rotateAngle, std::string rotateAxis)
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

void Light::initialize()
{
	loadObjectData();
	createVAOandVBOs();
	fillVBOs();
	configVertexAttributes();
	createMVP();
	getUniformLocations();
}

void Light::loadObjectData()
{
	ObjLoader objLoader;
	data = objLoader.advancedObjLoader(objectFileName);

	//std::cout << "1. In Light Class .obj file has been loaded succesfully: " << objectFileName << std::endl;
}

void Light::createVAOandVBOs()
{
	glGenVertexArrays(1, &vao);  // create VAO
	glBindVertexArray(vao);  // bind VAO

	glGenBuffers(1, &vbo);  // create VBO
	glGenBuffers(1, &ibo);  // create IBO

	//std::cout << "2. In Light Class VAO and VBO has been created." << std::endl;
	//std::cout << "VAO: " << vao << std::endl;
	//std::cout << "VBO: " << vbo << std::endl;
	//std::cout << "IBO: " << ibo << std::endl;
}

void Light::fillVBOs()
{
	glBindBuffer(GL_ARRAY_BUFFER, vbo);  // bind VBO
	glBufferData(GL_ARRAY_BUFFER, data.vertices.size() * sizeof(GL_FLOAT), 0, GL_STATIC_DRAW);  // reserve space
	glBufferSubData(GL_ARRAY_BUFFER, 0, data.vertices.size() * sizeof(GL_FLOAT), &data.vertices[0]);  // VERTEX COORDINATES
	glBindBuffer(GL_ARRAY_BUFFER, 0);  // unbind VBO

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);  // bind IBO
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, data.indices.size() * sizeof(GLuint), &data.indices[0], GL_STATIC_DRAW);  // INDICES
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);  // unbind IBO

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);

	//std::cout << "3. In Light Class VBOs have been filled with object data." << std::endl;
}

void Light::configVertexAttributes()
{
	// get attribute indexes from the relevant shader
	GLuint positionAttribIndex = glGetAttribLocation(shaderID, "in_vertexPosition");	// layout (location = 0) in vec3 in_vertexPosition;

	// enable vertex attribute array
	glEnableVertexAttribArray(positionAttribIndex);

	// calculate offsets
	GLintptr vOffset = 0 * sizeof(GL_FLOAT);
	int stride3f = 3 * sizeof(GL_FLOAT);

	// setup the pointers
	glVertexAttribPointer(positionAttribIndex, 3, GL_FLOAT, GL_FALSE, stride3f, (GLvoid*)vOffset);

	glBindVertexArray(0);  // unbind VAO

	//std::cout << "4. In Light Class vertex attributes have been configured." << std::endl;
	//std::cout << "positionAttribIndex: " << positionAttribIndex << std::endl;
}

void Light::createMVP()  // only once
{
	// create view matrix
	glm::mat4 viewMatrix = camera.CreateViewMatrix();  // create viewMatrix with default parameters

	// create projection matrix
	glm::mat4 projectionMatrix = glm::perspective(glm::radians(camera.fov), (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT, 0.1f, 100.0f);  // PERSPECTIVE PROJECTION

	// create MVP
	MVP = projectionMatrix * viewMatrix * modelMatrix;

	//std::cout << "5. In Light Class MVP has been created." << std::endl;
}

void Light::getUniformLocations()  // need to do this only once
{
	int MVPloc = glGetUniformLocation(shaderID, "MVP");

	int lightColorLoc = glGetUniformLocation(shaderID, "lightColor");

	// full up uniforms struct
	uniLocs.MVPloc = MVPloc;
	uniLocs.lightColorLoc = lightColorLoc;

	//std::cout << "6. In Light Class got uniform locations." << std::endl;
	//std::cout << "MVPloc: " << MVPloc << std::endl;
	//std::cout << "lightColorLoc: " << lightColorLoc << std::endl;
}

void Light::updateMVP()  // every frame
{
	glm::mat4 viewMatrix = camera.CreateViewMatrix();  // update in every frame (WASD and mouse)
	glm::mat4 projectionMatrix = glm::perspective(glm::radians(camera.fov), (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT, 0.1f, 100.0f);  // update in every frame (zoom)

	MVP = projectionMatrix * viewMatrix * modelMatrix;
}

void Light::uploadUniforms()  // in every frame
{
	// upload uniform variables
	glUniformMatrix4fv(uniLocs.MVPloc, 1, GL_FALSE, glm::value_ptr(MVP));
	glUniform3fv(uniLocs.lightColorLoc, 1, glm::value_ptr(lightColor));
}

void Light::render(Camera camera_)
{
	camera = camera_;  // update camera
	glUseProgram(shaderID);
	updateMVP();
	uploadUniforms();
	glBindVertexArray(vao);
	glDrawElements(GL_TRIANGLES, data.indices.size(), GL_UNSIGNED_INT, 0);
}
