#pragma once

#include "Window.h"
#include "stdexcept"

#include "GLFW/glfw3.h"

#include <iostream>

GLFWwindow* Window::m_Window = nullptr;

void Window::Init(unsigned width, unsigned height)
{
    if (!glfwInit()) {
        throw std::runtime_error("Failed to initialize glfw!");
    }
    m_Window = glfwCreateWindow(width, height, "CAD", nullptr, nullptr);
    if (!m_Window) {
        throw std::runtime_error("Failed to create window!");
    }
    glfwMakeContextCurrent(m_Window);

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
}

void Window::Destroy()
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

std::pair<unsigned, unsigned> Window::GetSize() {
    int width, height;
    glfwGetWindowSize(m_Window, &width, &height);
    return { width, height };
}

float Window::GetAspect() {
    auto [width, height] = GetSize();
    return float(width) / float(height);
}