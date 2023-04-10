#pragma once

#include "EventManager.h"
#include "../Window.h"
#include "../scene/Scene.h"
#include "../gui/GUI.h"
#include "../commands/CommandManager.h"
#include <iostream>



void EventManager::Tick()
{
    Window::PollEvents();
    if (m_MouseButtonDown && m_MouseButton == GLFW_MOUSE_BUTTON_RIGHT) {
        double mouseX, mouseY;
        glfwGetCursorPos(Window::GetGLFWWindow(), &mouseX, &mouseY);
        int dx = int(mouseX - m_PreviousMouseX);
        int dy = int(mouseY - m_PreviousMouseY);
        m_MouseDragged = m_MouseDragged || dx || dy;
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

    if (action == GLFW_PRESS && !m_MouseButtonDown) 
    {
        double mouseX, mouseY;
        glfwGetCursorPos(Window::GetGLFWWindow(), &mouseX, &mouseY);
        m_MouseDragged = false;
        m_MouseButtonDown = true;
        m_PreviousMouseX = mouseX;
        m_PreviousMouseY = mouseY;
        m_MouseButton = button;
        m_ClickModifiers = mods;
    }
    else if (action == GLFW_RELEASE && m_MouseButtonDown && button == m_MouseButton)
    {
        double mouseX, mouseY;
        glfwGetCursorPos(Window::GetGLFWWindow(), &mouseX, &mouseY);

        if (!m_MouseDragged) { // click!
            if (CommandManager::HasActiveCommand()) 
            {
                CommandManager::HandleClick((int)mouseX, (int)mouseY);
            }
            else 
            {
                Scene::HandleClick((int)mouseX, (int)mouseY, button, mods);
            }
        }
        m_MouseButtonDown = false;
    }
}

void EventManager::KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) 
{
    ImGuiIO& io = GUI::GetIO();
    if (io.WantCaptureKeyboard) return;

    if (m_MouseButtonDown && m_MouseButton == GLFW_MOUSE_BUTTON_RIGHT) {
        // navigation key
    }

    else if (action == GLFW_PRESS || action == GLFW_REPEAT)
        CommandManager::AddInput(key);
}
