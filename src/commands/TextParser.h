#pragma once

#include "glm/glm.hpp"

#include <string>

class TextParser {

public:

	static bool IsTypable(int character);
	static bool ParseVector3F(const std::string& text, glm::vec3& outVector);
	static bool ParseUnsignedInt(const std::string& text, unsigned& outUnsignedInt);
	static bool ParseInt(const std::string& text, unsigned& outInt);
	static bool ParseFloat(const std::string& text, float& outFloat);

private:

	static bool NextUnsignedInt(const std::string& text, int& i, unsigned& outUnsignedInt);
	static bool NextInt(const std::string& text, int& i, unsigned& outInt);
	static bool NextFloat(const std::string& text, int& i, float& outFloat);
	static bool IsDigit(char c);
	static int ToDigit(char c);
};