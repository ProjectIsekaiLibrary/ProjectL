#include "NavimashEditor.h"
#include "imgui.h"
//#include "Navigation.h"

#include <iostream>
#include <fstream>

EpicTool::NavimashEditor::NavimashEditor()
{

}

EpicTool::NavimashEditor::~NavimashEditor()
{

}

void EpicTool::NavimashEditor::Initialize()
{

}

void EpicTool::NavimashEditor::ShowWindow()
{

	ImGui::Begin("Navimash");

	ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "Navimash");

	ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "Agent");

	if (ImGui::SliderFloat("Height", &AgentHeight, 0.0f, 5.0f))
	{
		// 값 반영
	}

	if (ImGui::SliderFloat("Radius", &AgentRadius, 0.0f, 5.0f))
	{
		// 값 반영
	}

	if (ImGui::SliderFloat("Max Climb", &AgentMaxClimb, 0.0f, 5.0f))
	{
		// 값 반영
	}

	if (ImGui::SliderFloat("Max Slope", &AgentMaxSlope, 0.0f, 5.0f))
	{
		// 값 반영
	}

	if (ImGui::Button("Build"))
	{

	}

	ImGui::End();

}

void EpicTool::NavimashEditor::ShowWindow(bool* _open, std::vector<Object>& object)
{

}
