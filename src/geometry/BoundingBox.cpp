#pragma once

#include "BoundingBox.h"

#include <limits>

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
	for (const glm::vec3& point : points) AddPoint(t * glm::vec4(point, 1.0));
}