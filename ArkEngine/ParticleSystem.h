/// <summary>
/// 파티클을 생성 하기 위한 클래스
/// 
/// 김현재
/// </summary>
#pragma once

struct ID3D11Texture2D;
struct ID3D11DepthStencilView;
struct ID3D11ShaderResourceView;
struct ID3D11RenderTargetView;

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
			ParticleSystem();
			~ParticleSystem();

		public:
			// 시스템이 (재)설정된 후 흐른 시간
			float GetAge() const;

			void SetEyePos(const DirectX::XMFLOAT3& eyePosW);
			void SetEmitPos(const DirectX::XMFLOAT3& emitPosW);
			void SetEmitDir(const DirectX::XMFLOAT3& emitDirW);

			void Initialize(ID3D11Device* device, ParticleEffect* fx,
				ID3D11ShaderResourceView* texArraySRV, ID3D11ShaderResourceView* randomTexSRV,
				unsigned int maxParticle);

			void Reset();
			void Update(float deltaTime, float gameTime);
			void Draw(ID3D11DeviceContext* dc, ArkEngine::ICamera* p_Camera);

		private:
			void BuildVB(ID3D11Device* device);

			ParticleSystem(const ParticleSystem& rhs);
			ParticleSystem& operator = (const ParticleSystem& rhs);


		private:
			unsigned int _maxParticles;
			bool _firstRun;

			float _gameTime;
			float _timeStep;
			float _age;

			DirectX::XMFLOAT3 _eyePosW;
			DirectX::XMFLOAT3 _emitPosW;
			DirectX::XMFLOAT3 _emitDirW;

			ParticleEffect* _fx;

			ID3D11Buffer* _initVB;
			ID3D11Buffer* _drawVB;
			ID3D11Buffer* _streamOutVB;

			ID3D11ShaderResourceView* _texArraySRV;
			ID3D11ShaderResourceView* _randomTexSRV;

		private:
			ArkDevice* _arkDevice;
			ArkEffect* _arkEffect;

		};
	}

}


