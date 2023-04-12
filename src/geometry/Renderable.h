#pragma once

#include <stdint.h>

class Renderable {

public:

	virtual void Render() const = 0;
	virtual unsigned GetID() const = 0;

	virtual void AddSubSelection(uint32_t subID) {};
	virtual void RemoveSubSelection(uint32_t subID) {};
	virtual void ClearSubSelection() {};

	bool m_Selectable = true;
	bool m_Selected = false;

};