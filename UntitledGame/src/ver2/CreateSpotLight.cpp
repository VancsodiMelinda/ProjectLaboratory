#include "CreateSpotLight.h"

CreateSpotLight::CreateSpotLight()
{
	spotLightContainer.position = glm::vec3(2.0f, 2.0f, 2.0f);
	spotLightContainer.target = glm::vec3(0.0f, 0.0f, 0.0f);
	spotLightContainer.color = glm::vec3(1.0);

	spotLightContainer.cutOffAngle = 12.5f;
	spotLightContainer.ambientStrength = 0.1f;
	spotLightContainer.diffuseStrength = 0.8f;
	spotLightContainer.specularStrength = 1.0f;

	spotLightContainer.lightSpaceMatrix = createLightSpaceMatrix();
}

glm::mat4 CreateSpotLight::createLightSpaceMatrix()
{
	// create view matrix
	glm::mat4 lightViewMatrix = glm::lookAt(spotLightContainer.position, spotLightContainer.target, glm::vec3(0.0f, 1.0f, 0.0f));

	// create projection matrix
	float aspect = (float)POINT_SHADOW_WIDTH_ / (float)POINT_SHADOW_HEIGHT_;
	float nearPlane = 1.0f;
	float farPlane = 20.0f;
	glm::mat4 lightProjMatrix = glm::perspective(glm::radians(90.0f), aspect, nearPlane, farPlane);

	glm::mat4 lightSpaceMatrix = lightProjMatrix * lightViewMatrix;

	return lightSpaceMatrix;
}
