#pragma once
#include <windows.h>
#include "CommonHeader.h"

/// <summary>
/// 게임엔진의 코어 부분
/// 게임엔진 구동에 필요한 전반적인 부분을 초기화 및 구동
/// 
/// 이건호
/// </summary>

/// <summary>
/// 그래픽스 엔진 전방선언 부분
/// </summary>

namespace GInterface
{
	class GraphicsInterface;
	class GraphicsImage;
}

namespace KunrealEngine
{
	class SceneManager;

	class _DECLSPEC EngineCore
	{
	/// private으로 나중에 바꿔
	public:
		EngineCore();
		~EngineCore();

	public:
		void Initialize(HWND hwnd, HINSTANCE hInstance, int screenWidth, int screenHeight);
		void Release();

	private:
		void Update();
		void FixedUpdate();
		void LateUpdate();

	public:
		void UpdateAll();

		// 그래픽스 엔진 사용법에 따라 수정될 예정
		void Render();

		void SetEditorMousePos(POINT position);


	public:
		// 로직 테스트할때 여기다 다 만들어 함수 새로 파고 싶으면 또 만들어도 돼
		void PlayGround();
		void CheckMousePosition();
		void MakeObstacle();
		void RemoveObstacle();
		void ParticleTest();

	private:
		// 그래픽스 엔진 객체가 들어갈 부분	
		GInterface::GraphicsInterface* _gInterface;



		/// 임시용 이미지 컴포넌트
		GInterface::GraphicsImage* cursorimage;

	/// 임시로 델타타임
	public:
		float GetDeltaTime();

	private:
		// 인게임 마우스 포지션
		int _ingameMouseX = 0;
		int _ingameMouseY = 0;

		//임시용 에디터 마우스 포지션
		POINT _editorMousepos = {-1, -1};

		POINT _finalMousePosition = { 0,0 };

		float _timeMan = 1.0f;

		bool _isEditor;			// 에디터 실행인지
	};
}

