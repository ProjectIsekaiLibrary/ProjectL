#pragma once
#include <string>
#include <functional>
#include "SceneManager.h"
#include "Scene.h"
#include "Animator.h"
#include "GameObject.h"

// 보스의 상태
enum class BossStatus
{
	ENTER,				// 조우 시 플레이어 마주치기 전
	IDLE,				// 다음 공격 패턴을 정함

	CHASE,				// 플레이어 추적

	STAGGERED,			// 보스 무력화
	OFF_STAGGRED,
	DEAD,				// 사망

	PATTERN_READY,		// 공격 준비
	BASIC_ATTACK,		// 기본 공격
	CORE_ATTACK,		// 스킬 사용		// 스킬 구성에 따라 나뉠 예정
	PATTERN_END			// 패턴이 끝난 경우
};


// 보스의 기본적인 스탯 정보
struct BossBasicInfo
{
	BossBasicInfo()
		: _hp(0.0f), _phase(0), _armor(0.0f), _damage(0.0f), _attackRange(0.0f), _moveSpeed(20.0f), _rotationSpeed(100.0f), _baseAnimSpeed(30.0f),
		_staggeredGauge(0.0f), _maxHp(0.0f), _maxStaggeredGauge(0.0f)
	{};

	BossBasicInfo& SetHp(float hp) { _maxHp = hp; _hp = hp;   return *this; };
	BossBasicInfo& SetPhase(unsigned int phase) { _phase = phase;  return *this; };
	BossBasicInfo& SetArmor(float armor) { _armor = armor;  return *this; };
	BossBasicInfo& SetDamage(float damage) { _damage = damage;  return *this; };
	BossBasicInfo& SetAttackRange(float attackRange) { _attackRange = attackRange;  return *this; };
	BossBasicInfo& SetMoveSpeed(float moveSpeed) { _moveSpeed = moveSpeed;  return *this; };
	BossBasicInfo& SetRotateSpeed(float rotationSpeed) { _rotationSpeed = rotationSpeed;  return *this; };
	BossBasicInfo& SetBaseAnimationSpeed(float animSpeed) { _baseAnimSpeed = animSpeed;  return *this; };
	BossBasicInfo& SetsStaggeredGauge(float staggeredGauge) { _maxStaggeredGauge = staggeredGauge; _staggeredGauge = staggeredGauge;  return *this; };

	float GetMaxHP() { return _maxHp; };
	float GetMaxStaggeredGauage() { return _maxStaggeredGauge; };

	float _hp;				// 체력
	unsigned int _phase;	// 체력 비례 페이즈

	float _armor;			// 방어력
	float _damage;			// 데미지
	float _attackRange;		// 기본 공격 범위

	float _moveSpeed;		// 이동속도
	float _rotationSpeed;	// 회전 속도
	float _baseAnimSpeed;	// 애니메이션 속도

	float _staggeredGauge;	// 무력화 게이지

private:
	float _maxHp;				// 최대 체력
	float _maxStaggeredGauge;	// 무력화 게이지 최대치

	// 데미지 감소율
};

struct BossPattern
{
	enum class eAttackState
	{
		eNone,
		eParalysis,		// 경직
		ePush			// 피격
	};


	BossPattern()
		: _patternName(""), _animName(""), _damage(0.0f), _speed(0.0f), _range(0.0f), _afterDelay(0.0f), _effectName(""), _isWarning(false), _warningName("warningName"), _triggerHp(0.0f),
		_coolDown(0.0f), _rangeOffset(5.0f),
		_isActive(true), _maxColliderOnCount(1), _subObject(),
		_logic(), _initializeLogic(nullptr), _attackState(eAttackState::eNone), _logicIndex(0)
	{

	}

	~BossPattern()
	{
		for (auto object : _subObject)
		{
			KunrealEngine::SceneManager::GetInstance().GetCurrentScene()->DeleteGameObject(object);
		}
	}
	bool Play() 
	{ 
		// 모든 로직 백터를 돌며 끝날때까지
		if (_logicIndex < _logic.size())
		{
			// 현재 로직을 실행
			bool logicPlaying = _logic[_logicIndex]();

			// 현재 로직이 끝난다면
			if (logicPlaying == false)
			{
				// 현재 실행중이던 애니메이션을 멈춤
				auto object = KunrealEngine::SceneManager::GetInstance().GetCurrentScene()->GetObjectWithTag("BOSS");
				object->GetComponent<KunrealEngine::Animator>()->Stop();

				// 다음 로직으로 이동
				_logicIndex++;
			}
			// 아직 패턴이 끝나지 않아 패턴 실행중이라고 넘겨줌
			return true;
		}
		else
		{
			// 패턴이 끝나 패턴이 끝난 상태라고 넘겨줌
			_logicIndex--;
			return false;
		}
	}

	void Initialize() { if (_initializeLogic != nullptr) _logicIndex = 0; _initializeLogic(); }	// 초기화 실행 함수
	BossPattern& SetPatternName(const std::string& patterName) { _patternName = patterName; return *this; };
	BossPattern& SetAnimName(const std::string& animName) { _animName = animName; return *this; };
	BossPattern& SetDamage(float damage) { _damage = damage; return *this; };
	BossPattern& SetSpeed(float speed) { _speed = speed; return *this; };
	BossPattern& SetRange(float range) { _range = range; return *this; };
	BossPattern& SetAfterDelay(float afterDelay) { _afterDelay = afterDelay; return *this; };
	BossPattern& SetIsWarning(bool isWarning) { _isWarning = isWarning; return *this; };
	BossPattern& SetWarningName(const std::string& warningName) { _warningName = warningName; return *this; };
	BossPattern& SetTriggerHp(float hp) { _triggerHp = hp; return *this; };
	BossPattern& SetCoolDown(float coolDown) { _triggerHp = coolDown; return *this; };
	BossPattern& SetRangeOffset(float rangeOffset) { _rangeOffset = rangeOffset; return *this; };
	BossPattern& SetActive(bool isActive) { _triggerHp = isActive; return *this; };
	BossPattern& SetMaxColliderCount(unsigned int count) { _maxColliderOnCount = count; return *this; };
	BossPattern& SetLogic(std::function<bool()> logic, bool isRemainMesh = true) { _logic.emplace_back(logic); _isRemainMesh.emplace_back(isRemainMesh); return *this; };
	BossPattern& SetInitializeLogic(std::function<void()> initialize) { _initializeLogic = initialize; return *this; };
	BossPattern& SetAttackState(eAttackState attackState) { _attackState = attackState; return *this; };

	std::string _patternName;		// 패턴 이름

	std::string _animName;			// 패턴 애니메이션 이름
									
	float _damage;					// 데미지
									
	float _speed;					// 패턴, 애니메이션 스피드
									
	float _range;					// 패턴 범위
									
	float _afterDelay;				// 패턴 후 가만히 있는 시간 (초)
									
	std::string _effectName;		// 패턴 이펙트 텍스쳐 이름
									
	bool _isWarning;				// 스킬 범위의 장판 보여줄 것인가
	std::string _warningName;		// 장판 텍스쳐 이름

	float _triggerHp;				// 패턴이 발동될 조건의 hp

	float _coolDown;				// 패턴 쿨타임

	float _rangeOffset;				// 어느정도까지 보스가 쫓아올 것인지를 지정, offset을 줄일수록 가까운 거리까지 chase

	bool _isActive;					// 패턴 활성화 여부

	//경직인지 피격인지
	eAttackState _attackState;		// 패턴 상태 여부

	std::vector<std::function<bool()>> _logic;	// 패턴 로직

	std::function<void()> _initializeLogic;	// 매 패턴 초기화해줘야할 것들을 담아놓음

	unsigned int _maxColliderOnCount; // 패턴 중 콜라이더가 켜지는 횟수 지정 (기본 1)

	std::vector<KunrealEngine::GameObject*> _subObject;

	unsigned int _logicIndex;

	std::vector<bool> _isRemainMesh;	// 피격이 끝나도 메쉬가 남아있는가,
};