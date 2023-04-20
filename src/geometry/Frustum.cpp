#pragma once

#include "Frustum.h"

Frustum::Frustum(const Ray& tl, const Ray& br)
	: m_Origin(tl.GetOrigin())
{
	glm::vec3 up = { 0.0,0.0,1.0 };
	float deltaVert = tl.GetDirection().z - br.GetDirection().z;
	glm::vec3 tr = br.GetDirection() + glm::vec3{ 0.0, 0.0, deltaVert };
	glm::vec3 deltaHor = tr - tl.GetDirection();
	glm::vec3 bl = br.GetDirection() - deltaHor;

	m_N1 = glm::cross(tl.GetDirection(), tr);
	m_N2 = glm::cross(tr, br.GetDirection());
	m_N3 = glm::cross(br.GetDirection(), bl);
	m_N4 = glm::cross(bl, tl.GetDirection());
}

bool Frustum::Contains(glm::vec3 p) const
{
	glm::vec3 v = p - m_Origin;
	return
		glm::dot(m_N1, v) > 0.0f &&
		glm::dot(m_N2, v) > 0.0f &&
		glm::dot(m_N3, v) > 0.0f &&
		glm::dot(m_N4, v) > 0.0f;
}
