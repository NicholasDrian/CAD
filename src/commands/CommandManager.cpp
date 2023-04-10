#pragma once

#include "CommandManager.h"

#include "commands/FocusCommand.h"
#include "commands/PolyLineCommand.h"
#include "commands/CurveCommand.h"
#include "TextParser.h"
#include "../scene/Scene.h"

#include "GLFW/glfw3.h"

#include <unordered_set>
#include <iostream>


void CommandManager::AddInput(int c) {
	if (c == GLFW_KEY_ESCAPE) 
	{
		if (m_TextInput.size())
			m_TextInput.clear();
		else if (m_CurrentCommand) {
			m_CurrentCommand->Escape();
		} else
			Scene::ClearSelection();
	}
	else if (c == GLFW_KEY_DELETE) {
		if (!HasActiveCommand())
			Scene::DeleteSelection();
	}
	else if (c == GLFW_KEY_BACKSPACE) 
	{
		if (m_TextInput.size())
			m_TextInput.pop_back();
	}
	else if (c == GLFW_KEY_ENTER) 
	{
		if (HasActiveCommand())
		{
			m_CurrentCommand->TextInput(m_TextInput);
		}
		else 
		{
			if (m_TextInput.empty()) m_TextInput = m_PreviousCommand;

			if (m_TextInput == "FOCUS") {
				m_CurrentCommand = std::make_unique<FocusCommand>();
			}
			else if (m_TextInput == "POLYLINE") {
				m_CurrentCommand = std::make_unique<PolyLineCommand>();
			}
			else if (m_TextInput == "CURVE") {
				m_CurrentCommand = std::make_unique<CurveCommand>();
			}
			else if (m_TextInput == "") {

			}
			m_PreviousCommand = m_TextInput;
		}
		m_TextInput.clear();
	}
	else if (TextParser::IsTypable(c)) 
	{
		m_TextInput += c;
	}
	if (m_CurrentCommand && m_CurrentCommand->IsFinished()) m_CurrentCommand.reset();
}

void CommandManager::HandleClick(int x, int y) {
	if (m_CurrentCommand) {
		m_CurrentCommand->ClickInput(x, y);
		if (m_CurrentCommand->IsFinished()) m_CurrentCommand.reset();
	}
}

void CommandManager::Tick() {
	if (m_CurrentCommand) m_CurrentCommand->Tick();
}

std::string CommandManager::GetInstructions()
{
	if (m_CurrentCommand) return m_CurrentCommand->GetInstructions();
	return "Command: ";
}

void CommandManager::DrawButtons()
{
	if (m_CurrentCommand) m_CurrentCommand->DrawButtons();
}
