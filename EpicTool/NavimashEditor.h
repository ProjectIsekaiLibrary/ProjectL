#pragma once
#include "IWindow.h"


namespace KunrealEngine
{
	class Navigation;
}

struct ImDrawList;
struct ImVec2;

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
		void DrawCylinder(ImDrawList* drawList, ImVec2 windowPos, ImVec2 windowSize, float centerX, float centerY, float radius, float height);

	private:
		int _naviIndex;

		float _agentHeight;
		float _agentRadius;
		float _agentMaxClimb;
		float _agentMaxSlope;

		KunrealEngine::Navigation* _navimashEditor;
	};
}
