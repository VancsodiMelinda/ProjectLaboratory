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

const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;

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

	ObjLoader objLoader;
	std::string objFileName = "resources/Suzanne texture test custom unwrapped.obj";
	objectData test = objLoader.loadObjFileV2(objFileName);
	std::cout << "Loaded object file: " << objFileName << std::endl;

	Shader shader;
	shader.runShaderCode();

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

	int vSize = test.vertices.size();
	int vtSize = test.uvs.size();
	int vnSize = test.normals.size();
	int iSize = test.indices.size();

	std::cout << "vSize: " << vSize << std::endl;
	std::cout << "vtSize: " << vtSize << std::endl;
	std::cout << "vnSize: " << vnSize << std::endl;
	std::cout << "iSize: " << iSize << std::endl;

	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	GLuint vbo, ibo;
	glGenBuffers(1, &vbo);
	glGenBuffers(1, &ibo);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);  // bind VBO
	glBufferData(GL_ARRAY_BUFFER, (vSize + vtSize) * sizeof(GL_FLOAT), 0, GL_STATIC_DRAW);  // reserve space
	glBufferSubData(GL_ARRAY_BUFFER, 0, vSize * sizeof(GL_FLOAT), &test.vertices[0]);  // VERTEX COORDINATES
	glBufferSubData(GL_ARRAY_BUFFER, vSize * sizeof(GL_FLOAT), vtSize * sizeof(GL_FLOAT), &test.uvs[0]);  // TEXTURE COORDINATES
	glBindBuffer(GL_ARRAY_BUFFER, 0);  // unbind VBO

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);  // bind IBO
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, iSize * sizeof(GLuint), &test.indices[0], GL_STATIC_DRAW);  // INDICES
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);  // unbind IBO

	//glUseProgram(programObject);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);

	GLuint positionAttribIndex = glGetAttribLocation(shader.programObject, "in_vertexPosition");
	GLuint textureAttribIndex = glGetAttribLocation(shader.programObject, "in_textureCoords");

	std::cout << "positionAttribIndex: " << positionAttribIndex << std::endl;
	std::cout << "textureAttribIndex: " << textureAttribIndex << std::endl;

	glEnableVertexAttribArray(positionAttribIndex);
	glEnableVertexAttribArray(textureAttribIndex);

	GLintptr vOffset = 0 * sizeof(GL_FLOAT);
	GLintptr tOffset = vSize * sizeof(GL_FLOAT);
	int stride3 = 3 * sizeof(GL_FLOAT);
	int stride2 = 2 * sizeof(GL_FLOAT);

	glVertexAttribPointer(positionAttribIndex, 3, GL_FLOAT, GL_FALSE, stride3, 0);
	glVertexAttribPointer(textureAttribIndex, 2, GL_FLOAT, GL_FALSE, stride2, (GLvoid*)tOffset);

	// TEXTURE
	GLuint tex;
	glGenTextures(1, &tex);
	//glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, tex);  // bind texture

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
		std::cout << "imageWidth: " << imageWidth << std::endl << "imageHeight: " << imageHeight << std::endl << "nrChannels: " << nrChannels << std::endl;;
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, imageWidth, imageHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Texture image loading has failed!" << std::endl;
	}

	stbi_image_free(data);  // free the image memory
	//glUniform1i(glGetUniformLocation(programObject, "texSampler"), 0);  // ez vajon kell?

	// MODEL MATRIX
	glm::mat4 modelMatrix = glm::mat4(1.0f);  // 4x4 identity matrix
	//modelMatrix = glm::scale(modelMatrix, glm::vec3(0.5f, 0.5f, 0.5f));
	//modelMatrix = glm::rotate(modelMatrix, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));

	// VIEW MATRIX - FPS-style camera
	glm::mat4 viewMatrix = camera.CreateViewMatrix();  // create viewMatrix with default parameters

	// PROJECTION MATRIX
	orthographicMatrix = glm::ortho(-2.0f, 2.0f, -1.5f, 1.5f, 0.1f, 100.0f);  // ORTHOGRAPHIC PROJECTION
	perspectiveMatrix = glm::perspective(glm::radians(camera.fov), (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT, 0.1f, 100.0f);  // PERSPECTIVE PROJECTION
	projectionMatrix = perspectiveMatrix;

	// MVP
	glm::mat4 MVP = projectionMatrix * viewMatrix * modelMatrix;  // perspective projection
	int MVPlocation = glGetUniformLocation(shader.programObject, "MVP");
	std::cout << "MVPlocation: " << MVPlocation << std::endl;

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

		// recalculate viewMatrix and projectionMatrix in every farem in case of input
		//projectionMatrix = perspectiveMatrix;
		viewMatrix = camera.CreateViewMatrix();  // update in every frame (WASD and mouse)
		projectionMatrix = glm::perspective(glm::radians(camera.fov), (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT, 0.1f, 100.0f);  // update in every frame (zoom)
		MVP = projectionMatrix * viewMatrix * modelMatrix;
		glUniformMatrix4fv(MVPlocation, 1, GL_FALSE, glm::value_ptr(MVP));  // recalculate MVP in every frame

		glDrawElements(GL_TRIANGLES, iSize, GL_UNSIGNED_INT, 0);

		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();
	}

	//glUseProgram(0);
	glDisableVertexAttribArray(positionAttribIndex);
	glDisableVertexAttribArray(textureAttribIndex);
	//glDeleteProgram(shader.programObject);
	shader.cleanUpProgram();

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