#include "NavimashEditor.h"
#include "imgui.h"
#include "KunrealAPI.h"

#include <iostream>
#include <fstream>

EpicTool::NavimashEditor::NavimashEditor()
	:_naviIndex(0), _agentHeight(2.0f), _agentRadius(1.2f), _agentMaxClimb(0.9f), _agentMaxSlope(45.0f)
{
}

EpicTool::NavimashEditor::~NavimashEditor()
{

}

void EpicTool::NavimashEditor::Initialize()
{
	_navimashEditor = &KunrealEngine::Navigation::GetInstance();
	_navmeshpolys.resize(_navimashEditor->GetPackageSize());
}

void EpicTool::NavimashEditor::DrawCylinder(ImDrawList* drawList, ImVec2 windowPos, ImVec2 windowSize, float centerX, float centerY, float radius, float height)
{
	ImVec2 center(windowPos.x + centerX, windowPos.y + centerY);

	const int numSegments = 12;
	const float angleStep = 2 * 3.14 / numSegments;


	for (int i = 0; i < numSegments; ++i)
	{
		float angle1 = i * angleStep;
		float angle2 = (i + 1) * angleStep;

		ImVec2 p1(center.x + radius * cosf(angle1), center.y - radius * sinf(angle1));
		ImVec2 p2(center.x + radius * cosf(angle2), center.y - radius * sinf(angle2));


		drawList->AddLine(p1, p2, IM_COL32(0, 255, 0, 255));
		drawList->AddLine(ImVec2(p1.x, p1.y - height), ImVec2(p2.x, p2.y - height), IM_COL32(0, 255, 0, 255));
		drawList->AddLine(p1, ImVec2(p1.x, p1.y - height), IM_COL32(0, 255, 0, 255));
	}

	//drawList->AddCircleFilled(center, radius, IM_COL32(255, 0, 0, 255));
	drawList->AddCircle(ImVec2(center.x, center.y - ( _agentMaxClimb * 20.0f)), radius, IM_COL32(0, 255, 0, 255));
	//drawList->AddCircleFilled(ImVec2(center.x, center.y + height), radius, IM_COL32(255, 0, 0, 255));
}

void EpicTool::NavimashEditor::ShowWindow()
{

	ImGui::Begin("Navimash");

	ImVec2 windowPos = ImGui::GetWindowPos();
	ImVec2 windowSize = ImGui::GetWindowSize();

	ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "Navimash");

	ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "Agent");

	ImGui::SliderFloat("Height", &_agentHeight, 0.0f, 5.0f);

	ImGui::SliderFloat("Radius", &_agentRadius, 0.0f, 10.0f);

	ImGui::SliderFloat("Max Climb", &_agentMaxClimb, 0.0f, 5.0f);

	ImGui::SliderFloat("Max Slope", &_agentMaxSlope, 0.0f, 90.0f);

	if (ImGui::Button("Show"))
	{
		if (_navmeshpolys[_naviIndex] == nullptr)
		{
			return;
		}

		for (auto navpoly : _navmeshpolys)
		{
			if (navpoly == nullptr)
			{
				continue;
			}
			navpoly->SetActive(false);
		}
		_navmeshpolys[_naviIndex]->SetActive(true);
	}

	ImGui::SameLine();

	if (ImGui::Button("Build"))
	{
		_navimashEditor->HandleBuild(_naviIndex);

		if (_navmeshpolys[_naviIndex] != nullptr)
		{
			GRAPHICS->DeleteDebugObject(_navmeshpolys[_naviIndex]);
			_navmeshpolys[_naviIndex] = nullptr;
		}
		
		std::string name = "navmesh";
		name += std::to_string(_naviIndex);

		std::vector<DirectX::XMFLOAT3> vertices;
		std::vector<unsigned int> indices;
		_navimashEditor->GetNavmeshRenderInfo(0, vertices, indices);
		_navmeshpolys[_naviIndex] = GRAPHICS->CreateMapDebug(name.c_str(), vertices, indices);
		//_navmeshpolys[_naviIndex]->SetActive(false);
	}

	_navimashEditor->SetAgent(_naviIndex, _agentHeight, _agentMaxSlope, _agentRadius, _agentMaxClimb);

	ImGui::SameLine();

	ImGui::InputInt("ObjectIndex", &_naviIndex);
	
	if (ImGui::Button("Build")|| ImGui::Button("Build"))
	{
		_navimashEditor->GetAgent(_naviIndex, _agentHeight, _agentMaxSlope, _agentRadius, _agentMaxClimb);
	}

	DrawCylinder(ImGui::GetWindowDrawList(), windowPos, windowSize, 70.0f, 350.0f, (_agentRadius * 20.0f), (_agentHeight * 20.0f));

	ImGui::End();

}

void EpicTool::NavimashEditor::ShowWindow(bool* _open, std::vector<Object>& object)
{

}
