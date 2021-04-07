#include "CreateDirLight.h"

CreateDirLight::CreateDirLight()
{
	dirLightContainer.position = glm::vec3(0.0f, 3.0f, -2.0f);
	dirLightContainer.color = glm::vec3(1.0f);

	dirLightContainer.ambientStrength = 0.2f;
	dirLightContainer.diffuseStrength = 0.8f;
	dirLightContainer.specularStrength = 0.6f;

	dirLightContainer.lightSpaceMatrix = createLightSpaceMatrix();

	dirLightContainer.ID = -1;
}

CreateDirLight::CreateDirLight(glm::vec3 position, glm::vec3 color, float ambientStrength, float diffuseStrength, float specularStrength)
{
	dirLightContainer.position = position;
	dirLightContainer.color = color;

	dirLightContainer.ambientStrength = ambientStrength;
	dirLightContainer.diffuseStrength = diffuseStrength;
	dirLightContainer.specularStrength = specularStrength;

	dirLightContainer.lightSpaceMatrix = createLightSpaceMatrix();

	dirLightContainer.ID = -1;
}

glm::mat4 CreateDirLight::createLightSpaceMatrix()
{
	float nearPlane = 0.1f;
	float farPlane = 20.0f;

	//glm::vec3 position = dirLightContainer.position;
	//glm::vec3 target = dirLightContainer.target;
	//glm::vec3 front = glm::normalize(target - position);
	//glm::vec3 worldUp = dirLightContainer.worldUp;
	//glm::vec3 right = glm::normalize(glm::cross(worldUp, front));
	//glm::vec3 up = glm::cross(front, right);

	//glm::mat4 lightViewMatrix = glm::lookAt(dirLightContainer.position, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	glm::mat4 lightViewMatrix = glm::lookAt(dirLightContainer.position, dirLightContainer.target, glm::vec3(0.0f, 1.0f, 0.0f));
	glm::mat4 lightProjMatrix = glm::ortho(-4.0f, 4.0f, -3.0f, 3.0f, nearPlane, farPlane);

	glm::mat4 lightSpaceMatrix = lightProjMatrix * lightViewMatrix;

	return lightSpaceMatrix;
}


