#pragma once

#include <iostream>

#include <glm/glm.hpp>
#include "glm/gtc/matrix_transform.hpp"

#include "GlobalVariables.h"

struct SpotLightContainer
{
	glm::vec3 position;
	glm::vec3 target;
	glm::vec3 color;

	float cutOffAngle;
	float ambientStrength;
	float diffuseStrength;
	float specularStrength;

	glm::mat4 lightSpaceMatrix;
};

class CreateSpotLight
{
public:
	SpotLightContainer spotLightContainer;
	CreateSpotLight();  // create default light
	glm::mat4 createLightSpaceMatrix();
};

