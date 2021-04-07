#pragma once

#include <iostream>

#include <glm/glm.hpp>
#include "glm/gtc/matrix_transform.hpp"

struct DirLightContainer
{
	glm::vec3 position;
	glm::vec3 target;
	glm::vec3 color;

	float ambientStrength;
	float diffuseStrength;
	float specularStrength;

	glm::mat4 lightSpaceMatrix;

	int ID;
};

class CreateDirLight
{
public:
	DirLightContainer dirLightContainer;
	CreateDirLight();
	CreateDirLight(glm::vec3 position, glm::vec3 color, float ambientStrength, float diffuseStrength, float specularStrength);

	glm::mat4 createLightSpaceMatrix();
};

