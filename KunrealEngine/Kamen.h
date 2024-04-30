#pragma once
#include "KunrealAPI.h"
#include "Boss.h"
#include "Coroutine.h"

namespace KunrealEngine
{
	class _DECLSPEC Kamen : public Component, public Boss
	{
	public:
		Kamen();
		virtual ~Kamen();

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
		void LeftRightPattern();
		void RightLeftPattern();

		// 기본 패턴 조각들
	private:
		void CreateLeftAttack();
		void CreateRightAttack();
		void CreateTurn180();
		void SpellAttack();
		void CallAttack();
		void BackStepCallAttack();

		// 핵심 기믹 패턴
	private:
		void EmergenceAttack();

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
		GameObject* _call;
		GameObject* _lazer;

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
	};
}