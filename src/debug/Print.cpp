#pragma once

#include "Print.h"

#include <iostream>

void print(glm::vec3 v, bool newLine)
{
#ifdef CAD_DEBUG

	printf("(%.2f, %.2f, %.2f)", v.x, v.y, v.z);
	if (newLine) std::cout << '\n';

#endif
}

void print(const glm::mat4& m)
{
#ifdef CAD_DEBUG
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			std::cout << m[i][j] << ", ";
		} 
		std::cout << '\n';
	} 
#endif
}