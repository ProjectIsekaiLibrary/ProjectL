/// 2024.04.25 - 정진수(update 24.04.25)
/// 보스명 : 아라크네
/// 거미타입 보스
///
#pragma once
#include "KunrealAPI.h"
#include "Boss.h"
#include "Coroutine.h"

namespace KunrealEngine
{
	class _DECLSPEC Aracne : public Component, public Boss
	{
	public:
		Aracne();
		virtual ~Aracne();

	public:
		void Initialize() override;
		void Release() override;

		void FixedUpdate() override;
		void Update() override;
		void LateUpdate() override;

		void OnTriggerEnter() override;
		void OnTriggerStay() override;
		void OnTriggerExit() override;

		void SetActive(bool active) override;

		virtual void SetMesh() override;
		virtual void SetTexture() override;
		virtual void SetBossTransform() override;
		virtual void SetBossCollider() override;

	public:
		virtual void CreatePattern() override;

	private:
		void JumpAttack();
		void leftAttack();
		void RightAttack();
		void ShootingWeb();

	private:
		bool Move(DirectX::XMFLOAT3& startPos, DirectX::XMFLOAT3& targetPos, float speed);
		float CalculateParabolaHeight(DirectX::XMFLOAT3 start, DirectX::XMFLOAT3 end, DirectX::XMFLOAT2 current);

	private:	// 여긴 패턴만 넣어라
		BossPattern* _jumpAttack;

	private:	// 여긴 콜라이더만 넣어라
		GameObject* _colJumpAttack;

	private:	// 그 외
		const float jumpAttackRange = 10.0f;
		int call = 0;


	private:	// 코루틴
		Coroutine_Func(JumpAttackCo)
		{
			Aracne* some = this;
			auto animator = _boss->GetComponent<Animator>();
			DirectX::XMFLOAT3 target = some->_playerTransform->GetPosition();	// 패턴 끝낼 지점
			DirectX::XMFLOAT3 start = some->_bossTransform->GetPosition();	// 패턴 시작위치

			while (true)
			{
				if (!(animator->Play("Anim_Jump", 20.0f)))
				{
					animator->Stop();
					break;
				}

// 				if (25 < animator->GetCurrentFrame())
// 				{
// 					DirectX::XMFLOAT3 mine = some->_bossTransform->GetPosition();
// 					some->Move(mine, target, 10.0f);
// 				}
				Return_null;
			}

// 			while (true)
// 			{
// 				DirectX::XMFLOAT3 mine = some->_bossTransform->GetPosition();
// 				if (!(some->Move(mine, target, 30.0f)))
// 				{
// 					animator->Stop();
// 					break;
// 				}
// 				animator->Play("Anim_Fall", 10.0f, true);
// 				Return_null;
// 			}

			while (true)
			{
				if (!(animator->Play("Anim_Land", 30.0f)))
				{
					animator->Stop();
					break;
				}

				//DirectX::XMFLOAT3 mine = some->_bossTransform->GetPosition();
				//some->Move(mine, target, 30.0f);
				Return_null;
			}

			some->_status = BossStatus::IDLE;
		};
	};
}