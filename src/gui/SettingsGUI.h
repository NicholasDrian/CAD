#pragma once



class SettingsGUI {

public:

	static void Render();

private:

	inline static bool m_SnapCurve = false;
	inline static bool m_SnapPoint = false;
	inline static bool m_SnapGrid = false;

};