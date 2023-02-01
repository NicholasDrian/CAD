#pragma once

#include "Application.h"

#include "Window.h"
#include "render/Renderer.h"
#include "scene/Scene.h"
#include "render/VertexBuffer.h"
#include "render/IndexBuffer.h"
#include "render/VertexArray.h"

#include <iostream>
#include "GLFW/glfw3.h"

#include "iostream"

void Application::Run()
{
	Init();

	std::vector<Vertex> verts = {
		{{ -0.5, -0.5, 0.0 }, {0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}, 0 },
		{{ -0.5, 0.5, 0.0 }, {0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}, 0 },
		{{ 0.5, 0.5, 0.0 }, {0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}, 0 },
		{{ 0.5, -0.5, 0.0 }, {0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}, 0 }
	};

	std::vector<unsigned> indices = {
		0, 1, 2,
		0, 3, 2
	};

	std::shared_ptr<VertexBuffer> vbo = std::make_shared<VertexBuffer>(verts);
	std::shared_ptr<IndexBuffer> ebo = std::make_shared<IndexBuffer>(indices);

	VertexArray vao(vbo, ebo, PrimitiveType::Triangles);

	double targetFrameRate = 30, frame_start = glfwGetTime();

	while (!Window::ShouldClose()) {

		while (glfwGetTime() < frame_start + 1 / targetFrameRate);
		frame_start = glfwGetTime();

		Renderer::BeginRender();
		Renderer::Render(vao);

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

	Renderer::SetClearColor({ 0.0, 0.0, 1.0 });
}