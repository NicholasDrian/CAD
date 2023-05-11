#pragma once

#include <vector>
#include <string>

struct CommandInfoEntry {

	std::string m_Title;
	std::vector<std::string> m_BulletPoints;

};

class CommandInfoGUI {

public:

	static void Init();
	static void Render();

private:

	static std::vector<CommandInfoEntry> m_Entries;

};