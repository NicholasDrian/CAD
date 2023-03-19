#pragma once

#include "EventManager.h"
#include "../Window.h"
#include "../scene/Scene.h"
#include "../gui/GUI.h"
#include <iostream>

bool EventManager::m_MouseButtonDown = false;
int EventManager::m_ClickModifiers = 0;
int EventManager::m_MouseButton = -1;
double EventManager::m_InitialMouseX = -1;
double EventManager::m_InitialMouseY = -1;
double EventManager::m_PreviousMouseX = -1;
double EventManager::m_PreviousMouseY = -1;

void EventManager::Tick()
{
    Window::PollEvents();
    if (m_MouseButtonDown) {
        double mouseX, mouseY;
        glfwGetCursorPos(Window::GetGLFWWindow(), &mouseX, &mouseY);
        int dx = int(mouseX - m_PreviousMouseX);
        int dy = int(mouseY - m_PreviousMouseY);
        switch (m_ClickModifiers) {
        case GLFW_MOD_SHIFT:
            if (dx) Scene::GetCamera()->PanRight(dx);
            if (dy) Scene::GetCamera()->PanUp(dy);
            break;
        case GLFW_MOD_CONTROL:
            if (dy) Scene::GetCamera()->ZoomIn(dy);
            break;
        default:
            if (dx) Scene::GetCamera()->TurnRight(dx);
            if (dy) Scene::GetCamera()->TurnUp(dy);
        }
        m_PreviousMouseX = mouseX;
        m_PreviousMouseY = mouseY;
    }
}

void EventManager::WindowResizeCallback(GLFWwindow* window, int width, int height)
{
	Renderer::WindowResize(width, height);
}

void EventManager::MouseCallback(GLFWwindow* window, int button, int action, int mods)
{

    // ignore mouse event if handled by gui...
    ImGuiIO& io = GUI::GetIO();
    if (io.WantCaptureMouse) return;

    double mouseX, mouseY;
    glfwGetCursorPos(Window::GetGLFWWindow(), &mouseX, &mouseY);

    if (action == GLFW_PRESS && !m_MouseButtonDown) 
    {
        m_MouseButtonDown = true;
        m_InitialMouseX = mouseX;
        m_InitialMouseY = mouseY;
        m_PreviousMouseX = mouseX;
        m_PreviousMouseY = mouseY;
        m_MouseButton = button;
        m_ClickModifiers = mods;
    }
    if (action == GLFW_RELEASE && m_MouseButtonDown && button == m_MouseButton)
    {
        if (mouseX == m_InitialMouseX && mouseY == m_InitialMouseY) {
            // TODO: Click!
        }
        m_MouseButtonDown = false;
    }
}
