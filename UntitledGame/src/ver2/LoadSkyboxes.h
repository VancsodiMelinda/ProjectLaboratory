#pragma once

#include "CreateSkybox.h"
#include "ConstantVariables.h"
#include "CreateModel.h"
#include "Kamera.h"
#include "ProgramData.h"

#include <glm/gtc/type_ptr.hpp>

class LoadSkyboxes
{
public:
	SkyboxContainer skyboxes[NUMBER_OF_SKYBOXES];
	LoadSkyboxes();

private:
	struct UniformLocations {
		GLint MVPloc = 0;
		GLint skyboxLoc = 0;
	};
	UniformLocations uniformLocations;

public:
	void config(ObjectContainer& object, ProgramContainer programContainer);
	void render(ObjectContainer& object, ProgramContainer programContainer, Kamera& kamera);
};

