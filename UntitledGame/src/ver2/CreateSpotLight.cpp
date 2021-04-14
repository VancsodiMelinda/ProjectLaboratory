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

	spotLightContainer.constant = 1.0f;
	spotLightContainer.linear = 0.07f;
	spotLightContainer.quadratic = 0.017f;

	spotLightContainer.ID = -1;

	spotLightContainer.hasProjective = false;
	spotLightContainer.projectiveMapID = 0;

	spotLightContainer.lightSpaceMatrix = createLightSpaceMatrix();
}

glm::mat4 CreateSpotLight::createLightSpaceMatrix()
{
	// create view matrix
	glm::mat4 lightViewMatrix = glm::lookAt(spotLightContainer.position, spotLightContainer.target, glm::vec3(0.0f, 1.0f, 0.0f));

	// create projection matrix (perspective)
	float aspect = (float)POINT_SHADOW_WIDTH_ / (float)POINT_SHADOW_HEIGHT_;
	float nearPlane = 1.0f;
	float farPlane = 20.0f;
	glm::mat4 lightProjMatrix = glm::perspective(glm::radians(90.0f), aspect, nearPlane, farPlane);

	if (!spotLightContainer.hasProjective)
		lightProjMatrix = glm::ortho(-4.0f, 4.0f, -3.0f, 3.0f, nearPlane, farPlane);  // projection matrix (orthogonal)

	glm::mat4 lightSpaceMatrix = lightProjMatrix * lightViewMatrix;

	return lightSpaceMatrix;
}
