#pragma once

#include "LoadAssets.h"
#include "LoadPrograms.h"
#include "Kamera.h"
#include "LoadLights.h"

class Render
{
private:
	LoadAssets& assets;
	LoadPrograms& programs;
	Kamera& kamera;
	LoadLights& lights;
	GLuint programID = 0;
public:
	Render(LoadAssets& assets_, LoadPrograms& programs_, Kamera& kamera_, LoadLights& lights_);

private:
	void configAsset(ObjectContainer& object);
	void configVertexAttributes(ObjectContainer& object);
	void getUniformLocations();

	void renderAsset(ObjectContainer& object);
	void uploadUniforms(ObjectContainer& object);
	glm::mat4 updateMVP(glm::mat4 M, glm::mat4 V, glm::mat4 P);

	struct DefaultUniformLocs {
		int MVPloc = 0;
		int modelMatrixLoc = 0;
		int diffuseMapLoc = 0;
		int specularMapLoc = 0;
		int shininessLoc = 0;
	};
	DefaultUniformLocs uniformLocations;

public:
	void config();
	void render(Kamera& kamera_);
};

