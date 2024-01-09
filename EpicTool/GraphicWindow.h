#pragma once
#include "IWindow.h"


/// <summary>
/// 그래픽스 엔진에서 받은 화면을 띄울 창
/// 2023.10.19 배성근
/// </summary>
namespace EpicTool
{
    class GraphicWindow : public IWindow
    {
    public:
        GraphicWindow();
        ~GraphicWindow();

    public:

        virtual void ShowWindow() override; // 실제로 창을 출력하는 함수
        // virtual void ShowWindow(bool* p_open, std::vector<Object> object) override;
        virtual void ShowWindow(bool* _open, std::vector<Object>& object) override; // 기능없음

        void ShowWindow(void* _texture);
    };
}
