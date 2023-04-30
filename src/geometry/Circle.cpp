#pragma once

#include "Circle.h"
#include "../debug/Print.h"

#include "glm/gtc/constants.hpp"

const unsigned UNIT_CIRCLE_DEGREE = 2;
const std::vector<float> UNIT_CIRCLE_KNOTS {
	0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 2.0f, 2.0f, 3.0f, 3.0f, 4.0f, 4.0f, 4.0f
};
const std::vector<glm::vec3> UNIT_CIRCLE_POINTS
{
	{  1.0f,  0.0f, 0.0f }, {  1.0f,  1.0f, 0.0f }, {  0.0f,  1.0f, 0.0f }, 
	{ -1.0f,  1.0f, 0.0f }, { -1.0f,  0.0f, 0.0f }, { -1.0f, -1.0f, 0.0f },
	{  0.0f, -1.0f, 0.0f }, {  1.0f, -1.0f, 0.0f }, {  1.0f,  0.0f, 0.0f } 
};
const std::vector<float> UNIT_CIRCLE_WEIGHTS {
	1.0f, glm::root_two<float>() / 2.0f, 1.0f, glm::root_two<float>() / 2.0f, 
	1.0f, glm::root_two<float>() / 2.0f, 1.0f, glm::root_two<float>() / 2.0f, 1.0f
};

Circle::Circle(glm::vec3 a, glm::vec3 b, glm::vec3 c)
	: NURBS(UNIT_CIRCLE_POINTS, { 0,0,0,1 }, UNIT_CIRCLE_WEIGHTS, UNIT_CIRCLE_DEGREE, UNIT_CIRCLE_KNOTS)
{
	glm::vec3 ab = b - a, ac = c - a;
	glm::vec3 normal = glm::normalize(glm::cross(ab, ac));

	glm::vec3 ro = (a + b) / 2.0f;
	glm::vec3 rd = glm::normalize(glm::cross(ab, normal));
	Ray r(ro, rd);

	glm::vec3 po = (a + c) / 2.0f;
	glm::vec3 pn = glm::normalize(ac);
	glm::vec3 center;
	r.IntersectPlane(po, pn, center, true);

	float radious = glm::distance(a, center);

	Create(normal, center, radious);
}

Circle::Circle(glm::vec3 normal, glm::vec3 center, float radious)
	: NURBS(UNIT_CIRCLE_POINTS, { 0,0,0,1 }, UNIT_CIRCLE_WEIGHTS, UNIT_CIRCLE_DEGREE, UNIT_CIRCLE_KNOTS)
{
	Create(normal, center, radious);
}

void Circle::Create(glm::vec3 normal, glm::vec3 center, float radious)
{
	glm::vec3 x, y;
	if (normal.z < 0.0f) normal *= -1;
	if (glm::distance(normal, glm::vec3{ 0.0f, 0.0f, 1.0f}) < 0.00001f)
		x = glm::vec3{ 1.0f, 0.0f, 0.0f } *radious,
		y = glm::vec3{ 0.0f, 1.0f, 0.0f } *radious;
	else
		x = glm::normalize(glm::cross(normal, { 0.0f, 0.0f, 1.0f })) * radious,
		y = glm::normalize(glm::cross(normal, x)) * radious;

	normal *= radious;

	m_Model = {
		x.x,		x.y,		x.z,		0.0f,
		y.x,		y.y,		y.z,		0.0f,
		normal.x,	normal.y, 	normal.z,	0.0f,
		center.x,	center.y,	center.z,	1.0f,
	};

	m_ControlPolyLine->SetModel(m_Model);
}
