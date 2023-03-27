#pragma once

#include "shaders/ShaderManager.h"
#include "VertexArray.h"
#include "OpenGLUtils.h"

#include "glm/glm.hpp"

class Window;

enum class RenderMode {
	None,
	Default
};

class Renderer {

public:

	static void Init();
	static void Destroy();

	static void SetClearColor(glm::vec3 color);
	static void BeginRender();
	static void FinishRender();
	static void SetRenderMode(RenderMode mode);

	// Must be inline to be used in callback. not sure why
	inline static int ReadFrameBufferAtPixel(int x, int y) {
		GLCall(glBindFramebuffer(GL_FRAMEBUFFER, m_FrameBuffer));
		GLCall(glReadBuffer(GL_COLOR_ATTACHMENT1));
		int val;
		GLCall(glReadPixels(x, m_Height - y, 1, 1, GL_RED_INTEGER, GL_INT, &val));
		return val;
	}

	static void WindowResize(int width, int height);

private:

	static void InitFrameBuffer();
	static void DestroyFrameBuffer();

	inline static RenderMode m_RenderMode  = RenderMode::None;
	inline static GLuint m_FrameBuffer = 0;
	inline static GLuint m_ColorAttachment = 0;
	inline static GLuint m_IDAttachment = 0;
	inline static GLuint m_DepthAttachment = 0;
	inline static GLint m_Width = -1, m_Height = -1;

};