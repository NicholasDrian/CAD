#pragma once

#include "../Command.h"

class CommandPointsOn : public Command {

public:

	CommandPointsOn();

	virtual void TextInput(const std::string& input) override;
	virtual void ClickInput(int x, int y, int mods) override;
	virtual void DrawButtons() override;

	inline virtual void Escape() { m_Finished = true; }
	inline virtual bool IsFinished() const override { return m_Finished; }
	inline virtual std::string GetInstructions() const override { return m_Instructions; }

private:

	const std::string m_Instructions = "Select objects or type 'All'";
	bool m_Finished = false;

};