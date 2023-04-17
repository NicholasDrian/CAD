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

	static uint64_t ReadIDAtPixel(int x, int y);

	static void WindowResize(int width, int height);


	// test
	static void UnbindIDBuffer();
	static void BindIDBuffer();

private:

	static void InitFrameBuffer();
	static void DestroyFrameBuffer();

	inline static RenderMode m_RenderMode  = RenderMode::None;
	inline static GLuint m_FrameBuffer = 0, m_ColorAttachment = 0, m_IDAttachment = 0, m_DepthAttachment = 0, m_SubIDAttachment = 0;
	inline static GLint m_Width = -1, m_Height = -1;

};