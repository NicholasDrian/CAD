#pragma once

#include "Window.h"
#include "stdexcept"

#include "GLFW/glfw3.h"

Window::Window(unsigned width, unsigned height)
    : m_Width(width), m_Height(height)
{
    if (!glfwInit()) {
        throw std::runtime_error("Failed to initialize glfw!");
    }
    m_Window = glfwCreateWindow(m_Width, m_Height, "CAD", nullptr, nullptr);
    if (!m_Window) {
        throw std::runtime_error("Failed to create window!");
    }
    glfwMakeContextCurrent(m_Window);

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
}

Window::~Window()
{
    glfwTerminate();
}

bool Window::ShouldClose()
{
    return glfwWindowShouldClose(m_Window);
}

void Window::PollEvents()
{
    glfwPollEvents();
}

void Window::SwapBuffers() {
    glfwSwapBuffers(m_Window);
}
