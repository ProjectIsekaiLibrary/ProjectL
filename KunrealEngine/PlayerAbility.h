#pragma once

#include <DirectXMath.h>
#include "CommonHeader.h"
#include "Component.h"
#include "GameObject.h"
#include "Player.h"
#include "Ability.h"
#include "Projectile.h"
#include "BoxCollider.h"
#include "Coroutine.h"

namespace KunrealEngine
{
	class Boss;

	class _DECLSPEC PlayerAbility : public Component
	{
		friend class Player;
		friend class BattleUIManager;
	public:
		PlayerAbility();
		~PlayerAbility();

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
		std::vector<Ability*> _abilityContainer;
		Player* _playerComp;

		GameObject* _shot;			// Q 스킬 투사체 객체
		GameObject* _ice;			// W 스킬 객체
		GameObject* _area;			// E 스킬 객체
		GameObject* _meteor;		// R 스킬 운석 객체

		bool _isShotHit;			// 한 번만 데미지 받도록
		bool _isIceHit;
		bool _isAreaHit;
		bool _isMeteorHit;

		/// coroutine을 활용한 타이머 변수들
		bool _isShotReady;			// Q 쿨타임 조건

		bool _isIceReady;			// W 쿨타임 조건
		bool _destroyIce;			// W 소멸 조건을 위한 변수
		
		bool _isAreaReady;			// E 스킬 쿨타임 조건

		bool _isMeteorReady;		// R 쿨타임 조건

		/// BattleUIManager에 넘겨줄 쿨타임 체크 변수들
		bool _isShotDetected;
		bool _isIceDetected;
		bool _isAreaDetected;
		bool _isMeteorDetected;

	private:
		Boss* _currentBoss;			// 현재 맵의 보스
		float _currentDamage;		// 보스에 입힌 데미지

	private:
		void ResetShotPos();
		void CreateAbility1();

		void ResetIcePos();
		void CreateAbility2();
		
		void ResetAreaPos();
		void CreateAbility3();

		void ResetMeteorPos();
		void CreateAbility4();

	public:
		void AddToContanier(Ability* abil);

		// 맵이 바뀌었을 때 현재 보스를
		void SetCurrentBossObject();

		float GetDamage();

	private:
		/// 코루틴 타이머 함수들

		// Q스킬 쿨타임
		Coroutine_Func(shotCoolDown)
		{
			auto* ability = this;
			Waitforsecond(ability->_abilityContainer[0]->_cooldown);
			ability->_isShotReady = true;
		};

		// W스킬 쿨타임 
		Coroutine_Func(iceCoolDown)
		{
			auto* ability = this;
			Waitforsecond(ability->_abilityContainer[1]->_cooldown);
			ability->_isIceReady = true;
		};

		// W스킬 발동대기
		Coroutine_Func(iceStandby)
		{
			auto* ability = this;
			Waitforsecond(0.8f);		// 2초 뒤 실행

			ability->_destroyIce = false;	// 소멸 조건 초기화
			ability->_ice->SetActive(true);
			ability->_ice->GetComponent<Projectile>()->SetActive(true);
			ability->_ice->GetComponent<Projectile>()->ResetCondition();
		};

		// W스킬 소멸
		Coroutine_Func(iceDestroy)
		{
			auto* ability = this;
			Waitforsecond(3.0f);
			ability->_destroyIce = true;
			ability->_isIceReady = false;
		};

		// E스킬 쿨타임
		Coroutine_Func(AreaCoolDown)
		{
			auto* ability = this;
			Waitforsecond(ability->_abilityContainer[2]->_cooldown);
			ability->_isAreaReady = true;
		};

		// R스킬 쿨타임
		Coroutine_Func(meteorCoolDown)
		{
			auto* ability = this;
			Waitforsecond(ability->_abilityContainer[3]->_cooldown);
			ability->_isMeteorReady = true;
		};
	};
}

