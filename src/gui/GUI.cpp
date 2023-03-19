#pragma once

#include "GUI.h"

#include "../Window.h"
#include "Style.h"

#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include <GLFW/glfw3.h>

#include <string>
#include <stdexcept>
#include <iostream>

void GUI::Init() {
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

    setImGuiStyle();

    // setup platform/renderer bindings
    if (!ImGui_ImplGlfw_InitForOpenGL(Window::GetGLFWWindow(), true))
        throw std::runtime_error("error!");
    if (!ImGui_ImplOpenGL3_Init()) 
        throw std::runtime_error("error!");
}
void GUI::Destroy() {
    ImGui::DestroyContext();
}

void GUI::BeginRender() {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();


    ImGui::DockSpaceOverViewport(nullptr, ImGuiDockNodeFlags_PassthruCentralNode);
 
}

void GUI::Render() {

    BeginRender();


    static bool m_ShowDisplayOptions = false;
    static bool m_ShowGenerator = false;
    static bool m_ShowStats = false;

    if (ImGui::BeginMainMenuBar())
    {
        static char* buff = (char*) calloc(128, 1);
        if (ImGui::InputText("", buff, 128, ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_EscapeClearsAll)) {
            std::cout << buff << std::endl;
        }

        if (ImGui::BeginMenu("Menu"))
        {
            if (ImGui::MenuItem("Load New Animation", NULL)) {

            }
                

            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("View"))
        {
            if (ImGui::MenuItem(m_ShowDisplayOptions ? "Hide Display Options" : "Show Display Options")) {
                m_ShowDisplayOptions = !m_ShowDisplayOptions;
            }
            if (ImGui::MenuItem(m_ShowGenerator ? "Hide VAT Generator" : "Show VAT Generator")) {
                m_ShowGenerator = !m_ShowGenerator;
            }
            if (ImGui::MenuItem(m_ShowStats ? "Hide Stats" : "Show Stats")) {
                m_ShowStats = !m_ShowStats;
            }
            ImGui::EndMenu();
        }

        ImGui::EndMainMenuBar();
    }

    ImGui::Begin("Controls", NULL, ImGuiWindowFlags_NoResize);

    ImGuiTabBarFlags tab_bar_flags = ImGuiTabBarFlags_None;
    if (ImGui::BeginTabBar("MyTabBar", tab_bar_flags))
    {
        if (ImGui::BeginTabItem("Display"))
        {
            ImGui::Text("This is the Avocado tab!\nblah blah blah blah blah");
            ImGui::EndTabItem();
        }
        if (ImGui::BeginTabItem("Geometry"))
        {
            ImGui::Text("This is the Broccoli tab!\nblah blah blah blah blah");
            ImGui::EndTabItem();
        }
        if (ImGui::BeginTabItem("Help"))
        {
            ImGui::Text("This is the Cucumber tab!\nblah blah blah blah blah");
            ImGui::EndTabItem();
        }
        ImGui::EndTabBar();
    }

    EndRender();
    
}

void GUI::EndRender() {


    ImGui::End();

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