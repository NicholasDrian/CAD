#pragma once

#include "Application.h"

#include "Window.h"
#include "render/Renderer.h"
#include "scene/Scene.h"

#include "GLFW/glfw3.h"

void Application::Run()
{
	Init();
	double targetFrameRate = 30, frame_start = glfwGetTime();
	while (!Window::ShouldClose()) 
	{
		while (glfwGetTime() < frame_start + 1 / targetFrameRate);
		frame_start = glfwGetTime();
		Renderer::BeginRender();
		Scene::Render();
		Renderer::FinishRender();
		Window::PollEvents();
	}
	Terminate();
}

void Application::Terminate()
{
	Scene::Destroy();
	Renderer::Destroy();
	Window::Destroy();
}

void Application::Init() {
	Window::Init();
	Renderer::Init();
	Scene::Init();
	Renderer::SetClearColor({ 1.0, 0.0, 1.0 });
}