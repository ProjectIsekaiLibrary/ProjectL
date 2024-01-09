#pragma once
#include <vector>
#include "IWindow.h"

/// <summary>
/// 코드 분리 작업중
/// 하이어라키 부분을 담당함
/// </summary>

namespace KunrealEngine
{
    class GameObject;
}

namespace EpicTool
{

    class HierarchyWindow : public IWindow // 인터페이스가 필요할지 고민된다
    {
    public:
        HierarchyWindow();
        ~HierarchyWindow();

    public:

        void Initialize();

        virtual void ShowWindow() override; // 창을 출력
        virtual void ShowWindow(bool* p_open, std::vector<Object>& object) override;

        void ShowWindow(int& selectedObjectIndex);  // 하이어라키와 인스펙터에서만 사용

        void CreateObject(std::string objectName, int& count);

        void DeleteObject(std::vector<KunrealEngine::GameObject*>& gameObjectlist, int& selectedObjectIndex);

        void UpdateGameObject();  // 오브젝트의 순서를 바꿈

        //void RenderDragBox(int& selectedObjectIndex); // 드래그 시 작동을 관리 



    private:
        // 각 오브젝트의 갯수
        int _createEmptyCount;
        int _cubeCount;
        int _sphereCount;

        // 오브젝트 드롭을 관리할 변수
		int _draggedIndex;
		int _dropTargetIndex;

        // imgui 창을 계산하기 위한 변수
		float _hierarchyWindowX1;
		float _hierarchyWindowY1;
		float _hierarchyWindowX2;
		float _hierarchyWindowY2;

        //오브젝트 리스트
        std::vector<KunrealEngine::GameObject*> _gameObjectlist;

        // 드래그시 출현하는 반투명 박스 위치

    };
}

