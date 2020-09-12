#pragma once

#include <iostream>

#include "CreateDirShadow.h"
#include "LoadLights.h"
#include "LoadAssets.h"
#include "LoadPrograms.h"
#include "GlobalVariables.h"

class LoadShadows
{
private:
	LoadLights& lights;
	LoadAssets& assets;
	LoadPrograms& programs;

	ProgramContainer dirShadowProgramContainer;
public:
	DirShadowContainer dirShadows[2];
	//PointShadowContainer pointShadows[];
	LoadShadows(LoadLights& lights_, LoadAssets& assets_, LoadPrograms& programs_);

private:
	void loadDirShadows(int numberOfDirShadows);
	//void loadPointShadows();
	//void loadSpotShadows();

	void configDirShadow(ObjectContainer& object, GLuint programID);
	void renderDirShadow(DirLightContainer light, ObjectContainer& object, GLuint programID);

	struct UniformLocations {
		GLint MVPloc = 0;
	};
	UniformLocations uniformLocations;

public:
	void config();
	void render();
};

