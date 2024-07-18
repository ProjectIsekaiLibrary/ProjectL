#pragma once
#include "CommonHeader.h"
#include "Coroutine.h"

struct BossBasicInfo;

namespace KunrealEngine
{
	class GameObject;
	class Component;
	class PlayerAbility;
	class Ability;
	class Camera;
	
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

		void SetCamera(std::string name);
		std::vector<DirectX::XMFLOAT2> CamShake(float radius, int numPoints);
		std::vector<float> CamShakeLinear(float sigma, int numPoints);
		void CamShake(float harder);

		// 싱글톤
	public:
		static EventManager& GetInstance()
		{
			static EventManager _instance;
			return _instance;
		}

	public:
		GameObject* _player;
		GameObject* _boss;
		GameObject* _mainCamera;

	public:
		Player* _playerComp;
		Kamen* _bossComp;


		PlayerAbility* _playerAbill;

		bool _eventStart;

		bool _iscamfollow;
		float _camshakex;
		float _camshakez;

	private:
		unsigned int _insideSafeCount;

	public:
		// 플레이어가 보스에게 주는 데미지 계산
		void CalculateDamageToBoss(Ability* abil);

	private:
		// 보스가 플레이어에게 주는 데미지 계산
		void CalculateDamageToPlayer();

		// 보스가 플레이어에게 주는 데미지 계산2
		void CalculateDamageToPlayer2();

	private:
		// 스테이지가 바뀌었을 때 보스가 누구인지 알려주는 함수
		void SetBossObject();

		// 일단 여기다가... 위치 고민중
	private:
		const DirectX::XMVECTOR& SetWarningAttackDirection(GameObject* subObject);
		const DirectX::XMVECTOR& SetBossAttackDirection(GameObject* subObject);
		const DirectX::XMVECTOR& SetEgoAttackDirection(GameObject* subObject);

		_Coroutine(DashCamMove);

	public:
		// scene 전환 관련
		void MoveToTitleAfterDeath();
	};
}