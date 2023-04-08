#pragma once

class Renderable {

public:

	virtual void Render() const = 0;
	virtual unsigned GetID() const = 0;

	bool m_Selectable = true;
	bool m_Selected = false;

};