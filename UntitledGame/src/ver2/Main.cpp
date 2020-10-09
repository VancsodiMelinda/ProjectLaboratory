#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "../imgui/imgui.h"
#include "../imgui/imgui_impl_glfw.h"
#include "../imgui/imgui_impl_opengl3.h"

#include "LoadAssets.h"
#include "LoadPrograms.h"
#include "LoadLights.h"
#include "LoadShadows.h"
#include "LoadSkyboxes.h"
#include "GlobalVariables.h"
#include "KeyboardInput.h"
#include "Render.h"
#include "PostProcessing.h"
#include "LoadOutlines.h"
#include "Timer.h"
#include "Instrumentor.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

int main(void)
{
	GLFWwindow* window;

	/* Initialize the library */
	if (!glfwInit())
	{
		printf("ERROR: GLFW initialization was unsuccesful!\n");
		return -1;
	}

	/* Create a windowed mode window and its OpenGL context */
	//window = glfwCreateWindow(640, 480, "Untitled Game", glfwGetPrimaryMonitor(), NULL);
	//int WINDOW_WIDTH = 800;
	//int WINDOW_HEIGHT = 600;
	window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Untitled Game", NULL, NULL);
	if (!window)
	{
		printf("ERROR: Window creation was unsuccesful!\n");
		glfwTerminate();
		return -1;
	}

	// callbacks for input (keyboard input and mouse input)
	glfwSetKeyCallback(window, key_callback);
	//glfwSetCharCallback(window, character_callback);

	glfwSetCursorPosCallback(window, cursor_position_callback);
	//glfwSetCursorEnterCallback(window, cursor_enter_callback);
	//glfwSetMouseButtonCallback(window, mouse_button_callback);
	//glfwSetScrollCallback(window, scroll_callback);

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	/* Make the window's context current */
	glfwMakeContextCurrent(window);  // make current OpenGL context

	// initialize extension loader library here, for example glad
	gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

	// check if glad is loaded properly
	if (!gladLoadGL()) {
		printf("ERROR: GLAD initialization was unsuccesful!\n");
		return -1;
	}

	// check openGL version
	printf("OpenGL Version: %d.%d\n", GLVersion.major, GLVersion.minor);
	std::cout << std::endl;

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

	////////////////////////////////////////////////////////
	Instrumentor::Get().BeginSession("New profile");
	LoadAssets assets;
	Instrumentor::Get().EndSession();
	LoadPrograms programs;

	LoadLights lights;
	LoadShadows shadows(lights, assets, programs);
	LoadSkyboxes skybox;
	PostProcessing postProc(programs.programs[5]);
	LoadOutlines outlines(assets, programs.programs[6]);
	
	Render renderer(
		assets,
		programs,
		kamera,
		lights,
		shadows,
		skybox.skyboxes[1]
	);

	std::cout << "Configurations..." << std::endl;
	renderer.configAssets();
	lights.config(programs.programs[1]);
	shadows.config();
	skybox.config(programs.programs[4]);
	outlines.config();

	//glEnable(GL_DEPTH_TEST);

	while (!glfwWindowShouldClose(window))
	{
		//glFinish();
		//Timer timer;
		processKeyboardInput(window);	// WASD + R

		// specify clear values for the buffers
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClearDepth(1);  // default
		glClearStencil(0);  // default
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);  // clear all buffers

		// RENDER SHADOWS
		glViewport(0, 0, DIR_SHADOW_WIDTH, DIR_SHADOW_HEIGHT);
		glEnable(GL_DEPTH_TEST);
		glDisable(GL_STENCIL_TEST);
		shadows.render();

		// RENDER SCENE (OBJECTS AND LIGHTS)
		//glBindFramebuffer(GL_FRAMEBUFFER, postProc.postProcContainer.fbo);  // for post-processing
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_STENCIL_TEST);
		glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE); // replace value = ref value from glStencilFunc
		glStencilFunc(GL_ALWAYS, 1, 0xFF);
		//glStencilMask(0xFF);

		// each pixel we draw something, the stencil buffer becomes 1
		renderer.renderAssets(kamera);
		lights.render(programs.programs[1], kamera);
		
		// RENDER SKYBOX
		glDisable(GL_STENCIL_TEST);
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LEQUAL);
		skybox.render(programs.programs[4], kamera);
		glDepthFunc(GL_LESS);

		// RENDER OUTLINES
		glEnable(GL_STENCIL_TEST);
		glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
		//glStencilMask(0x00);
		glDisable(GL_DEPTH_TEST);  // draw outline on top of everything
		outlines.render(kamera);
		glStencilMask(0xFF);
		glStencilFunc(GL_ALWAYS, 1, 0xFF);
		glEnable(GL_DEPTH_TEST);

		// POST PROCESSING
		//postProc.render();

		glfwSwapBuffers(window);
		glfwPollEvents();
		//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
		//glFinish();
	}

	// Cleanup
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	glfwTerminate();  // terminate GLFW
	return 0;
}

