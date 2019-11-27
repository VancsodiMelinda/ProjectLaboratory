#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/ext.hpp>
#include <glm/gtx/string_cast.hpp>

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
#include "Light.h"
#include "Shadow.h"
#include "Test.h"
#include "Test2.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processKeyInput(GLFWwindow* window);
void renderQuad();

// global camera variables
Camera camera;  // create camera object with default constructor

float deltaTime = 0.0f;
float lastFrame = 0.0f;

glm::mat4 projectionMatrix;
glm::mat4 orthographicMatrix;
glm::mat4 perspectiveMatrix;
bool isPpressed;

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

	/////////////////////////// SHADER ///////////////////////////

	// shader for objects
	Shader objShader("src/VertexShader.txt", "src/FragmentShader.txt");
	//objShader.runShaderCode();
	objShader.initialize();

	// shader for lightsource object
	Shader lightObjShader("src/LightObjVertexShader.txt", "src/LightObjFragmentShader.txt");
	//lightObjShader.runShaderCode();
	lightObjShader.initialize();

	// shader for shadow
	Shader shadowShader("src/ShadowVS.txt", "src/ShadowFS.txt");
	//shadowShader.runShaderCode();
	shadowShader.initialize();

	// shader for quad
	Shader quadShader("src/DebugQuadVS.txt", "src/DebugQuadFS.txt");
	//quadShader.runShaderCode();
	quadShader.initialize();

	/////////////////////////// TEXTURE ///////////////////////////

	GLuint texture[5];

	Texture tex1("resources/Color Grid Texture.png");
	tex1.setUpTexture();
	texture[0] = tex1.textureID;

	Texture tex2("resources/rubik texture.png");
	tex2.setUpTexture();
	texture[1] = tex2.textureID;

	Texture tex3("resources/test grid.png");
	tex3.setUpTexture();
	texture[2] = tex3.textureID;

	Texture tex4("resources/Wood texture.jpg");
	tex4.setUpTexture();
	texture[3] = tex4.textureID;

	Texture tex5("resources/Velociraptor texture.png");
	tex5.setUpTexture();
	texture[4] = tex5.textureID;

	/////////////////////////// LIGHT ///////////////////////////
	//Light light("resources/RubiksCube.obj", lightObjShader.programObject, glm::vec3(3.0f, 1.0f, -3.0f), glm::vec3(0.6f, 0.6f, 0.6f), 0.6f, "x",
	//	camera, WINDOW_WIDTH, WINDOW_HEIGHT, glm::vec3(1.0f, 1.0f, 1.0f));
	//light.initialize();

	//%%%%%%%%%%%%%%%%%%%%%% TEST %%%%%%%%%%%%%%%%%%%%%%//
	/*
	Test test("resources/NewSuzanne.obj", glm::vec3(3.0f, 1.0f, -3.0f), glm::vec3(0.6f, 0.6f, 0.6f), 0.6f, "x");
	test.initialize();
	std::cout << "TEST: vao: " << test.vao << "; vbo: " << test.vbo << "; ibo: " << test.ibo << std::endl;

	
	glm::mat4 viewMatrix = camera.CreateViewMatrix();  // update in every frame (WASD and mouse)
	glm::mat4 projectionMatrix = glm::perspective(glm::radians(camera.fov), (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT, 0.1f, 100.0f);  // update in every frame (zoom)

	Test2 test2(objShader.programObject, test.vao, test.vbo, test.ibo, test.data, test.modelMatrix, viewMatrix, projectionMatrix);
	test2.initialize();
	*/

	//%%%%%%%%%%%%%%%%%%%%%% TEST %%%%%%%%%%%%%%%%%%%%%%//

	/////////////////////////// LIGHT ///////////////////////////

	Light light("resources/RubiksCube.obj", lightObjShader.programObject, glm::vec3(3.0f, 1.0f, -3.0f), glm::vec3(0.6f, 0.6f, 0.6f), 0.6f, "x",
		camera, WINDOW_WIDTH, WINDOW_HEIGHT, glm::vec3(1.0f, 1.0f, 1.0f));
	light.initialize();
	
	
	/////////////////////////// SHADOW ///////////////////////////
	
	float quadVertices[] = { // vertex attributes for a quad that fills the entire screen in Normalized Device Coordinates.
		// positions   // texCoords
		-1.0f,  1.0f,  0.0f, 1.0f,
		-1.0f, -1.0f,  0.0f, 0.0f,
		 1.0f, -1.0f,  1.0f, 0.0f,

		-1.0f,  1.0f,  0.0f, 1.0f,
		 1.0f, -1.0f,  1.0f, 0.0f,
		 1.0f,  1.0f,  1.0f, 1.0f
	};
	// screen quad VAO
	unsigned int quadVAO, quadVBO;
	glGenVertexArrays(1, &quadVAO);
	glGenBuffers(1, &quadVBO);
	glBindVertexArray(quadVAO);
	glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
	
	//Shadow sh(obj5.vao, shadowShader.programObject, obj5.data);

	// create 2D texture
	const unsigned int SHADOW_WIDTH = 1024;
	const unsigned int SHADOW_HEIGHT = 1024;
	
	
	Shadow shTest1("resources/NewSuzanne.obj", shadowShader.programObject, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.7f, 0.7f, 0.7f), 0.0f, "x",
		SHADOW_WIDTH, SHADOW_HEIGHT, light.lightPos);
	shTest1.initialize();
	
	/*
	Shadow shTest1(shadowShader.programObject, obj1.vao, obj1.vbo, obj1.ibo, obj1.modelMatrix, SHADOW_WIDTH, SHADOW_HEIGHT, light.lightPos);
	shTest1.initialize();
	*/

	Shadow shTest2("resources/NewSuzanne.obj", shadowShader.programObject, glm::vec3(2.0f, 0.0f, 0.0f), glm::vec3(0.4f, 0.4f, 0.4f), 180.0f, "y",
		SHADOW_WIDTH, SHADOW_HEIGHT, light.lightPos);
	shTest2.initialize();

	Shadow shTest3("resources/RubiksCube.obj", shadowShader.programObject, glm::vec3(-2.0f, 0.0f, 0.0f), glm::vec3(0.4f, 0.4f, 0.4f), 0.0f, "x",
		SHADOW_WIDTH, SHADOW_HEIGHT, light.lightPos);
	shTest3.initialize();

	Shadow shTest4("resources/Ground.obj", shadowShader.programObject, glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(5.0f, 1.0f, 5.0f), 0.0f, "x",
		SHADOW_WIDTH, SHADOW_HEIGHT, light.lightPos);
	shTest4.initialize();
	
	//Shadow shTest5("resources/Velociraptor.obj", shadowShader.programObject, glm::vec3(0.0f, 2.0f, 0.0f), glm::vec3(0.5f, 0.5f, 0.5f), 0.0f, "x",
		//SHADOW_WIDTH, SHADOW_HEIGHT, light.lightPos);
	//shTest5.initialize();
	
	

	// create texture, serves as the depth attachment fot the fbo
	GLuint shadowMap;
	glActiveTexture(GL_TEXTURE1);
	glGenTextures(1, &shadowMap);
	glBindTexture(GL_TEXTURE_2D, shadowMap);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_INT, NULL);  // allocate memory
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glBindTexture(GL_TEXTURE_2D, 0);

	// create framebuffer object
	GLuint fbo;
	glGenFramebuffers(1, &fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);
	std::cout << "fbo: " << fbo << std::endl;

	// attach shadowMap to the framebuffer
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, shadowMap, 0);  // attach texture to the framebuffer
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	
	// create render buffer object
	//unsigned int rbo;
	//glGenRenderbuffers(1, &rbo);
	//glBindRenderbuffer(GL_RENDERBUFFER, rbo);
	//glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, SHADOW_WIDTH, 600);
	//glBindRenderbuffer(GL_RENDERBUFFER, 0);
	//glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);
	

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE)
	{
		std::cout << "Framebuffer is good." << std::endl;
	}
	else
	{
		std::cout << "ERROR: Framebuffer is NOT good." << std::endl;
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	
	//int shadowMapLoc = glGetUniformLocation(objShader.programObject, "shadowMap");
	//quadShader.useShader();
	//glUniform1i(shadowMapLoc, 1);
	//std::cout << "_____________________" << std::endl;
	//std::cout << "shadowMapLoc: " << shadowMapLoc << std::endl;
	

	// create transformation matrixes to render from light point of view
	/*
	glBindVertexArray(obj1.vao);
	GLuint positionAttribIndex = glGetAttribLocation(shadowShader.programObject, "in_vertexPosition");	// layout (location = 0) in vec3 in_vertexPosition;
	std::cout << "positionAttribIndex: " << positionAttribIndex << std::endl;
	glEnableVertexAttribArray(positionAttribIndex);
	int stride3f = 3 * sizeof(GL_FLOAT);
	glVertexAttribPointer(positionAttribIndex, 3, GL_FLOAT, GL_FALSE, stride3f, 0);
	//glBindVertexArray(0);
	*/
	/*
	float nearPlane = 0.1f;
	float farPlane = 100.0f;
	glm::mat4 lightProjMatrix = glm::ortho(-2.0f, 2.0f, -1.5f, 1.5f, nearPlane, farPlane);
	glm::mat4 lightViewMatrix = glm::lookAt(light.lightPos, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	glm::mat4 modelMatrix = glm::mat4(1.0f);
	modelMatrix = glm::scale(modelMatrix, glm::vec3(0.7f, 0.7f, 0.7f));

	glm::mat4 lightSpaceMatrix = lightProjMatrix * lightViewMatrix;

	int lightSpaceMatrixLoc = glGetUniformLocation(shadowShader.programObject, "lightSpaceMatrix");
	int modelMatrixLoc = glGetUniformLocation(shadowShader.programObject, "modelMatrix");
	std::cout << "lightSpaceMatrixLoc: " << lightSpaceMatrixLoc << std::endl;
	std::cout << "modelMatrixLoc: " << modelMatrixLoc << std::endl;
	*/

	

	/////////////////////////// OBJECT ///////////////////////////

	Object obj1("resources/NewSuzanne.obj", objShader.programObject, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.7f, 0.7f, 0.7f), 0.0f, "x",
		camera, texture[0], WINDOW_WIDTH, WINDOW_HEIGHT, light.lightColor, light.lightPos, shTest1.MVP, shadowMap);
	obj1.initialize();

	//std::cout << "-----------------------" << std::endl;
	//std::cout << glm::to_string(shTest1.lightSpaceMatrix) << std::endl;
	//std::cout << "-----------------------" << std::endl;

	Object obj2("resources/NewSuzanne.obj", objShader.programObject, glm::vec3(2.0f, 0.0f, 0.0f), glm::vec3(0.4f, 0.4f, 0.4f), 180.0f, "y",
		camera, texture[2], WINDOW_WIDTH, WINDOW_HEIGHT, light.lightColor, light.lightPos, shTest2.MVP, shadowMap);
	obj2.initialize();

	Object obj3("resources/RubiksCube.obj", objShader.programObject, glm::vec3(-2.0f, 0.0f, 0.0f), glm::vec3(0.4f, 0.4f, 0.4f), 0.0f, "y",
		camera, texture[1], WINDOW_WIDTH, WINDOW_HEIGHT, light.lightColor, light.lightPos, shTest3.MVP, shadowMap);
	obj3.initialize();

	Object obj4("resources/Ground.obj", objShader.programObject, glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(5.0f, 1.0f, 5.0f), 0.0f, "y",
		camera, texture[3], WINDOW_WIDTH, WINDOW_HEIGHT, light.lightColor, light.lightPos, shTest4.MVP, shadowMap);
	obj4.initialize();

	//Object obj5("resources/Velociraptor.obj", objShader.programObject, glm::vec3(0.0f, 2.0f, 0.0f), glm::vec3(0.5f, 0.5f, 0.5f), 0.0f, "y",
		//camera, texture[3], WINDOW_WIDTH, WINDOW_HEIGHT, light.lightColor, light.lightPos);
	//obj5.initialize();


	glEnable(GL_DEPTH_TEST);

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		/* Render here */
		//glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
		//glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		//glEnable(GL_DEPTH_TEST);
		//glDepthFunc(GL_LESS);

		processKeyInput(window);  // changes cameraPosition

		// time calculations
		float currentFrame = (float)glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// specify clear values for the buffers
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClearDepth(1.0f);
		glClearStencil(0);

		/*
		glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);  // set viewport
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glEnable(GL_DEPTH_TEST);
		test2.render();
		*/

		
		// SHADOW CODE 1
		glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);  // set viewport
		glBindFramebuffer(GL_FRAMEBUFFER, fbo);  // bind application-created framebuffer
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);  // clear depth and color buffer
		glEnable(GL_DEPTH_TEST);  // if enabled, do depth comparison and update the depth buffer
		//glDrawBuffer(GL_NONE);
		//glReadBuffer(GL_NONE);
		shTest1.render();
		shTest2.render();
		shTest3.render();
		shTest4.render();

		//shTest5.render();
		//obj1.render(camera);

		/*
		// SHADOW CODE 2
		glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);  // set viewport
		glBindFramebuffer(GL_FRAMEBUFFER, 0);  // bind window-system-provided framebuffer
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);  // clear depth and color buffer
		glDisable(GL_DEPTH_TEST);
		//glDrawBuffer(GL_BACK);
		//glReadBuffer(GL_BACK);
		quadShader.useShader();
		glBindVertexArray(quadVAO);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, shadowMap);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		//glDrawArrays(GL_LINES, 0, 6);
		//renderQuad();
		*/

		
		// ORIGINAL RENDERING
		glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);  // set viewport
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glEnable(GL_DEPTH_TEST);
		light.render(camera);
		obj1.render(camera);
		obj2.render(camera);
		obj3.render(camera);
		obj4.render(camera);
		//obj5.render(camera);
		

		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();
	}

	glUseProgram(0);
	glBindVertexArray(0);
	//glDisableVertexAttribArray(positionAttribIndex);
	//glDisableVertexAttribArray(textureAttribIndex);
	//glDeleteProgram(shader.programObject);
	objShader.cleanUpProgram();
	lightObjShader.cleanUpProgram();

	//glDeleteFramebuffers(1, &fbo);

	glfwTerminate();  // terminate GLFW
	return 0;
}

/*
unsigned int quadVAO = 0;
unsigned int quadVBO;
void renderQuad()
{
	if (quadVAO == 0)
	{
		float quadVertices[] = {
			// positions        // texture Coords
			-1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
			-1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
			 1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
			 1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
		};
		// setup plane VAO
		glGenVertexArrays(1, &quadVAO);
		glGenBuffers(1, &quadVBO);
		glBindVertexArray(quadVAO);
		glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	}
	glBindVertexArray(quadVAO);
	//glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glDrawArrays(GL_TRIANGLES, 0, 4);
	glBindVertexArray(0);
}
*/

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, GLFW_TRUE);
	}
	else if (key == GLFW_KEY_P && action == GLFW_PRESS)
	{
		//glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

		// change projection mode (perspective vs ortohraphic)
		
		if (isPpressed)
		{
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
			isPpressed = false;
		}
		else if (!isPpressed)
		{
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
			isPpressed = true;
		}
		
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