#pragma once

#include "../Command.h"
#include "../../geometry/NURBS/NURBS.h"

class LoftCommand : public Command {

public:

	LoftCommand();
	~LoftCommand();

	virtual void TextInput(const std::string& input) override;
	virtual void ClickInput(int x, int y, int mods) override;
	virtual std::string GetInstructions() const override;

	virtual void Tick() override;

	inline virtual bool IsFinished() const override { return m_Finished; }
	inline virtual void Escape() override { m_Finished = true; }

private:

	std::vector<NURBS*> m_Curves;
	unsigned m_Degree = 2;
	bool m_Finished = false;

};