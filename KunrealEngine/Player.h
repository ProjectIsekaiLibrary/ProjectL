#pragma once

#include <DirectXMath.h>
#include "CommonHeader.h"
#include "Component.h"

namespace KunrealEngine
{
	class Transform;

	class _DECLSPEC Player : public Component
	{
		friend class PlayerMove;
		friend class PlayerAbility;
	public:
		// 플레이어의 상태
		enum class Status
		{
			IDLE,				// 평상시
			WALK,				// 이동
			DASH,				// 대시
			ABILITY,			// 스킬 사용		// 스킬 구성에 따라 나뉠 예정
			STAGGERED,			// 플레이어가 무력화 되었을 때		// 상태이상 등
			DEAD				// 사망
		};

		// 플레이어의 스탯 정보
		struct PlayerInfo
		{
			float _hp;				// 체력
			float _stamina;			// 기력

			float _moveSpeed;		// 이동 속도
			float _dashSpeed;		// 대시 속도
			float _dashRange;		// 대시 거리
			float _dashCooldown;	// 대시 쿨타임

			float _spellPower;		// 스킬 사용 시 추가공격력
			float _damageReduce;	// 피해 감소

			float _speedScale;		// 전체적인 속도에 영향

			PlayerInfo(float hp, float stamina, float moveSpeed, float dashSpeed, float dashRange, float dashCooldown, float spellpower, float damageReduce, float speedScale)
				:_hp(hp), _stamina(stamina), _moveSpeed(moveSpeed), _dashSpeed(dashSpeed), _dashRange(dashRange), _dashCooldown(dashCooldown), _spellPower(spellpower), _damageReduce(damageReduce), _speedScale(speedScale)
			{}
		};

	public:
		Player();
		~Player();

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
		Transform* _transform;					// 플레이어의 transform
		Status _playerStatus;					// 플레이어의 상태 이동 기절 등
		PlayerInfo _playerInfo;					// 플레이어 기본 정보 스탯 등
		DirectX::XMVECTOR _directionVector;		// 플레이어가 바라보는 방향 벡터

	private:
		// 플레이어의 상태에 따라 애니메이션 출력
		void AnimateByStatus();

	public:
		// 플레이어가 바라보는 방향벡터
		const DirectX::XMVECTOR GetDirectionVector();

		// 플레이어의 상태 반환
		const Status GetPlayerStatus();
	};
}

