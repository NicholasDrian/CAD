#pragma once

#include "GUI.h"

#include "../commands/CommandManager.h"
#include "../Window.h"
#include "../scene/Scene.h"
#include "CommandInfoGUI.h"
#include "SettingsGUI.h"
#include "Style.h"

#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <imgui_internal.h>

#include "ImGuizmo/ImGuizmo.h"
#include "AffineTransformWidget.h"

#include <GLFW/glfw3.h>

#include <string>
#include <stdexcept>
#include <iostream>

void GUI::Init() {

    IMGUI_CHECKVERSION();
    m_Context = ImGui::CreateContext();

    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

    // setup platform/renderer bindings
    if (!ImGui_ImplGlfw_InitForOpenGL(Window::GetGLFWWindow(), true))
        throw std::runtime_error("error!");
    if (!ImGui_ImplOpenGL3_Init("#version 420"))
        throw std::runtime_error("error!");

    setImGuiStyle();

    CommandInfoGUI::Init();
}
void GUI::Destroy() {
    ImGui::DestroyContext();
}

void GUI::BeginRender() {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    ImGuizmo::BeginFrame();
    ImGuizmo::Enable(true);

    ImGuiDockNodeFlags dockingFlags = 
        ImGuiDockNodeFlags_PassthruCentralNode | 
        ImGuiDockNodeFlags_NoDockingInCentralNode;
    ImGui::DockSpaceOverViewport(nullptr, dockingFlags);
}

void GUI::Render() {

    BeginRender();

    Scene::DrawGUI();


    if (ImGui::BeginMainMenuBar())
    {

        if (ImGui::BeginMenu("File"))
        {
            if (ImGui::MenuItem("Save All", NULL)) {
                // TODO
            }
            if (ImGui::MenuItem("Export Selected", NULL)) {
                // TODO
            }
            if (ImGui::MenuItem("Import", NULL)) {
                // TODO
            } 
            if (ImGui::MenuItem("Open File", NULL)) {
                // TODO
            }

            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("View"))
        {
            if (ImGui::MenuItem(m_ShowSettings ? "Hide Display Options" : "Show Display Options")) {
                m_ShowSettings = !m_ShowSettings;
            }
            if (ImGui::MenuItem(m_ShowDetails ? "Hide Details" : "Show Details")) {
                m_ShowDetails = !m_ShowDetails;
            }
            if (ImGui::MenuItem(m_ShowCommandInfo ? "Hide Command Info" : "Show Command Info")) {
                m_ShowCommandInfo = !m_ShowCommandInfo;
            }
            ImGui::EndMenu();
        }

        ImGui::EndMainMenuBar();
    }

    ImGuiViewportP* viewport = (ImGuiViewportP*)(void*)ImGui::GetMainViewport();
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_MenuBar;
    float height = ImGui::GetFrameHeight();
    if (ImGui::BeginViewportSideBar("##SecondaryMenuBar", viewport, ImGuiDir_Up, height, window_flags)) {
        if (ImGui::BeginMenuBar()) {
            CommandManager::DrawButtons();
            ImGui::Text(CommandManager::GetInstructions().c_str());
            ImGui::Text(CommandManager::GetInput().c_str());
            ImGui::EndMenuBar();
        }
        ImGui::End();
    }

    if (m_ShowCommandInfo || m_ShowDetails || m_ShowSettings) {
        ImGuiWindowFlags flags = ImGuiWindowFlags_NoCollapse;
        if (ImGui::Begin("Controls", NULL, flags) &&
            ImGui::BeginTabBar("MyTabBar", ImGuiTabBarFlags_None)) {
            if (m_ShowCommandInfo) {
                if (ImGui::BeginTabItem("Command Info")) {
                    CommandInfoGUI::Render();
                    ImGui::EndTabItem();
                }
            }
            if (m_ShowDetails) {
                if (ImGui::BeginTabItem("Details")) {
                    ImGui::EndTabItem();
                }
            }
            if (m_ShowSettings) {
                if (ImGui::BeginTabItem("Settings")) {
                    SettingsGUI::Render();
                    ImGui::EndTabItem();
                }
            }
            ImGui::EndTabBar();
            ImGui::End();
        }
    }

    EndRender();
    
}

void GUI::EndRender() {

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    // Update and Render additional Platform Windows
    // (Platform functions may change the current OpenGL context, so we save/restore it to make it easier to paste this code elsewhere.
    //  For this specific demo app we could also call glfwMakeContextCurrent(window) directly)
    GLFWwindow* backup_current_context = glfwGetCurrentContext();
    ImGui::UpdatePlatformWindows();
    ImGui::RenderPlatformWindowsDefault();
    glfwMakeContextCurrent(backup_current_context);
}