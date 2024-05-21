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

		void CreatesubObject();
	public:
		virtual void CreatePattern() override;


	private:
		// 1페이즈
		void ChargeAttack();// 돌진 공격 - 2페 시작하고 사라져야
		void DropWeb();		// 코어패턴을 위한 거미줄 드롭
		void LeftAttack();	// 좌수 공격
		void RightAttack();	// 우수 공격
		void FrontAttack();	// 양발 정면 찍기

		//2페이즈 개막 패턴
		void PullAllWeb();	// 깔아둔 거미줄 끌어올리기

		// 2페이즈
		void JumpAttack();	// 점프 - 내려찍기 공격
		void TailAttack();	// 꼬리 공격
		void ShootingWeb();	// 거미줄 쏘기 공격 - 부채꼴 방사형
		void Casting();		// 마술 시전(뭔 패턴일지는 미지수)

		void CreateOutsideSafe();

	private:
		bool Move(DirectX::XMFLOAT3& startPos, DirectX::XMFLOAT3& targetPos, float speed);
		float CalculateParabolaHeight(DirectX::XMFLOAT3 start, DirectX::XMFLOAT3 end, DirectX::XMFLOAT3 current);

	private:	// 여긴 패턴만 넣어라
		BossPattern* _jumpAttack;
		BossPattern* _ChargeAttack;
		BossPattern* _leftAttack;
		BossPattern* _righttAttack;
		BossPattern* _frontAttack;
		BossPattern* _tailAttack;
		BossPattern* _webthrow;
		BossPattern* _magicshow;

	private:	// 여긴 콜라이더만 넣어라
		GameObject* _colbodyAttack;
		GameObject* _colFrontAttack;
		GameObject* _colTailAttack;
		GameObject* _colLeftHand;
		GameObject* _colRightHand;

		GameObject* _websubobj;
		GameObject* _magicsubobj;

	private:	// 그 외
		int _call = 0;

		// 패턴 종료 전달용
		bool _jumpAttack_end = false;
		bool _chargeAttack_end = false;
		bool _webthrow_end = false;
		bool _magicshow_end = false;

	private:	// 코루틴
		Coroutine_Func(JumpAttackCo)
		{	// 보스의 점공패턴. 지금은 점프하고 날아서 플레이어에게 이동한 다음 떨어지는건데 
			// 나중에 이거 3차원에서 포물선의 공식 적용 해서 포물선을 그리면서 날아가게 바꿀거임
			// 일단 패턴 하나에 시간 너무 쓰는것 같아서 여기까지만

			Aracne* some = this;
			auto animator = _boss->GetComponent<Animator>();
			DirectX::XMFLOAT3 target = some->_playerTransform->GetPosition();	// 패턴 끝낼 지점
			DirectX::XMFLOAT3 start = some->_bossTransform->GetPosition();	// 패턴 시작위치
			some->_jumpAttack_end = true;

			while (true)
			{
				if (!(animator->Play("Anim_Jump", 20.0f)))
				{
					animator->Stop();
					break;
				}

				if (25 < animator->GetCurrentFrame())
				{
					DirectX::XMFLOAT3 mine = some->_bossTransform->GetPosition();
					some->Move(mine, target, 10.0f);
				}
				Return_null;
			}

			while (true)
			{
				DirectX::XMFLOAT3 mine = some->_bossTransform->GetPosition();
				if (!(some->Move(mine, target, 30.0f)))
				{
					animator->Stop();
					break;
				}
				animator->Play("Anim_Fall", 30.0f, true);
				Return_null;
			}

			some->_colbodyAttack->SetActive(true);
			while (true)
			{
				if (!(animator->Play("Anim_Land", 30.0f)))
				{
					some->_colbodyAttack->SetActive(false);
					animator->Stop();
					break;
				}
				DirectX::XMFLOAT3 rot = some->_bossTransform->GetRotation();
				some->_colbodyAttack->GetComponent<Transform>()->SetPosition(some->_bossTransform->GetPosition());
				some->_colbodyAttack->GetComponent<Transform>()->SetRotation(DirectX::XMFLOAT3(0, rot.y, 0));
				Return_null;
			}

			some->_jumpAttack_end = false;
		};

		Coroutine_Func(ChargeAttackCo)
		{
			Aracne* some = this;
			auto animator = _boss->GetComponent<Animator>();
			DirectX::XMFLOAT3 target = some->_playerTransform->GetPosition();	// 패턴 끝낼 지점
			DirectX::XMFLOAT3 start = some->_bossTransform->GetPosition();	// 패턴 시작위치
			some->_chargeAttack_end = true;

			some->_colbodyAttack->SetActive(true);
			while (true)
			{
				DirectX::XMFLOAT3 mine = some->_bossTransform->GetPosition();
				animator->Play("Run", 70.0f, true);

				DirectX::XMFLOAT3 rot = some->_bossTransform->GetRotation();
				some->_colbodyAttack->GetComponent<Transform>()->SetPosition(some->_bossTransform->GetPosition());
				some->_colbodyAttack->GetComponent<Transform>()->SetRotation(DirectX::XMFLOAT3(0, rot.y, 0));
				Return_null;

				if (!some->Move(mine, target, 50.0f))
				{
					animator->Stop();
					break;
				}
			}

			while (true)
			{
				if (!(animator->Play("Attak_Take_down", 70.0f)))
				{
					animator->Stop();
					break;
				}

				DirectX::XMFLOAT3 rot = some->_bossTransform->GetRotation();
				some->_colbodyAttack->GetComponent<Transform>()->SetPosition(some->_bossTransform->GetPosition());
				some->_colbodyAttack->GetComponent<Transform>()->SetRotation(DirectX::XMFLOAT3(0, rot.y, 0));
				Return_null;
			}

			some->_colbodyAttack->SetActive(false);
			some->_chargeAttack_end = false;
		};

		Coroutine_Func(WebShoot)
		{
			Aracne* some = this;
			auto subobj = some->_websubobj;
			DirectX::XMFLOAT3 target = some->_playerTransform->GetPosition();	// 패턴 끝낼 지점

			subobj->SetActive(true);
			subobj->GetComponent<BoxCollider>()->SetActive(false);
			subobj->GetComponent<TransparentMesh>()->Reset();
			subobj->GetComponent<TransparentMesh>()->SetActive(true);
			subobj->GetComponent<Transform>()->SetPosition(target.x, target.y + 1.0f, target.z);
			auto animator = _boss->GetComponent<Animator>();

			while (1)
			{

				if (subobj->GetComponent<TransparentMesh>()->PlayOnce())
				{
					// 장판 실행이 완료되면
					subobj->GetComponent<TransparentMesh>()->SetActive(false);
					break;
				}

				// 장판 실행
				animator->Play("Anim_Throw_a_web", 20.0f, true);
				Return_null;
			}

			// n초동안 콜라이더 실행
			subobj->GetComponent<BoxCollider>()->SetActive(true);
			Waitforsecond(2.0f);
			subobj->SetActive(false);

		};
	};
}