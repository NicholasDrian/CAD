#pragma once

#include "../Command.h"

class FocusCommand : public Command {

public:

	virtual void TextInput(const std::string& input) override;
	virtual void ClickInput(int x, int y) override;
	virtual bool IsFinished() const override { return m_Finished; };
	inline virtual void Escape() override { m_Finished = true; }

	inline virtual std::string GetInstructions() const override { return m_Instructions; }

private:

	bool m_Finished = false;

	inline static const std::string m_Instructions = "Click on or type out new focal point: ";

};