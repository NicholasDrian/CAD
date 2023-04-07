#pragma once

#include "CommandManager.h"

#include "FocusCommand.h"
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
		else if (m_CurrentCommand)
			m_CurrentCommand.reset();
		else
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
			if (m_TextInput == "FOCUS") {
				m_CurrentCommand = std::make_unique<FocusCommand>();
			}
			else if (m_TextInput == "") {

			}
			else if (m_TextInput == "") {

			}
			else if (m_TextInput == "") {

			}
		}
		m_TextInput.clear();
	}
	else if (TextParser::IsTypable(c)) 
	{
		m_TextInput += c;
	}
}

void CommandManager::HandleClick(int x, int y) {
	if (m_CurrentCommand) m_CurrentCommand->ClickInput(x, y);
}