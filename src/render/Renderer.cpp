#pragma once

#include "Renderer.h"
#include "Shader.h"
#include "../Window.h"
#include "../utils/OpenGLUtils.h"
#include "../scene/Scene.h"

#include "GL/glew.h"
#include "glm/glm.hpp"

#include <stdexcept>
#include <utility>

#include "iostream"

ShaderProgram* Renderer::m_Program = nullptr;
RenderMode Renderer::m_RenderMode = RenderMode::None;

GLuint Renderer::m_FrameBuffer = 0;
GLuint Renderer::m_ColorAttachment = 0;
GLuint Renderer::m_IDAttachment = 0;
GLint Renderer::m_Width, Renderer::m_Height;

GLuint Renderer::m_ViewProjLocation = 0;

void Renderer::Init()
{
	std::pair<unsigned, unsigned> size = Window::GetSize();
	m_Width = size.first;
	m_Height = size.second;

	if (glewInit() != GLEW_OK) {
		throw std::runtime_error("Failed to initialize glew!");
	}
	SetRenderMode(RenderMode::Default);
	InitFrameBuffer();
}

void Renderer::SetClearColor(glm::vec3 color) {
	GLCall(glClearColor(color.r, color.g, color.b, 1.0));
}

void Renderer::BeginRender() {
	GLCall(glBindFramebuffer(GL_FRAMEBUFFER, m_FrameBuffer));
	GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

	GLCall(glUniformMatrix4fv(m_ViewProjLocation, 1, GL_FALSE, &Scene::GetCamera()->GetViewProj()[0][0]));
}

void Renderer::FinishRender() {
	GLCall(glBindFramebuffer(GL_READ_FRAMEBUFFER, m_FrameBuffer));
	GLCall(glReadBuffer(GL_COLOR_ATTACHMENT0));
	GLCall(glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0));
	GLCall(glBlitFramebuffer(0, 0, m_Width, m_Height, 0, 0, m_Width, m_Height, GL_COLOR_BUFFER_BIT, GL_NEAREST));
	Window::SwapBuffers();
}

void Renderer::Render(const VertexArray& vertexArray)
{
	vertexArray.Bind();
	switch (vertexArray.GetPrimitiveType()) {
	case PrimitiveType::Triangles:
		GLCall(glDrawElements(GL_TRIANGLES, (GLsizei)vertexArray.GetIndexCount(), GL_UNSIGNED_INT, 0));
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
		auto [vertPath, fragPath] = GetShaderPaths();
		delete m_Program;
		m_Program = new ShaderProgram(Shader(vertPath, ShaderType::VertexShader), Shader(fragPath, ShaderType::FragmentShader));
		m_Program->Bind();
		m_ViewProjLocation = GLResult(glGetUniformLocation(m_Program->GetID(), "view_proj"));
	}
}

std::pair<const char*, const char*> Renderer::GetShaderPaths()
{
	switch (m_RenderMode) {
	case RenderMode::Default:
		return { "src/render/shaders/default.vert", "src/render/shaders/default.frag" };
	default:
		throw std::runtime_error("Unimplemented Render Mode");
	}
}

void Renderer::InitFrameBuffer()
{
	GLCall(glGenFramebuffers(1, &m_FrameBuffer));
	GLCall(glBindFramebuffer(GL_FRAMEBUFFER, m_FrameBuffer));

	GLCall(glGenTextures(1, &m_ColorAttachment));
	GLCall(glBindTexture(GL_TEXTURE_2D, m_ColorAttachment));
	GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_Width, m_Height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
	GLCall(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_ColorAttachment, 0));

	GLCall(glGenTextures(1, &m_IDAttachment));
	GLCall(glBindTexture(GL_TEXTURE_2D, m_IDAttachment));
	GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_R32I, m_Width, m_Height, 0, GL_RED_INTEGER, GL_INT, NULL));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
	GLCall(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, m_IDAttachment, 0));

	GLenum DrawBuffers[] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };
	GLCall(glDrawBuffers(2, DrawBuffers));

#ifdef CAD_DEBUG
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		throw std::runtime_error("Incomplete frame buffer!");
#endif

	GLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));

}

void Renderer::WindowResize(int width, int height) {
	m_Width = width; m_Height = height;
	DestroyFrameBuffer();
	InitFrameBuffer();
	glViewport(0, 0, m_Width, m_Height);
}

void Renderer::Destroy() {
	DestroyFrameBuffer();
	delete m_Program;
}

void Renderer::DestroyFrameBuffer()
{
	if (m_ColorAttachment) {
		GLCall(glDeleteTextures(1, &m_ColorAttachment));
		m_ColorAttachment = 0;
	}
	if (m_IDAttachment) {
		GLCall(glDeleteTextures(1, &m_IDAttachment));
		m_IDAttachment = 0;
	}
	if (m_FrameBuffer) {
		GLCall(glDeleteFramebuffers(1, &m_FrameBuffer));
		m_FrameBuffer = 0;
	}
}

