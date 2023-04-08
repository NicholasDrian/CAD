#pragma once

#include "Command.h"

#include <string>
#include <memory>
#include <unordered_map>

class CommandManager {

public:

	static void AddInput(int c);
	static void HandleClick(int x, int y);

	static void Tick();

	static std::string GetInstructions();

	inline static bool HasActiveCommand() { return (bool)m_CurrentCommand; }

	inline static const std::string& GetInput() { return m_TextInput; }
	

private:


	inline static std::string m_TextInput, m_PreviousCommand;
	inline static std::unique_ptr<Command> m_CurrentCommand;

};