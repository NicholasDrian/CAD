#pragma once

#include "Application.h"

#include "Window.h"
#include "render/Renderer.h"
#include "scene/Scene.h"

#include <iostream>
#include "GLFW/glfw3.h"

Application::Application()
	: m_Window(new Window()), m_Scene(new Scene())
{
	Renderer::InitRenderer();
	Renderer::SetClearColor({ 0.0, 0.0, 1.0 });

	std::vector<Vertex> verts = {
		{{ -0.5, -0.5, 0.0 }, {0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}, 0 },
		{{ -0.5, 0.5, 0.0 }, {0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}, 0 },
		{{ 0.5, 0.5, 0.0 }, {0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}, 0 },
		{{ 0.5, -0.5, 0.0 }, {0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}, 0 }
	};

	std::vector<unsigned> indecies = {
		0, 1, 2,
		0,3,2
	};

	VertexArray vao(verts, indecies, PrimitiveType::Triangles);

	float targetFrameRate = 2;
	float frame_start = glfwGetTime();

	while (!m_Window->ShouldClose()) {

		while (glfwGetTime() < frame_start + 1 / targetFrameRate);
		frame_start = glfwGetTime();
		std::cout << frame_start << std::endl;
		Renderer::BeginRender();
		Renderer::Render(vao);

		Renderer::FinishRender(m_Window);
		m_Window->PollEvents();
	}
}

Application::~Application()
{
	delete m_Scene;
	delete m_Window;
}