/// <summary>
/// 파티클을 생성 하기 위한 클래스
/// 
/// 김현재
/// </summary>
#pragma once
#include <random>


struct ID3D11Texture2D;
struct ID3D11DepthStencilView;
struct ID3D11ShaderResourceView;
struct ID3D11RenderTargetView;
struct ID3DX11EffectTechnique;
struct ID3DX11EffectMatrixVariable;
struct ID3DX11EffectScalarVariable;
struct ID3DX11EffectShaderResourceVariable;
struct ID3DX11Effect;

struct Particle
{
	DirectX::XMFLOAT3 InitialPos;
	DirectX::XMFLOAT3 InitialVel;
	DirectX::XMFLOAT2 Size;
	float Age;
	unsigned int Type;
};

namespace ArkEngine
{
	class ICamera;

	namespace ArkDX11
	{
		class ArkDevice;
		class ArkEffect;
		class Camera;
		class ParticleEffect;

		class ParticleSystem
		{
		public:
			ParticleSystem(const std::string& particleName, const std::string& fileName, unsigned int maxParticle);
			ParticleSystem(const std::string& particleName, const std::vector<std::string>& fileNameList, unsigned int maxParticle);
			~ParticleSystem();

		public:
			void Update(float deltaTime, float gameTime);
			void Draw(ArkEngine::ICamera* p_Camera);

		public:
			void Reset();
			void SetEmitPos(const DirectX::XMFLOAT3& emitPosW);
			void SetEmitDir(const DirectX::XMFLOAT3& emitDirW);
			
			void SetParticleSize(const DirectX::XMFLOAT2& particleSize);
			void SetEmitVelocity(float emitVelocity, bool isRandom);
			void SetParticleTime(float particleFadeTime, float particleLifeTime);

			float GetRandomFloat(float minNum, float maxNum);
			void SetEyePos(const DirectX::XMFLOAT3& eyePosW);
		private:
			// 시스템이 (재)설정된 후 흐른 시간
			float GetAge() const;

			
			void Initialize(const std::wstring& fileNameList, unsigned int maxParticle);
			void Initialize(const std::vector<std::wstring>& fileName, unsigned int maxParticle);

			ID3D11ShaderResourceView* CreateRandomTextureSRV();
			ID3D11ShaderResourceView* CreateTexture2DArraySRV(const std::vector<std::wstring>& fileNameList);
			ID3D11ShaderResourceView* CreateTexture2DArraySRV(const std::wstring& fileName);

		private:
			void BuildVB();
			void BuildDrawStreamVB();
			void SetEffect();

			
		private:
			bool _isRandom;
			float _particleLifeTime;
			float _particleFadeTime;
			std::string _fileName;

		private:
			unsigned int _maxParticles;
			bool _firstRun;

			float _gameTime;
			float _timeStep;
			float _age;

			DirectX::XMFLOAT3 _eyePosW;
			DirectX::XMFLOAT3 _emitPosW;
			DirectX::XMFLOAT3 _emitDirW;

			ID3D11Buffer* _initVB;
			ID3D11Buffer* _drawVB;
			ID3D11Buffer* _streamOutVB;

			ID3D11ShaderResourceView* _texArraySRV;

			ID3DX11EffectTechnique* _streamOutTech;
			ID3DX11EffectTechnique* _drawTech;

			ID3DX11EffectMatrixVariable* _viewProjEffect;
			ID3DX11EffectScalarVariable* _gameTimeEffect;
			ID3DX11EffectScalarVariable* _timeStepEffect;
			ID3DX11EffectVectorVariable* _eyePosWEffect;
			ID3DX11EffectVectorVariable* _emitPosWEffect;
			ID3DX11EffectVectorVariable* _emitDirWEffect;
			// 추가
			ID3DX11EffectVectorVariable* _particleSizeEffect;
			ID3DX11EffectVectorVariable* _emitVelocityEffect;
			ID3DX11EffectScalarVariable* _isRandomEffect;
			ID3DX11EffectScalarVariable* _particleFadeTimeEffect;
			ID3DX11EffectScalarVariable* _particleLifeTimeEffect;


			ID3DX11EffectShaderResourceVariable* _texArray;
			ID3DX11EffectShaderResourceVariable* _randomTex;

			DirectX::XMFLOAT3 _emitVelocity;
			DirectX::XMFLOAT2 _particleSize;

		private:
			std::string _particleName;
			ArkDevice* _arkDevice;
			ArkEffect* _arkEffect;

			void SetViewProj(DirectX::CXMMATRIX m);

			void SetGameTime(float f);
			void SetTimeStep(float f);

			void SetEyePosW(const DirectX::XMFLOAT3& v);
			void SetEmitPosW(const DirectX::XMFLOAT3& v);
			void SetEmitDirW(const DirectX::XMFLOAT3& v);

			void SetTexArray(ID3D11ShaderResourceView* tex);
			void SetRandomTex(ID3D11ShaderResourceView* tex);

			void SetParticleSizeW(const DirectX::XMFLOAT2& v);
			void SetEmitVelocityW(const DirectX::XMFLOAT3& v);

			void SetParticleTimeW(float f1, float f2);
		};
	}

}


