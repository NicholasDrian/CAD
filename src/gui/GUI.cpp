#pragma once

#include "GUI.h"

#include "../commands/CommandManager.h"
#include "../Window.h"
#include "Style.h"

#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <imgui_internal.h>

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

bool HY_ImGui_BeginMainStatusBar()
{
    ImGuiContext& g = *GImGui;
    ImGuiViewportP* viewport = g.Viewports[0];
    ImGuiWindow* menu_bar_window = ImGui::FindWindowByName("##MainStatusBar");

    // For the main menu bar, which cannot be moved, we honor g.Style.DisplaySafeAreaPadding to ensure text can be visible on a TV set.
    g.NextWindowData.MenuBarOffsetMinVal = ImVec2(g.Style.DisplaySafeAreaPadding.x, ImMax(g.Style.DisplaySafeAreaPadding.y - g.Style.FramePadding.y, 0.0f));

    // Get our rectangle at the top of the work area
    //__debugbreak();
    if (menu_bar_window == NULL || menu_bar_window->BeginCount == 0)
    {
        // Set window position
        // We don't attempt to calculate our height ahead, as it depends on the per-viewport font size. However menu-bar will affect the minimum window size so we'll get the right height.
        ImVec2 menu_bar_pos = { viewport->Pos.x + viewport->WorkOffsetMin.x, viewport->Pos.y + viewport->WorkOffsetMin.y };
        ImVec2 menu_bar_size = ImVec2(viewport->Size.x - viewport->WorkOffsetMin.x + viewport->WorkOffsetMax.x, 1.0f);
        ImGui::SetNextWindowPos(menu_bar_pos);
        ImGui::SetNextWindowSize(menu_bar_size);
    }

    // Create window
    ImGui::SetNextWindowViewport(viewport->ID); // Enforce viewport so we don't create our own viewport when ImGuiConfigFlags_ViewportsNoMerge is set.
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowMinSize, ImVec2(0, 0));    // Lift normal size constraint, however the presence of a menu-bar will give us the minimum height we want.
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_MenuBar;
    bool is_open = ImGui::Begin("##MainStatusBar", NULL, window_flags) && ImGui::BeginMenuBar();
    ImGui::PopStyleVar(2);

    // Report our size into work area (for next frame) using actual window size
    menu_bar_window = ImGui::GetCurrentWindow();
    if (menu_bar_window->BeginCount == 1)
        viewport->WorkOffsetMin.y += menu_bar_window->Size.y;

    g.NextWindowData.MenuBarOffsetMinVal = ImVec2(0.0f, 0.0f);
    if (!is_open)
    {
        ImGui::End();
        return false;
    }
    return true; //-V1020
}

void HY_ImGui_EndMainStatusBar()
{
    ImGui::EndMenuBar();

    // When the user has left the menu layer (typically: closed menus through activation of an item), we restore focus to the previous window
    // FIXME: With this strategy we won't be able to restore a NULL focus.
    ImGuiContext& g = *GImGui;
    if (g.CurrentWindow == g.NavWindow && g.NavLayer == ImGuiNavLayer_Main && !g.NavAnyRequest)
        ImGui::FocusTopMostWindowUnderOne(g.NavWindow, NULL);

    ImGui::End();
}

void GUI::Render() {

    BeginRender();


    static bool m_ShowDisplayOptions = false;
    static bool m_ShowGenerator = false;
    static bool m_ShowStats = false;

    if (ImGui::BeginMainMenuBar())
    {
        //ImGui::Text(CommandManager::GetInput().c_str());

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
    if (HY_ImGui_BeginMainStatusBar()) {
        ImGui::Text(CommandManager::GetInstructions().c_str());

        ImGui::Text(CommandManager::GetInput().c_str());
        HY_ImGui_EndMainStatusBar();
    }
  /*  if (ImGui::BeginMainMenuBar())
    {
        ImGui::Text(CommandManager::GetInstructions().c_str());

        ImGui::Text(CommandManager::GetInput().c_str());

        ImGui::EndMainMenuBar();
    }*/

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