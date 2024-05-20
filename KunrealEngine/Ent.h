#pragma once
#include "KunrealAPI.h" // 여기에 추가해줘야함
#include "Boss.h"
#include "Coroutine.h"

namespace KunrealEngine
{
	class _DECLSPEC Ent : public Component, public Boss
	{
	public:
		Ent();
		virtual ~Ent();

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
		void CreateSubObject();

	private:
		// 일반 범위공격
		void CreateLeftAttack();
		void CreateRightAttack();
		// 나무줄기 원거리 투척
		void CreateLeftRootShot();
		void CreateRightRootShot();
		// 랜덤 범위 공격
		void CreateRandomRootAttack();
		// 점프 범위 공격
		void CreateJumpAttack();

		// 눈에서 빔
		void CreateEyeLight();

		// 일반 강공격
		void CreateSwiping();

		// 페이즈 2

		// 화염 원거리 투척 (리치의 call과 유사한 패턴)
		void CreateLeftFireShot();
		void CreateRightFireShot();
		// 랜덤 범위 공격 (이펙트만 화염으로)
		void CreateRandomFireAttack();



	private:
		// 코어패턴, 기믹 생각

		void CreateCorePatternFirst();

		void CorePatternObjectFirst();

	private:
		float GetRandomRange(float center, float range);
		bool Move(DirectX::XMFLOAT3& startPos, DirectX::XMFLOAT3& targetPos, float speed);

	private:
		BossPattern* _leftAttack;
		BossPattern* _rightAttack;
		BossPattern* _bigRootShot;
		// 해당 변수 사용할지 고민중
		 
	private:
		// 패턴에 필요한 subObject들
		GameObject* _leftHand;
		GameObject* _rightHand;
		GameObject* _leftRoot;
		GameObject* _rightRoot;

		std::vector<GameObject*> _smallRootVector;

		std::vector<GameObject*> _treeObject;

		GameObject* _treeObjectReal;

		GameObject* _eyeLight;

		//GameObject* _eyeLightCollider;

	private:
		// Call 거리 체크용
		float _callMoveDistance;
		bool _isRotateFinish;

	private:
		bool _isCoreStart;  
		bool _isRandomStart;

		// logic
	private:
		std::function<void()> _callInitLogic;

	private:
		GameObject* _bigRootR;
		GameObject* _bigRootL;

		GameObject* _rootShot;

		GameObject* _colJumpAttack;

	private:
		bool _bigRootShotStart;

		bool _isIdleHealing;

		float _rootY;

		float randomX;

		float randomZ;

		bool isEyesLightStart;


	private:
		// 코어패턴에서 사용할 변수들

		int _successCountCoreFirst;

		std::vector<std::string> _objectOrderCoreFirst;


	private:	// 코루틴 대진수의 패턴 야미
		Coroutine_Func(JumpAttackCo)
		{	
			Ent* some = this;
			auto animator = _boss->GetComponent<Animator>();
			DirectX::XMFLOAT3 target = some->_playerTransform->GetPosition();	// 패턴 끝낼 지점
			DirectX::XMFLOAT3 start = some->_bossTransform->GetPosition();	// 패턴 시작위치

			while (true)
			{
				if (!(animator->Play("Anim_Jump_Start", 20.0f)))
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

			some->_colJumpAttack->SetActive(true);
			while (true)
			{
				if (!(animator->Play("Anim_Jump_End", 30.0f)))
				{
					some->_colJumpAttack->SetActive(false);
					animator->Stop();
					break;
				}
				DirectX::XMFLOAT3 rot = some->_bossTransform->GetRotation();
				some->_colJumpAttack->GetComponent<Transform>()->SetPosition(some->_bossTransform->GetPosition());
				some->_colJumpAttack->GetComponent<Transform>()->SetRotation(DirectX::XMFLOAT3(0, rot.y, 0));
				Return_null;
			}

			some->_status = BossStatus::IDLE;
		};
	};
}
