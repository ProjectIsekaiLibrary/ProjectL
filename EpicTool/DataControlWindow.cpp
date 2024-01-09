
#include "imgui.h"
#include "FileSave.h"
#include <nlohmann/json.hpp> 
#include <iostream>
#include <fstream>
#include "DataControlWindow.h"
#include "KunrealAPI.h"

#include <string.h>
 // NFD를 쓸것인데 아직 lib 빌드가 안되었다 확인해야함
// 수정 완료 이제 정상적으로 작동

/// 2023.10.19 배성근
/// 집에서는 ///가 세팅이 안되는 문제 // 임시해결 뭐가 문제인지는 아직 모름
/// lib가 svn에 안올라와서 빌드가 안되는 문제가 있다.
/// 


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

void EpicTool::DataControlWindow::ShowWindow(bool& close)
{
	std::string samplefilePath = _saveFilePath;

	static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

	_fileSave = nullptr;
	if (_fileSave == nullptr)
	{
		_fileSave = new FileSave();
	}

	if (_show_save_editor)
	{
		ImGui::Begin(" ", &_show_save_editor, ImGuiWindowFlags_MenuBar);

		if (ImGui::Button("Save"))
		{
			SaveToFile(samplefilePath);

		}
		ImGui::SameLine(); // 같은 라인에 배치

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
            _fileSave->OnFileSave(_saveFilePath);
        }
    }
}


