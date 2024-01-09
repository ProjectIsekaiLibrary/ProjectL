#pragma once
#include <vector>
#include "IWindow.h"

/// <summary>
/// 하이어라키, 인스펙터를 관리
/// 다른 창을 모두 관리할지 고민중
/// 2023.10.19 배성근
/// </summary>

/// <summary>
/// 하이어라키와 인스펙터를 따로 분리하였다
/// </summary>

namespace KunrealEngine
{
    class GameObject;
}


namespace EpicTool
{
    class InspectorWindow;
    class HierarchyWindow;

    class ShowWindowManager : public IWindow
    {
    public:
        ShowWindowManager();
        ~ShowWindowManager();

    public:

        void Initialize();

        virtual void ShowWindow() override; // 창을 출력

        virtual void ShowWindow(bool* _open, std::vector<Object>& object) override;

    private:

        int _selectedObjectIndex;  // 정말 쓰고싶지않은데 일단 임시로

        // 각 윈도우의 객체 설정
        InspectorWindow* _inspector;
        HierarchyWindow* _Hierarchy;
    };
}
