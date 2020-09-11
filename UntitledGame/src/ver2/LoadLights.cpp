#include "LoadLights.h"

LoadLights::LoadLights()
{
	std::cout << "Loading directional lights..." << std::endl;
	loadDirectionalLights();
	std::cout << std::endl;
}

void LoadLights::loadDirectionalLights()
{
	CreateDirLight light1;	// creates default directional light
	dirLights[0] = light1.dirLightContainer;

	CreateDirLight light2;
	light2.dirLightContainer.position = glm::vec3(-3.0f, 2.0f, 0.0f);
	light2.dirLightContainer.color = glm::vec3(0.2f, 0.0f, 0.0f);
	light2.dirLightContainer.ambientStrength = 0.5f;
	light2.dirLightContainer.diffuseStrength = 0.0f;
	light2.dirLightContainer.specularStrength = 1.0f;
	dirLights[1] = light2.dirLightContainer;
}
