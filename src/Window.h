#pragma once

#include <utility>

struct GLFWwindow;

class Window {

public:

	static void Init(unsigned width = 1200, unsigned height = 800);
	static void Destroy();

	static bool ShouldClose();

	static void PollEvents();

	static void SwapBuffers();

	static std::pair<unsigned, unsigned> GetSize();
	static float GetAspect();

	static inline GLFWwindow* GetGLFWWindow() { return m_Window; }

private:

	static GLFWwindow* m_Window;

};	