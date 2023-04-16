#pragma once

#include "../Command.h"
#include "../../geometry/PolyLine.h"

#include <string>
#include <memory>

class PolyLineCommand : public Command {

public:

	virtual void TextInput(const std::string& input) override;
	virtual void ClickInput(int x, int y) override;
	virtual void Tick() override;
	virtual void Escape() override;
	virtual void Render() const override;

	inline virtual bool IsFinished() const override { return m_Finished; };
	inline virtual std::string GetInstructions() const override { return m_Instructions; }

private:

	bool m_Finished = false;
	std::unique_ptr<PolyLine> m_PolyLine;

	const std::string m_Instructions = "Click or type to add a point: ";

};