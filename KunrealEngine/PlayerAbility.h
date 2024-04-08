#pragma once

#include <DirectXMath.h>
#include "CommonHeader.h"
#include "Component.h"
#include "Ability.h"

namespace KunrealEngine
{
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

		Ability* _currentSkill;
	private:
		std::vector<Ability*> _abilityContainer;

	public:
		void AddToContanier(Ability* abil);
	};
}

