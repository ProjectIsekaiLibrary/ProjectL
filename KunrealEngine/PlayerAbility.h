#pragma once

#include <DirectXMath.h>
#include "CommonHeader.h"
#include "Component.h"
#include "Player.h"
#include "Coroutine.h"

namespace KunrealEngine
{
	class Ability;

	class _DECLSPEC PlayerAbility : public Component
	{
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
		GameObject* _meteor;		// R 스킬 운석 객체

		bool _destroyIce;			// coroutine을 활용한 소멸 조건을 위한 변수

	private:
		void ResetShotPos();
		void CreateAbility1();

		void ResetIcePos();
		void CreateAbility2();

		void ResetMeteorPos();
		void CreateAbility4();

	public:
		void AddToContanier(Ability* abil);

		Coroutine_Func(iceTimer)
		{
			auto* ability = this;

			Waitforsecond(3.0f);

			ability->_destroyIce = true;
		};
	};
}

