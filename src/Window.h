#pragma once

#include <utility>

struct GLFWwindow;

class Window {

public:

	static void Init(unsigned width = 800, unsigned height = 600);
	static void Destroy();

	static bool ShouldClose();

	static void PollEvents();

	static void SwapBuffers();

	static std::pair<unsigned, unsigned> GetSize();
	static float GetAspect();

private:

	static GLFWwindow* m_Window;

};	