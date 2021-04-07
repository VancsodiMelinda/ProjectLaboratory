#pragma once

#include <iostream>

#include <glm/glm.hpp>
#include "glm/gtc/matrix_transform.hpp"

struct PointLightContainer
{
	glm::vec3 position;
	glm::vec3 color;

	float ambientStrength;
	float diffuseStrength;
	float specularStrength;

	float constant;
	float linear;
	float quadratic;

	int ID;
};

class CreatePointLight
{
public:
	PointLightContainer pointLightContainer;
	CreatePointLight();
	CreatePointLight(glm::vec3 position, glm::vec3 color,
		float ambientStrength, float diffuseStrength, float specularStrength,
		float constant, float linear, float quadratic);
};

