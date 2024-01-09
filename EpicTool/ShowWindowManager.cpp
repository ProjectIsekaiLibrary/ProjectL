#include "ShowWindowManager.h"
#include "imgui.h"
#include "imgui_impl_dx11.h"
#include "FileSave.h"
#include "KunrealAPI.h"
#include "SceneManager.h"
#include "InspectorWindow.h"
#include "HierarchyWindow.h"

EpicTool::ShowWindowManager::ShowWindowManager()
    :_selectedObjectIndex(-1), _inspector(nullptr), _Hierarchy(nullptr)
{

}

EpicTool::ShowWindowManager::~ShowWindowManager()
{

}



void EpicTool::ShowWindowManager::Initialize()
{
    _inspector = new InspectorWindow();
    _inspector->Initialize();
    _Hierarchy = new HierarchyWindow();
}

void EpicTool::ShowWindowManager::ShowWindow()
{ 
    std::vector<KunrealEngine::GameObject*> gameObjectlist = KunrealEngine::GetCurrentScene()->
        GetObjectList();

    IM_ASSERT(ImGui::GetCurrentContext() != NULL && "Missing Dear ImGui context. Refer to examples app!");

    ImGui::Begin("Hierarchy");  

    _Hierarchy->ShowWindow(_selectedObjectIndex);

    ImGui::End();
   
   
   // 매우 하드스러운데 템플릿을 통해 수정해볼까, 컴포넌트 부분 실제로 컴포넌트를 생성하는게 아님
                          // 수정이 필요할것이다.
    ImGui::Begin("inspector");

    _inspector->ShowWindow(_selectedObjectIndex);

    ImGui::End();
   
}



void EpicTool::ShowWindowManager::ShowWindow(bool* _open, std::vector<Object>& object)
{

}
