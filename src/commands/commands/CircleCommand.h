#pragma once

#include "../Command.h"
#include "../../geometry/NURBS/Circle.h"

#include <memory>

enum class CircleCommandMode {
	None,
	CenterPointPoint,
	NormalCenterRadious,
	PointPointPoint
};

class CircleCommand : public Command {

public:

	virtual void TextInput(const std::string& input) override;
	virtual void ClickInput(int x, int y, int mods) override;
	virtual std::string GetInstructions() const override;

	virtual void Tick() override;
	virtual void DrawButtons() override;
	virtual void Render() const override;

	inline virtual void Escape() override { m_Finished = true; }
	inline virtual bool IsFinished() const override { return m_Finished; }

private:

	glm::vec3 a, b, c;

	std::unique_ptr<Circle> m_Circle;

	CircleCommandMode m_Mode = CircleCommandMode::None;
	bool m_Finished = false;
	int m_Phase = 0;

};