#pragma once

#include "../render/Renderer.h"

#include "GLFW/glfw3.h"

#include <unordered_map>

const std::unordered_map<int, char> MOVEMENT_BITS = {
	{GLFW_KEY_W, 1 << 0},
	{GLFW_KEY_S, 1 << 1},
	{GLFW_KEY_A, 1 << 2},
	{GLFW_KEY_D, 1 << 3},
	{GLFW_KEY_E, 1 << 4},
	{GLFW_KEY_Q, 1 << 5},
};

class EventManager {

public:

	static void Tick();

	static void MouseCallback(GLFWwindow* window, int button, int action, int mods);
	static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

private:

	inline static float m_LastMovementUpdateTime;
	inline static char m_CameraMovementInput = 0;
	inline static bool m_MouseDragged = false;
	inline static bool m_MouseButtonDown = false;
	inline static int m_ClickModifiers = 0, m_MouseButton = -1;
	inline static double m_PreviousMouseX = -1, m_PreviousMouseY = -1, m_InitialMouseX = -1, m_InitialMouseY = -1;

};
