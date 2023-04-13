#pragma once

#include "Application.h"

#include "Window.h"
#include "render/Renderer.h"
#include "scene/Scene.h"
#include "events/EventManager.h"
#include "gui/GUI.h"
#include "commands/CommandManager.h"

#include "GLFW/glfw3.h"

void Application::Run()
{
	Init();
	double targetFrameRate = 30.0, frame_start = glfwGetTime();
	while (!Window::ShouldClose()) 
	{
		while (glfwGetTime() < frame_start + 1 / targetFrameRate);
		frame_start = glfwGetTime();
		Renderer::BeginRender();
		Scene::Render();
		GUI::Render();
		Renderer::FinishRender();
		EventManager::Tick();
		CommandManager::Tick();
	}
	Terminate();
}

void Application::Terminate()
{
	GUI::Destroy();
	Scene::Destroy();
	Renderer::Destroy();
	Window::Destroy();
}

void Application::Init() {
	Window::Init();
	Renderer::Init();
	Scene::Init();
	GUI::Init();

}