#include "Camera.h"

// default constructor
Camera::Camera()
{
	cameraPosition = glm::vec3(0.0f, 0.0f, 4.0f);  // 1st PARAM
	cameraFront = glm::normalize(glm::vec3(0.0f, 0.0f, -1.0f));
	worldUp = glm::vec3(0.0f, 1.0f, 0.0f);

	cameraTarget = glm::vec3(cameraPosition + cameraFront);  // 2nd PARAM
	//cameraDirection = glm::vec3(glm::normalize(cameraPosition - cameraTarget));  // UNIT VECTOR
	cameraRight = glm::vec3(glm::normalize(glm::cross(worldUp, cameraFront)));  // UNIT VECTOR
	cameraUp = glm::cross(cameraFront, cameraRight);  // UNIT VECTOR  3rd PARAM

	yaw = 0.0f;
	pitch = 0.0f;
	fov = 45.0f;
}

// parameterized constructor
Camera::Camera(glm::vec3 cameraPosition_, glm::vec3 cameraFront_, glm::vec3 worldUp_, float yaw_, float pitch_, float fov_)
{
	cameraPosition = cameraPosition_;  // 1st PARAM
	cameraFront = cameraFront_;
	worldUp = worldUp_;

	cameraTarget = glm::vec3(cameraPosition_ + cameraFront_);  // 2nd PARAM
	//cameraDirection = glm::vec3(glm::normalize(cameraPosition_ - cameraTarget));  // UNIT VECTOR
	cameraRight = glm::vec3(glm::normalize(glm::cross(worldUp_, cameraFront_)));  // UNIT VECTOR
	cameraUp = glm::cross(cameraFront_, cameraRight);  // UNIT VECTOR  3rd PARAM

	yaw = yaw_;
	pitch = pitch_;
	fov = fov_;
}

glm::mat4 Camera::CreateViewMatrix()
{
	//glm::mat4 viewMatrix = glm::lookAt(cameraPosition, cameraPosition + cameraFront, cameraUp);
	glm::mat4 viewMatrix = glm::lookAt(cameraPosition, cameraTarget, cameraUp);
	//glm::mat4 viewMatrix = glm::lookAt(glm::vec3(3.0f, 1.0f, -3.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));  // light point of view
	return viewMatrix;
}

void Camera::ProcessKeyInput(std::string button, float deltaTime)
{
	float cameraSpeed = 2.5f * deltaTime;
	if (button == "W")
	{
		cameraPosition += cameraSpeed * cameraFront;
		cameraTarget = glm::vec3(cameraPosition + cameraFront);
		//std::cout << "W" << std::endl;
	}
	else if (button == "S")
	{
		cameraPosition -= cameraSpeed * cameraFront;
		cameraTarget = glm::vec3(cameraPosition + cameraFront);
		//std::cout << "S" << std::endl;
	}
	else if (button == "A")
	{
		//cameraPosition -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
		cameraPosition += cameraRight * cameraSpeed;
		cameraTarget = glm::vec3(cameraPosition + cameraFront);
		//std::cout << "A" << std::endl;
	}
	else if (button == "D")
	{
		//cameraPosition += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
		cameraPosition -= cameraRight * cameraSpeed;
		cameraTarget = glm::vec3(cameraPosition + cameraFront);
		//std::cout << "D" << std::endl;
	}
}

void Camera::ProcessMouseMovement(float xOffset, float yOffset)  // pitch and yaw
{
	float sensitivity = 0.05f;
	xOffset *= sensitivity;
	yOffset *= sensitivity;

	yaw += xOffset;
	pitch += yOffset;

	if (pitch > 89.0f)
	{
		pitch = 89.0f;
	}
	else if (pitch < -89.0f)
	{
		pitch = -89.0f;
	}

	glm::vec3 front;
	front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	front.y = sin(glm::radians(pitch));
	front.z = cos(glm::radians(pitch)) * sin(glm::radians(yaw));

	cameraFront = glm::normalize(front);
	cameraRight = glm::normalize(glm::cross(worldUp, cameraFront));
	cameraUp = glm::cross(cameraFront, cameraRight);
	cameraTarget = glm::vec3(cameraPosition + cameraFront);
}


void Camera::ProcessMouseScroll(double yoffset)  // zoom in and out
{
	if (fov >= 1.0f && fov <= 45.0f)
	{
		fov -= (float)yoffset;
	}
	if (fov <= 1.0f)
	{
		fov = 1.0f;
	}
	if (fov >= 45.0f)
	{
		fov = 45.0f;
	}
}

