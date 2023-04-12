#pragma once

#include "Renderer.h"
#include "shaders/Shader.h"
#include "../Window.h"
#include "../scene/Scene.h"
#include "shaders/ShaderManager.h"

#include "GL/glew.h"
#include "glm/glm.hpp"

#include <stdexcept>
#include <utility>

#include "iostream"

void Renderer::Init()
{

	std::pair<unsigned, unsigned> size = Window::GetSize();
	m_Width = size.first;
	m_Height = size.second;

	if (glewInit() != GLEW_OK) {
		throw std::runtime_error("Failed to initialize glew!");
	}


	ShaderManager::Init();
	SetRenderMode(RenderMode::Default);
	InitFrameBuffer();
	SetClearColor({ 0.7, 0.7, 0.8 });

	GLCall(glEnable(GL_DEPTH_TEST));
	GLCall(glEnable(GL_POINT_SMOOTH));
	GLCall(glPointSize(3.0));
}

void Renderer::SetClearColor(glm::vec3 color) {
	GLCall(glClearColor(color.r, color.g, color.b, 1.0));
}

void Renderer::BeginRender() {
	GLCall(glBindFramebuffer(GL_FRAMEBUFFER, m_FrameBuffer));
	GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

	int val = 0;
	GLCall(glClearBufferiv(GL_COLOR, 1, &val));

	ShaderManager::UpdateGlobalUniforms();
}

void Renderer::FinishRender() {
	GLCall(glBindFramebuffer(GL_READ_FRAMEBUFFER, m_FrameBuffer));
	GLCall(glReadBuffer(GL_COLOR_ATTACHMENT0));
	GLCall(glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0));
	GLCall(glBlitFramebuffer(0, 0, m_Width, m_Height, 0, 0, m_Width, m_Height, GL_COLOR_BUFFER_BIT, GL_NEAREST));
	Window::SwapBuffers();
}

void Renderer::SetRenderMode(RenderMode mode)
{
	if (m_RenderMode != mode) {
		m_RenderMode = mode;
		//TODO
	}
}

void Renderer::InitFrameBuffer()
{
	GLCall(glGenFramebuffers(1, &m_FrameBuffer));
	GLCall(glBindFramebuffer(GL_FRAMEBUFFER, m_FrameBuffer));

	auto createBuffer = [&](GLuint& id, int internalFormat, int format, int type, int attachment) {
		GLCall(glGenTextures(1, &id));
		GLCall(glBindTexture(GL_TEXTURE_2D, id));
		GLCall(glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, m_Width, m_Height, 0, format, type, NULL));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
		GLCall(glFramebufferTexture2D(GL_FRAMEBUFFER, attachment, GL_TEXTURE_2D, id, 0));
	};

	createBuffer(m_ColorAttachment, GL_RGB, GL_RGB, GL_UNSIGNED_BYTE, GL_COLOR_ATTACHMENT0);
	createBuffer(m_IDAttachment, GL_R32I, GL_RED_INTEGER, GL_INT, GL_COLOR_ATTACHMENT1);
	createBuffer(m_SubIDAttachment, GL_R32I, GL_RED_INTEGER, GL_INT, GL_COLOR_ATTACHMENT2);
	createBuffer(m_DepthAttachment, GL_DEPTH_COMPONENT32F, GL_DEPTH_COMPONENT, GL_FLOAT, GL_DEPTH_ATTACHMENT);

	GLenum DrawBuffers[] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 };
	GLCall(glDrawBuffers(2, DrawBuffers));

#ifdef CAD_DEBUG
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		throw std::runtime_error("Incomplete frame buffer!");
#endif

}

void Renderer::WindowResize(int width, int height) {
	m_Width = width; m_Height = height;
	DestroyFrameBuffer();
	InitFrameBuffer();
	glViewport(0, 0, m_Width, m_Height);
}

void Renderer::Destroy() {
	DestroyFrameBuffer();
	ShaderManager::Destroy();
}

void Renderer::DestroyFrameBuffer()
{
	const auto destroy = [](GLuint& id) {
		if (id) {
			GLCall(glDeleteTextures(1, &id));
			id = 0;
		}
	};
	destroy(m_ColorAttachment);
	destroy(m_IDAttachment);
	destroy(m_SubIDAttachment);
	destroy(m_DepthAttachment);
	destroy(m_FrameBuffer);
}


int Renderer::ReadIDAtPixel(int x, int y) {
	GLCall(glBindFramebuffer(GL_FRAMEBUFFER, m_FrameBuffer));
	GLCall(glReadBuffer(GL_COLOR_ATTACHMENT1));
	int val;
	GLCall(glReadPixels(x, m_Height - y, 1, 1, GL_RED_INTEGER, GL_INT, &val));
	return val;
}


float Renderer::ReadDistanceAtPixel(int x, int y) {
	GLCall(glBindFramebuffer(GL_FRAMEBUFFER, m_FrameBuffer));
	float depth;
	GLCall(glReadPixels(x, m_Height - y, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &depth));

	const float zNorm = 2.0f * depth - 1.0f;
	const float near = Camera::GetNearPlane();
	const float far = Camera::GetFarPlane();
	const float distance = 2.0f * near * far / (far + near - zNorm * (far - near));

	return distance;
}