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

	static int ReadIDAtPixel(int x, int y);
	static float ReadDistanceAtPixel(int x, int y);

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