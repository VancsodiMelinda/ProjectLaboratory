#pragma once

#include "LoadAssets.h"
#include "Kamera.h"
#include "LoadLights.h"
#include "LoadPrograms.h"
#include "LoadShadows.h"
#include "CreateSkybox.h"
#include "ConstantVariables.h"
#include "Instrumentor.h"

class Render
{
private:
	//LoadAssets& assets;
	std::vector<ObjectContainer> models;  //
	LoadPrograms& programs;
	Kamera& kamera;
	LoadLights& lights;
	LoadShadows& shadows;
	SkyboxContainer skybox;
	GLuint programID = 0;
	int counter = 0;

public:
	//Render(LoadAssets& assets_, LoadPrograms& programs_, Kamera& kamera_, LoadLights& lights_, LoadShadows& shadows_, SkyboxContainer skybox_);
	Render(std::vector<ObjectContainer>& models_, LoadPrograms& programs_, Kamera& kamera_, LoadLights& lights_, LoadShadows& shadows_, SkyboxContainer skybox_);

private:
	void configAsset(ObjectContainer& object);
	void configVertexAttributes(ObjectContainer& object);
	void getUniformLocations();

	void renderAsset(ObjectContainer& object);
	void uploadUniforms(ObjectContainer& object);
	glm::mat4 updateMVP(glm::mat4 M, glm::mat4 V, glm::mat4 P);

	struct UniformLocations {
		int MVPloc = -1;
		int modelMatrixLoc = -1;
		int diffuseMapLoc = -1;
		int specularMapLoc = -1;
		int normalMapLoc = -1;
		int shininessLoc = -1;
		int hasNormalMapLoc = -1;
		int cameraPosLoc = -1;
		int cameraFarPlaneLoc = -1;
		int skyboxLoc = -1;
		//int dirLightLocs[NUMBER_OF_DIR_LIGHTS][7];
		//int pointLightLocs[NUMBER_OF_POINT_LIGHTS][9];
		std::vector<std::vector<int>> dirLightLocs_;
		std::vector<std::vector<int>> pointLightLocs_;
		std::vector<std::vector<int>> spotLightLocs;
		int IDloc = -1;
	};
	UniformLocations uniformLocations;

public:
	void generateIDs();
	void configAssets();
	void renderAssets(Kamera& kamera_);
};

