#pragma once

#include "ShaderProgram.h"
#include "VertexArray.h"

#include "glm/glm.hpp"

#include <vector>

class Window;

enum class RenderMode {
	None,
	Default
};

class Renderer {

public:

	static void InitRenderer();
	static void DestroyRenderer();

	static void SetClearColor(glm::vec3 color);
	static void BeginRender();
	static void FinishRender(Window* window);
	static void Render(const VertexArray& vertexArray);
	static void SetRenderMode(RenderMode mode);

	static std::vector<std::pair<const char*, ShaderType>> GetShaderPaths(RenderMode mode);

private:

	static RenderMode m_RenderMode;
	static ShaderProgram* m_Program;

};