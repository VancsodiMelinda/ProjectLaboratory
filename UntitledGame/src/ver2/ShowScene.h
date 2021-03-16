#pragma once

#include "LoadAssets.h"
#include "LoadPrograms.h"
#include "LoadLights.h"
#include "LoadShadows.h"
#include "LoadSkyboxes.h"
#include "PostProcessing.h"
#include "LoadOutlines.h"
#include "Render.h"
#include "GlobalVariables.h"
#include "SceneLoader.h"
#include "DeferredRendering.h"

#include <glm/glm.hpp>

#include <vector>
#include <string>
#include <iostream>

/*
* Scene with models and solid colors.
*/

class ShowScene
{
private:
	GLFWwindow* window;
	Kamera& kamera;
public:
	ShowScene(GLFWwindow* window_, Kamera& kamera_);
	void scene1();
	void scene2();
	void scene3();  // deferred rendering
};

