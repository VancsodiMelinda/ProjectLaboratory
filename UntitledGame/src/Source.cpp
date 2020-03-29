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

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos);
static void cursor_position_callback_2(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processKeyInput(GLFWwindow* window);
void renderQuad();
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
	Shader objShader("src/VertexShader.vert", "src/FragmentShader.frag");
	//objShader.runShaderCode();
	objShader.initialize();

	// shader for lightsource object
	Shader lightObjShader("src/LightObjVertexShader.vert", "src/LightObjFragmentShader.frag");
	//lightObjShader.runShaderCode();
	lightObjShader.initialize();

	// shader for shadow
	Shader shadowShader("src/ShadowVS.vert", "src/ShadowFS.frag");
	//shadowShader.runShaderCode();
	shadowShader.initialize();

	// shader for quad
	Shader quadShader("src/DebugQuadVS.vert", "src/DebugQuadFS.frag");
	//quadShader.runShaderCode();
	quadShader.initialize();

	/////////////////////////// TEXTURE ///////////////////////////

	GLuint texture[7];

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

	Texture tex6("resources/Colorful texture.jpg");
	tex6.setUpTexture();
	texture[5] = tex6.textureID;

	Texture tex7("resources/specularMap.png");
	tex7.setUpTexture();
	texture[6] = tex7.textureID;

	/////////////////////////// LIGHT ///////////////////////////

	//Light light("resources/RubiksCube.obj", lightObjShader.programObject, glm::vec3(2.0f, 1.0f, 2.0f), glm::vec3(0.2f, 0.2f, 0.2f), 0.6f, "x",
		//camera, WINDOW_WIDTH, WINDOW_HEIGHT, glm::vec3(1.0f, 1.0f, 1.0f));
	Light light("resources/RubiksCube.obj", lightObjShader.programObject, glm::vec3(0.0f, 1.0f, 2.0f), glm::vec3(0.2f, 0.2f, 0.2f), 0.6f, "x",
		camera, WINDOW_WIDTH, WINDOW_HEIGHT, glm::vec3(1.0f, 1.0f, 1.0f));
	light.initialize();
	
	
	/////////////////////////// SHOW SHADOW ///////////////////////////
	
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
	
	////////////////////////////// NEW TEST /////////////////////////
	const unsigned int SHADOW_WIDTH = 1024;
	const unsigned int SHADOW_HEIGHT = 1024;

	// LOAD OBJECT DATA
	//Data suzanneData("resources/NewSuzanne.obj");
	//suzanneData.initialize();

	Data groundData("resources/Ground.obj");
	groundData.initialize();

	Data cubeData("resources/RubiksCube.obj");
	cubeData.initialize();

	Data raptorData("resources/Velociraptor.obj");
	raptorData.initialize();

	// CREATE MODEL MATRIXES
	glm::mat4 suzanneModelMatrix = createModelMatrix(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), -35.0f, "x");
	glm::mat4 groundModelMatrix = createModelMatrix(glm::vec3(0.0f, -0.5f, 0.0f), glm::vec3(5.0f, 1.0f, 5.0f), 0.0f, "y");
	glm::mat4 cubeModelMatrix = createModelMatrix(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.5f, 0.5f, 0.5f), 20.0f, "y");
	glm::mat4 raptorModelMatrix = createModelMatrix(glm::vec3(0.0f, 0.4f, 0.0f), glm::vec3(0.5f, 0.5f, 0.5f), 0.0f, "y");

	////////////////////// NEW POINTLIGHT NEW /////////////////////////

	PointLightParams pointLightParams;
	{
		pointLightParams.position = glm::vec3(-1.0f, 1.0f, 0.0f);
		pointLightParams.color = glm::vec3(1.0f, 1.0f, 1.0f);

		pointLightParams.ambientStrength = 0.3f;
		pointLightParams.diffuseStrength = 0.6f;
		pointLightParams.specularStrength = 0.9f;

		pointLightParams.constant = 0.5f;
		pointLightParams.linear = 0.5f;
		pointLightParams.quadratic = 0.1f;

		pointLightParams.scale = glm::vec3(0.2f);
		pointLightParams.angle = 30.0f;
		pointLightParams.axes = "y";
	}

	//glm::vec3 pointlightPos = glm::vec3(1.0f);
	//glm::mat4 pointlightModelMatrix = createModelMatrix(pointLightParams.position, glm::vec3(0.2f, 0.2f, 0.2f), 20.0f, "y");

	//PointLight wut(pointLightParams, cubeData, lightObjShader.programObject, pointlightModelMatrix, camera);
	PointLight wut(pointLightParams, cubeData, lightObjShader.programObject, camera);
	wut.initialize();

	DirLightParams dirLightParams;
	{
		dirLightParams.position = glm::vec3(1.0f, 1.0f, -1.0f);
		dirLightParams.color = glm::vec3(1.0f, 1.0f, 1.0f);

		dirLightParams.ambientStrength = 0.3f;
		dirLightParams.diffuseStrength = 0.6f;
		dirLightParams.specularStrength = 0.9f;

		dirLightParams.scale = glm::vec3(0.4f);
		dirLightParams.angle = 40.0f;
		dirLightParams.axes = "x";
	}

	DirectionalLight gg(dirLightParams, cubeData, lightObjShader.programObject, camera);
	gg.initialize();

	// CREATE SHADOWS

	Data defaultData;
	LightBase defaultLight(defaultData, camera);

	Shadow shadow(defaultData, defaultLight);  // create shadowMap and fbo
	shadow.initRenderShadowMap(quadShader.programObject);

	//Shadow shadow;  // create shadowMap texture and fbo frame buffer object
	//shadow.initRenderShadowMap(quadShader.programObject);

	//Shadow suzanneShadow(shadowShader.programObject, suzanneData.vao, suzanneData.vbo, suzanneData.ibo, suzanneData.data, suzanneModelMatrix, light.lightPos, shadow.SHADOW_WIDTH, shadow.SHADOW_HEIGHT);
	//suzanneShadow.initialize();

	//Shadow groundShadow(shadowShader.programObject, groundData.vao, groundData.vbo, groundData.ibo, groundData.data, groundModelMatrix, light.lightPos, shadow.SHADOW_WIDTH, shadow.SHADOW_HEIGHT);
	//Shadow groundShadow(shadowShader.programObject, groundData.vao, groundData.vbo, groundData.ibo, groundData.data, groundModelMatrix, light.lightPos);
	//groundShadow.initialize();

	//Shadow cubeShadow(shadowShader.programObject, cubeData.vao, cubeData.vbo, cubeData.ibo, cubeData.data, cubeModelMatrix, light.lightPos, shadow.SHADOW_WIDTH, shadow.SHADOW_HEIGHT);
	//Shadow cubeShadow(shadowShader.programObject, cubeData.vao, cubeData.vbo, cubeData.ibo, cubeData.data, cubeModelMatrix, light.lightPos);
	//cubeShadow.initialize();

	//Shadow raptorShadow(shadowShader.programObject, raptorData.vao, raptorData.vbo, raptorData.ibo, raptorData.data, raptorModelMatrix, light.lightPos, shadow.SHADOW_WIDTH, shadow.SHADOW_HEIGHT);
	//Shadow raptorShadow(shadowShader.programObject, raptorData.vao, raptorData.vbo, raptorData.ibo, raptorData.data, raptorModelMatrix, dirLightParams.position);
	//Shadow raptorShadow(shadowShader.programObject, raptorData, raptorModelMatrix, dirLightParams.position);
	Shadow raptorShadow(shadowShader.programObject, raptorData, raptorModelMatrix, gg);
	raptorShadow.initialize();

	//Shadow groundShadow(shadowShader.programObject, groundData, groundModelMatrix, dirLightParams.position);
	Shadow groundShadow(shadowShader.programObject, groundData, groundModelMatrix, gg);
	groundShadow.initialize();

	//raptorShadow.init();

	// CREATE OBJECTS
	/*
	//Scene suzanne(objShader.programObject, suzanneData.vao, suzanneData.vbo, suzanneData.ibo, suzanneData.data, suzanneModelMatrix,
		//light.lightColor, light.lightPos, suzanneShadow.MVP, camera, WINDOW_WIDTH, WINDOW_HEIGHT, texture[5], shadow.shadowMap);
	//suzanne.initialize();

	Scene ground(objShader.programObject, groundData.vao, groundData.vbo, groundData.ibo, groundData.data, groundModelMatrix,
		light.lightColor, light.lightPos, groundShadow.MVP, camera, WINDOW_WIDTH, WINDOW_HEIGHT, texture[3], shadow.shadowMap, texture[6]);
	ground.initialize();

	//Scene cube(objShader.programObject, cubeData.vao, cubeData.vbo, cubeData.ibo, cubeData.data, cubeModelMatrix,
		//light.lightColor, light.lightPos, cubeShadow.MVP, camera, WINDOW_WIDTH, WINDOW_HEIGHT, texture[1], shadow.shadowMap);
	//cube.initialize();

	// THIS
	Scene raptor(objShader.programObject, raptorData.vao, raptorData.vbo, raptorData.ibo, raptorData.data, raptorModelMatrix,
		light.lightColor, light.lightPos, raptorShadow.MVP, camera, WINDOW_WIDTH, WINDOW_HEIGHT, texture[0], shadow.shadowMap, texture[6]);
	raptor.initialize();
	*/
	////////////////////////////// BRAND NEW //////////////////////////////
	//Scene2 wut2(raptorData, model1, objShader.programObject, camera, texture[0], texture[6], shadow.shadowMap, wut);
	Scene2 wut2(raptorData, raptorModelMatrix, objShader.programObject, camera, texture[0], texture[6], shadow.shadowMap, gg);
	wut2.initialize();

	//Scene2 wut3(cubeData, model2, objShader.programObject, camera, texture[1], texture[6], shadow.shadowMap, wut);
	//wut3.initialize();

	//Scene2 wut4(groundData, model3, objShader.programObject, camera, texture[3], texture[6], shadow.shadowMap, wut);
	Scene2 wut4(groundData, groundModelMatrix, objShader.programObject, camera, texture[3], texture[6], shadow.shadowMap, gg);
	wut4.initialize();
	
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
		//glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
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
		//glBindFramebuffer(GL_FRAMEBUFFER, fbo);  // bind application-created framebuffer
		glBindFramebuffer(GL_FRAMEBUFFER, shadow.fbo);  // bind application-created framebuffer
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);  // clear depth and color buffer
		glEnable(GL_DEPTH_TEST);  // if enabled, do depth comparison and update the depth buffer
		//glDrawBuffer(GL_NONE);
		//glReadBuffer(GL_NONE);


		glCullFace(GL_FRONT);  // this
		//suzanneShadow.render();
		//groundShadow.render();
		//cubeShadow.render();
		raptorShadow.render();
		groundShadow.render();
		glCullFace(GL_BACK);  // this

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
		glBindTexture(GL_TEXTURE_2D, shadow.shadowMap);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		//glDrawArrays(GL_LINES, 0, 6);
		//renderQuad();
		*/

		// this is a comment
		// ORIGINAL RENDERING
		glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);  // set viewport
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glEnable(GL_DEPTH_TEST);
		//light.render(camera);
		//suzanne.render(camera);
		//ground.render(camera);
		//cube.render(camera);
		//raptor.render(camera);

		//wut.render(camera);		// point light object
		gg.render(camera);
		wut2.render(camera);	// raptor
		//wut3.render(camera);	// cube
		wut4.render(camera);	// floor

		shadow.renderShadowMap();
		
		//wut.changeParams();
		gg.changeParams();

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