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
#include "../Scenes/Scene1.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

void APIENTRY debug_callback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam);

int main(void)
{
	GLFWwindow* window;

	/* Initialize the library */
	if (!glfwInit())
	{
		printf("ERROR: GLFW initialization was unsuccesful!\n");
		return -1;
	}

	// window creation hints
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);

	// create window
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

	Scene1 testScene(window, kamera);

	// OpenGL debug message callback
	/*
	glEnable(GL_DEBUG_OUTPUT);
	glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
	glDebugMessageCallback(debug_callback, NULL);
	glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, NULL, GL_TRUE);
	*/
	////////////////////////////////////////////////////////

	/*
	Instrumentor::Get().BeginSession("test profile");
	LoadAssets assets;  // ok
	LoadPrograms programs;  // ok
	LoadLights lights;  // ok
	LoadShadows shadows(lights, assets, programs);  // ok
	LoadSkyboxes skybox;  // ok
	PostProcessing postProc(programs.programs[5]);  // ok
	LoadOutlines outlines(assets, programs.programs[6]); // ok
	
	Render renderer(
		assets,
		programs,
		kamera,
		lights,
		shadows,
		skybox.skyboxes[1]
	);

	std::cout << "Configurations..." << std::endl;
	renderer.configAssets();  // ok
	lights.config(programs.programs[1]);  // ok
	shadows.config();  // ok
	skybox.config(programs.programs[4]);  // ok
	outlines.config();  // ok

	Instrumentor::Get().EndSession();
	//glEnable(GL_DEPTH_TEST);

	std::cout << "START OF GAME LOOP" << std::endl;
	int i = 1;
	Instrumentor::Get().BeginSession("game loop profile");
	while (!glfwWindowShouldClose(window))
	{
		std::string title = std::to_string(i) + ". loop";
		const char* title_ = title.c_str();
		InstrumentationTimer timer(title_);

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
		shadows.render();  // ok

		// RENDER SCENE (OBJECTS AND LIGHTS)
		//glBindFramebuffer(GL_FRAMEBUFFER, postProc.postProcContainer.fbo);  // for post-processing
		glBindFramebuffer(GL_FRAMEBUFFER, 0);  // render to default fbo
		glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_STENCIL_TEST);
		glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE); // replace value = ref value from glStencilFunc
		glStencilFunc(GL_ALWAYS, 1, 0xFF);
		//glStencilMask(0xFF);

		// each pixel we draw something, the stencil buffer becomes 1
		renderer.renderAssets(kamera);  // ok
		lights.render(programs.programs[1], kamera);  // ok
		
		// RENDER SKYBOX
		glDisable(GL_STENCIL_TEST);
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LEQUAL);
		skybox.render(programs.programs[4], kamera);  // ok
		glDepthFunc(GL_LESS);

		// comment start
		if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT))
		{
			glFlush();
			glFinish();
			postProc.selectObject();
		}
		
		// pick object
		
		if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT))
		{
			glFlush();
			glFinish();
			glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

			unsigned char pixel[4] = { 'x', 'x', 'x', 'x' };
			glReadPixels(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, pixel);  // color in [0-255] range
			std::cout << "pixel color: " << pixel[0] << ", " << pixel[1] << ", " << pixel[2] << std::endl;
			//int pickedID = pixel[0] + pixel[1] * 256 + pixel[2] * 256 * 256;
			//std::cout << "ID: " << pickedID << std::endl;
		}
		// comment end

		// RENDER OUTLINES
		glEnable(GL_STENCIL_TEST);
		glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
		//glStencilMask(0x00);
		glDisable(GL_DEPTH_TEST);  // draw outline on top of everything
		outlines.render(kamera);  // ok
		glStencilMask(0xFF);
		glStencilFunc(GL_ALWAYS, 1, 0xFF);
		glEnable(GL_DEPTH_TEST);
		

		// POST PROCESSING
		//postProc.render();

		glfwSwapBuffers(window);
		glfwPollEvents();
		//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
		//glFinish();
		i++;
	}
	Instrumentor::Get().EndSession();
	*/
	
	
	// Cleanup
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	glfwTerminate();  // terminate GLFW
	return 0;
}

void APIENTRY debug_callback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam)
{
	std::string sourceStr;
	switch (source)
	{
	case GL_DEBUG_SOURCE_API:
		sourceStr = "GL_DEBUG_SOURCE_API";
		break;
	case GL_DEBUG_SOURCE_WINDOW_SYSTEM:
		sourceStr = "GL_DEBUG_SOURCE_WINDOW_SYSTEM";
		break;
	case GL_DEBUG_SOURCE_SHADER_COMPILER:
		sourceStr = "GL_DEBUG_SOURCE_SHADER_COMPILER";
		break;
	case GL_DEBUG_SOURCE_THIRD_PARTY:
		sourceStr = "GL_DEBUG_SOURCE_THIRD_PARTY";
		break;
	case GL_DEBUG_SOURCE_APPLICATION:
		sourceStr = "GL_DEBUG_SOURCE_APPLICATION";
		break;
	case GL_DEBUG_SOURCE_OTHER:
		sourceStr = "GL_DEBUG_SOURCE_OTHER";
		break;
	}

	std::string typeStr;
	switch (type)
	{
	case GL_DEBUG_TYPE_ERROR:
		typeStr = "GL_DEBUG_TYPE_ERROR";
		break;
	case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
		typeStr = "GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR";
		break;
	case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
		typeStr = "GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR";
		break;
	case GL_DEBUG_TYPE_PORTABILITY:
		typeStr = "GL_DEBUG_TYPE_PORTABILITY";
		break;
	case GL_DEBUG_TYPE_PERFORMANCE:
		typeStr = "GL_DEBUG_TYPE_PERFORMANCE";
		break;
	case GL_DEBUG_TYPE_MARKER:
		typeStr = "GL_DEBUG_TYPE_MARKER";
		break;
	case GL_DEBUG_TYPE_PUSH_GROUP:
		typeStr = "GL_DEBUG_TYPE_PUSH_GROUP";
		break;
	case GL_DEBUG_TYPE_POP_GROUP:
		typeStr = "GL_DEBUG_TYPE_POP_GROUP";
		break;
	case GL_DEBUG_TYPE_OTHER:
		typeStr = "GL_DEBUG_TYPE_OTHER";
		break;
	}

	std::string severityStr;
	switch (severity)
	{
	case GL_DEBUG_SEVERITY_HIGH:
		severityStr = "GL_DEBUG_SEVERITY_HIGH";
		break;
		case GL_DEBUG_SEVERITY_MEDIUM:
		severityStr = "GL_DEBUG_SEVERITY_MEDIUM";
		break;
	case GL_DEBUG_SEVERITY_LOW:
		severityStr = "GL_DEBUG_SEVERITY_LOW";
		break;
	case GL_DEBUG_SEVERITY_NOTIFICATION:
		severityStr = "GL_DEBUG_SEVERITY_NOTIFICATION";
		break;
	}

	std::cout << "source=" << sourceStr << ", type=" << typeStr << ", id=" << id << ", severity: " << severityStr
		<< ", message:" << message << std::endl;
}