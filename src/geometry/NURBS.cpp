#pragma once

#include "NURBS.h"
#include "../render/Renderer.h"
#include "../debug/Print.h"

const unsigned SAMPLES_PER_EDGE = 20;

NURBS::NURBS(std::vector<glm::vec3> points, glm::vec4 color, std::vector<float> weights, unsigned degree, std::vector<float> knots, unsigned id)
	: m_Degree(degree), m_Knots(knots), m_ID(id), m_Color(color), m_Model(1.0f), m_Selected(false),
	m_ControlPolyLine(std::make_unique<PolyLine>(points, true, m_ID)), m_PointsOn(false)
{
	m_ControlPolyLine->PointsOn();
	if (weights.size() == 0) {
		for (const glm::vec3& point : points) m_Points.emplace_back(point.x, point.y, point.z, 1.0f);
	}
	else {
		for (int i = 0; i < points.size(); i++) {
			const glm::vec3& point = points[i];
			m_Points.emplace_back(point.x * weights[i], point.y * weights[i], point.z * weights[i], weights[i]);
		}
	}
	while (m_Degree > m_Points.size() - 1) m_Degree--;
	if (m_Knots.size() == 0) UpdateKnotVector();
	UpdateSamples();
	UpdateVertexArray();
}
	
void NURBS::Render() const
{
	if (!m_Selectable) Renderer::UnbindIDBuffer();
	m_VertexArrayLines->Render(m_Model, m_ID, m_Selectable, m_Selected);
	if (m_PointsOn) m_ControlPolyLine->Render();
	Renderer::BindIDBuffer();
}

void NURBS::BakeSelectionTransform(const glm::mat4& t)
{
	if (m_Selected) {
		m_Model = t * m_Model;
		m_ControlPolyLine->BakeSelectionTransform(t);
	}
	else {	
		m_ControlPolyLine->BakeSelectionTransform(t);
	}
}

void NURBS::SelectionTransformUpdated()
{
	if (!m_Selected && m_ControlPolyLine->IsSubSelected()) {
		std::vector<glm::vec3> newPositions = m_ControlPolyLine->GetControlPoints(true);
		// fix!
		std::vector<glm::vec4> newControlPoints(m_Points.size());
		for (int i = 0; i < m_Points.size(); i++) {
			newControlPoints[i] = glm::vec4(newPositions[i] * m_Points[i].w, m_Points[i].w);
		}
		m_Points = newControlPoints;
		UpdateSamples();
		m_VertexArrayLines->UpdateVertexPositions(m_Samples);
	}
}


void NURBS::Select()
{
	m_Selected = true;
	m_ControlPolyLine->Select();
}

void NURBS::UnSelect()
{
	m_Selected = false;
	m_ControlPolyLine->UnSelect();
}

AxisAlignedBoundingBox NURBS::GetBoundingBox() const
{
	std::vector<glm::vec3> points(m_Points.size());
	for (const glm::vec4& point : m_Points) points.emplace_back(
		point.x / point.w, point.y / point.w, point.z / point.w);
	AxisAlignedBoundingBox bb(points, m_Model);
	if (m_PointsOn) bb += m_ControlPolyLine->GetBoundingBox();
	return bb;
}


AxisAlignedBoundingBox NURBS::GetSubSelectionBoundingBox() const
{
	return m_ControlPolyLine->GetSubSelectionBoundingBox();
}

void NURBS::AddSubSelection(uint32_t subID)
{
	m_ControlPolyLine->AddSubSelection(subID);
}
void NURBS::RemoveSubSelection(uint32_t subID)
{
	m_ControlPolyLine->RemoveSubSelection(subID);
}
void NURBS::ClearSubSelection()
{
	m_ControlPolyLine->ClearSubSelection();
}

void NURBS::SelectWithinFrustum(const Frustum& frustum, bool inclusive)
{
	if (GetBoundingBox().FullyWithin(frustum)) {
		Select();
		return;
	}
	else if (inclusive) {
		for (int i = 0; i < m_Indecies.size() / 2; i++) {
			if (frustum.PartiallyContainsLine(
				m_Model * glm::vec4(m_Samples[m_Indecies[2 * i]], 1.0), 
				m_Model * glm::vec4(m_Samples[m_Indecies[2 * i + 1]], 1.0))) {
				Select();
				return;
			}
		}
	} else {
		bool contained = true;
		for (const glm::vec3& p : m_Samples) {
			if (!frustum.Contains(m_Model * glm::vec4(p, 1.0))) {
				contained = false;
				break;
			}
		}
		if (contained) {
			Select();
			return;
		}
	}
	if (m_PointsOn) m_ControlPolyLine->SelectWithinFrustum(frustum, inclusive);
	m_Selected = m_ControlPolyLine->IsSelected();
}

void NURBS::SubSelectWithinFrustum(const Frustum& frustum, bool inclusive)
{
	if (m_PointsOn) m_ControlPolyLine->SubSelectWithinFrustum(frustum, inclusive);
}

void NURBS::UnSelectWithinFrustum(const Frustum& frustum, bool inclusive)
{
	if (GetBoundingBox().FullyWithin(frustum)) {
		UnSelect();
	}
	else if (inclusive) {
		for (int i = 0; i < m_Indecies.size() / 2; i++) {
			if (frustum.PartiallyContainsLine(
				m_Model * glm::vec4(m_Samples[m_Indecies[2 * i]], 1.0), 
				m_Model * glm::vec4(m_Samples[m_Indecies[2 * i + 1]], 1.0))) {
				UnSelect();
				return;
			}
		}
	}
	else {
		bool contained = true;
		for (const glm::vec3& p : m_Samples) {
			if (!frustum.Contains(m_Model * glm::vec4(p, 1.0))) {
				contained = false;
				return;
			}
		}
		if (contained) {
			UnSelect();
			return;
		}
	}
	if (m_PointsOn) m_ControlPolyLine->UnSelectWithinFrustum(frustum, inclusive);
	m_Selected = m_ControlPolyLine->IsSelected();
}

void NURBS::UnSubSelectWithinFrustum(const Frustum& frustum, bool inclusive)
{
	if (m_PointsOn) m_ControlPolyLine->UnSubSelectWithinFrustum(frustum, inclusive);
}

void NURBS::AddControlPoint(const glm::vec3& point, bool incrementDegree)
{
	m_Points.emplace_back(point.x, point.y, point.z, 1.0f);
	m_Degree += incrementDegree;
	UpdateKnotVector();
	UpdateSamples();
	UpdateVertexArray();
	m_ControlPolyLine->AddPoint(point);
}

void NURBS::ChangeDegree(unsigned degree)
{
	while (degree >= m_Points.size()) degree--;
	m_Degree = degree;
	UpdateKnotVector();
	UpdateSamples();
	m_VertexArrayLines->UpdateVertexPositions(m_Samples);
}

void NURBS::UpdateLastPoint(const glm::vec3& point)
{
	m_Points.back() = {point.x, point.y, point.z, 1.0f};
	UpdateSamples(); // shouldnt need to update all samples
	m_VertexArrayLines->UpdateVertexPositions(m_Samples);
	m_ControlPolyLine->UpdateLast(point);
}

void NURBS::RemoveLastPoint()
{
	m_Points.pop_back();
	if (m_Degree > m_Points.size() - 1) m_Degree--;
	UpdateKnotVector();
	UpdateSamples();
	UpdateVertexArray();
	m_ControlPolyLine->RemoveLast();
}

std::vector<float> NURBS::BasisFuncs(float u) const
{
	int i = KnotSpan(u);
	std::vector<float> res(m_Degree + 1);
	res[0] = 1.0f;
	auto left = [&](int j) { return u - m_Knots[i - j + 1]; };
	auto right = [&](int j) { return m_Knots[i + j] - u; };
	for (unsigned j = 1; j <= m_Degree; j++) {
		float saved = 0.0f;
		for (unsigned r = 0; r < j; r++) {
			float temp = res[r] / (right(r + 1U) + left(j - r));
			res[r] = saved + right(r + 1U) * temp;
			saved = left(j - r) * temp;
		}
		res[j] = saved;
	}
	return res;
}

int NURBS::KnotSpan(float u) const
{
	// improved algo from NURBS Bible!
	int n = (int)m_Knots.size() - m_Degree - 2;
	int l = m_Degree, h = n, m;
	while (l < h) {
		m = (l + h) / 2;
		if (u >= m_Knots[m + 1]) l = m + 1;
		else h = m;
	}
	return l;
}


glm::vec3 NURBS::Sample(float t) const
{
	float u = t * (m_Knots.back() - m_Knots[0]);
	int knotSpan = KnotSpan(u);
	std::vector<float> basisFuncs = BasisFuncs(u);
	glm::vec4 res = { 0.0f, 0.0f, 0.0f, 0.0f };
	for (unsigned i = 0; i <= m_Degree; i++) {
		res += basisFuncs[i] * m_Points[knotSpan - m_Degree + i];
	}
	return glm::vec3{ res.x, res.y, res.z } / res.w;
}

void NURBS::UpdateKnotVector()
{
	m_Knots.clear();
	for (unsigned i = 0; i <= m_Degree; i++) m_Knots.push_back(0.0f);
	for (float i = 1.0f; i < m_Points.size() - m_Degree; i++)  m_Knots.push_back(i);
	for (unsigned i = 0; i <= m_Degree; i++) m_Knots.push_back((float)m_Points.size() - m_Degree);
}

void NURBS::UpdateSamples()
{
	m_Samples.clear();
	m_Indecies.clear();
	int sampleCount = SAMPLES_PER_EDGE * ((int)m_Points.size() - 1);
	for (int i = 0; i <= sampleCount; i++) {
		m_Samples.push_back(Sample((float)i / sampleCount));
		m_Indecies.push_back(i);
		m_Indecies.push_back(i + 1);
	}
	m_Indecies.pop_back();
	m_Indecies.pop_back();
}

void NURBS::UpdateVertexArray()
{
	m_VertexArrayLines = std::make_unique<VertexArrayLines>(m_Samples, m_Color, m_ID, m_Indecies, 2.0f, 0U);
}

glm::vec3 NURBS::Intersect(Ray r, uint32_t subID) const
{
	return r.ClosestPointOnLine(m_Samples[subID], m_Samples[subID + 1]);
}


void NURBS::ElevateDegree(unsigned n)
{
	DebugPrint();

	// TODO: factor, clean, fix
	// bug if n > degree + 1;
	std::vector<std::vector<float>> BezierAlphas(m_Degree + n + 1, std::vector(m_Degree + 1, 0.0f));
	std::vector<glm::vec4> BezControls(m_Degree + 1), ElvevatedBezControls(m_Degree + n + 1), NextBezControls(m_Degree - 1);
	std::vector<float> Alphas(m_Degree - 1);

	auto Bin = [](int a, int b) -> int {
		int res = 1.0;
		for (int i = a; i > a - b; i--) res *= i;
		for (int i = 2; i <= b; i++) res /= i;
		return res;
	};

	int DistinctKnots = 1;
	for (int i = 1; i < m_Knots.size(); i++)
		if (m_Knots[i] != m_Knots[i - 1]) DistinctKnots++;

	std::vector<glm::vec4> NewPoints(m_Points.size() + n * (DistinctKnots - 1));
	std::vector<float> NewKnots(m_Knots.size() + n * DistinctKnots);
	unsigned NewDegree = m_Degree + n;

	// Compute Bezier Coeficients

	BezierAlphas[0][0] = BezierAlphas[NewDegree][m_Degree] = 1.0f;
	for (unsigned i = 1; i <= NewDegree / 2; i++) {
		float inv = 1.0 / Bin(NewDegree, i);
		float mpi = std::min(m_Degree, i);
		for (unsigned j = std::max(0U, i - n); j <= mpi; j++)
			BezierAlphas[i][j] = inv * Bin(m_Degree, j) * Bin(n, i - j);
	}
	for (unsigned i = NewDegree / 2 + 1; i <= NewDegree - 1; i++) {
		float mpi = std::min(m_Degree, i);
		for (unsigned j = std::max(0U, i - n); j <= mpi; j++)
			BezierAlphas[i][j] = BezierAlphas[NewDegree - i][m_Degree - j];
	}

	// Initialize First Segment

	unsigned mh = NewDegree, kind = NewDegree + 1;
	int r = -1;
	unsigned a = m_Degree, b = m_Degree + 1, cind = 1;
	float ua = m_Knots[0];
	NewPoints[0] = m_Points[0];
	for (int i = 0; i <= NewDegree; i++) NewKnots[i] = ua;
	for (int i = 0; i <= m_Degree; i++) BezControls[i] = m_Points[i];

	while (b < m_Knots.size() - 1) {
		int i = b;
		while (b < m_Knots.size() - 1 && m_Knots[b] == m_Knots[b + 1]) b++;
		int mul = b - i + 1;
		mh += mul + n;
		float ub = m_Knots[b];
		int oldr = r;
		r = m_Degree - mul;
		int lbz = (oldr > 0) ? (oldr + 2) / 2 : 1;
		int rbz = (r > 0) ? NewDegree - (r + 1) / 2 : NewDegree;
		// Insert knots
		if (r > 0) { 
			float number = ub - ua;
			for (int k = m_Degree; k > mul; k--) Alphas[k - mul - 1] = number / (m_Knots[a + k] - ua);
			for (int j = 1; j <= r; j++) {
				int save = r - j;
				int s = mul + j;
				for (int k = m_Degree; k >= s; k--) {
					BezControls[k] = Alphas[k - s] * BezControls[k] + (1.0f - Alphas[k - s]) * BezControls[k - 1];
				}
				NextBezControls[save] = BezControls[m_Degree];
			}
		}
		// Elevate Bezier 
		for (unsigned i = lbz; i <= NewDegree; i++) {
			ElvevatedBezControls[i] = {0.0f, 0.0f, 0.0f, 0.0f};
			for (unsigned j = std::max(0U, i - n); j <= std::min(m_Degree, i); j++) {
				ElvevatedBezControls[i] += BezierAlphas[i][j] * BezControls[j];
			}
		}
		//  Remove knots
		if (oldr > 1) {
			int first = kind - 2;
			int last = kind;
			float den = ub - ua;
			float bet = (ub - NewKnots[kind - 1]) / den;
			for (int tr = 1; tr < oldr; tr++) {
				int i = first, j = last, kj = j - kind + 1;
				while (j - i > tr) {
					if (i < cind) {
						float alf = (ub - NewKnots[i]) / (ua - NewKnots[i]);
						NewPoints[i] = alf * NewPoints[i] + (1.0f - alf) * NewPoints[i - 1];
					}
					if (j >= lbz) {
						if (j - tr <= kind - NewDegree - oldr) {
							float gam = (ub - NewKnots[j - tr]) / den;
							ElvevatedBezControls[kj] = gam * ElvevatedBezControls[kj] +
								(1.0f - gam) * ElvevatedBezControls[kj + 1];
						}
						else
						{
							ElvevatedBezControls[kj] = bet * ElvevatedBezControls[kj] +
								(1.0f - bet) * ElvevatedBezControls[kj + 1];
						}
					}
					i++;
					j--;
					kj--;
				}
				first--;
				last++;
			}
		}

		if (a != m_Degree) {
			for (int i = 0; i < NewDegree - oldr; i++) {
				NewKnots[kind++] = ua;
			}
		}
		for (int j = lbz; j <= rbz; j++) {
			NewPoints[cind++] = ElvevatedBezControls[j];
		}
		if (b < m_Knots.size() - 1) {
			for (int j = 0; j < r; j++) BezControls[j] = NextBezControls[j];
			for (int j = r; j <= m_Degree; j++) BezControls[j] = m_Points[b - m_Degree + j];
			a = b++;
			ua = ub;
		}
		else {
			for (i = 0; i <= NewDegree; i++) {
				NewKnots[kind + i] = ub;
			}
		}
	}

	m_Points = NewPoints;
	m_Knots = NewKnots;
	m_Degree = NewDegree;
	
	UpdateSamples();
	UpdateVertexArray();

	std::vector<glm::vec3> controlPoints;
	for (const auto& p : m_Points) controlPoints.push_back(glm::vec3(p.x, p.y, p.z) / p.w);
	m_ControlPolyLine = std::make_unique<PolyLine>(controlPoints, true, m_ID);
	m_ControlPolyLine->PointsOn();
	m_ControlPolyLine->SetModel(m_Model);

	DebugPrint();
}

void NURBS::DebugPrint()
{
	printf("Degree %d\n", m_Degree);
	printf("Point count %d\n", m_Points.size());
	printf("Knot count %d\n", m_Knots.size());
	//std::cout << "knots ";
	//for (float f : m_Knots) std::cout << f << ", "; std::cout << std::endl;
	//for (const auto& p : m_Points) print(p, true);
	std::cout << std::endl;
}
