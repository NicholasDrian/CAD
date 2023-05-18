#pragma once

#include "Ray.h"
#include "glm/gtx/intersect.hpp"

#include "../debug/Print.h"

Ray::Ray(glm::vec3 origin, glm::vec3 direction)
	:m_Origin(origin), m_Direction(glm::normalize(direction)) 
{

}

glm::vec3 Ray::At(float t) const {
	return m_Origin + m_Direction * t;
}

bool Ray::IntersectPlane(glm::vec3 origin, glm::vec3 normal, float& outTime, bool allowNegativeT) const 
{
	float denominator = glm::dot(normal, m_Direction);
	if (denominator == 0.0f) return false;
	outTime = glm::dot(origin - m_Origin, normal) / denominator;
	return allowNegativeT || outTime >= 0.0f;
}

bool Ray::IntersectPlane(glm::vec3 origin, glm::vec3 normal, glm::vec3& outPoint, bool allowNegativeT) const
{
	float outTime;
	bool res = IntersectPlane(origin, normal, outTime, allowNegativeT);
	outPoint = At(outTime);
	return res;
}

bool Ray::IntersectPlane(const glm::vec3& p1, const glm::vec3& p2, const glm::vec3& p3, float& outTime, bool allowNegativeT) const
{
	glm::vec3 normal = glm::normalize(glm::cross(p2 - p1, p3 - p1));
	float denominator = glm::dot(normal, m_Direction);
	if (denominator == 0.0f) return false;
	outTime = glm::dot(p1 - m_Origin, normal) / denominator;
	return allowNegativeT || outTime >= 0.0f;
}

bool Ray::IntersectPlane(const glm::vec3& p1, const glm::vec3& p2, const glm::vec3& p3, glm::vec3& outPoint, bool allowNegativeT) const
{
	float outTime;
	bool res = IntersectPlane(p1, p2, p3, outTime, allowNegativeT);
	outPoint = At(outTime);
	return res;
}

bool Ray::IntersectTriangle(const glm::vec3& p1, const glm::vec3& p2, const glm::vec3& p3, glm::vec3& outPoint, bool allowNegativeT) const
{
	glm::vec3 normal = glm::normalize(glm::cross(p2 - p1, p3 - p1));
	float denominator = glm::dot(normal, m_Direction);
	if (denominator == 0.0f) return false;
	float outTime = glm::dot(p1 - m_Origin, normal) / denominator;
	if (!allowNegativeT && outTime < 0.0f) return false;

	outPoint = At(outTime);
	float d1 = glm::dot(normal, glm::cross(outPoint - p1, p2 - p1));
	float d2 = glm::dot(normal, glm::cross(outPoint - p2, p3 - p2));
	float d3 = glm::dot(normal, glm::cross(outPoint - p3, p1 - p3));

	// remove one of these lines for back face culling.
	return 
		d1 > 0.0f && d2 > 0.0f && d3 > 0.0f || 
		d1 < 0.0f && d2 < 0.0f && d3 < 0.0f;
}

bool Ray::IntersectTriangle(const glm::vec3& p1, const glm::vec3& p2, const glm::vec3& p3, float& outTime, bool allowNegativeT) const
{
	glm::vec3 normal = glm::normalize(glm::cross(p2 - p1, p3 - p1));
	float denominator = glm::dot(normal, m_Direction);
	if (denominator == 0.0f) return false;
	outTime = glm::dot(p1 - m_Origin, normal) / denominator;
	if (!allowNegativeT && outTime < 0.0f) return false;

	glm::vec3 p = At(outTime);

	glm::vec3 v1 = p - p1;
	glm::vec3 v2 = p - p2;
	glm::vec3 v3 = p - p3;
	glm::vec3 e1 = p2 - p1;
	glm::vec3 e2 = p3 - p2;
	glm::vec3 e3 = p1 - p3;

	float d1 = glm::dot(normal, glm::cross(v1, e1));
	float d2 = glm::dot(normal, glm::cross(v2, e2));
	float d3 = glm::dot(normal, glm::cross(v3, e3));

	// remove one of these lines for back face culling.
	return
		d1 > 0.0f && d2 > 0.0f && d3 > 0.0f ||
		d1 < 0.0f && d2 < 0.0f && d3 < 0.0f;
}

bool Ray::IntersectBoundingBox(const AxisAlignedBoundingBox& bb, glm::vec3& outPoint) const
{
	//print(bb);
	//print(*this);
	

	// test!!!
	glm::vec3 a = { bb.MinX(), bb.MinY(), bb.MinZ() };
	glm::vec3 b = { bb.MaxX(), bb.MaxY(), bb.MaxZ() };

	//print(a); print(b, true);

	if (m_Direction.x < 0.0f) std::swap(a.x, b.x);
	if (m_Direction.y < 0.0f) std::swap(a.y, b.y);
	if (m_Direction.z < 0.0f) std::swap(a.z, b.z);
	glm::vec3 trash;
	float txmin, tymin, tzmin, txmax, tymax, tzmax;
	bool bxmin = IntersectPlane(a, { 1.0f, 0.0f, 0.0f }, txmin, true);
	bool bymin = IntersectPlane(a, { 0.0f, 1.0f, 0.0f }, tymin, true);
	bool bzmin = IntersectPlane(a, { 0.0f, 0.0f, 1.0f }, tzmin, true);
	bool bxmax = IntersectPlane(b, { 1.0f, 0.0f, 0.0f }, txmax, true);
	bool bymax = IntersectPlane(b, { 0.0f, 1.0f, 0.0f }, tymax, true);
	bool bzmax = IntersectPlane(b, { 0.0f, 0.0f, 1.0f }, tzmax, true);

	float end = std::min(txmax, std::min(tymax, tzmax));
	float start = std::max(txmin, std::max(tymin, tzmin));
	//std::cout << "start" << start << "end" << end << std::endl;
	if (end < 0.0f || start > end) return false;

	outPoint = (start >= 0.0f) ? At(start) : At(end);
	//print("true!\n");
	return true;
}

glm::vec3 Ray::ClosestPointOnLine(const glm::vec3& startP, const glm::vec3& endP) const
{
	glm::vec3 a = m_Direction;
	glm::vec3 b = glm::normalize(endP - startP);
	glm::vec3 B = startP;
	if (a == b) return B;

	glm::vec3 A = m_Origin;
	glm::vec3 c = B - A;

	float ab = glm::dot(a, b);
	float ac = glm::dot(a, c);
	float bc = glm::dot(b, c);
	float aa = glm::dot(a, a);
	float bb = glm::dot(b, b);

	return B + b * ((ab * ac - bc * aa) / (aa * bb - ab * ab));
}

glm::vec3 Ray::ClosestPointOnLine(const glm::vec3& startP, const glm::vec3& endP, float& outT, float& outDistance) const
{
	glm::vec3 a = m_Direction;
	glm::vec3 b = glm::normalize(endP - startP);
	glm::vec3 B = startP;
	if (a == b) return B;

	glm::vec3 A = m_Origin;
	glm::vec3 c = B - A;

	float ab = glm::dot(a, b);
	float ac = glm::dot(a, c);
	float bc = glm::dot(b, c);
	float aa = glm::dot(a, a);
	float bb = glm::dot(b, b);

	float denom = aa * bb - ab * ab;

	if (denom == 0) throw std::runtime_error("Your fired!");

	outT = (ab * bc + ac * bb) / denom;

	float timeLine = ((ab * ac - bc * aa) / denom);


	glm::vec3 res = (timeLine < 0) ? startP : (timeLine > glm::distance(startP, endP)) ? endP : B + b * timeLine;
	glm::vec3 other = A + a * outT;

	outDistance = glm::distance(res, other);
	//print(outDistance, true);
	return res;
}
