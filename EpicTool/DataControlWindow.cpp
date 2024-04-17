
#include "imgui.h"
//#include "FileSave.h"
#include <nlohmann/json.hpp> 
#include <iostream>
#include <fstream>
#include "DataControlWindow.h"
#include "KunrealAPI.h"
#include "Serialize.h"
#include "Deserialize.h"
#include "ExportObj.h"

#include <string.h>
 // NFD를 쓸것인데 아직 lib 빌드가 안되었다 확인해야함
// 수정 완료 이제 정상적으로 작동

/// 2023.10.19 배성근
/// 집에서는 ///가 세팅이 안되는 문제 // 임시해결 뭐가 문제인지는 아직 모름
/// lib가 svn에 안올라와서 빌드가 안되는 문제가 있다.
/// 

/// <summary>
/// 2024.03.14 배성근
/// 에디터 사용중 로드를 통한 덮어쓰기를 구현하고자 했으나 
/// 충돌로 인해 보류
/// </summary>

EpicTool::DataControlWindow::DataControlWindow()
    :_show_save_editor(true), _save_Scene(true), _new_Scene(true), _opt_padding(false), _fileSave(nullptr)
{

}

EpicTool::DataControlWindow::~DataControlWindow()
{

}

void EpicTool::DataControlWindow::ShowWindow()
{

}

void EpicTool::DataControlWindow::ShowWindow(bool& close) //세이브 버튼 다른 곳으로 이동 해야할듯
{
	std::string samplefilePath = _saveFilePath;

	static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;



	

	if (_show_save_editor)
	{
		ImGui::Begin(" ", &_show_save_editor, ImGuiWindowFlags_MenuBar);

		if (ImGui::Button("Save"))
		{
			SaveToFile(samplefilePath);

		}
		ImGui::SameLine();
		if (ImGui::Button("ExportObj"))
		{
			_exportObj->ExportToObj("testObj.obj");
		}

		ImGui::SameLine(); // 같은 라인에 배치

		if (ImGui::Button("Load"))
		{
			LoadToFile(samplefilePath);
		}

		ImGui::SameLine();

		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				if (ImGui::MenuItem("New Scene", NULL, &_new_Scene))
				{

				}
				if (ImGui::MenuItem("Save Scene", NULL, &_save_Scene))
				{

				}

				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("Window"))
			{
				if (ImGui::MenuItem("Inspector"), NULL, &_save_Scene)  // 변수 임시로 넣어둠 window들을 조작할수 있는 창임
					// 윈도우를 조절하던 변수를 참조를 통해 가져와서 만지는 것이 좋을까?
				{

				}
				if (ImGui::MenuItem("Hierarchy"), NULL, &_save_Scene)
				{

				}
				if (ImGui::MenuItem("GameWindow"), NULL, &_save_Scene)
				{

				}
				if (ImGui::MenuItem("GraphicsWindow"), NULL, &_save_Scene)
				{

				}
				if (ImGui::MenuItem("ResouceWindow"), NULL, &_save_Scene)
				{

				}

				ImGui::EndMenu();
			}
			ImGui::EndMenuBar();
		}

		ImGui::End();
	}
	else
	{
		close = true;
	}
	
}

void EpicTool::DataControlWindow::ShowWindow(bool* _open, std::vector<Object>& object)
{

}

void EpicTool::DataControlWindow::Initialize()
{
	_exportObj = new ExportObj();
}
 
void EpicTool::DataControlWindow::SaveToFile(const std::string& filePath)
{
    nfdchar_t* outPath = NULL;
    nfdresult_t result = NFD_SaveDialog("json", "json", &outPath);


    if (result == NFD_OKAY) {
        if (outPath) {
            std::string chosenPath = outPath;
            if (chosenPath.rfind(".json") == std::string::npos)
            {
                chosenPath += ".json";
            }

            _saveFilePath = chosenPath;

            free(outPath);
			_serialize = new Serialize();  // 루프문에 없어면 터지네?
			_serialize->SaveFile(_saveFilePath);
        }
    }
}

void EpicTool::DataControlWindow::LoadToFile(const std::string& filePath)
{
	nfdchar_t* outPath = NULL;
	nfdresult_t result = NFD_OpenDialog("json", NULL, &outPath);

	if (result == NFD_OKAY)
	{
		if (outPath)
		{
			std::string chosenPath = outPath;

			_loadFilePath = chosenPath;

			free(outPath);


			KunrealEngine::GetCurrentScene()->GetObjectList().clear();

			Deserialize* _deserialize = new Deserialize();
			_deserialize->Initialize(_loadFilePath);			
		}
	}
}

