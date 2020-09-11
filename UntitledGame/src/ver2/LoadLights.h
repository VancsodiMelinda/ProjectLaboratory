#pragma once

#include <iostream>

#include "CreateDirLight.h"

class LoadLights
{
public:
	DirLightContainer dirLights[2];
	LoadLights();

private:
	void loadDirectionalLights();
	//void loadPointLights();
	//void loadSpotLights();
};

