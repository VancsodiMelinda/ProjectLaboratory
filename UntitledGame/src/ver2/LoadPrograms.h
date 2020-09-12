#pragma once

#include <iostream>

#include "CreateProgram.h"

class LoadPrograms
{
public:
	ProgramContainer programs[3];

	LoadPrograms();

private:
	ProgramContainer loadDefaultProgram();
	ProgramContainer loadObjectProgram();
	ProgramContainer loadLightProgram();
	ProgramContainer loadDirShadowProgram();
	//ProgramContainer loadOmniShadowProgram();
	//ProgramContainer skyBoxProgram();
};

