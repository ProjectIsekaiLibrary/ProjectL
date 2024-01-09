/// <summary>
/// 디퍼드 렌더링을 위한 
/// 디퍼드 버퍼를 생성하기 위한 클래스
/// 
/// 윤종화
/// </summary>

#pragma once
struct ID3D11Texture2D;
struct ID3D11RenderTargetView;
struct ID3D11ShaderResourceView;


// GBuffer에 들어갈 요소들
enum class eBUFFERTYPE
{
	GBUFFER_POSITION,
	GBUFFER_DIFFUSE,
	GBUFFER_NORMALMAP,
	GBUFFER_DEPTH,

	// G버퍼 요소의 총 갯수
	GBUFFER_COUNT		
};


namespace ArkEngine
{
	namespace ArkDX11
	{
		class ArkDevice;

		class deferredBuffer
		{
		public:
			deferredBuffer(int width, int height);
			~deferredBuffer();

		public:
			void Initialize();
			void Finalize();

		public:
			void SetRenderTargets();
			void ClearRenderTargets(float color[4]);

		public:
			ID3D11ShaderResourceView* GetSRV(int index);

		private:
			void CreateRenderTargetTexture(ArkEngine::ArkDX11::ArkDevice* pDeivce);
			void CreateRenderTargetView(ArkEngine::ArkDX11::ArkDevice* pDeivce);
			void CreateShaderResourceView(ArkEngine::ArkDX11::ArkDevice* pDeivce);

		private:
			int _textureWidth;
			int _textureHeight;

			// texture를 또 사용할 일이 없으면 srv까지 만든 후에 release 시키고 멤버로도 가지고 있지 않을 예정
			ID3D11Texture2D* _renderTargetTextureArray[static_cast<int>(eBUFFERTYPE::GBUFFER_COUNT)];
			ID3D11RenderTargetView* _renderTargetViewArray[static_cast<int>(eBUFFERTYPE::GBUFFER_COUNT)];
			ID3D11ShaderResourceView* _shaderResourceViewArray[static_cast<int>(eBUFFERTYPE::GBUFFER_COUNT)];
		};
	}
}