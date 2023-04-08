#pragma once

#include <string>

class Command {
	
public:

	virtual void TextInput(const std::string& input) = 0;
	virtual void ClickInput(int x, int y) = 0;
	virtual std::string GetInstructions() const = 0;
	virtual bool IsFinished() const = 0;
	virtual void Tick() {}

};