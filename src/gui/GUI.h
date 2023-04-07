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

};