#pragma once 

#include "glm/glm.hpp"

#include "GL/glew.h"

class Vertex {

public:

	Vertex(glm::vec3 position, glm::vec3 normal, glm::vec3 color, GLint id);
	glm::vec3 m_Position, m_Normal, m_Color;
	GLint m_ID;

};