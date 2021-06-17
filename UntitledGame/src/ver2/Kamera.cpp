#include "Kamera.h"

Kamera::Kamera()
{
	// camera for demo screen shots
	/*
	cameraContainer.cameraPosition = glm::vec3(4.0f, 2.5f, 4.0f);
	cameraContainer.cameraTarget = glm::vec3(-2.0f, 0.0f, -2.0f);
	cameraContainer.cameraFront = glm::normalize(cameraContainer.cameraTarget - cameraContainer.cameraPosition);
	glm::vec3 worldUp = glm::vec3(0.0f, 1.0f, 0.0f);
	cameraContainer.cameraRight = glm::vec3(glm::normalize(glm::cross(worldUp, cameraContainer.cameraFront)));
	cameraContainer.cameraUp = glm::cross(cameraContainer.cameraFront, cameraContainer.cameraRight);
	*/

	
	cameraContainer.cameraFront		= glm::normalize(glm::vec3(0.0f, 0.0f, -1.0f));
	cameraContainer.cameraPosition	= glm::vec3(0.0f, 0.1f, 3.3f);
	//cameraContainer.cameraPosition = glm::vec3(0.0f, fixHeight, 0.0f);  // FOR FIXED CAMERA
	cameraContainer.cameraTarget	= cameraContainer.cameraPosition + cameraContainer.cameraFront;
	glm::vec3 worldUp				= glm::vec3(0.0f, 1.0f, 0.0f);
	cameraContainer.cameraRight		= glm::vec3(glm::normalize(glm::cross(worldUp, cameraContainer.cameraFront)));
	cameraContainer.cameraUp		= glm::cross(cameraContainer.cameraFront, cameraContainer.cameraRight);
	
	updateViewMatrix();
	updateProjectionMatrix();
}

void Kamera::updateViewMatrix()
{
	cameraContainer.viewMatrix = glm::lookAt(cameraContainer.cameraPosition, cameraContainer.cameraTarget, cameraContainer.cameraUp);
}

void Kamera::updateProjectionMatrix()
{
	cameraContainer.projectionMatrix = glm::perspective(glm::radians(cameraContainer.fov), (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT, cameraContainer.nearPlane, cameraContainer.farPlane);
}

void Kamera::processKeyboardInput(GLFWwindow* window)  // gets called every frame
{
	InstrumentationTimer timer("processKeyboardInput");

	// time calculations
	float currentFrame = (float)glfwGetTime();
	deltaTime = currentFrame - lastFrame;
	lastFrame = currentFrame;

	float cameraSpeed = 0.1f * deltaTime;
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
		processKeyInput("W", deltaTime);
	}
	else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		processKeyInput("S", deltaTime);
	}
	else if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		processKeyInput("A", deltaTime);
	}
	else if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		processKeyInput("D", deltaTime);
	}
	else if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
	{
		processKeyInput("R", deltaTime);
	}
}

void Kamera::processKeyInput(std::string button, float deltaTime)
{
	//float cameraSpeed = 5.0f * deltaTime;
	float cameraSpeed = 5.0f * deltaTime;
	glm::vec3 worldUp = glm::vec3(0.0f, 1.0f, 0.0f);

	if (button == "W")
	{
		cameraContainer.cameraPosition += cameraSpeed * cameraContainer.cameraFront;
		//cameraContainer.cameraPosition.y = fixHeight;  //
		cameraContainer.cameraTarget = glm::vec3(cameraContainer.cameraPosition + cameraContainer.cameraFront);
		//std::cout << "W" << std::endl;
		updateViewMatrix();
		updateProjectionMatrix();
	}
	else if (button == "S")
	{
		cameraContainer.cameraPosition -= cameraSpeed * cameraContainer.cameraFront;
		//cameraContainer.cameraPosition.y = fixHeight;  //
		cameraContainer.cameraTarget = glm::vec3(cameraContainer.cameraPosition + cameraContainer.cameraFront);
		//std::cout << "S" << std::endl;
		updateViewMatrix();
		updateProjectionMatrix();
	}
	else if (button == "A")
	{
		//cameraPosition -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
		cameraContainer.cameraPosition += cameraContainer.cameraRight * cameraSpeed;
		//cameraContainer.cameraPosition.y = fixHeight;  //
		cameraContainer.cameraTarget = glm::vec3(cameraContainer.cameraPosition + cameraContainer.cameraFront);
		//std::cout << "A" << std::endl;
		updateViewMatrix();
		updateProjectionMatrix();
	}
	else if (button == "D")
	{
		//cameraPosition += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
		cameraContainer.cameraPosition -= cameraContainer.cameraRight * cameraSpeed;
		//cameraContainer.cameraPosition.y = fixHeight;  //
		cameraContainer.cameraTarget = glm::vec3(cameraContainer.cameraPosition + cameraContainer.cameraFront);
		//std::cout << "D" << std::endl;
		updateViewMatrix();
		updateProjectionMatrix();
	}
	else if (button == "R")  // rotate camera around origo
	{
		cameraContainer.cameraPosition -= cameraContainer.cameraRight * 0.05f;  // 0.05f
		//cameraContainer.cameraPosition.y = fixHeight;  //
		cameraContainer.cameraFront = glm::vec3(cameraContainer.cameraPosition - cameraContainer.cameraTarget);
		cameraContainer.cameraRight = glm::normalize(glm::cross(worldUp, cameraContainer.cameraFront));
		//std::cout << "R" << std::endl;
		updateViewMatrix();
		updateProjectionMatrix();
	}
}

void Kamera::processMouseMovement(float xOffset, float yOffset)
{
	glm::vec3 worldUp = glm::vec3(0.0f, 1.0f, 0.0f);
	float sensitivity = 0.05f;
	xOffset *= sensitivity;
	yOffset *= sensitivity;

	cameraContainer.yaw += xOffset;
	cameraContainer.pitch += yOffset;

	if (cameraContainer.pitch > 89.0f)
	{
		cameraContainer.pitch = 89.0f;
	}
	else if (cameraContainer.pitch < -89.0f)
	{
		cameraContainer.pitch = -89.0f;
	}

	glm::vec3 newFront = cameraContainer.cameraFront;
	newFront.x = cos(glm::radians(cameraContainer.yaw)) * cos(glm::radians(cameraContainer.pitch));
	newFront.y = sin(glm::radians(cameraContainer.pitch));
	newFront.z = sin(glm::radians(cameraContainer.yaw)) * cos(glm::radians(cameraContainer.pitch));

	cameraContainer.cameraFront = glm::normalize(newFront);
	cameraContainer.cameraRight = glm::normalize(glm::cross(worldUp, cameraContainer.cameraFront));
	cameraContainer.cameraUp = glm::cross(cameraContainer.cameraFront, cameraContainer.cameraRight);
	cameraContainer.cameraTarget = glm::vec3(cameraContainer.cameraPosition + cameraContainer.cameraFront);

	updateViewMatrix();
	updateProjectionMatrix();
}

void Kamera::processMouseScroll(double yoffset)
{
	if (cameraContainer.fov >= 1.0f && cameraContainer.fov <= 45.0f)
	{
		cameraContainer.fov -= (float)yoffset;
	}
	if (cameraContainer.fov <= 1.0f)
	{
		cameraContainer.fov = 1.0f;
	}
	if (cameraContainer.fov >= 45.0f)
	{
		cameraContainer.fov = 45.0f;
	}
	
	updateProjectionMatrix();
}

