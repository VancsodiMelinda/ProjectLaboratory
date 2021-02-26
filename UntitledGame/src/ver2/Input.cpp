#include "Input.h"

Input::Input(Kamera kamera_)
{
	kamera = kamera_;
}

void Input::processKeyboardInput(GLFWwindow* window)
{
	// time calculations
	float currentFrame = (float)glfwGetTime();
	deltaTime = currentFrame - lastFrame;
	lastFrame = currentFrame;

	float cameraSpeed = 2.5f * deltaTime;
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
		kamera.processKeyInput("W", deltaTime);
	}
	else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		kamera.processKeyInput("S", deltaTime);
	}
	else if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		kamera.processKeyInput("A", deltaTime);
	}
	else if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		kamera.processKeyInput("D", deltaTime);
	}
	else if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
	{
		kamera.processKeyInput("R", deltaTime);
	}
}

void Input::cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (!p_press)
	{
		if (firstMouse)
		{
			lastX = (float)xpos;
			lastY = (float)ypos;
			firstMouse = false;
		}

		float xOffset = (float)xpos - lastX;
		float yOffset = lastY - (float)ypos;

		lastX = (float)xpos;
		lastY = (float)ypos;

		kamera.processMouseMovement(xOffset, yOffset);
	}
}

void Input::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, GLFW_TRUE);
	}
	else if (key == GLFW_KEY_P && action == GLFW_PRESS)
	{
		if (p_press)
		{
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);  // lock cursor
			//glfwSetCursorPosCallback(window, cursor_position_callback);
		}
		else if (!p_press)
		{
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);  // free cursor
			//glfwSetCursorPosCallback(window, cursor_position_callback_2);
		}

		p_press = !p_press;
	}
}
