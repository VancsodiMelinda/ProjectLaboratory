#include "CreatePointLight.h"

CreatePointLight::CreatePointLight()
{
	pointLightContainer.position = glm::vec3(0.0f, 3.0f, 0.0f);
	pointLightContainer.color = glm::vec3(1.0f, 1.0f, 1.0f);

	pointLightContainer.ambientStrength = 0.2f;
	pointLightContainer.diffuseStrength = 0.8f;
	pointLightContainer.specularStrength = 0.5f;

	pointLightContainer.constant = 1.0f;
	pointLightContainer.linear = 0.07f;
	pointLightContainer.quadratic = 0.017f;

	pointLightContainer.ID = -1;
}

CreatePointLight::CreatePointLight(glm::vec3 position, glm::vec3 color,
	float ambientStrength, float diffuseStrength, float specularStrength,
	float constant, float linear, float quadratic)
{
	pointLightContainer.position = position;
	pointLightContainer.color = color;

	pointLightContainer.ambientStrength = ambientStrength;
	pointLightContainer.diffuseStrength = diffuseStrength;
	pointLightContainer.specularStrength = specularStrength;

	pointLightContainer.constant = constant;
	pointLightContainer.linear = linear;
	pointLightContainer.quadratic = quadratic;

	pointLightContainer.ID = -1;
}
