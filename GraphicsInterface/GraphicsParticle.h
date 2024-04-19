#pragma once

namespace GInterface
{
	class GraphicsParticle
	{
		virtual ~GraphicsParticle() {};

	public:
		// 파티클 실행
		virtual void play() abstract;
		// 파티클 멈춤
		virtual void Pause() abstract;
		// 파티클 종료
		virtual void Stop() abstract;

	public:
		// 파티클 사이즈 가져오기
		virtual DirectX::XMFLOAT2 GetSize() abstract;
		// 파티클 속도 가져오기
		virtual float GetVelocity() abstract;
		// 파티클 랜덤 여부 가져오기
		virtual bool GetRandom() abstract;

	public:
		// 파티클 사이즈 설정
		virtual void SetSize(float x, float y) abstract;
		// 파티클 속도 설정
		virtual void SetVelocity(float speed) abstract;
		// 파티클 랜덤값 부여 여부 설정
		virtual void SetRandom(bool tf) abstract;
	};
}