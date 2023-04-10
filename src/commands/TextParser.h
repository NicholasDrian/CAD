#pragma once

#include "glm/glm.hpp"

#include <string>

class TextParser {

public:

	static bool IsTypable(int character);

	static bool ParseVector3F(const std::string& text, glm::vec3& outVector);
	static bool ParseUnsignedInt(const std::string& text, unsigned& outUnsignedInt);

private:

	static bool NextUnsignedInt(const std::string& text, int& i, unsigned& outUnsignedInt);
	static bool NextFloat(const std::string& text, int& i, float& outFloat);
	static bool IsDigit(char c);
	static int ToDigit(char c);
};