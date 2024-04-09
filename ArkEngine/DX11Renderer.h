/// <summary>
/// IRenderer 인터페이스를 상속받은 렌더러
/// DX11을 이용한 렌더링
/// 
/// 윤종화
/// </summary>

#pragma once
#include <memory>
#include <wrl/client.h>
#include "IRenderer.h"
#include "GraphicsInterface.h"

enum D3D_FEATURE_LEVEL;
struct ID3D11Device;
struct ID3D11DeviceContext;
struct IDXGISwapChain;
struct ID3D11RenderTargetView;
struct ID3D11DepthStencilView;
struct ID3D11RasterizerState;
struct ID3D11DepthStencilState;
struct DXGI_SWAP_CHAIN_DESC;

namespace GInterface
{
	class GraphicsRenderable;
}

namespace ArkEngine
{
	class ICamera;
	class ICubeMap;
	class IdirLight;

	namespace ArkDX11
	{
		class Camera;
		class DeferredRenderer;
		class DWFont;

		// 추가
		class ParticleSystem;
	}

	namespace FBXLoader
	{
		class AssimpTool;
	}
}

namespace DirectX
{
	struct XMFLOAT3;
}

namespace KunrealEngine
{
	namespace KunrealMath
	{
		struct Float3;
	}
}

namespace ArkEngine
{
	namespace ArkDX11
	{
		class DX11Renderer : public GInterface::GraphicsInterface
		{
		public:
			DX11Renderer();
			~DX11Renderer();

		public:
			virtual void Initialize(long long hwnd, int clientWidth, int clientHeight) override;

			virtual void Initialize(long long hwnd, int clientWidth, int clientHeight, float backGroundColor[4]) override;

			virtual void Update() override;

			virtual void Render() override;

			virtual void Finalize() override;

		public:
			virtual GInterface::GraphicsRenderable* CreateRenderable(const char* fileName, bool isSold) override;
			virtual void DeleteRenderable(GInterface::GraphicsRenderable* renderable) override;
			virtual GInterface::GraphicsRenderable* GetPickedRenderable(int mouseX, int mouseY) override;

		public:
			virtual GInterface::GraphicsDebug* CreateDebugCube(const char* objectName, float width, float height, float depth) override;
			virtual GInterface::GraphicsDebug* CreateDebugSphere(const char* objectName, float radius) override;

			virtual void DeleteDebugObject(GInterface::GraphicsDebug* debugObject) override;

		public:
			virtual void CreateDebugLine(DirectX::XMFLOAT3 vertex1, DirectX::XMFLOAT3 vertex2, DirectX::XMFLOAT4 color) override;
			virtual void DeleteLine(int index) override;
			virtual void DeleteLine(DirectX::XMFLOAT3 vertex1, DirectX::XMFLOAT3 vertex2) override;
			virtual void DeleteAllLine() override;

		public:
			virtual void CreateCubeMap(const char* cubeMapName, const char* textureName, bool isCube) override;
			virtual void DeleteCubeMap(const char* cubeMapName) override;
			virtual std::vector<std::string> GetCubeMapList() override;
			virtual void SetMainCubeMap(std::string cubeMapName) override;

		public:
			virtual GInterface::GraphicsImage* CreateImage(const char* imageName) override;
			virtual void DeleteImage(GInterface::GraphicsImage* image) override;
			virtual GInterface::GraphicsImage* GetPickedImage(int mouseX, int mouseY) override;

		public:
			virtual GInterface::GraphicsCamera* CreateCamera(DirectX::XMFLOAT3 cameraPosition, DirectX::XMFLOAT3 targetPosition) override;
			virtual void DeleteCamera(GInterface::GraphicsCamera* camera) override;
			virtual GInterface::GraphicsCamera* GetMainCamera() override;
			virtual void SetMainCamera(GInterface::GraphicsCamera* camera) override;

		public:
			virtual GInterface::GraphicsDirLight* CreateDirectionalLight(DirectX::XMFLOAT4 ambient, DirectX::XMFLOAT4 diffuse, DirectX::XMFLOAT4 specular, DirectX::XMFLOAT3 direction) override;
			virtual GInterface::GraphicsPointLight* CreatePointLight(DirectX::XMFLOAT4 ambient, DirectX::XMFLOAT4 diffuse, DirectX::XMFLOAT4 specular, DirectX::XMFLOAT3 position, float range) override;

		public:
			// End 키를 눌렀을때 활성화되는 텍스트
			virtual void DrawDebugText(int posX, int posY, int fontSize, const char* text, ...) override;
			virtual void DrawColorText(int posX, int posY, int fontSize, DirectX::XMFLOAT4 color, const char* text, ...) override;

		public:
			virtual void* GetRenderingImage() override;

			virtual void* GetDevice() override;

			virtual void* GetDeviceContext() override;

			virtual void* GetRenderTargetView() override;

			virtual const std::vector<std::string> GetRenderableNameList() override;

			virtual const std::vector<std::string> GetTextureNameList() override;

		public:
			virtual DirectX::XMFLOAT3 ScreenToWorldPoint(int mouseX, int mouseY) override;

		public:
			// IMGUIZMO를 사용하기 위해 메인 카메라의 View행렬을 반환
			virtual const DirectX::XMFLOAT4X4& GetViewMatrix() override;

			// IMGUIZMO를 사용하기 위해 메인 카메라의 Proj행렬을 반환
			virtual const DirectX::XMFLOAT4X4& GetProjMatrix() override;

			// 특정 본을 따라가도록 하는 Transform 행렬 반환
			virtual const DirectX::XMFLOAT4X4& GetTransform(GInterface::GraphicsRenderable* renderable, const std::string& boneName) override;

			// 네비 메쉬를 위한 모든 메쉬들의 버텍스의 월드좌표 반환
			virtual const std::vector<std::vector<std::vector<DirectX::XMFLOAT3>>> GetAllMeshVertex() override;
			// 네비 메쉬를 위한 모든 인덱스들의 버텍스의 월드좌표 반환
			virtual const std::vector<std::vector<std::vector<unsigned int>>> GetAllMeshIndex() override;

		public:
			void CreateShadowViewPort(int shadowWidth, int shadowHeight);

		public:
			void CreateShadowCamera(DirectX::XMFLOAT3 cameraPosition, DirectX::XMFLOAT3 targetPosition);

		private:
			void BeginShadowRender();
			void BeginRender();
			void FinalRender();
			void EndRender();

		private:
			void CreateDevice();			// ID3D11Device 인터페이스와 DeviceContext 인터페이스를 생성
			void CheckQualification();	// 하드웨어가 4X MSAA를 위한 품질 수준을 지원하는지 점검
			DXGI_SWAP_CHAIN_DESC GetSwapChainDesc(long long hwnd);// 생성할 교환 사슬의 특성을 서술하는 DXGI_SWAP_CHAIN_DEXC 구조체를 채움
			void CreateSwapChain(long long hwnd);			// SWAP CHAIN을 생성
			void CreateRenderTargetView();	// SWAP CHAIN의 후면 버퍼에 대한 랜더 대상 뷰를 생성
			void CreateDepthStencilView();		// 깊이 스탠실 버퍼와 그에 연결되는 깊이 스탠실 뷰를 생성
			void BindView();				// 랜더타켓 뷰와 깊이 스탠실 뷰를 파이프 라인의 출력 병합기 단계에 묶기
			void SetViewport();
			void SetShadowViewport();				// 화면 크기로 뷰포트 설정
			void SetViewportWithDefaultCamera();				// 뷰포트 설정
			void CreateRenderState();		// 렌더 상태 집합
			void CreateDepthStecilState();	// 깊이 버퍼 설정

		private:
			void SetResourceManager();
			void CreateFont();
			void CreateEffect();
			void CreateTexture();
			void CreateNewTexture();
			void CreateASEParser();
			void CreateDefaultCamera(DirectX::XMFLOAT3 pos, DirectX::XMFLOAT3 target, DirectX::XMFLOAT3 worldUp);

		private:
			void InitializeAssimpTool();

		private:
			void InitializeWIC();

		private:
			float GetAspectRatio();
			float GetShadowRatio();

		private:
			void CreateDirLight(DirectX::XMFLOAT4 ambient, DirectX::XMFLOAT4 diffuse, DirectX::XMFLOAT4 specular, DirectX::XMFLOAT3 direction);
			void CreatePoLight(DirectX::XMFLOAT4 ambient, DirectX::XMFLOAT4 diffuse, DirectX::XMFLOAT4 specular, DirectX::XMFLOAT3 position, float range);

		private:
			void SetPickingTexture();
			UINT Picking(int mouseX, int mouseY);

		private:
			DirectX::XMFLOAT3 GetMyPosition(DirectX::XMFLOAT3 direction, DirectX::XMFLOAT3 targetPos);

		private:
			bool Equal(const DirectX::XMFLOAT3& a, const DirectX::XMFLOAT3& b)
			{
				return a.x == b.x && a.y == b.y && a.z == b.z;
			}

		private:
			Microsoft::WRL::ComPtr<ID3D11Device> _device;
			Microsoft::WRL::ComPtr<ID3D11DeviceContext> _deviceContext;

			unsigned int _4xMsaaQuality;

			bool _enable4xMsaa;

			Microsoft::WRL::ComPtr<IDXGISwapChain> _swapChain;

			Microsoft::WRL::ComPtr<ID3D11RenderTargetView> _renderTargetView;

			Microsoft::WRL::ComPtr <ID3D11DepthStencilView> _depthStencilView;

			Microsoft::WRL::ComPtr<ID3D11RasterizerState> _solidRS;
			Microsoft::WRL::ComPtr<ID3D11RasterizerState> _wireRS;
			Microsoft::WRL::ComPtr<ID3D11RasterizerState> _shadowRS;

			Microsoft::WRL::ComPtr<ID3D11DepthStencilState> _depthStencilState;
			Microsoft::WRL::ComPtr<ID3D11DepthStencilState> _depthStencilStateDisable;


			float _backGroundColor[4];

		private:
			std::unique_ptr<ArkEngine::ArkDX11::DWFont> _font;

			ArkEngine::ICamera* _originMainCamera;
			ArkEngine::ICamera* _mainCamera;


			ArkEngine::ICubeMap* _mainCubeMap;

		private:
			int _clientWidth;
			int _clientHeight;

			int _shadowWidth;
			int _shadowHeight;

			bool _isDebugMode;

		private:
			std::vector<ArkEngine::IdirLight*> _dirLightList;

		private:
			ID3D11ShaderResourceView* _renderingImageView;

		private:
			std::unique_ptr<ArkEngine::ArkDX11::DeferredRenderer> _deferredRenderer;

			/// <summary>
			/// 김현재 추가
			/// </summary>
		private:
			std::unique_ptr<ArkEngine::FBXLoader::AssimpTool> _assimpTool;

		private:
			ID3D11Texture2D* _colorTexture;

		private:
			D3D11_VIEWPORT _viewPort;
			D3D11_VIEWPORT _shadowViewPort;

			//추가
		private:
			std::unique_ptr<ArkEngine::ArkDX11::ParticleSystem> _particle;

		};
	}
}