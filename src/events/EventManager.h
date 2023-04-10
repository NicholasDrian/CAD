#pragma once

#include "../render/Renderer.h"

#include "GLFW/glfw3.h"

struct GLFWwindow;

class EventManager {

public:

	static void Tick();

	static void WindowResizeCallback(GLFWwindow* window, int width, int height);
	static void MouseCallback(GLFWwindow* window, int button, int action, int mods);
	static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

private:

	inline static float m_LastMovementUpdateTime;
	inline static int m_CameraMovementInput = 0;
	inline static bool m_MouseDragged = false;
	inline static bool m_MouseButtonDown = false;
	inline static int m_ClickModifiers = 0, m_MouseButton = -1;
	inline static double m_PreviousMouseX = -1, m_PreviousMouseY = -1;

};
