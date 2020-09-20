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

	LoadAssets assets;		// ObjectContainer models[1];
	LoadPrograms programs;	// array of shaders
	LoadLights lights;		// multiple arrays of different lights
	LoadShadows shadows(lights, assets, programs);
	LoadSkyboxes skybox;
	
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
	lights.config(assets.models[0], assets.models[1], programs.programs[1]);
	shadows.config();
	//skybox.config(assets.models[0], programs.programs[3]);

	glEnable(GL_DEPTH_TEST);

	while (!glfwWindowShouldClose(window))
	{
		processKeyboardInput(window);	// WASD + R

		// specify clear values for the buffers
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		//glClearDepth(1.0f);
		//glClearStencil(0);

		// render shadows
		glViewport(0, 0, DIR_SHADOW_WIDTH, DIR_SHADOW_HEIGHT);
		shadows.render();

		// render scene
		glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		renderer.renderAssets(kamera);
		lights.render(assets.models[0], assets.models[1], programs.programs[1], kamera);

		// render skybox
		glDepthFunc(GL_LEQUAL);		// passes if the incoming depth value is less than or equal to the stored depth value
		skybox.render(assets.models[0], programs.programs[3], kamera);
		glDepthFunc(GL_LESS);	// back to default: passes if the incoming depth value is less than the stored depth value 

		glfwSwapBuffers(window);
		glfwPollEvents();
	}







	// Cleanup
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	glfwTerminate();  // terminate GLFW
	return 0;
}

