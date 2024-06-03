#pragma once
#include "CommonHeader.h"

struct BossBasicInfo;

namespace KunrealEngine
{
	class GameObject;
	class Component;
	class PlayerAbility;
	
	class Player;
	class Kamen;

	class EventManager
	{
	private:
		EventManager();
		~EventManager();

	public:
		void Initialize();
		void Release();

		void Update();

		// 싱글톤
	public:
		static EventManager& GetInstance()
		{
			static EventManager _instance;
			return _instance;
		}

	private:
		GameObject* _player;
		GameObject* _boss;

	private:
		Player* _playerComp;
		Kamen* _bossComp;

		PlayerAbility* _playerAbill;

		bool _eventStart;

	private:
		// 플레이어가 보스에게 주는 데미지 계산
		void CalculateDamageToBoss();

		// 보스가 플레이어에게 주는 데미지 계산
		void CalculateDamageToPlayer();

	private:
		// 스테이지가 바뀌었을 때 보스가 누구인지 알려주는 함수
		void SetBossObject();
	};
}