#pragma once

#include <DirectXMath.h>
#include "CommonHeader.h"
#include "Component.h"

namespace GInterface
{
	class GraphicsParticle;
}

namespace KunrealEngine
{
	class _DECLSPEC Particle : public Component
	{
		friend class GameObject;
	public:
		Particle();
		~Particle();

		void Initialize() override;
		void Release() override;

		void FixedUpdate() override;
		void Update() override;
		void LateUpdate() override;

		void OnTriggerEnter() override;
		void OnTriggerStay() override;
		void OnTriggerExit() override;

		void SetActive(bool active) override;

	private:
		// 그래픽스의 파티클 객체
		GInterface::GraphicsParticle* _particle;

	public:
		// 어떤 파티클을 출력할 것인가
		void SetParticleEffect(std::string name, std::string fileName, unsigned int maxParticle);

		// 파티클 방출 혹은 분사 
		void Start();

		// 파티클 방출/분사를 중단
		void Stop();

		// 파티클을 처음 위치부터 다시 분사
		void Reset();

		// 파티클 위치
		void SetParticlePos(float x, float y, float z);

		// 파티클 크기 설정
		void SetParticleSize(float x, float y);

		// 파티클 방출 강도 및 방출각도를 무작위로 할것인지 여부
		void SetParticleVelocity(float velocity, bool isRandom);

		// 파티클이 fadeout 되는 시간 및 완전히 사라지는 시간 설정	// 두 개가 같을 필요 없음
		void SetParticleDuration(float fade, float life);

		// 파티클에 색을 입히는 함수	// 기본적으로 색을 가지고 있음
		// 0.0 ~ 1.0
		void AddParticleColor(float x, float y, float z);

		// 파티클 방출 방향 설정
		void SetParticleDirection(float x, float y, float z);

	};
}

