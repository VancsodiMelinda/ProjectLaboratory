#include "ShowScene.h"

ShowScene::ShowScene(GLFWwindow* window_, Kamera& kamera_) : kamera(kamera_)
{
	window = window_;
}

void ShowScene::scene1()
{
	//Instrumentor::Get().BeginSession("test profile");
	// load
	LoadAssets assets;
	LoadPrograms programs;
	LoadLights lights;
	LoadShadows shadows(lights, assets.models_, programs);
	LoadSkyboxes skybox;
	PostProcessing postProc(programs.programs[5]);
	LoadOutlines outlines(assets.models_, programs.programs[6]);

	Render renderer(
		assets.models_,
		programs,
		kamera,
		lights,
		shadows,
		skybox.skyboxes[1]
	);

	// config
	renderer.configAssets();
	lights.config(programs.programs[1]);
	shadows.config();
	skybox.config(programs.programs[4]);
	outlines.config();
	//Instrumentor::Get().EndSession();

	//int i = 1;
	//Instrumentor::Get().BeginSession("game loop profile");
	// render
	while (!glfwWindowShouldClose(window))
	{
		//std::string title = std::to_string(i) + ". loop";
		//const char* title_ = title.c_str();
		//InstrumentationTimer timer(title_);

		kamera.processKeyboardInput(window);	// WASD + R

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

		/*
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
		*/


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
		//i++;
	}
	//Instrumentor::Get().EndSession();
}

void ShowScene::scene2()
{
	//LoadAssets assets;
	std::string pathStr = "resources/assimp/test/test.obj";
	//std::string pathStr = "resources/assimp/deferred rendering/Solar system.obj";
	SceneLoader assets(&pathStr[0]);
	LoadPrograms programs;
	LoadLights lights;
	LoadShadows shadows(lights, assets.models, programs);
	LoadSkyboxes skybox;
	PostProcessing postProc(programs.programs[5]);
	//LoadOutlines outlines(assets.models, programs.programs[6]);

	Render renderer(
		assets.models,
		programs,
		kamera,
		lights,
		shadows,
		skybox.skyboxes[1]
	);

	// config
	renderer.configAssets();
	lights.config(programs.programs[1]);
	shadows.config();
	skybox.config(programs.programs[4]);
	//outlines.config();
	//Instrumentor::Get().EndSession();

	//int i = 1;
	//Instrumentor::Get().BeginSession("game loop profile");
	// render
	while (!glfwWindowShouldClose(window))
	{
		//std::string title = std::to_string(i) + ". loop";
		//const char* title_ = title.c_str();
		//InstrumentationTimer timer(title_);

		kamera.processKeyboardInput(window);	// WASD + R

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

		/*
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
		*/


		// RENDER OUTLINES
		/*
		glEnable(GL_STENCIL_TEST);
		glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
		//glStencilMask(0x00);
		glDisable(GL_DEPTH_TEST);  // draw outline on top of everything
		outlines.render(kamera);  // ok
		glStencilMask(0xFF);
		glStencilFunc(GL_ALWAYS, 1, 0xFF);
		glEnable(GL_DEPTH_TEST);
		*/

		// POST PROCESSING
		//postProc.render();

		glfwSwapBuffers(window);
		glfwPollEvents();
		//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
		//glFinish();
		//i++;
	}
	//Instrumentor::Get().EndSession();
}

void ShowScene::scene3()
{
	DeferredRendering dr(kamera);

	dr.createFboAndAttachments();
	dr.initGeometryPass();
	dr.initLightingPass();

	glEnable(GL_DEPTH_TEST);

	while (!glfwWindowShouldClose(window))
	{
		kamera.processKeyboardInput(window);

		dr.renderGeometryPass();
		dr.renderLightingPass();

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
}
