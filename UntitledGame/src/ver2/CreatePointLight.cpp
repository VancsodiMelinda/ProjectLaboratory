#include "CreatePointLight.h"

CreatePointLight::CreatePointLight()
{
	pointLightContainer.lightSpaceMatrix = createLightSpaceMatrix();

	std::cout << "OK: Loaded point light." << std::endl;
}

glm::mat4 CreatePointLight::createLightSpaceMatrix()
{
	float nearPlane = 0.1f;
	float farPlane = 20.0f;

	glm::mat4 lightViewMatrix = glm::lookAt(pointLightContainer.position, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	glm::mat4 lightProjMatrix = glm::ortho(-4.0f, 4.0f, -3.0f, 3.0f, nearPlane, farPlane);

	glm::mat4 lightSpaceMatrix = lightProjMatrix * lightViewMatrix;

	return lightSpaceMatrix;
}
