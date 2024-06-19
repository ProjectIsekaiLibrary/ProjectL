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
		void CreateParticleObject();

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
		void EmergenceAttackPattern();

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
		void CreateBossRandomInsideWarning();

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
		void CreateReverseEmergence();
		void CreateEmergence();

		// 핵심 기믹 패턴
	private:
		void CreateEmergenceAttack();


		// 분신용 편의성 함수
	private:
		DirectX::XMVECTOR GetEgoDirection();

	public:
		const DirectX::XMFLOAT3& GetSwordDirection();

	private:
		BossPattern* _leftAttack;
		BossPattern* _rightAttack;
		BossPattern* _spellAttack;
		BossPattern* _callAttack;
		BossPattern* _call2Attack;
		BossPattern* _bossRandomInsideWarning;

		BossPattern* _backStep;
		BossPattern* _turn180;
		BossPattern* _teleport;
		BossPattern* _teleportWithLook;
		BossPattern* _reverseEmergence;
		BossPattern* _emergence;

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
		GameObject* _bossInsideWarning;
		GameObject* _bossInsideAttack;

		GameObject* _swordInsideWarning;
		GameObject* _swordInsideAttack;
		GameObject* _swordOutsideWarning;
		GameObject* _swordDonutWarning1;
		GameObject* _swordDonutWarning2;
		GameObject* _swordDonutWarning3;

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

		GameObject* _egoInsideWarning;
		GameObject* _egoInsideAttack;

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
		DirectX::XMFLOAT3 _emergencePos;

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

		// 파티클
	private:
		std::vector<GameObject*> _particleBossfire1;
		std::vector<GameObject*> _particleBossfire2;

		std::vector<GameObject*> _particleEgofire1;
		std::vector<GameObject*> _particleEgofire2;

		std::vector<GameObject*> _particleCall2;
		std::vector<GameObject*> _particleEgoCall2;

		std::vector<GameObject*> _particleSwordOutsideAttack;
		std::vector<GameObject*> _particleSwordDonutAttack;
	};
}