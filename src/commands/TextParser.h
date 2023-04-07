#pragma once

#include "glm/glm.hpp"

#include <string>

class TextParser {

public:

	static bool IsTypable(int character);

	static bool ParseVector3F(const std::string& text, glm::vec3& outVector);



};