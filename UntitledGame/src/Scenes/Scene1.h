#pragma once

#include "../ver2/LoadAssets.h"
#include "../ver2/LoadPrograms.h"
#include "../ver2/LoadLights.h"
#include "../ver2/LoadShadows.h"
#include "../ver2/LoadSkyboxes.h"
#include "../ver2/PostProcessing.h"
#include "../ver2/LoadOutlines.h"
#include "../ver2/Render.h"
#include "../ver2/GlobalVariables.h"

#include <glm/glm.hpp>

#include <vector>
#include <string>

/*
* Scene with models and solid colors.
*/

class Scene1
{
public:
	Scene1(GLFWwindow* window, Kamera& kamera);
};

