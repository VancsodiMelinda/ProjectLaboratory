#pragma once

#include "LoadAssets.h"
#include "Kamera.h"
#include "LoadLights.h"
#include "LoadPrograms.h"
#include "LoadShadows.h"
#include "CreateSkybox.h"
#include "ConstantVariables.h"

class Render
{
private:
	LoadAssets& assets;
	LoadPrograms& programs;
	Kamera& kamera;
	LoadLights& lights;
	LoadShadows& shadows;
	SkyboxContainer skybox;
	GLuint programID = 0;
public:
	Render(LoadAssets& assets_, LoadPrograms& programs_, Kamera& kamera_, LoadLights& lights_, LoadShadows& shadows_, SkyboxContainer skybox_);

private:
	void configAsset(ObjectContainer& object);
	void configVertexAttributes(ObjectContainer& object);
	void getUniformLocations();

	void renderAsset(ObjectContainer& object);
	void uploadUniforms(ObjectContainer& object);
	glm::mat4 updateMVP(glm::mat4 M, glm::mat4 V, glm::mat4 P);

	struct UniformLocations {
		int MVPloc = 0;
		int modelMatrixLoc = 0;
		int diffuseMapLoc = 0;
		int specularMapLoc = 0;
		int shininessLoc = 0;
		int cameraPosLoc = 0;
		int cameraFarPlaneLoc = 0;
		int skyboxLoc = 0;
		int dirLightLocs[NUMBER_OF_DIR_LIGHTS][7];
		int pointLightLocs[NUMBER_OF_POINT_LIGHTS][9];
	};
	UniformLocations uniformLocations;

public:
	void configAssets();
	void renderAssets(Kamera& kamera_);
};

