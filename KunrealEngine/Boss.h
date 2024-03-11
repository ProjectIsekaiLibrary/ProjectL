/// <summary>
/// 보스가 상속받을 클래스
/// 보스가 기본적으로 지닐 정보를 담고 있음
/// 
/// 윤종화 정진수
/// </summary>

#pragma once
#include <vector>
#include "BossStruct.hpp"

namespace DirectX
{
	struct XMFLOAT3;
}

namespace KunrealEngine
{ 
	class GameObject;

	class Boss
	{
	public:
		Boss();
		virtual ~Boss();

	public:
		// 반드시 호출
		void Initialize(GameObject* boss);
		// 반드시 호출
		void Update();
		

	// 상태에 따른 함수들
	public:
		virtual void Idle();
		virtual void Chase();
		virtual void Hit();
		virtual void Staggred();
		virtual void Dead();
		virtual void BasicAttack();
		virtual void CoreAttack();
		virtual void PatternEnd();

	public:
		void SetInfo(BossBasicInfo info);
		// 기본 패턴 만든 목록을 넣기
		void SetBasicPatternList(std::vector<BossPattern*>* basicPatternList);
		// 코어 패턴 만든 목록을 넣기
		void SetCorePatternList(std::vector<BossPattern*>* corePatternList);

	private:
		static bool CompareCorePattern(const BossPattern* pattern1, const BossPattern* pattern2);

		void PutCorePatternInQueue();

	private:
		float CaculateDistance(const DirectX::XMFLOAT3& bossPosition, const DirectX::XMFLOAT3& playerPosition);

		int GetRandomNumber(int maxNum);

		bool MoveToPlayer(DirectX::XMFLOAT3 targetPos, float speed, float patternRange);

		void TeleportToPlayer();

		void LookAtPlayer();

	protected:
		BossStatus _status;
		BossBasicInfo _info;

		GameObject* _boss;

		GameObject* _player;

		std::vector<BossPattern*>* _basicPattern;
		std::vector<BossPattern*>* _corePattern;

		int _patternIndex;

		float _distance;

	private:
		bool _isCorePattern;
	};
}