#pragma once

struct GLFWwindow;

class Window {

public:

	Window(unsigned width = 800, unsigned height = 600);
	~Window();

	bool ShouldClose();

	void PollEvents();

	void SwapBuffers();

private:

	unsigned m_Width, m_Height;
	GLFWwindow* m_Window;

};	