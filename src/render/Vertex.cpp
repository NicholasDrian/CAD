#pragma once

#include "Vertex.h"

Vertex::Vertex(glm::vec3 position, glm::vec3 normal, glm::vec3 color, GLint id) 
	: m_Position(position), m_Normal(normal), m_Color(color), m_ID(id) {}