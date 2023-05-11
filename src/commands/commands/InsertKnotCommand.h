#pragma once

#include "../Command.h"

class InsertKnotCommand : public Command {

public:

	virtual void TextInput(const std::string& input) override;
	virtual void ClickInput(int x, int y, int mods) override;
	virtual std::string GetInstructions() const override;

	inline virtual bool IsFinished() const override { return m_Finished; }
	inline virtual void Escape() override { m_Finished = true; }

private:

	bool m_Finished = false;

};