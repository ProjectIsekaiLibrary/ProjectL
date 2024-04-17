#include "NavimashEditor.h"
#include "imgui.h"
#include "KunrealAPI.h"

#include <iostream>
#include <fstream>

EpicTool::NavimashEditor::NavimashEditor()
	:_naviIndex(0), _agentHeight(0), _agentRadius(0), _agentMaxClimb(0), _agentMaxSlope(0)
{

}

EpicTool::NavimashEditor::~NavimashEditor()
{

}

void EpicTool::NavimashEditor::Initialize()
{
	_navimashEditor = &KunrealEngine::Navigation::GetInstance();
}

void EpicTool::NavimashEditor::ShowWindow()
{

	ImGui::Begin("Navimash");

	ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "Navimash");

	ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "Agent");

	ImGui::SliderFloat("Height", &_agentHeight, 0.0f, 5.0f);

	ImGui::SliderFloat("Radius", &_agentRadius, 0.0f, 10.0f);

	ImGui::SliderFloat("Max Climb", &_agentMaxClimb, 0.0f, 5.0f);

	ImGui::SliderFloat("Max Slope", &_agentMaxSlope, 0.0f, 90.0f);

	if (ImGui::Button("Build"))
	{
		_navimashEditor->SetAgent(_naviIndex, _agentHeight, _agentMaxSlope, _agentRadius, _agentMaxClimb);
		_navimashEditor->HandleBuild(_naviIndex);
	}

	ImGui::SameLine();

	ImGui::InputInt("ObjectIndex", &_naviIndex);

	ImGui::End();

}

void EpicTool::NavimashEditor::ShowWindow(bool* _open, std::vector<Object>& object)
{

}
