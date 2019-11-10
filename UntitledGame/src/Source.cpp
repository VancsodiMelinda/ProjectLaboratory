#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/ext.hpp>

#include <stdio.h>
#include <iostream>

#include <string>
#include <fstream>

#include <list>
#include <sstream>
#include <vector>
#include <algorithm>

#include "Transformation.h"
#include "ObjLoader.h"
#include "Camera.h"
#include "Shader.h"
#include "Texture.h"
#include "Object.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
//std::string shaderSourceCodeReader(std::string shaderFileName);
//GLuint setUpShader(GLenum shaderType, std::string shaderFileName, GLuint programObject);
//void shaderErrorHandling(GLuint shaderObject);
//void linkingErrorHandling(GLuint programObject);
//void cleanUpShader(GLuint programObject, GLuint shaderObject);
//void loadObjFile(std::string objFileName, std::vector<float>& vVector, std::vector<int>& indVector);
void processKeyInput(GLFWwindow* window);


// global camera variables
Camera camera;  // create object with default constructor

float deltaTime = 0.0f;
float lastFrame = 0.0f;

glm::mat4 projectionMatrix;
glm::mat4 orthographicMatrix;
glm::mat4 perspectiveMatrix;
bool isPerspective;

//const int WINDOW_WIDTH = 800;
//const int WINDOW_HEIGHT = 600;
const int WINDOW_WIDTH = 1280;
const int WINDOW_HEIGHT = 720;

float lastX = (float)WINDOW_WIDTH / 2.0f;
float lastY = (float)WINDOW_HEIGHT / 2.0f;
bool firstMouse;

int main(void)
{
	GLFWwindow* window;

	/* Initialize the library */
	if (!glfwInit())
	{
		printf("Error: GLFW initialization was unsuccesful!\n");
		return -1;
	}

	/* Create a windowed mode window and its OpenGL context */
	//window = glfwCreateWindow(640, 480, "Untitled Game", glfwGetPrimaryMonitor(), NULL);
	//int WINDOW_WIDTH = 800;
	//int WINDOW_HEIGHT = 600;
	window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Untitled Game", NULL, NULL);
	if (!window)
	{
		printf("Error: Window creation was unsuccesful!\n");
		glfwTerminate();
		return -1;
	}

	glfwSetKeyCallback(window, key_callback);  // keyboard input: key input - key callback
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPosCallback(window, cursor_position_callback); // mouse input: cursor position - cursor position callback
	glfwSetScrollCallback(window, scroll_callback);  // scroll input

	/* Make the window's context current */
	glfwMakeContextCurrent(window);  // make current OpenGL context

	// initialize extension loader library here, for example glad
	gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

	// check if glad is loaded properly
	if (!gladLoadGL()) {
		printf("Error: GLAD initialization was unsuccesful!\n");
		return -1;
	}

	// check openGL version
	printf("OpenGL Version: %d.%d\n", GLVersion.major, GLVersion.minor);

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	// load objects
	//ObjLoader objLoader;
	// load Suzanne model
	//std::string objFileName = "resources/Suzanne texture test custom unwrapped.obj";
	//objectData test = objLoader.loadObjFileV2(objFileName);
	//std::cout << "Loaded object file: " << objFileName << std::endl;

	// test loading with advanced obj loader
	
	ObjLoader objLoader;

	/*
	std::string objFileName_ = "resources/NewSuzanne.obj";
	objectData notSplitted = objLoader.advancedObjLoader(objFileName_);
	std::cout << "Loaded object file: " << objFileName_ << std::endl;
	std::cout << "THIS v: " << notSplitted.vertices.size() << std::endl;
	std::cout << "THIS vt: " << notSplitted.uvs.size() << std::endl;
	std::cout << "THIS vn: " << notSplitted.normals.size() << std::endl;
	std::cout << "THIS i: " << notSplitted.indices.size() << std::endl;
	*/

	// load cube model (lightsource object)
	std::string lightObjFileName = "resources/texturePracticeSplitted.obj";
	objectData lightSourceObject = objLoader.loadObjFileV2(lightObjFileName);
	std::cout << "Loaded light source object: " << lightObjFileName << std::endl;

	// create plane
	objectData plane;
	plane.vertices = { -1.0f, -1.0f, 1.0f,
						1.0f, -1.0f, 1.0f,
						1.0f, -1.0f, -1.0f,
						-1.f, -1.0f, -1.0f };
	plane.uvs = { 0.0f, 0.0f,
				  1.0f, 0.0f,
				  1.0f, 1.0f,
				  0.0f, 1.0f };
	
	plane.normals = { 0.0f, 1.0f, 0.0f,
					  0.0f, 1.0f, 0.0f,
					  0.0f, 1.0f, 0.0f,
					  0.0f, 1.0f, 0.0f };
	
	plane.indices = { 0, 1, 2,
					  0, 2, 3 };

	// shader for objects
	Shader objShader("src/VertexShader.txt", "src/FragmentShader.txt");
	objShader.runShaderCode();

	// shader for lightsource object
	Shader lightObjShader("src/LightObjVertexShader.txt", "src/LightObjFragmentShader.txt");
	lightObjShader.runShaderCode();

	// SHADER CODE
	/*
	GLuint programObject = glCreateProgram();  // empty program object
	GLuint vertexShaderObject = setUpShader(GL_VERTEX_SHADER, "src/VertexShader.txt", programObject);  // create, compile and attach vertex shader
	GLuint fragmentShaderObject = setUpShader(GL_FRAGMENT_SHADER, "src/FragmentShader.txt", programObject);  // create, compile and attach fragment shader
	

	// before linking you have to set up attribute locations, do NOT do this if layout(location = #) is in the shader code
	//glBindAttribLocation(programObject, positionLocation, "in_vertexPosition");
	//glBindAttribLocation(programObject, colorLocation, "in_vertexColor");
	//glBindAttribLocation(programObject, textureLocation, "in_vertexTexture");
	
	glLinkProgram(programObject);
	linkingErrorHandling(programObject);
	cleanUpShader(programObject, vertexShaderObject);
	cleanUpShader(programObject, fragmentShaderObject);
	//glUseProgram(programObject);
	*/
	////////////////////////////////////////////

	//int vSize = test.vertices.size();
	//int vtSize = test.uvs.size();
	//int vnSize = test.normals.size();
	//int iSize = test.indices.size();

	//std::cout << "vSize: " << vSize << std::endl;
	//std::cout << "vtSize: " << vtSize << std::endl;
	//std::cout << "vnSize: " << vnSize << std::endl;
	//std::cout << "iSize: " << iSize << std::endl;

	/*
	GLuint vao;
	glGenVertexArrays(1, &vao);  // create VAO
	glBindVertexArray(vao);  // bind 1st VAO

	GLuint vbo, ibo;  // create VBOs
	glGenBuffers(1, &vbo);
	glGenBuffers(1, &ibo);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);  // bind VBO
	glBufferData(GL_ARRAY_BUFFER, (notSplitted.vertices.size() + notSplitted.uvs.size() + notSplitted.normals.size()) * sizeof(GL_FLOAT), 0, GL_STATIC_DRAW);  // reserve space
	glBufferSubData(GL_ARRAY_BUFFER, 0, notSplitted.vertices.size() * sizeof(GL_FLOAT), &notSplitted.vertices[0]);															// VERTEX COORDINATES
	glBufferSubData(GL_ARRAY_BUFFER, notSplitted.vertices.size() * sizeof(GL_FLOAT), notSplitted.uvs.size() * sizeof(GL_FLOAT), &notSplitted.uvs[0]);								// TEXTURE COORDINATES
	glBufferSubData(GL_ARRAY_BUFFER, (notSplitted.vertices.size() + notSplitted.uvs.size()) * sizeof(GL_FLOAT), notSplitted.normals.size() * sizeof(GL_FLOAT), &notSplitted.normals[0]);	// NORMAL COORDINATES
	glBindBuffer(GL_ARRAY_BUFFER, 0);  // unbind VBO

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);  // bind IBO
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, notSplitted.indices.size() * sizeof(GLuint), &notSplitted.indices[0], GL_STATIC_DRAW);  // INDICES
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);  // unbind IBO

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);

	GLuint positionAttribIndex = glGetAttribLocation(objShader.programObject, "in_vertexPosition");		// layout (location = 0) in vec3 in_vertexPosition;
	GLuint textureAttribIndex = glGetAttribLocation(objShader.programObject, "in_textureCoords");		// layout (location = 1) in vec2 in_textureCoords;
	GLuint normalAttribIndex = glGetAttribLocation(objShader.programObject, "in_normalVec");			// layout (location = 2) in vec3 in_normalVec;

	std::cout << "THIS positionAttribIndex: " << positionAttribIndex << std::endl;
	std::cout << "THIS textureAttribIndex: " << textureAttribIndex << std::endl;
	std::cout << "THIS normalAttribIndex: " << normalAttribIndex << std::endl;

	glEnableVertexAttribArray(positionAttribIndex);
	glEnableVertexAttribArray(textureAttribIndex);
	glEnableVertexAttribArray(normalAttribIndex);

	GLintptr vOffset = 0 * sizeof(GL_FLOAT);
	GLintptr tOffset = notSplitted.vertices.size() * sizeof(GL_FLOAT);
	GLintptr nOffset = (notSplitted.vertices.size() + notSplitted.uvs.size()) * sizeof(GL_FLOAT);
	int stride3 = 3 * sizeof(GL_FLOAT);
	int stride2 = 2 * sizeof(GL_FLOAT);

	glVertexAttribPointer(positionAttribIndex, 3, GL_FLOAT, GL_FALSE, stride3, (GLvoid*)vOffset);
	glVertexAttribPointer(textureAttribIndex, 2, GL_FLOAT, GL_FALSE, stride2, (GLvoid*)tOffset);
	glVertexAttribPointer(normalAttribIndex, 3, GL_FLOAT, GL_FALSE, stride3, (GLvoid*)nOffset);

	glBindVertexArray(0);  // unbind 1st VAO
	*/
	
	// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
	GLuint lightVAO;
	glGenVertexArrays(1, &lightVAO);
	glBindVertexArray(lightVAO);  // bind 2nd VAO

	GLuint lightVbo, lightIbo;  // create VBOs
	glGenBuffers(1, &lightVbo);
	glGenBuffers(1, &lightIbo);

	glBindBuffer(GL_ARRAY_BUFFER, lightVbo);  // bind VBO
	glBufferData(GL_ARRAY_BUFFER, lightSourceObject.vertices.size() * sizeof(GL_FLOAT), 0, GL_STATIC_DRAW);  // reserve space
	glBufferSubData(GL_ARRAY_BUFFER, 0, lightSourceObject.vertices.size() * sizeof(GL_FLOAT), &lightSourceObject.vertices[0]);  // VERTEX COORDINATES
	//glBufferSubData(GL_ARRAY_BUFFER, lightSourceObject.vertices.size() * sizeof(GL_FLOAT), lightSourceObject.uvs.size() * sizeof(GL_FLOAT), &lightSourceObject.uvs[0]);  // TEXTURE COORDINATES
	glBindBuffer(GL_ARRAY_BUFFER, 0);  // unbind VBO

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, lightIbo);  // bind IBO
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, lightSourceObject.indices.size() * sizeof(GLuint), &lightSourceObject.indices[0], GL_STATIC_DRAW);  // INDICES
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);  // unbind IBO

	glBindBuffer(GL_ARRAY_BUFFER, lightVbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, lightIbo);

	GLuint lightPosAttrIndex = glGetAttribLocation(lightObjShader.programObject, "in_vertexPosition");
	//GLuint textureAttribIndex = glGetAttribLocation(objShader.programObject, "in_textureCoords");
	std::cout << "THAT lightPosAttrIndex: " << lightPosAttrIndex << std::endl;

	glEnableVertexAttribArray(lightPosAttrIndex);
	//glEnableVertexAttribArray(textureAttribIndex);
	
	GLintptr light_vOffset = 0 * sizeof(GL_FLOAT);
	//GLintptr tOffset2 = lightSourceObject.vertices.size() * sizeof(GL_FLOAT);

	int stride3 = 3 * sizeof(GL_FLOAT);
	int stride2 = 2 * sizeof(GL_FLOAT);

	glVertexAttribPointer(lightPosAttrIndex, 3, GL_FLOAT, GL_FALSE, stride3, (GLvoid*)light_vOffset);
	//glVertexAttribPointer(textureAttribIndex, 2, GL_FLOAT, GL_FALSE, stride2, (GLvoid*)(lightSourceObject.vertices.size() * sizeof(GL_FLOAT)));

	glBindVertexArray(0);  // unbind 2nd VAO

	// %%%%%%%%%%%%%%%%%%%%
	GLuint planeVAO;
	glGenVertexArrays(1, &planeVAO);
	glBindVertexArray(planeVAO);  // bind 3rd VAO

	GLuint planeVbo, planeIbo;  // create VBOs
	glGenBuffers(1, &planeVbo);
	glGenBuffers(1, &planeIbo);

	glBindBuffer(GL_ARRAY_BUFFER, planeVbo);  // bind VBO
	glBufferData(GL_ARRAY_BUFFER, (plane.vertices.size() + plane.uvs.size() + plane.normals.size()) * sizeof(GL_FLOAT), 0, GL_STATIC_DRAW);  // reserve space
	glBufferSubData(GL_ARRAY_BUFFER, 0, plane.vertices.size() * sizeof(GL_FLOAT), &plane.vertices[0]);																// VERTEX COORDINATES
	glBufferSubData(GL_ARRAY_BUFFER, plane.vertices.size() * sizeof(GL_FLOAT), plane.uvs.size() * sizeof(GL_FLOAT), &plane.uvs[0]);									// TEXTURE COORDINATES
	glBufferSubData(GL_ARRAY_BUFFER, (plane.vertices.size() + plane.uvs.size()) * sizeof(GL_FLOAT), plane.normals.size() * sizeof(GL_FLOAT), &plane.normals[0]);	// NORMAL COORDINATES
	glBindBuffer(GL_ARRAY_BUFFER, 0);  // unbind VBO

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, planeIbo);  // bind IBO
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, plane.indices.size() * sizeof(GLuint), &plane.indices[0], GL_STATIC_DRAW);  // INDICES
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);  // unbind IBO

	glBindBuffer(GL_ARRAY_BUFFER, planeVbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, planeIbo);

	GLuint positionAttribIndex_ = glGetAttribLocation(objShader.programObject, "in_vertexPosition");		// layout (location = 0) in vec3 in_vertexPosition;
	GLuint textureAttribIndex_ = glGetAttribLocation(objShader.programObject, "in_textureCoords");		// layout (location = 1) in vec2 in_textureCoords;
	GLuint normalAttribIndex_ = glGetAttribLocation(objShader.programObject, "in_normalVec");			// layout (location = 2) in vec3 in_normalVec;

	std::cout << "THIS positionAttribIndex_: " << positionAttribIndex_ << std::endl;
	std::cout << "THIS textureAttribIndex_: " << textureAttribIndex_ << std::endl;
	std::cout << "THIS normalAttribIndex_: " << normalAttribIndex_ << std::endl;

	glEnableVertexAttribArray(positionAttribIndex_);
	glEnableVertexAttribArray(textureAttribIndex_);
	glEnableVertexAttribArray(normalAttribIndex_);

	GLintptr plane_vOffset = 0 * sizeof(GL_FLOAT);
	GLintptr plane_tOffset = plane.vertices.size() * sizeof(GL_FLOAT);
	GLintptr plane_nOffset = (plane.vertices.size() + plane.uvs.size()) * sizeof(GL_FLOAT);

	glVertexAttribPointer(positionAttribIndex_, 3, GL_FLOAT, GL_FALSE, stride3, (GLvoid*)plane_vOffset);
	glVertexAttribPointer(textureAttribIndex_, 2, GL_FLOAT, GL_FALSE, stride2, (GLvoid*)plane_tOffset);
	glVertexAttribPointer(normalAttribIndex_, 2, GL_FLOAT, GL_FALSE, stride3, (GLvoid*)plane_nOffset);

	glBindVertexArray(0);  // unbind 2nd VAO

	//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
	///// TEXTURE
	//GLuint tex;
	//glGenTextures(1, &tex);
	/*
	GLuint texture[2];
	glGenTextures(2, texture);
	std::cout << "textureIDs: " << texture[0] << " and " << texture[1] << std::endl;

	//glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture[0]);  // bind texture

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);  // wrapping
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);  // wrapping
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);  // filtering
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);  // filtering

	// load texture image: https://github.com/nothings/stb/blob/master/stb_image.h
	int imageWidth, imageHeight, nrChannels;
	std::string textureFileName = "resources/test grid.png";
	stbi_set_flip_vertically_on_load(true);
	unsigned char* data = stbi_load(textureFileName.c_str(), &imageWidth, &imageHeight, &nrChannels, 0);
	if (data)
	{
		std::cout << "Texture image has been loaded succesfully: " << textureFileName << std::endl;
		std::cout << "imageWidth: " << imageWidth << std::endl << "imageHeight: " << imageHeight << std::endl << "nrChannels: " << nrChannels << std::endl;
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, imageWidth, imageHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Texture image loading has failed!" << std::endl;
	}

	stbi_image_free(data);  // free the image memory
	//glUniform1i(glGetUniformLocation(programObject, "texSampler"), 0);  // ez vajon kell?

	//%%%%%%%%%%%%%%%%%
	glBindTexture(GL_TEXTURE_2D, texture[1]);  // bind texture

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);  // wrapping
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);  // wrapping
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);  // filtering
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);  // filtering

	int imageWidth_, imageHeight_, nrChannels_;
	std::string textureFileName_ = "resources/rubik texture.png";
	stbi_set_flip_vertically_on_load(true);
	unsigned char* data_ = stbi_load(textureFileName_.c_str(), &imageWidth_, &imageHeight_, &nrChannels_, 0);
	if (data_)
	{
		std::cout << "Texture image has been loaded succesfully: " << textureFileName_ << std::endl;
		std::cout << "imageWidth: " << imageWidth_ << std::endl << "imageHeight: " << imageHeight_ << std::endl << "nrChannels: " << nrChannels_ << std::endl;
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, imageWidth_, imageHeight_, 0, GL_RGBA, GL_UNSIGNED_BYTE, data_);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Texture image loading has failed!" << std::endl;
	}

	stbi_image_free(data_);  // free the image memory
	*/

	
	GLuint texture[3];

	Texture tex1("resources/Color Grid Texture.png");
	tex1.setUpTexture();
	texture[0] = tex1.textureID;

	Texture tex2("resources/rubik texture.png");
	tex2.setUpTexture();
	texture[1] = tex2.textureID;

	Texture tex3("resources/test grid.png");
	tex3.setUpTexture();
	texture[2] = tex3.textureID;

	std::cout << "textureIDs: " << texture[0] << " and " << texture[1] << " and " << texture[2] << std::endl;

	// MODEL MATRIX
	glm::mat4 modelMatrix = glm::mat4(1.0f);  // 4x4 identity matrix
	//modelMatrix = glm::translate(modelMatrix, glm::vec3(100.0f, 0.0f, 0.0f));
	//modelMatrix = glm::scale(modelMatrix, glm::vec3(0.5f, 0.5f, 0.5f));
	//modelMatrix = glm::rotate(modelMatrix, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	int modelMatrixLoc = glGetUniformLocation(objShader.programObject, "modelMatrix");  // THIS
	std::cout << "modelMatrixLoc: " << modelMatrixLoc << std::endl;

	// VIEW MATRIX - FPS-style camera
	glm::mat4 viewMatrix = camera.CreateViewMatrix();  // create viewMatrix with default parameters

	// PROJECTION MATRIX
	orthographicMatrix = glm::ortho(-2.0f, 2.0f, -1.5f, 1.5f, 0.1f, 100.0f);  // ORTHOGRAPHIC PROJECTION
	perspectiveMatrix = glm::perspective(glm::radians(camera.fov), (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT, 0.1f, 100.0f);  // PERSPECTIVE PROJECTION
	projectionMatrix = perspectiveMatrix;

	// MVP
	glm::mat4 MVP = projectionMatrix * viewMatrix * modelMatrix;  // perspective projection
	int MVPlocation = glGetUniformLocation(objShader.programObject, "MVP");
	std::cout << "MVPlocation: " << MVPlocation << std::endl;

	// light stuff
	glm::vec3 lightColor = glm::vec3(1.0f, 1.0f, 1.0f);  // white light
	glm::vec3 lightPos = glm::vec3(3.0f, 1.0f, -3.0f);  // position of light object
	int lightColorLoc = glGetUniformLocation(objShader.programObject, "lightColor");
	int lightPosLoc = glGetUniformLocation(objShader.programObject, "lightPos");
	std::cout << "lightColorLoc: " << lightColorLoc << std::endl;
	std::cout << "lightPosLoc: " << lightPosLoc << std::endl;

	objShader.useShader();
	glUniform3fv(lightColorLoc, 1, glm::value_ptr(lightColor));
	glUniform3fv(lightPosLoc, 1, glm::value_ptr(lightPos));
	modelMatrix = glm::mat4(1.0f);
	modelMatrix = glm::scale(modelMatrix, glm::vec3(5.0f, 1.0f, 5.0f));
	glUniformMatrix4fv(modelMatrixLoc, 1, GL_FALSE, glm::value_ptr(modelMatrix));  // THIS

	/////////////////////////// use Object Class ///////////////////////////
	Object obj1("resources/NewSuzanne.obj", objShader.programObject, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.7f, 0.7f, 0.7f), 0.0f, "x",
		camera, texture[0], WINDOW_WIDTH, WINDOW_HEIGHT, glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(3.0f, 1.0f, -3.0f));
	obj1.initialize();

	Object obj2("resources/NewSuzanne.obj", objShader.programObject, glm::vec3(2.0f, 0.0f, 0.0f), glm::vec3(0.4f, 0.4f, 0.4f), 180.0f, "y",
		camera, texture[2], WINDOW_WIDTH, WINDOW_HEIGHT, glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(3.0f, 1.0f, -3.0f));
	obj2.initialize();

	Object obj3("resources/texturePractice.obj", objShader.programObject, glm::vec3(-2.0f, 0.0f, 0.0f), glm::vec3(0.4f, 0.4f, 0.4f), 0.0f, "y",
		camera, texture[1], WINDOW_WIDTH, WINDOW_HEIGHT, glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(3.0f, 1.0f, -3.0f));
	obj3.initialize();

	//glUseProgram(programObject); // modify the value of uniform variables (glUniformMatrix4fv) after calling glUseProgram

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		/* Render here */
		//glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LESS);

		processKeyInput(window);  // changes cameraPosition

		// time calculations
		float currentFrame = (float)glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		viewMatrix = camera.CreateViewMatrix();  // update in every frame (WASD and mouse)
		projectionMatrix = glm::perspective(glm::radians(camera.fov), (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT, 0.1f, 100.0f);  // update in every frame (zoom)

		//obj.render(camera);

		// draw Suzanne
		/*
		objShader.useShader();
		// recalculate viewMatrix and projectionMatrix in every farem in case of input
		//projectionMatrix = perspectiveMatrix;
		modelMatrix = glm::mat4(1.0f);
		//modelMatrix = glm::translate(modelMatrix, glm::vec3(10.0f, 0.0f, 0.0f));
		viewMatrix = camera.CreateViewMatrix();  // update in every frame (WASD and mouse)
		projectionMatrix = glm::perspective(glm::radians(camera.fov), (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT, 0.1f, 100.0f);  // update in every frame (zoom)
		MVP = projectionMatrix * viewMatrix * modelMatrix;
		glUniformMatrix4fv(MVPlocation, 1, GL_FALSE, glm::value_ptr(MVP));  // recalculate MVP in every frame
		glUniformMatrix4fv(modelMatrixLoc, 1, GL_FALSE, glm::value_ptr(modelMatrix));  // THIS

		glUniform3fv(lightColorLoc, 1,  glm::value_ptr(lightColor));
		glUniform3fv(lightPosLoc, 1, glm::value_ptr(lightPos));

		glBindVertexArray(vao);
		//glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture[0]);  // bind texture
		glDrawElements(GL_TRIANGLES, notSplitted.indices.size(), GL_UNSIGNED_INT, 0);
		*/
		
		// draw plane
		objShader.useShader();
		modelMatrix = glm::mat4(1.0f);
		modelMatrix = glm::scale(modelMatrix, glm::vec3(5.0f, 1.0f, 5.0f));
		MVP = projectionMatrix * viewMatrix * modelMatrix;
		glUniformMatrix4fv(MVPlocation, 1, GL_FALSE, glm::value_ptr(MVP));  // recalculate MVP in every frame
		glUniformMatrix4fv(modelMatrixLoc, 1, GL_FALSE, glm::value_ptr(modelMatrix));  // THIS

		glUniform3fv(lightColorLoc, 1, glm::value_ptr(lightColor));
		glUniform3fv(lightPosLoc, 1, glm::value_ptr(lightPos));

		glBindVertexArray(planeVAO);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture[2]);  // bind texture
		glDrawElements(GL_TRIANGLES, plane.indices.size(), GL_UNSIGNED_INT, 0);


		// draw light source
		lightObjShader.useShader();
		modelMatrix = glm::mat4(1.0f);
		modelMatrix = glm::translate(modelMatrix, lightPos);
		modelMatrix = glm::scale(modelMatrix, glm::vec3(0.5f, 0.5f, 0.5f));
		MVP = projectionMatrix * viewMatrix * modelMatrix;
		glUniformMatrix4fv(MVPlocation, 1, GL_FALSE, glm::value_ptr(MVP));  // recalculate MVP in every frame

		glBindVertexArray(lightVAO);
		//glActiveTexture(GL_TEXTURE0);
		//glBindTexture(GL_TEXTURE_2D, texture[1]);  // bind texture
		glDrawElements(GL_TRIANGLES, lightSourceObject.indices.size(), GL_UNSIGNED_INT, 0);

		obj1.render(camera);
		obj2.render(camera);
		obj3.render(camera);

		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();
	}

	//glUseProgram(0);
	glBindVertexArray(0);
	//glDisableVertexAttribArray(positionAttribIndex);
	//glDisableVertexAttribArray(textureAttribIndex);
	//glDeleteProgram(shader.programObject);
	objShader.cleanUpProgram();
	lightObjShader.cleanUpProgram();

	glfwTerminate();  // terminate GLFW
	return 0;
}



static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, GLFW_TRUE);
	}
	else if (key == GLFW_KEY_P && action == GLFW_PRESS)
	{
		// change projection mode (perspective vs ortohraphic)
		/*
		if (isPerspective)
		{
			projectionMatrix = orthographicMatrix;
			isPerspective = false;
		}
		else if (!isPerspective)
		{
			projectionMatrix = perspectiveMatrix;
			isPerspective = true;
		}
		*/
	}
}

void processKeyInput(GLFWwindow* window)  // gets called every frame
{
	float cameraSpeed = 2.5f * deltaTime;
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
		camera.ProcessKeyInput("W", deltaTime);
	}
	else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		camera.ProcessKeyInput("S", deltaTime);
	}
	else if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		camera.ProcessKeyInput("A", deltaTime);
	}
	else if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		camera.ProcessKeyInput("D", deltaTime);
	}
}

static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos)  // changes cameraFront
{
	if (firstMouse)
	{
		lastX = (float)xpos;
		lastY = (float)ypos;
		firstMouse = false;
	}

	float xOffset = (float)xpos - lastX;
	float yOffset = lastY - (float)ypos;

	lastX = (float)xpos;
	lastY = (float)ypos;

	camera.ProcessMouseMovement(xOffset, yOffset);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(yoffset);
}

/*
std::string shaderSourceCodeReader(std::string shaderFileName)
{
	std::ifstream ifs(shaderFileName, std::ifstream::in);
	std::string line;
	std::stringstream ss;

	if (ifs.is_open())
	{
		while (std::getline(ifs, line))
		{
			ss << line << std::endl;
		}
	}
	else
	{
		std::cout << "Shader file is NOT open: " << shaderFileName << std::endl;
	}

	return ss.str();
}


GLuint setUpShader(GLenum shaderType, std::string shaderFileName, GLuint programObject)
{
	GLuint shaderObject = glCreateShader(shaderType);  // empty shader object
	std::string shaderSrc = shaderSourceCodeReader(shaderFileName);  // read shader source code
	const GLchar* shaderSrc_ = (const GLchar*)shaderSrc.c_str();  // convert string to GLchar*
	glShaderSource(shaderObject, 1, &shaderSrc_, 0);  // give the shader src to the shader object, copy shader into internal memory
	glCompileShader(shaderObject);  // compile shader object

	shaderErrorHandling(shaderObject);

	glAttachShader(programObject, shaderObject);  // attach shader object to the program object
	return shaderObject;
}

void shaderErrorHandling(GLuint shaderObject)
{
	GLint isCompiled = 0;  // shader compilation error checking
	glGetShaderiv(shaderObject, GL_COMPILE_STATUS, &isCompiled);

	if (!isCompiled)
	{
		std::cout << "Error: Shader compilation has failed!" << std::endl;
		glDeleteShader(shaderObject);
	}
	else if (isCompiled)
	{
		std::cout << "Shader compilation was successful!" << std::endl;
	}
}

void linkingErrorHandling(GLuint programObject)
{
	GLint isLinked = 0;  // program linking error checking
	glGetProgramiv(programObject, GL_LINK_STATUS, &isLinked);

	if (!isLinked)
	{
		std::cout << "Error: Program linking has failed!" << std::endl;
		glDeleteProgram(programObject);
	}
	else if (isLinked)
	{
		std::cout << "Program linking was successful!" << std::endl;
	}
}

void cleanUpShader(GLuint programObject, GLuint shaderObject)
{
	glDetachShader(programObject, shaderObject);
	glDeleteShader(shaderObject);
}
*/