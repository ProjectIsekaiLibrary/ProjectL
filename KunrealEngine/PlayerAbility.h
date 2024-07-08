#pragma once

#include "ToolBox.h"
#include "Component.h"
#include "GameObject.h"
#include "Player.h"
#include "Ability.h"
#include "Projectile.h"
#include "BoxCollider.h"
#include "Coroutine.h"
#include "Particle.h"
#include "Transform.h"
#include "TimeManager.h"

namespace KunrealEngine
{
	class Boss;
	class Particle;

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
		GameObject* _laser;			// E 스킬 객체
		GameObject* _meteor;		// R 스킬 운석 객체

		bool _isShotHit;			// 한 번만 데미지 받도록
		bool _isIceHit;
		bool _isLaserHit;
		bool _isMeteorHit;

		/// coroutine을 활용한 타이머 변수들
		bool _isShotReady;			// Q 쿨타임 조건

		bool _isIceReady;			// W 쿨타임 조건
		bool _destroyIce;			// W 소멸 조건을 위한 변수
		
		bool _isLaserReady;			// E 스킬 쿨타임 조건
		bool _destroyLaser;			// E 소멸 조건을 위한 변수

		bool _isMeteorReady;		// R 쿨타임 조건

		/// BattleUIManager에 넘겨줄 쿨타임 체크 변수들
		bool _isShotDetected;
		bool _isIceDetected;
		bool _isLaserDetected;
		bool _isMeteorDetected;

	private:
		Boss* _currentBoss;			// 현재 맵의 보스
		float _currentDamage;		// 보스에 입힌 데미지

	/// 파티클 배치용 멤버변수
	private:
		GameObject* _shotParticle2;
		GameObject* _shotParticle3;
		GameObject* _shotParticle4;

		GameObject* _shotParticleHit1;
		GameObject* _shotParticleHit2;
		GameObject* _shotParticleHit3;

		GameObject* _iceParticle1;
		GameObject* _iceParticle2;
		GameObject* _iceParticle3;
		GameObject* _iceParticleHit1;
		GameObject* _iceParticleHit2;

		GameObject* _laserParticle1;
		GameObject* _laserParticle2;
		GameObject* _laserParticle3;
		GameObject* _laserParticle4;

		GameObject* _meteorParticle2;
		GameObject* _meteorParticle3;
		GameObject* _meteorParticle4;

		GameObject* _meteorParticleHit1;
		GameObject* _meteorParticleHit2;
		GameObject* _meteorParticleHit3;
		GameObject* _meteorParticleHit4;

	private:
		// q 스킬 체크용 변수
		bool _isShotEnded;
		float _shotParticleTimer;

		// w 스킬 체크용 변수
		bool _isIceEnded;
		float _iceParticleTimer;

		// e 스킬 체크용 변수
		bool _isLaserStarted;
		bool _isLaserEnded;
		float _laserParticleTimer;


		// r 스킬 체크용 변수
		bool _isMeteorEnded;
		float _meteorParticleTimer;
		 
		// 비활성화 되었을 경우
	private:
		void ResetShotPos();
		void CreateAbility1();

		void ResetIcePos();
		void CreateAbility2();
		
		void ResetLaserPos();
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
			_iceParticle1->GetComponent<Particle>()->SetActive(false);
			_iceParticle2->GetComponent<Particle>()->SetActive(false);
			_iceParticle3->GetComponent<Particle>()->SetActive(false);
			_iceParticleHit1->GetComponent<Particle>()->SetActive(true);
			_iceParticleHit2->GetComponent<Particle>()->SetActive(true);
			_iceParticleHit1->SetActive(true);
			_iceParticleHit2->SetActive(true);
			ability->_destroyIce = true;
			ability->_isIceReady = false;
			Waitforsecond(0.5f);
			_iceParticleHit1->GetComponent<Particle>()->SetActive(false);
			_iceParticleHit2->GetComponent<Particle>()->SetActive(false);
			_iceParticleHit1->SetActive(false);
			_iceParticleHit2->SetActive(false);
		};

		// E스킬 쿨타임
		Coroutine_Func(LaserCoolDown)
		{
			auto* ability = this;
			Waitforsecond(ability->_abilityContainer[2]->_cooldown);
			ability->_isLaserReady = true;
		};

		// E스킬 발동 대기
		Coroutine_Func(LaserStandby)
		{
			auto* ability = this;
			Waitforsecond(0.9f);		// 2초 뒤 실행

			ability->_laser->SetActive(true);
			_laserParticle1->SetActive(true);
			_laserParticle2->SetActive(true);
			//_laserParticle3->SetActive(true);
			_laserParticle4->SetActive(true);

			_laserParticle1->GetComponent<Particle>()->SetActive(true);
			_laserParticle2->GetComponent<Particle>()->SetActive(true);
			//_laserParticle3->GetComponent<Particle>()->SetActive(true);
			_laserParticle4->GetComponent<Particle>()->SetActive(true);
		};

		// E스킬 소멸조건
		Coroutine_Func(laserDestroy)
		{
			auto* ability = this;
			Waitforsecond(1.8f);

			float delta = 0;
			while (true)
			{
				delta += TimeManager::GetInstance().GetDeltaTime();
				ability->_laserParticle1->GetComponent<Particle>()->SetParticleSize((50 - (delta * 25)) * ToolBox::GetRandomFloat(0.8f, 1.0f), (50 - (delta * 25)) * ToolBox::GetRandomFloat(0.8f, 1.0f));
				ability->_laserParticle2->GetComponent<Particle>()->SetParticleSize((20 - (delta * 10)) * ToolBox::GetRandomFloat(0.8f, 1.0f), (20 - (delta * 10)) * ToolBox::GetRandomFloat(0.8f, 1.0f));
				ability->_laserParticle3->GetComponent<Particle>()->SetParticleSize((50 - (delta * 25)) * ToolBox::GetRandomFloat(0.8f, 1.0f), (50 - (delta * 25)) * ToolBox::GetRandomFloat(0.8f, 1.0f));
				ability->_laserParticle4->GetComponent<Particle>()->SetParticleSize((50 - (delta * 25)) * ToolBox::GetRandomFloat(0.8f, 1.0f), (50 - (delta * 25)) * ToolBox::GetRandomFloat(0.8f, 1.0f));

				if (delta > 2) break;
				Return_null;
			}
			ability->_laserParticle1->SetActive(false);
			ability->_laserParticle2->SetActive(false);
			ability->_laserParticle3->SetActive(false);
			ability->_laserParticle4->SetActive(false);
			ability->_destroyLaser = true;
			ability->_isLaserReady = false;
			ability->_isLaserStarted = false;

		};

		//E스킬 취소 시 해당 크기로부터 줄어들도록
		Coroutine_Func(LaserFadeOut)
		{
			auto* ability = this;
			Particle* laserP1 = ability->_laserParticle1->GetComponent<Particle>();
			Particle* laserP2 = ability->_laserParticle2->GetComponent<Particle>();
			Particle* laserP3 = ability->_laserParticle3->GetComponent<Particle>();
			Particle* laserP4 = ability->_laserParticle4->GetComponent<Particle>();

			float half1 = laserP1->GetParticleSize().x * 0.5f;
			float half2 = laserP2->GetParticleSize().x * 0.5f;
			float half3 = laserP3->GetParticleSize().x * 0.5f;
			float half4 = laserP4->GetParticleSize().x * 0.5f;

			float delta = 0;
			while (true)
			{
				/// 이 부분 상수말고 변수 써야함
				delta += TimeManager::GetInstance().GetDeltaTime();

				if (laserP1->GetParticleSize().x > 0.0f)
				{
					laserP1->SetParticleSize((laserP1->GetParticleSize().x - (delta * half1)) * ToolBox::GetRandomFloat(0.8f, 1.0f), (laserP1->GetParticleSize().y - (delta * half1)) * ToolBox::GetRandomFloat(0.8f, 1.0f));
				}
				
				if (laserP2->GetParticleSize().x > 0.0f)
				{
					laserP2->SetParticleSize((laserP2->GetParticleSize().x - (delta * half2)) * ToolBox::GetRandomFloat(0.8f, 1.0f), (laserP2->GetParticleSize().y - (delta * half2)) * ToolBox::GetRandomFloat(0.8f, 1.0f));
				}
				
				if (laserP3->GetParticleSize().x > 0.0f)
				{
					laserP3->SetParticleSize((laserP3->GetParticleSize().x - (delta * half3)) * ToolBox::GetRandomFloat(0.8f, 1.0f), (laserP3->GetParticleSize().y - (delta * half3)) * ToolBox::GetRandomFloat(0.8f, 1.0f));
				}

				if (laserP4->GetParticleSize().x > 0.0f)
				{
					laserP4->SetParticleSize((laserP4->GetParticleSize().x - (delta * half4)) * ToolBox::GetRandomFloat(0.8f, 1.0f), (laserP4->GetParticleSize().x - (delta * half4)) * ToolBox::GetRandomFloat(0.8f, 1.0f));
				}

				if (delta > 2) break;
				Return_null;
			}
			ability->_laserParticle1->SetActive(false);
			ability->_laserParticle2->SetActive(false);
			ability->_laserParticle3->SetActive(false);
			ability->_laserParticle4->SetActive(false);
			ability->_destroyLaser = true;
			ability->_isLaserReady = false;
			ability->_isLaserStarted = false;
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

