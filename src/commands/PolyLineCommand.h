#pragma once

#include "Command.h"
#include "../geometry/PolyLine.h"

#include <string>

class PolyLineCommand : public Command {

public:

	virtual void TextInput(const std::string& input) override;
	virtual void ClickInput(int x, int y) override;
	virtual void Tick() override;

	inline virtual bool IsFinished() const override { return m_Finished; };
	inline virtual std::string GetInstructions() const override { return m_Instructions; }

private:

	bool m_Finished = false;
	PolyLine* m_PolyLine = nullptr;

	const std::string m_Instructions = "Click or type to add a point: ";

};