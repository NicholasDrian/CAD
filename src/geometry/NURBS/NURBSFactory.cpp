#pragma once

#include "NURBSFactory.h"
#include "NURBSUtils.h"

NURBSurface NURBSFactory::Loft(const std::vector<NURBS*>& curves, int degree)
{
    int degreeU = std::min(degree, (int)curves.size() - 1);

    // match degrees
    int degreeV = 0;
    for (const auto& c : curves) degreeV = std::max(degreeV, (int)c->GetDegree());
    for (auto& c : curves) if (c->GetDegree() < degreeV) c->ElevateDegree(degreeV - c->GetDegree());
    
    // match knots
    auto equals = [](float a, float b) { return abs(a - b) < 0.000001f; };
    for (auto& c : curves) c->NormalizeKnots();
    for (int i = 0; i < curves[0]->GetKnotCount(); i++) {
        float smallest = std::numeric_limits<float>::infinity();
        bool finished = true;
        for (const auto& c : curves) {
            if (i < c->GetKnotCount()) {
                finished = false;
                smallest = std::min(smallest, c->GetKnot(i));
            }
        }
        if (finished) break;
        for (auto& c : curves) {
            if (!equals(c->GetKnot(i), smallest)) c->InsertKnot(smallest);
        }
    }

    // loft
    std::vector<std::vector<glm::vec4>> points(curves.size());
    for (int i = 0; i < curves.size(); i++) {
        NURBS* curve = curves[i];
        for (int j = 0; j < curve->GetNumControlPoints(); j++) {
            points[i].push_back(curve->GetModel() * curve->GetControlPoints()[j]);
        }
    }
    std::vector<float> knotsU = NURBSUtils::GenericKnotVector(curves.size(), degreeU);

    return NURBSurface(points, knotsU, curves[0]->GetKnots(), degreeU, degreeV);

}
