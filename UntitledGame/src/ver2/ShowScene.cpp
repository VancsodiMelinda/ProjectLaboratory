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

	int selectedID = -1;
	//int i = 1;
	//Instrumentor::Get().BeginSession("game loop profile");
	// render
	while (!glfwWindowShouldClose(window))
	{
		//std::string title = std::to_string(i) + ". loop";
		//const char* title_ = title.c_str();
		//InstrumentationTimer timer(title_);

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		kamera.processKeyboardInput(window);	// WASD + R

		// specify clear values for the buffers
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glClearDepth(1);  // default
		glClearStencil(0);  // default
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);  // clear all buffers

		// RENDER SHADOWS
		glViewport(0, 0, DIR_SHADOW_WIDTH, DIR_SHADOW_HEIGHT);
		glEnable(GL_DEPTH_TEST);
		glDisable(GL_STENCIL_TEST);
		shadows.render();  // ok

		// RENDER SCENE (OBJECTS AND LIGHTS)
		glBindFramebuffer(GL_FRAMEBUFFER, postProc.postProcContainer.fbo);  // for post-processing
		//glBindFramebuffer(GL_FRAMEBUFFER, 0);  // render to default fbo
		glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_STENCIL_TEST);
		glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE); // replace value = ref value from glStencilFunc
		glStencilFunc(GL_ALWAYS, 1, 0xFF);
		//glStencilMask(0xFF);

		// each pixel we draw something, the stencil buffer becomes 1
		renderer.renderAssets(kamera);  // ok
		//lights.render(programs.programs[1], kamera);  // ok

		/*
		// RENDER SKYBOX
		glDisable(GL_STENCIL_TEST);
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LEQUAL);
		skybox.render(programs.programs[4], kamera);  // ok
		glDepthFunc(GL_LESS);
		*/

		/*
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

		/*
		// RENDER OUTLINES
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
		postProc.render();

		if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS && isFisrtPress)
		{
			isFisrtPress = false;
			glFlush();
			glFinish();
			selectedID = postProc.selectObject();
			showGUI = !showGUI;
		}

		if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_RELEASE)
		{
			isFisrtPress = true;
		}

		if (showGUI)
			renderer.changeParams(selectedID);

		// Rendering ImGui
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		glfwSwapBuffers(window);
		glfwPollEvents();
		//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
		//glFinish();
		//i++;
	}
	//Instrumentor::Get().EndSession();
}

void ShowScene::skyboxScene()
{
	//LoadAssets assets;
	std::string pathStr = "resources/assimp/solar system/solar system.obj";
	SceneLoader assets(&pathStr[0]);
	LoadPrograms programs;
	LoadLights lights("space");
	LoadShadows shadows(lights, assets.models, programs);
	LoadSkyboxes skybox;

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

	glEnable(GL_DEPTH_TEST);
	
	// game loop
	while (!glfwWindowShouldClose(window))
	{
		kamera.processKeyboardInput(window);

		// specify clear values for the buffers
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClearDepth(1);
		glClearStencil(0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

		
		// RENDER SHADOWS
		glViewport(0, 0, DIR_SHADOW_WIDTH, DIR_SHADOW_HEIGHT);
		glEnable(GL_DEPTH_TEST);
		glDisable(GL_STENCIL_TEST);
		shadows.render();
		

		// RENDER SCENE (OBJECTS AND LIGHTS)
		glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		renderer.renderAssets(kamera);
		//lights.render(programs.programs[1], kamera);

		
		// RENDER SKYBOX
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LEQUAL);
		skybox.render(programs.programs[4], kamera);
		glDepthFunc(GL_LESS);
		

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	//Instrumentor::Get().EndSession();
}


/// <summary>
/// Scene with 9 textured sphere objects (moon) and 24 pointlights around them
/// </summary>
void ShowScene::deferredRenderingScene()
{
	DeferredRendering dr(kamera);

	dr.createFboAndAttachments();
	dr.initGeometryPass();
	dr.initLightingPass();
	dr.initLightModel();

	glEnable(GL_DEPTH_TEST);

	while (!glfwWindowShouldClose(window))
	{
		kamera.processKeyboardInput(window);

		dr.renderGeometryPass();
		dr.renderLightingPass();
		dr.renderLightModel();

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
}


void ShowScene::myRoom()
{
	// load
	std::string pathStr = "resources/assimp/my room/my room.obj";
	SceneLoader assets(&pathStr[0]);

	LoadPrograms programs;
	LoadLights lights("room");
	LoadShadows shadows(lights, assets.models, programs);
	LoadSkyboxes skybox;  // not used
	PostProcessing postProc(programs.programs[5]);

	// init renderer
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
	//skybox.config(programs.programs[4]);

	glEnable(GL_DEPTH_TEST);

	// render
	while (!glfwWindowShouldClose(window))
	{
		kamera.processKeyboardInput(window);	// WASD + R

		// specify clear values for the buffers
		//glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glClearDepth(1);  // default
		glClearStencil(0);  // default

		// clear all buffers of the default fbo
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

		// RENDER SHADOWS
		glViewport(0, 0, DIR_SHADOW_WIDTH, DIR_SHADOW_HEIGHT);
		//glEnable(GL_DEPTH_TEST);
		//glDisable(GL_STENCIL_TEST);
		shadows.render();

		// RENDER SCENE AND LIGHTS
		//glBindFramebuffer(GL_FRAMEBUFFER, postProc.postProcContainer.fbo);  // for post-processing
		//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		//glEnable(GL_DEPTH_TEST);

		glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
		renderer.renderAssets(kamera);
		//lights.render(programs.programs[1], kamera);

		//postProc.render();

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
}

void ShowScene::demoScene()
{
	// load models with textures
	std::string pathStr = "resources/assimp/demo/demo scene.obj";
	SceneLoader assets(&pathStr[0]);

	// load puzzle model with textures separately
	CreateTexture diffuseMap("resources/assimp/demo/six gray 4px.png", TextureType::diffuseMap);
	CreateTexture specularMap(TextureType::specularMap);
	CreateModel six("resources/assimp/demo/little nightmares logo.obj", glm::vec3(0.0f, 1.5f, 14.5f), glm::vec3(0.65f), 0.0f, "y");
	six.objectContainer.material.diffuseMap = diffuseMap.textureContainer.ID;
	six.objectContainer.material.specularMap = specularMap.textureContainer.ID;
	assets.models.push_back(six.objectContainer);

	// load other stuff
	LoadPrograms programs;
	LoadLights lights("demo");
	LoadShadows shadows(lights, assets.models, programs);
	LoadSkyboxes skybox;  // not used
	PostProcessing postProc(programs.programs[5]);  // for object selection

	// init renderer
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
	//skybox.config(programs.programs[4]);

	glEnable(GL_DEPTH_TEST);
	int selectedID = -1;

	float timeInterval = 0.0f;
	float lastFrameTime = 0.0f;

	// render
	while (!glfwWindowShouldClose(window))
	{
		float currentFrameTime = (float)glfwGetTime();
		timeInterval = currentFrameTime - lastFrameTime;
		lastFrameTime = currentFrameTime;

		//std::cout << "FPS: " << (1.0f / timeInterval) << std::endl;

		ImGui_ImplOpenGL3_NewFrame();  // IMGUI
		ImGui_ImplGlfw_NewFrame();  // IMGUI
		ImGui::NewFrame();  // IMGUI

		kamera.processKeyboardInput(window);	// WASD + R

		// specify clear values for the buffers
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClearDepth(1);  // default
		glClearStencil(0);  // default

		// clear all buffers of the default fbo
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

		// RENDER SHADOWS
		//glViewport(0, 0, DIR_SHADOW_WIDTH, DIR_SHADOW_HEIGHT);
		//glEnable(GL_DEPTH_TEST);
		//glDisable(GL_STENCIL_TEST);
		shadows.renderDynamic(renderer.models);

		// RENDER SCENE AND LIGHTS
		glBindFramebuffer(GL_FRAMEBUFFER, postProc.postProcContainer.fbo);  // for post-processing
		//glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		renderer.renderAssets(kamera);
		lights.render(programs.programs[1], kamera);

		// POST PROCESSING
		postProc.render();

		// OBJECT SELECTION
		if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS && isFisrtPress)
		{
			isFisrtPress = false;
			glFlush();
			glFinish();
			selectedID = postProc.selectObject();
			showGUI = !showGUI;
		}

		if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_RELEASE)
		{
			isFisrtPress = true;
		}

		if (true)
			renderer.changeParams(selectedID);

		ImGui::Render();  // IMGUI
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());  // IMGUI

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
}

void ShowScene::lightsScene()
{
	// load models with textures
	//std::string pathStr = "resources/assimp/light scenes/dir lights scene/desk area.obj";  // dir light scene
	//std::string pathStr = "resources/assimp/light scenes/dir lights scene/pcf scene.obj";  // dir light pcf scene
	//std::string pathStr = "resources/assimp/light scenes/point light scene/point light scene.obj";  // point light scene
	std::string pathStr = "resources/assimp/light scenes/point light scene/spot light scene.obj";  // spot light scene
	SceneLoader assets(&pathStr[0]);

	// load other stuff
	LoadPrograms programs;
	LoadLights lights("lights");
	LoadShadows shadows(lights, assets.models, programs);
	LoadSkyboxes skybox;  // not used

	// init renderer
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
	//skybox.config(programs.programs[4]);

	glEnable(GL_DEPTH_TEST);

	// render
	while (!glfwWindowShouldClose(window))
	{
		kamera.processKeyboardInput(window);	// WASD + R

		// specify clear values for the buffers
		glClearColor(0.0f, 1.0f, 1.0f, 1.0f);
		glClearDepth(1);  // default
		glClearStencil(0);  // default

		// clear all buffers of the default fbo
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

		// RENDER SHADOWS
		shadows.render();

		// RENDER SCENE AND LIGHTS
		glEnable(GL_DEPTH_TEST);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		renderer.renderAssets(kamera);
		lights.render(programs.programs[1], kamera);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
}

void ShowScene::normalMapping()
{
	std::string pathStr = "resources/assimp/moon/moon.obj";  // scene with normal map
	//std::string pathStr = "resources/assimp/moon/moon2.obj";  // scene without normal map
	SceneLoader assets(&pathStr[0]);

	// load other stuff
	LoadPrograms programs;
	LoadLights lights("normal");
	LoadShadows shadows(lights, assets.models, programs);
	LoadSkyboxes skybox;  // not used

	// init renderer
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
	//skybox.config(programs.programs[4]);

	glEnable(GL_DEPTH_TEST);

	// render
	while (!glfwWindowShouldClose(window))
	{
		kamera.processKeyboardInput(window);	// WASD + R

		// specify clear values for the buffers
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClearDepth(1);  // default
		glClearStencil(0);  // default

		// clear all buffers of the default fbo
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

		// RENDER SHADOWS
		shadows.render();

		// RENDER SCENE AND LIGHTS
		glEnable(GL_DEPTH_TEST);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		renderer.renderAssets(kamera);
		lights.render(programs.programs[1], kamera);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
}

void ShowScene::environmentMapping()
{
	//std::string pathStr = "resources/assimp/Suzanne/suzanne.obj";  // scene for enfironment mapping
	std::string pathStr = "resources/assimp/moon/moon.obj";
	SceneLoader assets(&pathStr[0]);

	// load other stuff
	LoadPrograms programs;
	LoadLights lights("normal");
	LoadShadows shadows(lights, assets.models, programs);
	LoadSkyboxes skybox;

	// init renderer
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

	glEnable(GL_DEPTH_TEST);

	// render
	while (!glfwWindowShouldClose(window))
	{
		kamera.processKeyboardInput(window);	// WASD + R

		// specify clear values for the buffers
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClearDepth(1);  // default
		glClearStencil(0);  // default

		// clear all buffers of the default fbo
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

		// RENDER SHADOWS
		//shadows.render();

		// RENDER SCENE AND LIGHTS
		glEnable(GL_DEPTH_TEST);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		renderer.renderAssets(kamera);
		//lights.render(programs.programs[1], kamera);

		// RENDER SKYBOX
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LEQUAL);
		skybox.render(programs.programs[4], kamera);
		glDepthFunc(GL_LESS);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
}

void ShowScene::outlining()
{
	std::string pathStr = "resources/assimp/outline/rubiks cube.obj";  // scene for outlining
	SceneLoader assets(&pathStr[0]);

	// load other stuff
	LoadPrograms programs;
	LoadLights lights("normal");
	LoadShadows shadows(lights, assets.models, programs);
	LoadSkyboxes skybox;
	LoadOutlines outlines(assets.models, programs.programs[6]);

	// init renderer
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
	//skybox.config(programs.programs[4]);
	outlines.config();

	glEnable(GL_DEPTH_TEST);

	// render
	while (!glfwWindowShouldClose(window))
	{
		kamera.processKeyboardInput(window);	// WASD + R

		// specify clear values for the buffers
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glClearDepth(1);  // default
		glClearStencil(0);  // default

		// clear all buffers of the default fbo
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

		// RENDER SHADOWS
		//shadows.render();

		// RENDER SCENE AND LIGHTS
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_STENCIL_TEST);
		glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
		glStencilFunc(GL_ALWAYS, 1, 0xFF);

		renderer.renderAssets(kamera);
		//lights.render(programs.programs[1], kamera);

		// RENDER OUTLINES
		glEnable(GL_STENCIL_TEST);
		glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
		//glStencilMask(0x00);
		glDisable(GL_DEPTH_TEST);  // draw outline on top of everything
		outlines.render(kamera);  // ok
		glStencilMask(0xFF);
		glStencilFunc(GL_ALWAYS, 1, 0xFF);
		glEnable(GL_DEPTH_TEST);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
}

void ShowScene::benchmarking()
{
	
	// load
	std::string pathStr = "resources/assimp/my room/my room.obj";
	SceneLoader assets(&pathStr[0]);  // ok
	LoadPrograms programs;  // ok
	LoadLights lights("normal");  // ok
	LoadShadows shadows(lights, assets.models, programs);  // ok
	LoadSkyboxes skybox;  // ok
	PostProcessing postProc(programs.programs[5]);  // ok

	Render renderer(
		assets.models,
		programs,
		kamera,
		lights,
		shadows,
		skybox.skyboxes[1]
	);

	// config
	renderer.configAssets();  // ok
	lights.config(programs.programs[1]);  // ok
	shadows.config();  // ok
	skybox.config(programs.programs[4]);  // ok
	

	Instrumentor::Get().BeginSession("game loop", "game_loop2.json");

	//float timeInterval = 0.0f;
	//float lastFrameTime = 0.0f;
	
	int i = 1;
	
	while (!glfwWindowShouldClose(window))
	{
		//float currentFrameTime = (float)glfwGetTime();
		//timeInterval = currentFrameTime - lastFrameTime;
		//lastFrameTime = currentFrameTime;
		//std::cout << "FPS: " << (1.0f / timeInterval) << std::endl;

		std::string title = std::to_string(i) + ". loop";
		const char* title_ = title.c_str();
		InstrumentationTimer timer(title_);

		kamera.processKeyboardInput(window);	// ok

		// specify clear values for the buffers
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glClearDepth(1);  // default
		glClearStencil(0);  // default
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);  // clear all buffers

		// RENDER SHADOWS
		glViewport(0, 0, DIR_SHADOW_WIDTH, DIR_SHADOW_HEIGHT);
		shadows.render();  // ok

		// RENDER SCENE (OBJECTS AND LIGHTS)
		glBindFramebuffer(GL_FRAMEBUFFER, postProc.postProcContainer.fbo);  // for post-processing
		//glBindFramebuffer(GL_FRAMEBUFFER, 0);  // render to default fbo
		glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glEnable(GL_DEPTH_TEST);

		renderer.renderAssets(kamera);  // ok
		lights.render(programs.programs[1], kamera);  // ok
		
		// RENDER SKYBOX
		glDisable(GL_STENCIL_TEST);
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LEQUAL);
		skybox.render(programs.programs[4], kamera);  // ok
		glDepthFunc(GL_LESS);

		// POST PROCESSING
		postProc.render();  // ok

		glfwSwapBuffers(window);
		glfwPollEvents();

		i++;
		//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
		//glFinish();
		
		if (i > 100)
			break;
	}

	Instrumentor::Get().EndSession();
}
