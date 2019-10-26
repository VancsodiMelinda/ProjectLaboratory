#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/ext.hpp>
#include <string>
#include <stdio.h>
#include <iostream>

class Camera
{
public:
	// viewMatrix attributes you specify
	glm::vec3 cameraPosition;
	glm::vec3 cameraFront;
	glm::vec3 worldUp;
	// viewMatrix attributes that gets calculated
	glm::vec3 cameraTarget;
	//glm::vec3 cameraDirection;
	glm::vec3 cameraRight;
	glm::vec3 cameraUp;
	// projectionMatrix attributes
	float yaw;
	float pitch;
	float fov;

	Camera();  // default constructor
	Camera(glm::vec3 cameraPosition_, glm::vec3 cameraFront_, glm::vec3 cameraUp_, float yaw_, float pitch_, float fov_);  // parameterized constructor

	glm::mat4 CreateViewMatrix();

	void ProcessKeyInput(std::string button, float deltaTime);
	void ProcessMouseMovement(float xOffset, float yOffset);
	void ProcessMouseScroll(double yoffset);
};

