#pragma once

#include "Window.h"
#include "events/EventManager.h"
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

    glfwSetMouseButtonCallback(m_Window, EventManager::MouseCallback);
    glfwSetKeyCallback(m_Window, EventManager::KeyCallback);
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

std::pair<unsigned, unsigned> Window::GetCursorPosition()
{
    double x, y;
    glfwGetCursorPos(m_Window, &x, &y);
    return { (int)x, (int)y };
}

void Window::SwapBuffers() {
    glfwSwapBuffers(m_Window);
}

std::pair<unsigned, unsigned> Window::GetSize() {
    int width, height;
    glfwGetWindowSize(m_Window, &width, &height);
    return { width, height };
}


std::pair<unsigned, unsigned> Window::GetPos() {
    int x, y;
    glfwGetWindowPos(m_Window, &x, &y);
    return {x, y};
}

float Window::GetAspect() {
    auto [width, height] = GetSize();
    return float(width) / float(height);
}

bool Window::IsWithinLocal(unsigned x, unsigned y)
{
    auto [sizex, sizey] = GetSize();
    return x > 0 && y > 0 && x < sizex && y < sizey;
}