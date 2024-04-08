#pragma once
#include "CommonHeader.h"

namespace KunrealEngine
{
	class GameObject;
	class Component;
	class Ability;

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
		

		// 보스가 플레이어에게 주는 데미지 계산
		void CalculateDamageToPlayer();

	public:
		// 스테이지가 바뀌었을 때 보스가 누구인지 알려주는 함수
		void SetBossObject();

		// 플레이어가 보스에게 주는 데미지 계산
		void CalculateDamageToBoss(Ability abil);
	};
}