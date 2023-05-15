#pragma once 

#include "imgui.h"

class GUI {

public:

	static void Init();
	static void Destroy();

	static void BeginRender();
	static void Render();
	static void EndRender();

	inline static ImGuiIO& GetIO() { return ImGui::GetIO(); }

private:

	inline static bool m_ShowSettings = true;
	inline static bool m_ShowCommandInfo = true;
	inline static bool m_ShowDetails = true;

	inline static ImGuiContext* m_Context;

};