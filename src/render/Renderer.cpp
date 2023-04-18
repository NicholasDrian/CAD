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
#include <iostream>

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
	GLCall(glPointSize(5.0));

	GLCall(glEnable(GL_BLEND));
	GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

	GLCall(glProvokingVertex(GL_FIRST_VERTEX_CONVENTION));

	// todo: glEnable(GL_MULTISAMPLE);

	const GLubyte* renderer = glGetString(GL_RENDERER);
	const GLubyte* vendor = glGetString(GL_VENDOR);
	const GLubyte* version = glGetString(GL_VERSION);
	const GLubyte* glslVersion =
		glGetString(GL_SHADING_LANGUAGE_VERSION);

	GLint major, minor;
	glGetIntegerv(GL_MAJOR_VERSION, &major);
	glGetIntegerv(GL_MINOR_VERSION, &minor);

	printf("GL Vendor            : %s\n", vendor);
	printf("GL Renderer          : %s\n", renderer);
	printf("GL Version (string)  : %s\n", version);
	printf("GL Version (integer) : %d.%d\n", major, minor);
	printf("GLSL Version         : %s\n", glslVersion);
}

void Renderer::SetClearColor(glm::vec3 color) {
	GLCall(glClearColor(color.r, color.g, color.b, 1.0));
}

void Renderer::BeginRender() {

	auto [x,y] = Window::GetSize();
	if (x != m_Width || y != m_Height) {
		WindowResize(x, y);
	}

	GLCall(glBindFramebuffer(GL_FRAMEBUFFER, m_FrameBuffer));
	GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

	GLuint val[] = { 0, 0 };
	GLCall(glClearBufferuiv(GL_COLOR, 1, val));

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

void Renderer::UnbindIDBuffer()
{
	GLCall(glBindFramebuffer(GL_FRAMEBUFFER, m_FrameBuffer));
	GLenum DrawBuffers[] = { GL_COLOR_ATTACHMENT0 };
	GLCall(glDrawBuffers(1, DrawBuffers));
}
void Renderer::BindIDBuffer()
{
	GLCall(glBindFramebuffer(GL_FRAMEBUFFER, m_FrameBuffer));
	GLenum DrawBuffers[] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };
	GLCall(glDrawBuffers(2, DrawBuffers));
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
	createBuffer(m_IDAttachment, GL_RG32I, GL_RG_INTEGER, GL_INT, GL_COLOR_ATTACHMENT1);
	createBuffer(m_DepthAttachment, GL_DEPTH_COMPONENT32F, GL_DEPTH_COMPONENT, GL_FLOAT, GL_DEPTH_ATTACHMENT);

	GLenum DrawBuffers[] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };
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


uint64_t Renderer::ReadIDAtPixel(int x, int y) {
	GLCall(glBindFramebuffer(GL_FRAMEBUFFER, m_FrameBuffer));
	GLCall(glReadBuffer(GL_COLOR_ATTACHMENT1));
	uint64_t val;
	GLCall(glReadPixels(x, m_Height - y, 1, 1, GL_RG_INTEGER, GL_INT, &val));
	return val;
}
