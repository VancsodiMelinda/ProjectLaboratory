#include "Camera.h"

// default constructor
Camera::Camera()
{
	cameraPosition = glm::vec3(0.0f, 0.0f, 4.0f);
	cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
	cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
}

// parameterized constructor
Camera::Camera(glm::vec3 cameraPosition_, glm::vec3 cameraFront_, glm::vec3 cameraUp_)
{
	cameraPosition = cameraPosition_;
	cameraFront = cameraFront_;
	cameraUp = cameraUp_;
}

glm::mat4 Camera::CreateViewMatrix()
{
	glm::mat4 viewMatrix = glm::lookAt(cameraPosition, cameraPosition + cameraFront, cameraUp);
	return viewMatrix;
}
