/// <summary>
/// 보스가 상속받을 클래스
/// 보스가 기본적으로 지닐 정보를 담고 있음
/// 
/// 윤종화 정진수
/// </summary>

#pragma once
#include <vector>
#include "BossStruct.hpp"
#include "Coroutine.h"
#include "CommonHeader.h"

namespace DirectX
{
	struct XMFLOAT3;
}

namespace KunrealEngine
{ 
	class GameObject;
	class BoxCollider;

	class _DECLSPEC Boss
	{
	public:
		Boss();
		virtual ~Boss();

	public:
		// 반드시 호출
		void Initialize(GameObject* boss);
		// 반드시 호출
		void Update();

	public:
		// 하위 오브젝트를 설정
		virtual void CreateSubObject() abstract;
		// 어떠한 모델로 렌더링 할 것인지를 지정
		virtual void SetMesh() abstract;
		// 콜라이더를 어떻게 입힐지를 지정
		virtual void SetCollider() abstract;

	public:
		// 어떠한 텍스쳐를 입힐 것인지를 지정
		virtual void SetTexture();

	public:
		// 보스의 포지션 지정
		virtual void SetBossTransform();

	// 상태에 따른 함수들
	public:
		virtual void Enter();
		virtual void Idle();
		virtual void Chase();
		virtual void Hit();
		virtual void Attack();
		virtual void Staggred();
		virtual void OffStaggred();
		virtual void Dead();
		virtual void BasicAttack();
		virtual void CoreAttack();
		virtual void PatternEnd();

	public:
		// 보스의 정보 넣기
		void SetInfo(BossBasicInfo& info);

		// 기본 패턴 만든 목록을 넣기
		void SetBasicPatternList(std::vector<BossPattern*>* basicPatternList);
		
		// 코어 패턴 만든 목록을 넣기
		void SetCorePatternList(std::vector<BossPattern*>* corePatternList);

		// 현재 보스의 상태 가져오기
		const BossStatus& GetStatus();

		// 데미지 체크용 콜라이더 활성화 여부
		void SetColliderState(bool tf);

	private:
		void SetParentToSubObject();

		static bool CompareCorePattern(const BossPattern* pattern1, const BossPattern* pattern2);

		void SortCorePattern();

	private:
		float CalculateAngle(const DirectX::XMFLOAT3& bossPosition, const DirectX::XMFLOAT3& playerPosition);

		bool MoveToPlayer(DirectX::XMFLOAT3 targetPos, float speed, float patternRange);

		bool MoveToPlayer(DirectX::XMFLOAT3 startPos, DirectX::XMFLOAT3 targetPos, float speed, float patternRange);

		void TeleportToPlayer();

		bool LookAtPlayer(float agnle, float rotateSpeed);

		void RegisterCollider();

		void UpdateMoveNode();

		void UpdateMoveNode(DirectX::XMFLOAT3 targetPos);

	protected:
		BossStatus _status;
		BossBasicInfo _info;

		GameObject* _boss;

		GameObject* _player;

		std::vector<BossPattern*> _basicPattern;
		std::vector<BossPattern*> _corePattern;

		std::vector<KunrealEngine::BoxCollider*> _subColliderList;

		int _patternIndex;

		float _distance;

		// 무기, 장신구 등 메쉬나 콜라이더 처리할 것들
		std::vector<GameObject*> _subObjectList;

		// 한 패턴 내에 콜라이더가 켜지는 수 
		unsigned int _maxColliderOnCount;

	private:
		bool _isCorePattern;

	private:
		bool _isStart;
		bool _isHit;

		private:
		Transform* _bossTransform;
		Transform* _playerTransform;

		std::vector<std::pair<DirectX::XMFLOAT3, DirectX::XMFLOAT3>> _stopover;
		int _nodeCount;

	private:
		Coroutine_Func(patternEnd)
		{
			Boss* boss = this;

			Waitforsecond(1.0);

			if (boss->_isCorePattern)
			{
				(boss->_corePattern).pop_back();

				boss->_isCorePattern = false;
			}

			boss->_status = BossStatus::IDLE;

			boss->_patternIndex = -1;
		};
	};
}