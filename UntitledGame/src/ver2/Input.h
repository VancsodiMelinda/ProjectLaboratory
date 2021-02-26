#pragma once
#include <GLFW/glfw3.h>

#include "../ver2/GlobalVariables.h"
#include "../ver2/Kamera.h"

class Input
{
public:
	Input(Kamera kamera_);

private:
	Kamera kamera;
	float deltaTime = 0.0f;
	float lastFrame = 0.0f;
	float lastX = (float)WINDOW_WIDTH / 2.0f;
	float lastY = (float)WINDOW_HEIGHT / 2.0f;
	bool firstMouse = true;
	bool p_press = false;

public:
	void processKeyboardInput(GLFWwindow* window);
	void cursor_position_callback(GLFWwindow* window, double xpos, double ypos);
	void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
};

