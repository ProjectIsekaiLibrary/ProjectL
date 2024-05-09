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

		// 페이즈 2

		// 화염 원거리 투척 (리치의 call과 유사한 패턴)
		void CreateLeftFireShot();
		void CreateRightFireShot();
		// 랜덤 범위 공격 (이펙트만 화염으로)
		void CreateRandomFireAttack();


	private:
		BossPattern* _leftAttack;
		BossPattern* _rightAttack;
		BossPattern* _turn180;

		BossPattern* _spellAttack;
		BossPattern* _callAttack;
		BossPattern* _backStep;

	private:
		// 패턴에 필요한 subObject들
		GameObject* _leftHand;
		GameObject* _rightHand;
		GameObject* _leftRoot;
		GameObject* _rightRoot;

		GameObject* _smallRoot;

		std::vector<GameObject*> _fakeBoss;

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
	};
}
