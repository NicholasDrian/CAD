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
    if (m_MouseButtonDown) {
        if (m_MouseButton == GLFW_MOUSE_BUTTON_RIGHT) {
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

            if (m_CameraMovementInput) {
                float now = (float)glfwGetTime();
                float delta = (now - m_LastMovementUpdateTime) * 200.0f;
                m_LastMovementUpdateTime = now;
                if (m_CameraMovementInput & MOVEMENT_BITS.at(GLFW_KEY_W)) Scene::GetCamera()->MoveForward(delta);
                if (m_CameraMovementInput & MOVEMENT_BITS.at(GLFW_KEY_S)) Scene::GetCamera()->MoveForward(-delta);
                if (m_CameraMovementInput & MOVEMENT_BITS.at(GLFW_KEY_A)) Scene::GetCamera()->PanRight(delta);
                if (m_CameraMovementInput & MOVEMENT_BITS.at(GLFW_KEY_D)) Scene::GetCamera()->PanRight(-delta);
                if (m_CameraMovementInput & MOVEMENT_BITS.at(GLFW_KEY_E)) Scene::GetCamera()->PanUp(delta);
                if (m_CameraMovementInput & MOVEMENT_BITS.at(GLFW_KEY_Q)) Scene::GetCamera()->PanUp(-delta);

            }
        } 
        else if (m_MouseButton == GLFW_MOUSE_BUTTON_LEFT)
        {
            double left, top;
            glfwGetCursorPos(Window::GetGLFWWindow(), &left, &top);
            int l = (int)left;
            int t = (int)top;
            int r = (int)m_InitialMouseX;
            int b = (int)m_InitialMouseY;
            m_MouseDragged = l != r || t != b;
            if (l > r) std::swap(l, r);
            if (t > b) std::swap(t, b);
            Scene::UpdateSelectionRectangle(l, t, r, b);
        }
    }
}

void EventManager::MouseCallback(GLFWwindow* window, int button, int action, int mods)
{

    // ignore new clicks if handled by imgui...
    ImGuiIO& io = GUI::GetIO();
    if (io.WantCaptureMouse && action == GLFW_PRESS) return;

    if (action == GLFW_PRESS && !m_MouseButtonDown) 
    {
        double mouseX, mouseY;
        glfwGetCursorPos(Window::GetGLFWWindow(), &mouseX, &mouseY);
        m_MouseDragged = false;
        m_MouseButtonDown = true;
        m_InitialMouseX = m_PreviousMouseX = mouseX;
        m_InitialMouseY = m_PreviousMouseY = mouseY;

        m_MouseButton = button;
        m_ClickModifiers = mods;
    }
    else if (action == GLFW_RELEASE && m_MouseButtonDown && button == m_MouseButton)
    {
        double mouseX, mouseY;
        glfwGetCursorPos(Window::GetGLFWWindow(), &mouseX, &mouseY);

        if (!m_MouseDragged) // click!
        { 
            if (CommandManager::HasActiveCommand())  CommandManager::HandleClick((int)mouseX, (int)mouseY, mods);
            else  Scene::HandleClick((int)mouseX, (int)mouseY, mods);
        }
        else 
        {
            if (m_MouseButton == GLFW_MOUSE_BUTTON_LEFT) {
                const bool inclusive = m_InitialMouseX > mouseX;
                Scene::ApplySelectionRectangle(inclusive, mods);
            }
        }
        m_MouseButtonDown = false;
        m_CameraMovementInput = 0;
    }
}

void EventManager::KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) 
{
    ImGuiIO& io = GUI::GetIO();
    if (io.WantCaptureKeyboard) return;

    if (m_MouseButtonDown && m_MouseButton == GLFW_MOUSE_BUTTON_RIGHT) {
        if (MOVEMENT_BITS.contains(key)) {
            if (action == GLFW_PRESS) {
                m_CameraMovementInput |= MOVEMENT_BITS.at(key);
                m_LastMovementUpdateTime = (float)glfwGetTime();
            }
            else if (action == GLFW_RELEASE) {
                m_CameraMovementInput &= ~MOVEMENT_BITS.at(key);
            }
        }
    }

    else if (action == GLFW_PRESS || action == GLFW_REPEAT)
        CommandManager::AddInput(key);
}
