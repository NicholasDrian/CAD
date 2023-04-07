#pragma once

#include "Command.h"

class FocusCommand : public Command {

public:

	virtual void TextInput(const std::string& input) override;
	virtual void ClickInput(int x, int y) override;
	inline virtual std::string GetInstructions() const override { return m_Instructions; }

private:

	inline static const std::string m_Instructions = "Click on or type out new focal point.";

};