#pragma once

#include <iostream>

#include <glad/glad.h>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "CreateDirLight.h"
#include "Kamera.h"
#include "ModelData.h"
#include "ProgramData.h"

class LoadLights
{
public:
	DirLightContainer dirLights[2];
	//PointLightContainer pointLights[];
	//SpotLightContainer spotLights[];
	LoadLights();

private:
	void loadDirectionalLights();
	//void loadPointLights();
	//void loadSpotLights();

	void configLight(ObjectContainer& object, GLuint programID);
	void renderLight(DirLightContainer& dirLight, ObjectContainer& object, GLuint programID, Kamera& kamera);

	struct UniformLocations {
		GLint MVPloc = 0;
		GLint colorLoc = 0;
	};
	UniformLocations uniformLocations;

public:
	void config(ObjectContainer& object, ProgramContainer programContainer);
	void render(ObjectContainer& object, ProgramContainer programContainer, Kamera& kamera);
};

