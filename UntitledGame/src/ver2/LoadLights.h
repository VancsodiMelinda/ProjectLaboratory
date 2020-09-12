#pragma once

#include <iostream>

#include <glad/glad.h>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "CreateDirLight.h"
#include "CreatePointLight.h"
#include "Kamera.h"
#include "ModelData.h"
#include "ProgramData.h"
#include "ConstantVariables.h"

class LoadLights
{
public:
	DirLightContainer dirLights[NUMBER_OF_DIR_LIGHTS];
	PointLightContainer pointLights[NUMBER_OF_POINT_LIGHTS];
	//SpotLightContainer spotLights[];
	LoadLights();

private:
	void loadDirectionalLights();
	void loadPointLights();
	//void loadSpotLights();

	void configLight(ObjectContainer& object, GLuint programID);
	void renderDirLight(DirLightContainer& dirLight, ObjectContainer& object, GLuint programID, Kamera& kamera);
	void renderPointLight(PointLightContainer& pointLight, ObjectContainer& object, GLuint programID, Kamera& kamera);

	struct UniformLocations {
		GLint MVPloc = 0;
		GLint colorLoc = 0;
	};
	UniformLocations uniformLocations;

public:
	void config(ObjectContainer& dirLightObject, ObjectContainer& pointLightObject, ProgramContainer programContainer);
	void render(ObjectContainer& dirLightObject, ObjectContainer& pointLightObject, ProgramContainer programContainer, Kamera& kamera);
};

