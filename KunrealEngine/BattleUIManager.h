#pragma once
#include "CommonHeader.h"
#include "KunrealAPI.h"
#include "Coroutine.h"

namespace KunrealEngine
{
	class EventManager;

	class BattleUIManager : public Component
	{
	public:
		BattleUIManager();
		~BattleUIManager();

		// Component의 가상함수들 오버라이드
		void Initialize() override;
		void Release() override;

		void FixedUpdate() override;
		void Update() override;
		void LateUpdate() override;

		void OnTriggerEnter() override;
		void OnTriggerStay() override;
		void OnTriggerExit() override;

		void SetActive(bool active) override;

	public:
		void SetBossHpbar();
		void SetPlayerHpBar();
		void SetSkillcool1();
		void SetSkillcool2();
		void SetSkillcool3();
		void SetSkillcool4();

	private:
		KunrealEngine::GameObject* _battleuibox;	// 부모가 되어줄 오브젝트(Empty)

		KunrealEngine::GameObject* _ui_skill1_cool;	// 1번 스킬 쿨다운
		KunrealEngine::GameObject* _ui_skill2_cool;	// 2번 스킬 쿨다운
		KunrealEngine::GameObject* _ui_skill3_cool;	// 3번 스킬 쿨다운
		KunrealEngine::GameObject* _ui_skill4_cool;	// 4번 스킬 쿨다운

		KunrealEngine::GameObject* _potion;			// 포션 먹기
		KunrealEngine::GameObject* _dash;			// 대쉬(회피?)

		KunrealEngine::GameObject* _playerhp_bar;		// 플레이어 체력바
		KunrealEngine::GameObject* _playerhp_bar_downGauge;	// 보스 체력바 체력 감소 연출용
		KunrealEngine::GameObject* _bosshp_bar;			// 보스 체력바
		KunrealEngine::GameObject* _bosshp_bar_downGauge;	// 보스 체력바 체력 감소 연출용
		float booshp_targetscale;
		float playerhp_targetscale;

	private:
		EventManager* _eventmanager;
		const float _bosshpsize;
		const float _playerhpsize;
		const float _skillcoolsize;

		_Coroutine(bossdowngauge);
		_Coroutine(playerdowngauge);
		_Coroutine(skillgauge1);
		_Coroutine(skillgauge2);
		_Coroutine(skillgauge3);
		_Coroutine(skillgauge4);
	};
}