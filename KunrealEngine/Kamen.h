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
		~Kamen();

		void Initialize() override;
		void Release() override;

		void FixedUpdate() override;
		void Update() override;
		void LateUpdate() override;

		void OnTriggerEnter() override;
		void OnTriggerStay() override;
		void OnTriggerExit() override;

		void SetActive(bool active) override;

		virtual void CreateSubObject() override;
		virtual void SetMesh() override;
		virtual void SetTexture() override;
		virtual void SetCollider() override;
		virtual void SetBossTransform() override;

	private:
		BoxCollider* _leftHand;
		BoxCollider* _rightHand;

	private:
		void LeftAttackOnce();
		void RightAttackOnce();


		Coroutine_Func(function)
		{
			Kamen* kamen = this;

			Waitforsecond(0.5f);
			kamen->_status = BossStatus::IDLE;
			kamen->_patternIndex = -1;
		};
	};
}