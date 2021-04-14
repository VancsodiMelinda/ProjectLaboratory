#pragma once

#include <iostream>

#include <glad/glad.h>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/projection.hpp>
#include <glm/gtx/vector_angle.hpp>

#include "CreateDirLight.h"
#include "CreatePointLight.h"
#include "CreateSpotLight.h"
#include "Kamera.h"
#include "ModelData.h"
#include "ProgramData.h"
#include "ConstantVariables.h"
#include "CreateModel.h"
#include "Instrumentor.h"
#include "CreateTexture.h"

class LoadLights
{
public:
	//DirLightContainer dirLights[NUMBER_OF_DIR_LIGHTS];
	std::vector<DirLightContainer> dirLights_;

	//PointLightContainer pointLights[NUMBER_OF_POINT_LIGHTS];
	std::vector<PointLightContainer> pointLights_;

	//SpotLightContainer spotLights[];
	std::vector<SpotLightContainer> spotLights;

	LoadLights();
	LoadLights(std::string scene);

private:
	void loadDirectionalLights();
	void loadPointLights();
	void loadSpotLights();

	void loadLightModels();
	ObjectContainer models[3];

	void configLight(ObjectContainer& object, GLuint programID);
	void renderDirLight(DirLightContainer& dirLight, ObjectContainer& object, GLuint programID, Kamera& kamera);
	void renderPointLight(PointLightContainer& pointLight, ObjectContainer& object, GLuint programID, Kamera& kamera);
	void renderSpotLight(SpotLightContainer& spotLight, ObjectContainer& object, GLuint programID, Kamera& kamera);

	struct UniformLocations {
		GLint MVPloc = 0;
		GLint colorLoc = 0;
	};
	UniformLocations uniformLocations;

public:
	//void config(ObjectContainer& dirLightObject, ObjectContainer& pointLightObject, ProgramContainer programContainer);
	//void render(ObjectContainer& dirLightObject, ObjectContainer& pointLightObject, ProgramContainer programContainer, Kamera& kamera);
	void config(ProgramContainer programContainer);
	void render(ProgramContainer programContainer, Kamera& kamera);
};

