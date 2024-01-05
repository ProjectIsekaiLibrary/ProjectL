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
		void InitializeInputSystem();
		void Finalize();

	private:
		void Update();
		void FixedUpdate();
		void LateUpdate();

	public:
		void UpdateAll();

		// 그래픽스 엔진 사용법에 따라 수정될 예정
		void Render();

		void* GetRenderingImage();

		void* GetDevice();

		void* GetDeviceContext();

		void* GetRenderTargetView();

	private:
		// 그래픽스 엔진 객체가 들어갈 부분	
		GInterface::GraphicsInterface* _gInterface;

	/// 임시로 델타타임
	public:
		float GetDeltaTime();
	};
}

