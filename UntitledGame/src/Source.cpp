#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

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
#include "Data.h"
#include "Scene.h"

#include "PointLight.h"
#include "Scene2.h"
#include "DirectionalLight.h"
#include "Skybox.h"
#include "PointShadow.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos);
static void cursor_position_callback_2(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processKeyInput(GLFWwindow* window);
glm::mat4 createModelMatrix(glm::vec3 translate, glm::vec3 scale, float rotateAngle, std::string rotateAxis);

// global camera variables
Camera camera;  // create camera object with default constructor
//Camera camera(glm::vec3(0.0f, 2.0f, -3.0f), glm::vec3(0.0f, 0.0f, 0.0f));

float deltaTime = 0.0f;
float lastFrame = 0.0f;

glm::mat4 projectionMatrix;
glm::mat4 orthographicMatrix;
glm::mat4 perspectiveMatrix;
bool isPpressed = false;

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

	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	
	// Setup Dear ImGui style
	ImGui::StyleColorsDark();
	//ImGui::StyleColorsClassic();

	// Setup Platform/Renderer bindings
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	const char* glsl_version = "#version 130";
	ImGui_ImplOpenGL3_Init(glsl_version);

	/////////////////////////// SHADER ///////////////////////////

	// shader for objects
	Shader objShader("src/shaders/Object.vert", "src/shaders/Object.frag");
	objShader.initialize();

	// shader for lightsource object
	Shader lightObjShader("src/shaders/Light.vert", "src/shaders/Light.frag");
	lightObjShader.initialize();

	// shader for shadow
	Shader shadowShader("src/shaders/Shadow.vert", "src/shaders/Shadow.frag");
	shadowShader.initialize();

	// shader for shadowmap quad
	Shader quadShader("src/shaders/DebugQuad.vert", "src/shaders/DebugQuad.frag");
	quadShader.initialize();

	// skybox shader is created in the Skybox class

	// shader for omnidirectional shadow
	Shader shadowBoxShader("src/shaders/ShadowBox.vert", "src/shaders/ShadowBox.geom", "src/shaders/ShadowBox.frag");

	/////////////////////////// TEXTURE ///////////////////////////

	GLuint texture[8];

	Texture tex1("resources/diffuse maps/color grid.png");
	tex1.setUpTexture();
	texture[0] = tex1.textureID;

	Texture tex2("resources/rubik texture.png");
	tex2.setUpTexture();
	texture[1] = tex2.textureID;

	Texture tex3("resources/diffuse maps/uv grid.png");
	tex3.setUpTexture();
	texture[2] = tex3.textureID;

	Texture tex4("resources/Wood texture.jpg");
	tex4.setUpTexture();
	texture[3] = tex4.textureID;

	Texture tex5("resources/Velociraptor texture.png");
	tex5.setUpTexture();
	texture[4] = tex5.textureID;

	Texture tex6("resources/Colorful texture.jpg");
	tex6.setUpTexture();
	texture[5] = tex6.textureID;

	Texture tex7("resources/specularMap.png");
	tex7.setUpTexture();
	texture[6] = tex7.textureID;

	Texture tex8("resources/specular maps/white.png");
	tex8.setUpTexture();
	texture[7] = tex8.textureID;
	
	////////////////////////////// NEW TEST /////////////////////////
	const unsigned int SHADOW_WIDTH = 1024;
	const unsigned int SHADOW_HEIGHT = 1024;

	/////////////////////////// LOAD OBJECTS ///////////////////////////
	Data suzanneData("resources/NewSuzanne.obj");
	suzanneData.initialize();

	Data groundData("resources/Ground.obj");
	groundData.initialize();

	Data cubeData("resources/RubiksCube.obj");
	cubeData.initialize();

	Data raptorData("resources/Velociraptor.obj");
	raptorData.initialize();

	Data sphereData("resources/objects/sphere.obj");
	sphereData.initialize();

	// CREATE MODEL MATRIXES - translate, scale, rotate
	glm::mat4 suzanneModelMatrix = createModelMatrix(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), -35.0f, "x");
	glm::mat4 groundModelMatrix = createModelMatrix(glm::vec3(0.0f, -0.5f, 0.0f), glm::vec3(5.0f, 1.0f, 5.0f), 0.0f, "y");
	glm::mat4 cubeModelMatrix = createModelMatrix(glm::vec3(-3.0f, 0.0f, 0.0f), glm::vec3(0.5f, 0.5f, 0.5f), 20.0f, "y");
	glm::mat4 raptorModelMatrix = createModelMatrix(glm::vec3(0.0f, 0.4f, 0.0f), glm::vec3(0.5f, 0.5f, 0.5f), 0.0f, "y");

	/////////////////////////// LIGHTS ///////////////////////////
	// TODO: pointlight
	
	PointLightParams pointLightParams;
	{
		pointLightParams.position = glm::vec3(-1.0f, 1.0f, 0.0f);
		pointLightParams.color = glm::vec3(1.0f, 1.0f, 1.0f);

		pointLightParams.ambientStrength = 0.3f;
		pointLightParams.diffuseStrength = 0.6f;
		pointLightParams.specularStrength = 0.9f;

		pointLightParams.constant = 0.2f;
		pointLightParams.linear = 0.1f;
		pointLightParams.quadratic = 0.1f;

		pointLightParams.scale = glm::vec3(0.2f);
		pointLightParams.angle = 30.0f;
		pointLightParams.axes = "y";
	}

	PointLight pointLight(pointLightParams, cubeData, lightObjShader.programObject, camera);
	pointLight.initialize();
	

	// DONE: directional light
	DirLightParams dirLightParams;
	{
		dirLightParams.position = glm::vec3(1.0f, 1.0f, -1.0f);
		dirLightParams.color = glm::vec3(1.0f, 1.0f, 1.0f);

		dirLightParams.ambientStrength = 0.3f;
		dirLightParams.diffuseStrength = 0.6f;
		dirLightParams.specularStrength = 0.9f;

		dirLightParams.scale = glm::vec3(0.2f);
		dirLightParams.angle = 0.0f;
		dirLightParams.axes = "x";
	}

	DirectionalLight dirLight(dirLightParams, sphereData, lightObjShader.programObject, camera);
	dirLight.initialize();

	/////////////////////////// SHADOW ///////////////////////////

	Data defaultData;
	LightBase defaultLight(defaultData, camera);

	Shadow shadow(defaultData, defaultLight);  // create shadowMap and fbo
	shadow.initRenderShadowMap(quadShader.programObject);


	Shadow raptorShadow(shadowShader.programObject, raptorData, raptorModelMatrix, pointLight);
	raptorShadow.initialize();

	Shadow groundShadow(shadowShader.programObject, groundData, groundModelMatrix, pointLight);
	groundShadow.initialize();

	Shadow cubeShadow(shadowShader.programObject, cubeData, cubeModelMatrix, pointLight);
	cubeShadow.initialize();

	//raptorShadow.init();

	/////////////////////////// SHADOW CUBE ///////////////////////////
	PointShadow raptorPoint(shadowBoxShader.programObject, raptorData, raptorModelMatrix, pointLight);
	raptorPoint.initialize();  // create texture and fbo

	PointShadow groundPoint(shadowBoxShader.programObject, groundData, groundModelMatrix, pointLight);

	PointShadow cubePoint(shadowBoxShader.programObject, cubeData, cubeModelMatrix, pointLight);

	/////////////////////////// SCENE ///////////////////////////
	Scene2 raptorObject(raptorData, raptorModelMatrix, objShader.programObject, camera, texture[2], texture[6],
		shadow.shadowMap, pointLight, texture[5], raptorPoint.textureID);
	raptorObject.initialize();

	Scene2 groundObject(groundData, groundModelMatrix, objShader.programObject, camera, texture[3], texture[6],
		shadow.shadowMap, pointLight, texture[5], raptorPoint.textureID);
	groundObject.initialize();

	Scene2 cubeObject(cubeData, cubeModelMatrix, objShader.programObject, camera, texture[1], texture[7],
		shadow.shadowMap, pointLight, texture[5], raptorPoint.textureID);
	cubeObject.initialize();

	// SKYBOX
	Skybox skybox(cubeData);


	bool show_demo_window = true;
	bool show_another_window = true;
	ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
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

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		processKeyInput(window);  // changes cameraPosition

		// time calculations
		float currentFrame = (float)glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// specify clear values for the buffers
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClearDepth(1.0f);
		glClearStencil(0);
		
		// RENDER SHADOW
		glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);  // set viewport
		glBindFramebuffer(GL_FRAMEBUFFER, shadow.fbo);  // bind application-created framebuffer
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);  // clear depth and color buffer
		glEnable(GL_DEPTH_TEST);  // if enabled, do depth comparison and update the depth buffer

		glCullFace(GL_FRONT);  // this
		cubeShadow.render();
		raptorShadow.render();
		groundShadow.render();
		glCullFace(GL_BACK);  // this

		// RENDER OMNIDIRECTIONAL SHADOW
		glViewport(0, 0, 1024, 1024);
		glBindFramebuffer(GL_FRAMEBUFFER, raptorPoint.fbo);
		glClear(GL_DEPTH_BUFFER_BIT);
		glEnable(GL_DEPTH_TEST);

		raptorPoint.render();
		groundPoint.render();
		cubePoint.render();

		// RENDER SCENE
		glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);  // set viewport
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glEnable(GL_DEPTH_TEST);

		//dirLight.render(camera);
		pointLight.render(camera);
		raptorObject.render(camera);
		groundObject.render(camera);
		cubeObject.render(camera);

		shadow.renderShadowMap();
		
		//dirLight.changeParams();
		pointLight.changeParams();
		

		// render skybox last
		skybox.render(camera);

		// Rendering ImGui
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

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

	// Cleanup
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

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
		//glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

		// change projection mode (perspective vs ortohraphic)
		
		
		if (isPpressed)
		{
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);  // eats cursor
			glfwSetCursorPosCallback(window, cursor_position_callback);
			isPpressed = false;
		}
		else if (!isPpressed)  // first press
		{
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);  // frees cursor
			glfwSetCursorPosCallback(window, cursor_position_callback_2);
			isPpressed = true;
		}
		
	}
}

/*
void changeMouseInputMode(GLFWwindow* window)
{
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
*/

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

static void cursor_position_callback_2(GLFWwindow* window, double xpos, double ypos) {}


void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(yoffset);
}

glm::mat4 createModelMatrix(glm::vec3 translate, glm::vec3 scale, float rotateAngle, std::string rotateAxis)
{
	glm::mat4 modelMatrix = glm::mat4(1.0f);

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

	return modelMatrix;
}