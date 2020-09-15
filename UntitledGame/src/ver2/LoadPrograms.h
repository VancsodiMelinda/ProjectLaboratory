#pragma once

#include <iostream>

#include "CreateProgram.h"
#include "ConstantVariables.h"

class LoadPrograms
{
public:
	ProgramContainer programs[NUMBER_OF_PROGRAMS];

	LoadPrograms();

private:
	ProgramContainer loadDefaultProgram();
	ProgramContainer loadObjectProgram();
	ProgramContainer loadLightProgram();
	ProgramContainer loadDirShadowProgram();
	//ProgramContainer loadOmniShadowProgram();
	ProgramContainer loadSkyboxProgram();
};

