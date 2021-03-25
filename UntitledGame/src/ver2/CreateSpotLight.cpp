#include "CreateSpotLight.h"

CreateSpotLight::CreateSpotLight()
{
	//spotLightContainer.lightSpaceMatrix = createLightSpaceMatrix();
}

glm::mat4 CreateSpotLight::createLightSpaceMatrix()
{
	float nearPlane = 0.1f;
	float farPlane = 20.0f;

	glm::mat4 lightViewMatrix = glm::lookAt(spotLightContainer.position, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	glm::mat4 lightProjMatrix = glm::ortho(-4.0f, 4.0f, -3.0f, 3.0f, nearPlane, farPlane);

	glm::mat4 lightSpaceMatrix = lightProjMatrix * lightViewMatrix;

	return lightSpaceMatrix;
}
