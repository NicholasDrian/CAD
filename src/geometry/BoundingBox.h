#pragma once

#include "glm/glm.hpp"
#include "Frustum.h"

#include <algorithm>
#include <vector>

class AxisAlignedBoundingBox {

public:

	AxisAlignedBoundingBox();
	AxisAlignedBoundingBox(float minx, float maxx, float miny, float maxy, float minz, float maxz);
	AxisAlignedBoundingBox(const std::vector<glm::vec3>& points);
	AxisAlignedBoundingBox(const std::vector<glm::vec3>& points, const glm::mat4& t);

	bool Vaid() const;

	void AddPoint(const glm::vec3& p);
	void AddPoints(const std::vector<glm::vec3>& points);
	void AddPoints(const std::vector<glm::vec3>& points, const glm::mat4& t);
	glm::vec3 GetCenter() const;

	bool PartiallyWithin(const Frustum& frustum) const;
	bool FullyWithin(const Frustum& frustum) const;

	friend AxisAlignedBoundingBox operator+(const AxisAlignedBoundingBox& b1, const AxisAlignedBoundingBox& b2)
	{
		return AxisAlignedBoundingBox(
			std::min(b1.MinX(), b2.MinX()),
			std::max(b1.MaxX(), b2.MaxX()),
			std::min(b1.MinY(), b2.MinY()),
			std::max(b1.MaxY(), b2.MaxY()),
			std::min(b1.MinZ(), b2.MinZ()),
			std::max(b1.MaxZ(), b2.MaxZ())
		);
	}

	AxisAlignedBoundingBox& operator+=(const AxisAlignedBoundingBox& b)
	{
		m_MinX = std::min(m_MinX, b.MinX());
		m_MaxX = std::max(m_MaxX, b.MaxX());
		m_MinY = std::min(m_MinY, b.MinY());
		m_MaxY = std::max(m_MaxY, b.MaxY());
		m_MinZ = std::min(m_MinZ, b.MinZ());
		m_MaxZ = std::max(m_MaxZ, b.MaxZ());
		return *this;
	}

	inline float MinX() const { return m_MinX; }
	inline float MinY() const { return m_MinY; }
	inline float MinZ() const { return m_MinZ; }
	inline float MaxX() const { return m_MaxX; }
	inline float MaxY() const { return m_MaxY; }
	inline float MaxZ() const { return m_MaxZ; }

private:

	float m_MinX, m_MaxX, m_MinY, m_MaxY, m_MinZ, m_MaxZ;

};