#pragma once

#include "AffineTransformWidget.h"
#include "../Window.h"
#include "../scene/Scene.h"

#include "imgui.h"
#include "ImGuizmo/ImGuizmo.h"

#include "glm/gtx/transform.hpp"
#include "glm/gtx/matrix_decompose.hpp"

namespace ImGuizmo {
	const OPERATION OPP = SCALE | ROTATE_X | ROTATE_Y | ROTATE_Z | TRANSLATE;
}

AffineTransformWidget::AffineTransformWidget(const AxisAlignedBoundingBox& bb)
{
	m_Transform = glm::translate(bb.GetCenter());
	m_InitialTransform = m_Transform;
}

glm::mat4 AffineTransformWidget::GetDelta() const 
{ 
	return m_Transform * glm::inverse(m_InitialTransform);
}

void AffineTransformWidget::Draw() {

	auto [sizex, sizey] = Window::GetSize();
	auto [posx, posy] = Window::GetPos();
	ImGuizmo::SetRect((float)posx, (float)posy, (float)sizex, (float)sizey);

	const float* view = &Scene::GetCamera()->GetView()[0][0];
	const float* proj = &Scene::GetCamera()->GetProj()[0][0];
	float* mat = &m_Transform[0][0];
	
	ImGuizmo::Manipulate(view, proj, ImGuizmo::OPP, ImGuizmo::LOCAL, mat);
}