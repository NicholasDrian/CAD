#pragma once

//#include "shaders/ShaderProgram.h"
#include "shaders/ShaderManager.h"
#include "VertexArray.h"

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
	static void Render(VertexArray* vertexArray);
	static void SetRenderMode(RenderMode mode);

	static void WindowResize(int width, int height);

	static std::pair<const char*, const char*> GetShaderPaths();

private:

	static void InitFrameBuffer();
	static void DestroyFrameBuffer();

	static RenderMode m_RenderMode;
	static ShaderManager* m_ShaderManager;

	static GLuint m_FrameBuffer;
	static GLuint m_ColorAttachment;
	static GLuint m_IDAttachment;
	static GLint m_Width, m_Height;

};