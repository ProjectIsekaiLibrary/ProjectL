#pragma once
#include <windows.h>
#include "CommonHeader.h"
#include "../GraphicsInterface/GraphicsHeaders.h"

/// <summary>
/// 그래픽스 인터페이스 클래스의 기능을 다양한 부분에서 사용하기 위한 레이어
/// 싱글톤화해서 어디서든 호출 가능하게
/// 
/// 이건호
/// </summary>

#define GRAPHICS KunrealEngine::GraphicsSystem::GetInstance().GetGraphics()

namespace GInterface
{
	class GraphicsInterface;
	class GraphicsCamera;
}

namespace KunrealEngine
{
	class _DECLSPEC GraphicsSystem
	{
	private:
		GraphicsSystem();
		~GraphicsSystem();

	public:
		// 싱글톤 객체 반환
		static GraphicsSystem& GetInstance();

		void Initialize(HWND hwnd, int screenWidth, int screenHeight);
		void Finalize();

		GInterface::GraphicsInterface* GetGraphics();

	public:
		// 큐브맵 생성
		void CreateCubeMap(const char* cubeMapName, const char* textureName = "", bool isCube = false);

		// 큐브맵 삭제	// 생성 시 사용했던 이름으로 탐색해서 삭제함
		void DeleteCubeMap(const char* cubeMapName);

		// 화면에 출력 될 메인 큐브맵 설정
		// 현재 프로젝트에서는 하나의 큐브맵만 출력 가능
		void SetMainCubeMap(std::string cubeMapName);

		// 큐브맵 리스트 반환
		const std::vector<std::string> GetCubeMapList();

	public:
		const std::vector<std::string> GetRenderableList();

		const std::vector<std::string> GetTextureList();
		 
	private:
		GInterface::GraphicsInterface* _graphics;
	};
}
