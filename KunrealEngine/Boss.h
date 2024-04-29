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

enum class BossStatus;
struct BossPattern;
struct BossBasicInfo;

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
		void DebugTest();

	public:
		// 반드시 호출
		void Initialize(GameObject* boss);
		// 반드시 호출
		void Update();

	public:
		// 어떠한 모델로 렌더링 할 것인지를 지정
		virtual void SetMesh() abstract;

	public:
		// 패턴 생성
		virtual void CreatePattern() abstract;

	public:
		// 어떠한 텍스쳐를 입힐 것인지를 지정
		virtual void SetTexture();

	public:
		// 보스의 포지션 지정
		virtual void SetBossTransform();

	public:
		const std::pair<DirectX::XMFLOAT3, DirectX::XMFLOAT3> GetBossPosition();

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
		virtual void PatternReady();
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

		// 현재 보스가 바라보는 방향 벡터 가져오기
		const DirectX::XMVECTOR GetDirection();

	public:
		// 보스 히트 판정용 콜라이더 생성
		virtual void SetBossCollider();

		void SetSubObject(bool tf);

		/// 보스 패턴 구현할때 가져다 쓰세요.
	public:
		// 플레이어 바라보도록 회전시키기
		bool RotateToTarget(const DirectX::XMFLOAT3& targetPos);

		bool Rotate(float angle, float speed);

		// 특정 포지션으로 이동시키기 (raycast true시 무언가에 막히면 거기까지만 찾아감)
		bool Move(DirectX::XMFLOAT3& targetPos, float speed, bool roateToTarget, bool rayCast);

		std::function<bool()> CreateBackStepLogic(BossPattern* pattern, float moveSpeed, float distance);

		std::function<bool()> CreateBasicAttackLogic(BossPattern* pattern, GameObject* subObject, float activeColliderFrame);

		std::function<bool()> CreateBasicAttackLogic(BossPattern* pattern, const std::string& animName, GameObject* subObject, float activeColliderFrame);

		// 플레이어를 바라보도록 텔레포트
		void TeleportToPlayer();

		// 지정한 곳으로 텔레포트
		bool Teleport(const DirectX::XMFLOAT3& targetPos, bool lookAtPlayer, float hideTime = 0.0f);

		// 지정한 곳으로 강제 이동
		void ForceMove(const DirectX::XMFLOAT3& targetPos);

	public:
		void SetMaxColliderOnCount(unsigned int index);

	private:
		static bool CompareCorePattern(const BossPattern* pattern1, const BossPattern* pattern2);

		void SortCorePattern();

		void SetSubObjectScene();

	private:
		bool LookAtPlayer(float angle, float rotateSpeed);

		float CalculateAngle(const DirectX::XMFLOAT3& bossPosition, const DirectX::XMFLOAT3& playerPosition);

		bool MoveToPlayer(DirectX::XMFLOAT3& startPos, DirectX::XMFLOAT3& targetPos, float speed);

		void UpdateMoveNode();

		void UpdateMoveNode(DirectX::XMFLOAT3& targetPos);

		void CalculateDirection();

		bool MoveToTarget(DirectX::XMFLOAT3& startPos, DirectX::XMFLOAT3& targetPos, float speed);

		// 백스탭 이동
		bool BackStep(float speed, float distance);

	protected:
		BossStatus _status;
		BossBasicInfo _info;

		GameObject* _boss;

		GameObject* _player;

		std::vector<BossPattern*> _basicPattern;
		std::vector<BossPattern*> _corePattern;
		BossPattern* _nowPattern;

		int _patternIndex;
		int _exPatternIndex;

		float _distance;

		// 한 패턴 내에 콜라이더가 켜지는 수 
		unsigned int _maxColliderOnCount;

		Transform* _bossTransform;
		Transform* _playerTransform;

	private:
		bool _isCorePattern;

	private:
		bool _isStart;
		bool _isHit;
		bool _isRotateFinish;

	private:
		std::vector<std::pair<DirectX::XMFLOAT3, DirectX::XMFLOAT3>> _stopover;
		int _nodeCount;

		DirectX::XMFLOAT3 _direction;

		DirectX::XMFLOAT3 _prevPos;
		DirectX::XMFLOAT3 _backStepPos;

	private:
		float _goalAngle;

	private:
		bool _isMoving;
		bool _isRotate;
		bool _backStepReady;

		bool _isHideFinish;

	private:
		Coroutine_Func(patternEnd)
		{
			Boss* boss = this;

			auto delay = _nowPattern->_afterDelay;
			Waitforsecond(delay);

			// 코어 패턴이었다면
			if (boss->_isCorePattern)
			{
				// 실행한 코어 패턴을 백터에서 제거
				(boss->_corePattern).pop_back();

				boss->_isCorePattern = false;
			}

			// 패턴 초기화할 것들 초기화
			boss->_nowPattern->Initialize();

			boss->_status = BossStatus::IDLE;

			// 기본 패턴을 실행할 수 있도록 초기화
			boss->_patternIndex = -1;

			boss->_nowPattern = nullptr;
		};

		Coroutine_Func(TeleportWithHide)
		{
			Boss* boss = this;

			boss->_boss->GetComponent<MeshRenderer>()->SetActive(false);
			boss->_boss->GetComponent<BoxCollider>()->SetActive(false);

			Waitforsecond(3.0f);

			boss->_boss->GetComponent<MeshRenderer>()->SetActive(true);
			boss->_boss->GetComponent<BoxCollider>()->SetActive(true);

			boss->_isHideFinish = true;
		};

	};
}