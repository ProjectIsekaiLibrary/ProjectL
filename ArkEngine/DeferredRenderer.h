/// <summary>
/// 디퍼드 렌더링을 진행하기 위한
/// 렌더러 클래스
/// 
/// 윤종화
/// </summary>

#pragma once

struct ID3DX11EffectTechnique;
struct ID3DX11EffectVariable;
struct ID3DX11EffectScalarVariable;
struct ID3DX11EffectVectorVariable;
struct ID3DX11EffectShaderResourceVariable;

namespace ArkEngine
{
	class ICamera;

	namespace ArkDX11
	{
		class deferredBuffer;
		class ArkDevice;
		class ArkEffect;
		class ArkBuffer;
		struct Material;
	}
}

namespace DirectX
{
	struct XMFLOAT3;
}

namespace ArkEngine
{
	namespace ArkDX11
	{
		class DeferredRenderer
		{
		public:
			DeferredRenderer(int clientWidth, int clientHeight);
			~DeferredRenderer();

		public:
			void Initailize();
			void BeginRender();
			void Update(ArkEngine::ICamera* pCamera);
			void Render();
			void Finalize();

		public:
			ArkEngine::ArkDX11::deferredBuffer* GetDeferredBuffer();

		private:
			void SetEffect();
			void BuildQuadBuffers();

		private:
			void SetDirLight();
			void SetPointLight();

		private:
			ID3DX11EffectTechnique* _tech;

			ID3DX11EffectScalarVariable* _fxDirLightCount;
			ID3DX11EffectScalarVariable* _fxPointLightCount;


			ID3DX11EffectVariable* _fxDirLights;
			ID3DX11EffectVariable* _fxPointLights;

			ID3DX11EffectVectorVariable* _fxEyePosW;

			ID3DX11EffectShaderResourceVariable* _positionMap;
			ID3DX11EffectShaderResourceVariable* _normalMap;
			ID3DX11EffectShaderResourceVariable* _diffuseMap;
			ID3DX11EffectShaderResourceVariable* _emissionMap;
			ID3DX11EffectShaderResourceVariable* _materialMap;



		private:
			ArkEngine::ArkDX11::deferredBuffer* _deferredBuffer;

		private:
			DirectX::XMFLOAT3 _eyePosW;

		private:
			ArkEngine::ArkDX11::ArkDevice* _arkDevice;
			ArkEngine::ArkDX11::ArkEffect* _arkEffect;
			ArkEngine::ArkDX11::ArkBuffer* _arkBuffer;


		private:
			float _backGroundColor[4];

		};
	}
}


