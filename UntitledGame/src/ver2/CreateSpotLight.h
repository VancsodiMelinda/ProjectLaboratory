#pragma once

#include <iostream>

#include <glm/glm.hpp>
#include "glm/gtc/matrix_transform.hpp"

struct SpotLightContainer
{
	glm::vec3 position = glm::vec3(0.0f);
	glm::vec3 target = glm::vec3(-1.0f, 0.0f, 0.0f);
	glm::vec3 direction = target - position;
	glm::vec3 color = glm::vec3(1.0);
	float cutOffAngle = 12.5f;

	float ambientStrength = 0.1f;
	float diffuseStrength = 0.8f;
	float specularStrength = 1.0f;

	glm::mat4 lightSpaceMatrix = glm::mat4(1.0);
};

class CreateSpotLight
{
public:
	SpotLightContainer spotLightContainer;
	CreateSpotLight();
	glm::mat4 createLightSpaceMatrix();
};

