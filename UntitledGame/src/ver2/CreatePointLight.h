#pragma once

#include <iostream>

#include <glm/glm.hpp>
#include "glm/gtc/matrix_transform.hpp"

struct PointLightContainer
{
	glm::vec3 position = glm::vec3(0.0f, 3.0f, 0.0f);
	glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f);

	float ambientStrength = 0.2f;
	float diffuseStrength = 0.8f;
	float specularStrength = 0.5f;

	float constant = 1.0f;
	float linear = 0.07f;
	float quadratic = 0.017f;

	//glm::mat4 lightSpaceMatrix = glm::mat4(1.0);
};

class CreatePointLight
{
public:
	PointLightContainer pointLightContainer;
	CreatePointLight();
	CreatePointLight(glm::vec3 position, glm::vec3 color,
		float ambientStrength, float diffuseStrength, float specularStrength,
		float constant, float linear, float quadratic);

	//glm::mat4 createLightSpaceMatrix();
};

