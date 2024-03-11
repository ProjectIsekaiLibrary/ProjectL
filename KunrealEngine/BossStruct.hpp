#pragma once
#include <string>
#include <functional>

// 보스의 상태
enum class BossStatus
{
	IDLE,				// 조우시, 다음 공격 패턴을 정함

	CHASE,				// 플레이어 추적

	STAGGERED,			// 보스 무력화
	DEAD,				// 사망

	BASIC_ATTACK,		// 기본 공격
	CORE_ATTACK,		// 스킬 사용		// 스킬 구성에 따라 나뉠 예정
	PATTERN_END			// 패턴이 끝난 경우
};


// 보스의 기본적인 스탯 정보
struct BossBasicInfo
{
	BossBasicInfo()
		: _hp(0.0f), _phase(0), _armor(0.0f), _damage(0.0f), _moveSpeed(0.0f), _staggeredGauge(0.0f)
	{};

	BossBasicInfo& SetHp(float hp) { _hp = hp;  return *this; };
	BossBasicInfo& SetPhase(unsigned int phase) { _phase = phase;  return *this; };
	BossBasicInfo& SetArmor(float armor) { _armor = armor;  return *this; };
	BossBasicInfo& SetDamage(float damage) { _damage = damage;  return *this; };
	BossBasicInfo& SetMoveSpeed(float moveSpeed) { _moveSpeed = moveSpeed;  return *this; };
	BossBasicInfo& SetsStaggeredGauge(float staggeredGauge) { _staggeredGauge = staggeredGauge;  return *this; };

	float _hp;				// 체력
	unsigned int _phase;	// 체력 비례 페이즈

	float _armor;			// 방어력
	float _damage;			// 데미지

	float _moveSpeed;		// 이동속도

	float _staggeredGauge;	// 무력화 게이지
};

struct BossPattern
{
	BossPattern()
		: _animName(""), _damage(0.0f), _speed(0.0f), _range(0.0f), _afterDelay(0.0f), _effectName(""), _isWarning(false), _warningName("warningName"), _triggerHp(0.0f), 
		_coolDown(0.0f), _isActive(true),
		_logic(nullptr)
	{
	}

	void Play() { if (_logic!= nullptr) _logic(); }		// 패턴 실행 함수

	BossPattern& SetAnimName(const std::string& animName) { _animName = animName; return *this; };
	BossPattern& SetDamage(float damage) { _damage = damage; return *this; };
	BossPattern& SetSpeed(float speed) { _speed = speed; return *this; };
	BossPattern& SetRange(float range) { _range = range; return *this; };
	BossPattern& SetAfterDelay(float afterDelay) { _afterDelay = afterDelay; return *this; };
	BossPattern& SetIsWarning(bool isWarning) { _isWarning = isWarning; return *this; };
	BossPattern& SetWarningName(const std::string& warningName) { _warningName = warningName; return *this; };
	BossPattern& SetTriggerHp(float hp) { _triggerHp = hp; return *this; };
	BossPattern& SetCoolDown(float coolDown) { _triggerHp = coolDown; return *this; };
	BossPattern& SetActive(bool isActive) { _triggerHp = isActive; return *this; };
	BossPattern& SetLogic(std::function<void()> logic) { _logic = logic; return *this; };

	std::string _animName;			// 패턴 애니메이션 이름
									
	float _damage;					// 데미지
									
	float _speed;					// 패턴, 애니메이션 스피드
									
	float _range;					// 패턴 범위
									
	float _afterDelay;				// 패턴 후 가만히 있는 시간
									
	std::string _effectName;		// 패턴 이펙트 텍스쳐 이름
									
	bool _isWarning;				// 스킬 범위의 장판 보여줄 것인가
	std::string _warningName;		// 장판 텍스쳐 이름

	float _triggerHp;				// 패턴이 발동될 조건의 hp

	float _coolDown;				// 패턴 쿨타임

	bool _isActive;					// 패턴 활성화 여부

	std::function<void()> _logic;	// 패턴 로직
};