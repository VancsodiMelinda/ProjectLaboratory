#pragma once

#include <iostream>

#include "CreateDirShadow.h"
#include "CreatePointShadow.h"
#include "LoadLights.h"
#include "LoadAssets.h"
#include "LoadPrograms.h"
#include "GlobalVariables.h"
#include "ConstantVariables.h"
#include "Instrumentor.h"

class LoadShadows
{
private:
	LoadLights& lights;
	//LoadAssets& assets;
	std::vector<ObjectContainer> models;  //
	LoadPrograms& programs;

	ProgramContainer dirShadowProgramContainer;
	ProgramContainer pointShadowProgramContainer;
public:
	//DirShadowContainer dirShadows[NUMBER_OF_DIR_LIGHTS];
	//PointShadowContainer pointShadows[NUMBER_OF_POINT_LIGHTS];
	std::vector<DirShadowContainer> dirShadows_;
	std::vector<PointShadowContainer> pointShadows_;
	std::vector<DirShadowContainer> spotShadows;

	//LoadShadows(LoadLights& lights_, LoadAssets& assets_, LoadPrograms& programs_);
	LoadShadows(LoadLights& lights_, const std::vector<ObjectContainer>& models_, LoadPrograms& programs_);

private:
	void loadDirShadows();
	void loadPointShadows();
	void loadSpotShadows();

	void configDirShadow(ObjectContainer& object, GLuint programID);
	void renderDirShadow(DirLightContainer light, ObjectContainer& object, GLuint programID);

	void configPointShadow(ObjectContainer& object, GLuint programID);
	void renderPointShadow(PointLightContainer light, ObjectContainer& object, GLuint programID);

	void renderSpotShadow(SpotLightContainer light, ObjectContainer& object, GLuint programID);

	struct UniformLocations {
		GLint MVPloc = 0;
	};
	UniformLocations uniformLocations;

	struct UniformLocations_ {
		GLint modelMatrixLoc = 0;
		GLint VPsLoc = 0;
		GLint lightPosLoc = 0;
		GLint farPlaneLoc = 0;
	};
	UniformLocations_ uniformLocations_;

public:
	void config();
	void render();
};

