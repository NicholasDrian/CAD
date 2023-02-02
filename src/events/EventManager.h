#pragma once

#include "../render/Renderer.h"

#include "GLFW/glfw3.h"

class GLFWwindow;

class EventManager {

public:

	inline static void WindowResize(GLFWwindow* window, int width, int height) 
	{
		Renderer::WindowResize(width, height);
	}

};
