#include "HierarchyWindow.h"
#include "imgui.h"
#include "imgui_impl_dx11.h"
#include "FileSave.h"
#include "KunrealAPI.h"
#include "SceneManager.h"
#include "DebugWindow.h"
#include "DebugType.h"

#include <algorithm>

EpicTool::HierarchyWindow::HierarchyWindow()
    : _createEmptyCount(1), _cubeCount(1), _sphereCount(1), _draggedIndex(-1), _dropTargetIndex(-1), _gameObjectlist(NULL),
	_hierarchyWindowX1(0), _hierarchyWindowX2(0), _hierarchyWindowY1(0), _hierarchyWindowY2(0)//, _isHierarchySelected(false)
{

}

EpicTool::HierarchyWindow::~HierarchyWindow()
{

}

void EpicTool::HierarchyWindow::Initialize()
{
	//_hierarchyWindowX1 = ImGui::GetWindowPos().x;
	//_hierarchyWindowY1 = ImGui::GetWindowPos().y;
	//_hierarchyWindowX2 = _hierarchyWindowX1 + ImGui::GetWindowSize().x;
	//_hierarchyWindowY2 = _hierarchyWindowY1 + ImGui::GetWindowSize().y;
	

}

void EpicTool::HierarchyWindow::ShowWindow(int& selectedObjectIndex)
{
	_gameObjectlist = KunrealEngine::GetCurrentScene()->
	    GetObjectList();

	static bool show_Context_Menu = false;
	static int isDragReleased = -1;

	_draggedIndex = -1;
	_dropTargetIndex = -1;

	_DebugType = DebugType::None;
 
        for (int i = 0; i < _gameObjectlist.size(); ++i)
        {
            ImGui::PushID(i);         

			if (ImGui::IsWindowHovered() && !ImGui::IsMouseHoveringRect(ImVec2(_hierarchyWindowX1, _hierarchyWindowY1), ImVec2(_hierarchyWindowX2, _hierarchyWindowY2), false) &&
				ImGui::IsMouseClicked(0))
			{
				selectedObjectIndex = -1;
			}

            if (ImGui::Selectable(_gameObjectlist[i]->GetObjectName().c_str(), (i == selectedObjectIndex)))
            {
				if (i == selectedObjectIndex)
				{
					selectedObjectIndex = -1;
					//_isHierarchySelected = false;
				}
                else
                {
					//_isHierarchySelected = true;
                    selectedObjectIndex = i;
                }
            }

			//if (i == selectedObjectIndex && ImGui::IsMouseDragging(0)) // 드래그 했을때 작동
			//{
			//	ImVec2 currentMousePos = ImGui::GetIO().MousePos; //현재 마우스 위치

			//	ImGui::GetForegroundDrawList()->AddRectFilled(
			//		ImVec2(currentMousePos.x, currentMousePos.y),
			//		ImVec2(currentMousePos.x + 100, currentMousePos.y + 20), // 하얀 상자 출력 부분
			//		IM_COL32(255, 255, 255, 100));
   //             isDragReleased = 0;

			//}
			if (ImGui::IsMouseReleased(0) && isDragReleased == 0) // 드래그 후 마우스를 뗐을 때 처리 
			{
				//UpdateGameObject();
				isDragReleased = -1;
			}
            ImVec2 currentMousePos;

			if (ImGui::IsItemHovered() && _draggedIndex != -1 && _draggedIndex != i)
			{
				_dropTargetIndex = i;
			}

            ImGui::PopID();
        }

        if (ImGui::IsMouseReleased(1) && ImGui::IsWindowHovered(ImGuiHoveredFlags_RootAndChildWindows))
        {
            ImGui::CloseCurrentPopup();
            show_Context_Menu = true;					// 서브를 불러서 팝업에서 팝업을 부름 => 삭제
            ImGui::OpenPopup("Context");
        }

        //if (ImGui::IsMouseReleased(0) && ImGui::IsWindowHovered(ImGuiHoveredFlags_RootAndChildWindows) && selectedObjectIndex == -1)
        //{
        //    int a = 0;
        //}

        // "Hierarchy" 윈도우 내에서 컨텍스트 메뉴 호출
        if (show_Context_Menu)
        {
            if (ImGui::BeginPopupContextItem("Context"))
            {
                if (selectedObjectIndex == -1)
                {
					if (ImGui::MenuItem("Create Empty"))
					{
						std::string objectName = "Create Empty";
						// 조건문으로 하기
						CreateObject(objectName, _createEmptyCount);
						//_debugWindow->SetDebugType(DebugType::CreateObject);
						_DebugType = DebugType::CreateObject;
					}
					if (ImGui::MenuItem("Cube"))  // 이 부분은 클릭할때 추가로 띄워야하기에 수정해야함
					{
						std::string objectName = "Cube";
						CreateObject(objectName, _cubeCount);
						//_debugWindow->SetDebugType(DebugType::CreateObject);
						_DebugType = DebugType::CreateObject;
					}
					if (ImGui::MenuItem("Sphere"))
					{
						std::string objectName = "Sphere";
						CreateObject(objectName, _sphereCount);
						//_debugWindow->SetDebugType(DebugType::CreateObject);
						_DebugType = DebugType::CreateObject;
					}
                }
                if (!_gameObjectlist.empty() && selectedObjectIndex != -1)
                {
                    if (ImGui::MenuItem("Delete"))
                    {
						//_debugWindow->SetDebugType(DebugType::DeleteObject);
						_DebugType = DebugType::DeleteObject;
						_deleteObjectName = _gameObjectlist[selectedObjectIndex]->GetObjectName();

						DeleteObject(_gameObjectlist, selectedObjectIndex);

                    }
                }

                ImGui::EndPopup();

            }
        }

        if (ImGui::IsWindowFocused(ImGuiFocusedFlags_RootAndChildWindows))
        {
            if (ImGui::IsKeyReleased(ImGuiKey_Delete) && !_gameObjectlist.empty() && selectedObjectIndex != -1)
            {
				//_debugWindow->SetDebugType(DebugType::DeleteObject);
				_DebugType = DebugType::DeleteObject;
				_deleteObjectName = _gameObjectlist[selectedObjectIndex]->GetObjectName();

				DeleteObject(_gameObjectlist, selectedObjectIndex);

            }

        }


}


void EpicTool::HierarchyWindow::CreateObject(std::string objectName, int& count)
{
	_gameObjectlist = KunrealEngine::GetCurrentScene()->
		GetObjectList();

    int tempCount = count;

	auto iter = find_if(_gameObjectlist.begin(), _gameObjectlist.end(), [&](KunrealEngine::GameObject* object)
		{
			return object->GetObjectName() == objectName;
		});

	if (iter != _gameObjectlist.end())
	{
		KunrealEngine::GetScene("Main")->CreateObject(objectName + "(" + std::to_string(count) + ")");
        count = tempCount + 1;
	}
	else
	{
        KunrealEngine::GetScene("Main")->CreateObject(objectName);
        ++count;
	}
}

void EpicTool::HierarchyWindow::DeleteObject(std::vector<KunrealEngine::GameObject*>& gameObjectlist, int& selectedObjectIndex)
{
    ImGui::Separator();
    std::string deleteObjectName = gameObjectlist[selectedObjectIndex]->GetObjectName();
    selectedObjectIndex = -1;

    KunrealEngine::GetCurrentScene()->DeleteGameObject(deleteObjectName);

    ImGui::Separator();
}

void EpicTool::HierarchyWindow::UpdateGameObject()  // 리스트 업데이트 부분 현재는 사용 x
{
	if (_draggedIndex != -1 && _dropTargetIndex != -1)
	{
        KunrealEngine::GameObject* draggedItem = _gameObjectlist[_draggedIndex];
        _gameObjectlist.erase(_gameObjectlist.begin() + _draggedIndex);
        _gameObjectlist.insert(_gameObjectlist.begin() + _dropTargetIndex, draggedItem);

        _draggedIndex = -1;
        _dropTargetIndex = -1;
    }
}

void EpicTool::HierarchyWindow::GetDebugType(DebugType& instance)
{
	instance = _DebugType;
}

void EpicTool::HierarchyWindow::GetDeleteObjectName(std::string& objectName)
{
	objectName = _deleteObjectName;
}

//bool EpicTool::HierarchyWindow::GetIsHierarchySelected()
//{
//	return this->_isHierarchySelected;
//}

void EpicTool::HierarchyWindow::ShowWindow()
{

}


void EpicTool::HierarchyWindow::ShowWindow(bool* p_open, std::vector<Object>& object)
{

}
