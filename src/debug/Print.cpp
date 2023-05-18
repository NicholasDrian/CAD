#pragma once

#include "Print.h"

#include "../geometry/NURBS/NURBS.h"
#include "../geometry/PolyLine.h"
#include "../geometry/Mesh.h"
#include "../geometry/NURBS/NURBSurface.h"
#include "../scene/ConstructionPlane.h"

#include <iostream>

void print(glm::vec3 v, bool newLine)
{
#ifdef CAD_DEBUG

	printf("(%.2f, %.2f, %.2f)", v.x, v.y, v.z);
	if (newLine) std::cout << '\n';

#endif
}

void print(glm::vec4 v, bool newLine)
{
#ifdef CAD_DEBUG

	printf("(%.2f, %.2f, %.2f, %.2f)", v.x, v.y, v.z, v.w);
	if (newLine) std::cout << '\n';

#endif
}

void print(float f, bool newLine)
{
#ifdef CAD_DEBUG
	printf("%.2f", f);
	if (newLine) std::cout << '\n';
#endif
}

void print(AxisAlignedBoundingBox bb)
{
	printf("BB: X:[%.2f, %.2f], Y:[%.2f, %.2f], Z:[%.2f, %.2f]\n",
		bb.MinX(), bb.MaxX(), bb.MinY(), bb.MaxY(), bb.MinZ(), bb.MaxZ());
}

void print(BVHNode* node, int depth)
{
	if (node == nullptr) return;
	for (int i = 0; i < depth; i++) std::cout << '\t';
	std::cout << node->m_Indices.size() << ' ';
	print(node->m_BoundingBox);
	print(node->m_ChildA.get(), depth + 1);
	print(node->m_ChildB.get(), depth + 1);
}

void print(Renderable* r)
{
	if (NURBS* nurbs = dynamic_cast<NURBS*>(r)) print("NURBS");
	else if (PolyLine* nurbs = dynamic_cast<PolyLine*>(r)) print("POLY LINE");
	else if (NURBSurface* nurbs = dynamic_cast<NURBSurface*>(r)) print("NURBSurface");
	else if (Mesh* nurbs = dynamic_cast<Mesh*>(r)) print("MESH");
	else if (ConstructionPlane* nurbs = dynamic_cast<ConstructionPlane*>(r)) print("CONSTRUCTION PLANE");
	else print("TODO: VOID PRINT(RENDERABLE* R);");
	//todo


}

void print(Ray r)
{
	printf("Ray - O:");
	print(r.GetOrigin());
	printf(" D:");
	print(r.GetDirection(), true);
}

void print(const char* s)
{
	printf(s);
}

void print(const glm::mat4& m)
{
#ifdef CAD_DEBUG
	printf("%.2f, %.2f, %.2f, %.2f \n",   m[0][0], m[0][1], m[0][2], m[0][3]);
	printf("%.2f, %.2f, %.2f, %.2f \n",   m[1][0], m[1][1], m[1][2], m[1][3]);
	printf("%.2f, %.2f, %.2f, %.2f \n",   m[2][0], m[2][1], m[2][2], m[2][3]);
	printf("%.2f, %.2f, %.2f, %.2f \n\n", m[3][0], m[3][1], m[3][2], m[3][3]);
#endif
}