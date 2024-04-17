#pragma once
#include "IWindow.h"



namespace EpicTool
{
	class NavimashEditor : public IWindow
	{
	public:
		NavimashEditor();
		~NavimashEditor();

		virtual void ShowWindow() override; // 실제로 창을 출력하는 함수
		// virtual void ShowWindow(bool* p_open, std::vector<Object> object) override;
		virtual void ShowWindow(bool* _open, std::vector<Object>& object) override; // 기능없음

		void Initialize();

	private:
		float AgentHeight;
		float AgentRadius;
		float AgentMaxClimb;
		float AgentMaxSlope;
	};
}
