#pragma once
#include "CommonHeader.h"
#include "KunrealAPI.h"

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

	private:
		KunrealEngine::GameObject* _battleuibox;	// 부모가 되어줄 오브젝트(Empty)

		KunrealEngine::GameObject* _ui_skill1;		// 1번 스킬
		KunrealEngine::GameObject* _ui_skill1_cool;	// 1번 스킬 쿨다운
		KunrealEngine::GameObject* _ui_skill2;		// 2번 스킬
		KunrealEngine::GameObject* _ui_skill2_cool;	// 2번 스킬 쿨다운
		KunrealEngine::GameObject* _ui_skill3;		// 3번 스킬
		KunrealEngine::GameObject* _ui_skill3_cool;	// 3번 스킬 쿨다운
		KunrealEngine::GameObject* _ui_skill4;		// 4번 스킬
		KunrealEngine::GameObject* _ui_skill4_cool;	// 4번 스킬 쿨다운

		KunrealEngine::GameObject* _potion;			// 포션 먹기
		KunrealEngine::GameObject* _dash;			// 대쉬(회피?)

		KunrealEngine::GameObject* _playerhp_bar;		// 플레이어 체력바
		KunrealEngine::GameObject* _bosshp_bar;			// 보스 체력바

	private:
		EventManager* _eventmanager;
		const float _bosshpsize;
		const float _playerhpsize;
	};
}