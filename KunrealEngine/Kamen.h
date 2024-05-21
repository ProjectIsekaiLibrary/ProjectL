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

	public:
		void GamePattern();

	private:
		void CreateSubObject();
		
	private:
		void BasicPattern();

	private:
		void LeftRightPattern();
		void RightLeftPattern();
		void BackStepCallPattern();
		void TeleportSpellPattern();
		void TeleportTurnClockPattern();
		void TeleportTurnAntiClockPattern();

		// 코어 패턴
	private:
		void CoreEmmergencePattern();

		// 기본 패턴 조각들
	private:
		void CreateLeftAttack();
		void CreateRightAttack();
		void CreateSpellAttack();
		void CreateCallAttack();
		void CreateCall2Attack();

	private:
		void CreateTurn180();
		void CreateBackStep();
		void CreateTeleportToCenter();
		void CreateTeleportToCenterWithLook();
		void CreateTurnClockWise();
		void CreateTurnAntiClockWise();

		void CreateOutsideSafe();

		//void CreateMultiCall();

		// 핵심 기믹 패턴
	private:
		void CreateEmergenceAttack();

	private:
		BossPattern* _leftAttack;
		BossPattern* _rightAttack;
		BossPattern* _spellAttack;
		BossPattern* _callAttack;
		BossPattern* _call2Attack;

		BossPattern* _backStep;
		BossPattern* _turn180;
		BossPattern* _teleport;
		BossPattern* _teleportWithLook;
		BossPattern* _turnClockWise;
		BossPattern* _turnAntiClockWise;
		BossPattern* _outsideSafe;

	private:
		BossPattern* _emergence9Lich;

	private:
		// 패턴에 필요한 subObject들
		GameObject* _leftHand;
		GameObject* _rightHand;
		GameObject* _call;
		GameObject* _call2;
		GameObject* _lazer;

		GameObject* _insideWarning;
		GameObject* _outsideAttack;

		GameObject* _sword;

		std::vector<GameObject*> _fakeBoss;

		std::vector<GameObject*> _callVec;

	private:
		// Call 거리 체크용
		float _callMoveDistance;
		unsigned int _call2PrevStep;
		bool _isRotateFinish;

	private:
		bool _isCoreStart;
		bool _isRandomStart;

	private:
		unsigned int _targetIndex;

	private:
		float _insideWarningTimer;

	// logic
	private:
		std::function<void()> _callInitLogic;
	};
}