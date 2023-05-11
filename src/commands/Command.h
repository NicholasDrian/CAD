#pragma once

#include <string>

class Command {
	
public:

	virtual ~Command() {};

	virtual void TextInput(const std::string& input) = 0;
	virtual void ClickInput(int x, int y, int mods) = 0;
	virtual std::string GetInstructions() const = 0;
	virtual bool IsFinished() const = 0;
	virtual void Escape() = 0;

	virtual void Tick() {}
	virtual void DrawButtons() {}
	virtual void Render() const {}
};