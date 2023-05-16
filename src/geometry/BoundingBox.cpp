#pragma once

#include "BoundingBox.h"

#include "../debug/Print.h"

#include <limits>
#include <stdexcept>

#define INF std::numeric_limits<float>::infinity()
#define NEG_INF -std::numeric_limits<float>::infinity()


AxisAlignedBoundingBox::AxisAlignedBoundingBox() 
	: m_MinX(INF), m_MaxX(NEG_INF), m_MinY(INF), m_MaxY(NEG_INF), m_MinZ(INF), m_MaxZ(NEG_INF)
{

}

AxisAlignedBoundingBox::AxisAlignedBoundingBox(float minx, float maxx, float miny, float maxy, float minz, float maxz) 
	: m_MinX(minx), m_MaxX(maxx), m_MinY(miny), m_MaxY(maxy), m_MinZ(minz), m_MaxZ(maxz)
{

}


AxisAlignedBoundingBox::AxisAlignedBoundingBox(const std::vector<glm::vec3>& points, const glm::mat4& t)
	: AxisAlignedBoundingBox()
{
	AddPoints(points, t);
}

AxisAlignedBoundingBox::AxisAlignedBoundingBox(const std::vector<glm::vec4>& points, const glm::mat4& t) 
	: AxisAlignedBoundingBox()
{
	for (const auto& p : points) {
		glm::vec4 point = t * p;
		AddPoint(glm::vec3{point.x, point.y, point.z} / point.w);
	}
}

bool AxisAlignedBoundingBox::Vaid() const
{
	return
		m_MinX <= m_MaxX &&
		m_MinY <= m_MaxY &&
		m_MinZ <= m_MaxZ;
}

AxisAlignedBoundingBox::AxisAlignedBoundingBox(const std::vector<glm::vec3>& points)
	: AxisAlignedBoundingBox()
{
	AddPoints(points);
}

glm::vec3 AxisAlignedBoundingBox::GetCenter() const
{
	return {
		(m_MinX + m_MaxX) / 2.0f,
		(m_MinY + m_MaxY) / 2.0f,
		(m_MinZ + m_MaxZ) / 2.0f
	};
}

bool AxisAlignedBoundingBox::PartiallyWithin(const Frustum& frustum) const
{
	// return 
	//		at least one point of bb within frustum ||
	//		at least one ray of frustum within bb...

	throw std::runtime_error("not implemented");
}

bool AxisAlignedBoundingBox::FullyWithin(const Frustum& frustum) const
{
	return
		frustum.Contains({ m_MinX, m_MinY, m_MinZ }) &&
		frustum.Contains({ m_MinX, m_MinY, m_MaxZ }) &&
		frustum.Contains({ m_MinX, m_MaxY, m_MinZ }) &&
		frustum.Contains({ m_MinX, m_MaxY, m_MaxZ }) &&
		frustum.Contains({ m_MaxX, m_MinY, m_MinZ }) &&
		frustum.Contains({ m_MaxX, m_MinY, m_MaxZ }) &&
		frustum.Contains({ m_MaxX, m_MaxY, m_MinZ }) &&
		frustum.Contains({ m_MaxX, m_MaxY, m_MaxZ });
}


void AxisAlignedBoundingBox::AddPoint(const glm::vec3& p)
{
	m_MinX = std::min(m_MinX, p.x);
	m_MaxX = std::max(m_MaxX, p.x);
	m_MinY = std::min(m_MinY, p.y);
	m_MaxY = std::max(m_MaxY, p.y);
	m_MinZ = std::min(m_MinZ, p.z);
	m_MaxZ = std::max(m_MaxZ, p.z);
}
void AxisAlignedBoundingBox::AddPoints(const std::vector<glm::vec3>& points)
{
	for (const glm::vec3& point : points) AddPoint(point);
}


void  AxisAlignedBoundingBox::AddPoints(const std::vector<glm::vec3>& points, const glm::mat4& t)
{
	for (const glm::vec3& point : points) {
		glm::vec4 p = t * glm::vec4(point, 1.0);
		AddPoint(glm::vec3{p.x, p.y, p.z} / p.w);
	}
}

