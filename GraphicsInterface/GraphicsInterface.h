/// <summary>
/// 게임 엔진과 그래픽스 엔진을 연결해주는 인터페이스
/// 상의하에 계속해서 추가하고 수정해 나갈 예정
/// 
/// 이건호 윤종화
/// </summary>

/// <summary>
/// IRenderable = 3D 오브젝트
/// </summary>

#pragma once
#include <string>
#include <vector>

namespace KunrealEngine
{
	namespace KunrealMath
	{
		struct Float3;
	}
}

namespace DirectX
{
	struct XMFLOAT3;
	struct XMFLOAT4;
	struct XMMATRIX;
}

namespace ArkEngine
{
	class IRenderable;
}

namespace GInterface
{
	class GraphicsRenderable;
	class GraphicsCamera;
	class GraphicsDirLight;
	class GraphicsPointLight;

	class GraphicsInterface
	{
	public:
		virtual ~GraphicsInterface() {}

	public:
		// 그래픽스 엔진 생성 후 초기화 할 때 사용
		virtual void Initialize(long long hwnd, int clientWidth, int clientHeight) abstract;
		// 그래픽스 엔진 생성 후 원하는 색깔로 초기화 할 때 사용
		virtual void Initialize(long long hwnd, int clientWidth, int clientHeight, float backGroundColor[4]) abstract;
		// 그래픽스 엔진 내의 모든 객체 업데이트
		virtual void Update() abstract;
		// 그래픽스 엔진 내의 모든 객체 렌더링
		virtual void Render() abstract;
		// 그래픽스 엔진이 종료될 때 사용
		virtual void Finalize() abstract;

	public:
		// 렌더러블 오브젝트를 생성 후 이를 반환
		virtual GInterface::GraphicsRenderable* CreateRenderable(const char* fileName, const char* textureName = nullptr, bool isSold = true) abstract;
		// 렌더러블 오브젝트 삭제
		virtual void DeleteRenderable(GInterface::GraphicsRenderable* renderable) abstract;

	public:
		// 큐브맵 생성
		virtual void CreateCubeMap(const char* cubeMapName, const char* textureName="", bool isCube = false) abstract;
		// 큐브맵 삭제
		virtual void DeleteCubeMap(const char* cubeMapName) abstract;
		// 큐브맵 리스트 가져오기
		virtual std::vector<std::string> GetCubeMapList() abstract;
		// 현재 렌더링 되는 메인 큐브맵 가져오기
		virtual void SetMainCubeMap(std::string cubeMapName) abstract;

	public:
		// 카메라를 생성 후 반환
		virtual GInterface::GraphicsCamera* CreateCamera(KunrealEngine::KunrealMath::Float3 cameraPosition = { 0.0f, 0.0f, 0.0f }, KunrealEngine::KunrealMath::Float3 targetPosition = { 0.0f, 0.0f, 0.0f }) abstract;
		// 카메라 객체를 삭제
		virtual void DeleteCamera(GInterface::GraphicsCamera* camera) abstract;
		// 현재 렌더링하고 있는 메인 카메라 가져오기
		virtual GInterface::GraphicsCamera* GetMainCamera() abstract;
		// 매개인자로 넣은 카메라를 메인 카메라로 설정
		virtual void SetMainCamera(GInterface::GraphicsCamera* camera) abstract;

	public:
		// Directional Light 생성
		virtual GInterface::GraphicsDirLight* CreateDirectionalLight(KunrealEngine::KunrealMath::Float4 ambient, KunrealEngine::KunrealMath::Float4 diffuse, KunrealEngine::KunrealMath::Float4 specular, KunrealEngine::KunrealMath::Float3 direction) abstract;
		virtual GInterface::GraphicsPointLight* CreatePointLight(KunrealEngine::KunrealMath::Float4 ambient, KunrealEngine::KunrealMath::Float4 diffuse, KunrealEngine::KunrealMath::Float4 specular, KunrealEngine::KunrealMath::Float3 position, float range) abstract;


	public:
		// End 키를 통해 텍스트 출력 활성화 / 비활성화
		virtual void DrawDebugText(int posX, int posY, const char* text, ...) abstract;

	public:
		// IMGUI에서 렌더링시키기 위해 이미지를 넘겨줌
		virtual void* GetRenderingImage() abstract;

		// IMGUI에 디바이스를 넘겨주기 위함
		virtual void* GetDevice() abstract;

		// IMGUI에 디바이스 컨텍스트를 넘겨주기 위함
		virtual void* GetDeviceContext() abstract;

		// IMGUI에 RenderTargetView를 넘겨주기 위함
		virtual void* GetRenderTargetView() abstract;

		// IMGUI에 그릴 수 있는 파싱된 오브젝트의 파일명을 넘겨주기 위함
		virtual const std::vector<std::string> GetRenderableNameList() abstract;

		// IMGUI에 그릴 수 있는 파싱된 텍스쳐의 파일명을 넘겨주기 위함
		virtual const std::vector<std::string> GetTextureNameList() abstract;
	};

	//public:
	//	//그래픽스 엔진 초기화
	//	virtual void Initialize(long long hWnd, int width, int height) abstract;
	//	//배경색까지 포함한 초기화
	//	//카메라에 따라서 배경색을 다르게?
	//	virtual void Initialize(long long hWnd, int width, int height, float bgColor[4]) abstract;
	//
	//	//그린다
	//	virtual void BeginRender() abstract;
	//	virtual void EndRender() abstract;
	//
	//	//그래픽스 엔진에 자료를 넘겨줘야한다 매개변수는 임시
	//	//가만히 있는 오브젝트인데도 계속 넘겨준다? 비효율 이를 위한거 하나 더 만들자
	//	//넘겨줄 구조체를 생각해보자 나중에 분리하더라도
	//	virtual void SetMeshData(DirectX::XMMATRIX worldTM) abstract;
	//	virtual void SetSkinnedMeshData(DirectX::XMMATRIX worldTM) abstract;
	//
	//	virtual void 그려줘() abstract;
	//
	//	//큐브 그려줘
	//	//이렇게 하나하나 다 하지말고 넘겨줄 구조체를 구상해서 그에 맞게 렌더가 되도록
	//	//일단 그려보고
	//	virtual void DrawMesh(DirectX::XMMATRIX worldTM, float 무엇인가들어갈것입니다) abstract;
	//
	//	//MeshObject
	//	//파일 이름을 적는게 아니라 파일이 있는 폴더 경로를 입력
	//	//??
	//	virtual ArkEngine::IRenderable* CreateRenderable(const char* fbxPath) abstract;
	//
	//	virtual void Finalize() abstract;
};

	// 그래픽스 엔진을 생성한 후 반환
	extern "C" __declspec(dllexport) GInterface::GraphicsInterface* CreateGraphicsEngine();

	// 그래픽스 엔진을 종료
	extern "C" __declspec(dllexport) void ReleaseGraphicsEngine(GInterface::GraphicsInterface* instance);