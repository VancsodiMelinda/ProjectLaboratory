#pragma once

#include <GLFW/glfw3.h>

#include <string>
#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "GlobalVariables.h"
#include "Instrumentor.h"

// these values change in every frame as the camera moves
struct CameraContainer
{
	glm::vec3 cameraPosition = glm::vec3();
	glm::vec3 cameraFront = glm::vec3();
	glm::vec3 cameraTarget = glm::vec3();
	glm::vec3 cameraRight = glm::vec3();
	glm::vec3 cameraUp = glm::vec3();

	float yaw = 270.0f;
	float pitch = 0.0f;
	float fov = 45.0f;

	glm::mat4 viewMatrix = glm::mat4(1.0f);
	glm::mat4 projectionMatrix = glm::mat4(1.0f);

	float nearPlane = 0.1f;
	float farPlane = 100.0f;
};


class Kamera
{
public:
	CameraContainer cameraContainer;

	Kamera();

private:
	void updateViewMatrix();
	void updateProjectionMatrix();
	float deltaTime = 0.0f;
	float lastFrame = 0.0f;
	float fixHeight = 1.5f;

public:
	void processKeyboardInput(GLFWwindow* window);
	void processKeyInput(std::string button, float deltaTime);
	void processMouseMovement(float xOffset, float yOffset);
	void processMouseScroll(double yoffset);
};

