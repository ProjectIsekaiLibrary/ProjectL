#pragma once

namespace GInterface
{
	class GraphicsParticle
	{
	public:
		virtual ~GraphicsParticle() {};

	public:
		// 파티클 실행
		//virtual void play() abstract;
		//// 파티클 멈춤
		//virtual void Pause() abstract;
		//// 파티클 종료
		//virtual void Stop() abstract;

	public:
		//// 파티클 사이즈 가져오기
		//virtual DirectX::XMFLOAT2 GetSize() abstract;
		//// 파티클 속도 가져오기
		//virtual float GetVelocity() abstract;
		//// 파티클 랜덤 여부 가져오기
		//virtual bool GetRandom() abstract;

	public:
		//// 파티클 사이즈 설정
		//virtual void SetSize(float x, float y) abstract;
		//// 파티클 속도 설정
		//virtual void SetVelocity(float speed) abstract;
		//// 파티클 랜덤값 부여 여부 설정
		//virtual void SetRandom(bool tf) abstract;

		// 파티클의 위치 설정
		virtual void SetEmitPos(const DirectX::XMFLOAT3& emitPos) abstract;
		// 파티클의 크기 설정
		virtual void SetParticleSize(const DirectX::XMFLOAT2& particleSize) abstract;
		// 파티클의 방출 쌔기와 랜덤방출 설정
		virtual void SetEmitVelocity(float emitVelocity, bool isRandom) abstract;
		// 파티클의 방출 후 점점색이 바래지는 시간(Fade)과 완전소멸시간(Life) 설정
		virtual void SetParticleTime(float particleFadeTime, float particleLifeTime) abstract;
		// 파티클의 색상 설정(RGB)
		virtual void SetParticleColor(const DirectX::XMFLOAT3& particleColor) abstract;
		// 파티클의 방출 방향 설정
		virtual void SetParticleDirection(const DirectX::XMFLOAT3& particleDirection) abstract;
	
		// 파티클을 다시 0부터 시작하게 만든다
		virtual void Reset() abstract;
	};
}