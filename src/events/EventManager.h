#pragma once

#include "../render/Renderer.h"

#include "GLFW/glfw3.h"

class GLFWwindow;

class EventManager {

public:

	static void Tick();

	static void WindowResizeCallback(GLFWwindow* window, int width, int height);

	static void MouseCallback(GLFWwindow* window, int button, int action, int mods);

private:

	static bool m_MouseButtonDown;
	static int m_ClickModifiers, m_MouseButton;
	static double m_InitialMouseX, m_InitialMouseY;
	static double m_PreviousMouseX, m_PreviousMouseY;

};
