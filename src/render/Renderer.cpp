#pragma once

#include "Renderer.h"
#include "Shader.h"
#include "../Window.h"
#include "../utils/OpenGLUtils.h"

#include "GL/glew.h"

#include <stdexcept>

ShaderProgram* Renderer::m_Program = nullptr;
RenderMode Renderer::m_RenderMode = RenderMode::None;

void Renderer::InitRenderer()
{
	if (glewInit() != GLEW_OK) {
		throw std::runtime_error("Failed to initialize glew!");
	}
	SetRenderMode(RenderMode::Default);
}

void Renderer::SetClearColor(glm::vec3 color) {
	GLCall(glClearColor(color.r, color.g, color.b, 1.0));
}

void Renderer::BeginRender() {
	GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
}

void Renderer::FinishRender(Window* window) {
	window->SwapBuffers();
}

void Renderer::Render(const VertexArray& vertexArray)
{
	vertexArray.Bind();
	switch (vertexArray.GetPrimitiveType()) {
	case PrimitiveType::Triangles:
		GLCall(glDrawElements(GL_TRIANGLES, vertexArray.GetIndexCount(), GL_UNSIGNED_INT, 0));
		break;
	case PrimitiveType::Lines:
		throw std::runtime_error("todo");
		break;
	case PrimitiveType::Points:
		throw std::runtime_error("todo");
		break;
	}
}
void Renderer::SetRenderMode(RenderMode mode)
{
	if (m_RenderMode != mode) {
		m_RenderMode = mode;
		if (m_Program) delete m_Program;
		std::vector<Shader*> shaders;
		for (const auto& pair : GetShaderPaths(mode)) {
			//Shader shader(pair.first, pair.second);
			shaders.push_back(new Shader(pair.first, pair.second));
			//shaders.push_back(Shader(pair.first, pair.second));
		}
		m_Program = new ShaderProgram(shaders);
		for (Shader* shader : shaders) delete shader;
	}
}

std::vector<std::pair<const char*, ShaderType>> Renderer::GetShaderPaths(RenderMode mode)
{
	switch (mode) {
	case RenderMode::Default:
		return {
			{"src/render/shaders/default.vert", ShaderType::VertexShader},
			{"src/render/shaders/default.frag", ShaderType::FragmentShader}
		};
	default:
		throw std::runtime_error("Unimplemented Render Mode");
	}
}

