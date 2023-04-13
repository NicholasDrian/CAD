#pragma once

#include "../scene/Scene.h"

#include "imgui.h"
#include "ImGuizmo/ImGuizmo.h"

class AffineTransformWidget {

public:

	static void test() {
		ImGuiIO& io = ImGui::GetIO();
		ImGuizmo::SetRect(0, 0, io.DisplaySize.x, io.DisplaySize.y);
		const float* view = &Scene::GetCamera()->GetView()[0][0];
		const float* proj = &Scene::GetCamera()->GetProj()[0][0];
		glm::mat4 mat{
			{1,0,0,0},
			{0,1,0,0},
			{0,0,1,0},
			{0,0,0,1}
		};
		ImGuizmo::OPERATION opp = ImGuizmo::SCALE | ImGuizmo::ROTATE_X | ImGuizmo::ROTATE_Y | ImGuizmo::ROTATE_Z | ImGuizmo::TRANSLATE;
		ImGuizmo::Manipulate(view, proj, opp, ImGuizmo::WORLD, &mat[0][0], NULL, NULL);
	}

private:


};