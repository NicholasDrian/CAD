#pragma once

#include "CommandManager.h"

#include "commands/FocusCommand.h"
#include "commands/PolyLineCommand.h"
#include "commands/CurveCommand.h"
#include "commands/PointsOnCommand.h"
#include "commands/PointsOffCommand.h"
#include "commands/CircleCommand.h"
#include "commands/ElevateCurveDegreeCommand.h"
#include "commands/InsertKnotCommand.h"
#include "commands/LoftCommand.h"

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
			else if (m_TextInput == "POINTS ON") {
				m_CurrentCommand = std::make_unique<CommandPointsOn>();
			}
			else if (m_TextInput == "POINTS OFF") {
				m_CurrentCommand = std::make_unique<CommandPointsOff>();
			}
			else if (m_TextInput == "CIRCLE") {
				m_CurrentCommand = std::make_unique<CircleCommand>();
			}
			else if (m_TextInput == "ELEVATE CURVE DEGREE") {
				m_CurrentCommand = std::make_unique<ElevateCurveDegreeCommand>();
			}
			else if (m_TextInput == "INSERT KNOT") {
				m_CurrentCommand = std::make_unique<InsertKnotCommand>();
			}
			else if (m_TextInput == "LOFT") {
				m_CurrentCommand = std::make_unique<LoftCommand>();
			}
			m_PreviousCommand = m_TextInput;
		}
		m_TextInput.clear();
	}
	else if (TextParser::IsTypable(c)) 
	{
		m_TextInput += c;
	}
}

void CommandManager::HandleClick(int x, int y, int mods) {
	if (m_CurrentCommand) {
		m_CurrentCommand->ClickInput(x, y, mods);
	}
}

void CommandManager::Tick() {
	if (m_CurrentCommand) {
		m_CurrentCommand->Tick();
		if (m_CurrentCommand->IsFinished()) m_CurrentCommand.reset();
	}
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

void CommandManager::Render()
{
	if (m_CurrentCommand) m_CurrentCommand->Render();
}