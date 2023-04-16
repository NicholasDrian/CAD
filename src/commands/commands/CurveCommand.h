#pragma once

#include "../Command.h"
#include "../../geometry/NURBS.h"

#include <memory>

enum class CurveCommandMode {
	AddPoint,
	ChangeDegree
};

class CurveCommand : public Command {

public:

	virtual void TextInput(const std::string& input) override;
	virtual void ClickInput(int x, int y) override;
	virtual void Tick() override;
	virtual void Escape() override;
	virtual void DrawButtons() override;
	virtual void Render() const override;

	inline virtual bool IsFinished() const override { return m_Finished; };
	virtual std::string GetInstructions() const override;

private:

	CurveCommandMode m_Mode = CurveCommandMode::AddPoint;

	bool m_Finished = false;

	unsigned m_Degree = 3;

	std::unique_ptr<NURBS> m_Curve;

	// Used to avoid adding new temporary point until the mouse moves
	int m_PrevX = -1, m_PrevY = -1;
	bool m_NeedsExtraPoint = false;

};