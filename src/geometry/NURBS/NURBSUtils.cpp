#pragma once

#include "NURBSUtils.h"


int NURBSUtils::Span(const std::vector<float>& knots, float u, int p)
{
	// improved algo from NURBS Bible!
	int n = (int)knots.size() - p - 2;
	int l = p, h = n, m;
	while (l < h) {
		m = (l + h) / 2;
		if (u >= knots[m + 1]) l = m + 1;
		else h = m;
	}
	return l;
}

std::vector<float> NURBSUtils::BasisFuncs(const std::vector<float>& knots, float u, int p)
{
	int i = Span(knots, u, p);
	std::vector<float> res(p + 1);
	res[0] = 1.0f;
	auto left = [&](int j) { return u - knots[i - j + 1]; };
	auto right = [&](int j) { return knots[i + j] - u; };
	for (int j = 1; j <= p; j++) {
		float saved = 0.0f;
		for (int r = 0; r < j; r++) {
			float temp = res[r] / (right(r + 1U) + left(j - r));
			res[r] = saved + right(r + 1U) * temp;
			saved = left(j - r) * temp;
		}
		res[j] = saved;
	}
	return res;
}

std::vector<float> NURBSUtils::GenericKnotVector(int pointCount, int degree)
{
	std::vector<float> res;
	for (int i = 0; i <= degree; i++) res.push_back(0.0f);
	for (float i = 1.0f; i < pointCount - degree; i++)  res.push_back(i);
	for (int i = 0; i <= degree; i++) res.push_back((float)pointCount - (float)degree);
	return res;
}
