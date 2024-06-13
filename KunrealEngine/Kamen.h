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

		virtual void Idle() override;

		virtual void SpecialAttack2() override;

	public:
		void GamePattern();

	private:
		void CreateSubObject();
		
	private:
		void BasicPattern();

	private:
		// 기본 패턴
		void LeftRightPattern();
		void RightLeftPattern();
		void BackStepCallPattern();
		void TeleportSpellPattern();

		// 검 단독 패턴
		void SwordTurnClockPattern();
		void SwordTurnAntiClockPattern();
		void SwordLinearAttackPattern();
		void SwordChopPattern();

		// 검 WITH 카멘
		void BasicSwordAttackPattern();

		// 코어 패턴
	private:
		void CoreEmmergencePattern();

		// 기본 패턴 조각들
	private:
		void CreateLeftAttack();
		void CreateLeftAttackThrowingFire();
		void CreateRightAttack();
		void CreateRightAttackThrowingFire();
		void CreateSpellAttack();
		void CreateCallAttack();
		void CreateCall2Attack();

		// 검 with 카멘 패턴
	private:
		void CreateSwordAttack();

		// 검 단독 패턴
	private:
		void CreateSwordEmergence();
		void CreateSwordHide();

		void CraeteSwordTurnClockWise();
		void CreateSwordTurnAntiClock();
		void CreateOutsideSafe();
		void CreateInsideSafe();
		void CreateDonutSafe();

		void CreateSwordLookPlayer();

		void CreateSwordLinearReady();
		void CreateSwordLinearAttack();

		void CreateSwordChopAttack();

		// 카멘 이동 관련
	private:
		void CreateTurn180();
		void CreateBackStep();
		void CreateTeleportToCenter();
		void CreateTeleportToCenterWithLook();
		void CreateTurnClockWise();
		void CreateTurnAntiClockWise();

		// 핵심 기믹 패턴
	private:
		void CreateEmergenceAttack();


		// 분신용 편의성 함수
	private:
		DirectX::XMVECTOR GetEgoDirection();

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

		BossPattern* _leftFireAttack;
		BossPattern* _rightFireAttack;

		BossPattern* _turnClockWise;
		BossPattern* _turnAntiClockWise;
		BossPattern* _outsideSafe;
		BossPattern* _insideSafe;
		BossPattern* _donutSafe;

		BossPattern* _basicSwordAttack;
		BossPattern* _swordEmmergence;
		BossPattern* _swordHide;
		BossPattern* _swordTurnClockWise;
		BossPattern* _swordTurnAntiClockWise;
		BossPattern* _swordLinearAtack;
		BossPattern* _swordLinearReady;
		BossPattern* _swordLookPlayer;
		BossPattern* _swordChopAttack;

	private:
		BossPattern* _emergence9Lich;

	private:
		// 패턴에 필요한 subObject들
		GameObject* _leftHand;
		GameObject* _rightHand;
		GameObject* _call;
		GameObject* _call2;
		GameObject* _lazer;
		GameObject* _lazerCollider;

		GameObject* _insideWarning;
		GameObject* _insideAttack;
		GameObject* _outsideWarning;
		GameObject* _donutWarning1;
		GameObject* _donutWarning2;
		GameObject* _donutWarning3;

		GameObject* _freeSword;
		GameObject* _freeSwordCollider;
		GameObject* _swordPath;

		std::vector<GameObject*> _handFire;

		GameObject* _leftHandBone;
		GameObject* _rightHandBone;

		GameObject* _alterEgo;

		std::vector<GameObject*> _egoHandFire;

		GameObject* _egoCall2;

		GameObject* _egoLazer;
		GameObject* _egoLazerCollider;

		GameObject* _egoLeftHandBone;
		GameObject* _egoRightHandBone;

		std::vector<GameObject*> _fakeBoss;

	private:
		// Call 거리 체크용
		float _callMoveDistance;
		unsigned int _call2PrevStep;
		bool _isRotateFinish;

		unsigned int _egoCall2PrevStep;


	private:
		std::vector<bool> _handFireReady;
		std::vector<DirectX::XMFLOAT3> _handFireDir;

	private:
		std::vector<bool> _egoHandFireReady;
		std::vector<DirectX::XMFLOAT3> _egoHandFireDir;

	private:
		bool _isCoreStart;
		bool _isRandomStart;

	private:
		unsigned int _targetIndex;

	private:
		std::function<void()> _callInitLogic;

	private:
		DirectX::XMFLOAT3 _centerPos;

	private:
		float _swordRotateAngle;

		DirectX::XMFLOAT3 _swordStartPos;
		DirectX::XMFLOAT3 _swordOriginPos;

		float _circleWarningSize;

		float _swordLinearDistance;

		DirectX::XMFLOAT3 _swordDirection;
		DirectX::XMFLOAT3 _swordRotation;

		float _swordChopSpeed;

		float _timer;

		float _warningMaxTimer;

	private:
		bool _isSpecial2Ready;
		bool _isSpecial2Playing;
		bool _isEgoAppearInit;
		bool _isEgoAppearFinish;
		bool _isEgoAttackReady;
		bool _isEgoAttack;
		float _egoTimer;
	};
}