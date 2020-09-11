#pragma once

#include <iostream>

#include <glm/glm.hpp>
#include "glm/gtc/matrix_transform.hpp"

struct DirLightContainer
{
	glm::vec3 position	= glm::vec3(0.0f);
	glm::vec3 color		= glm::vec3(1.0f);

	float ambientStrength	= 0.0f;
	float diffuseStrength	= 0.0f;
	float specularStrength	= 0.0f;

	glm::mat4 lightSpaceMatrix = glm::mat4(1.0);
};

class CreateDirLight
{
public:
	DirLightContainer dirLightContainer;
	CreateDirLight();

private:
	glm::mat4 createLightSpaceMatrix();
};

