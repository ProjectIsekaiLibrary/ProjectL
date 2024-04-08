#include <random>
#include <algorithm>
#include <DirectXMath.h>
#include "GameObject.h"
#include "Animator.h"
#include "TimeManager.h"
#include "Transform.h"
#include "SceneManager.h"
#include "Scene.h"
#include "Player.h"
#include "ToolBox.h"
#include "BoxCollider.h"
#include "Boss.h"

// 임시 애니메이션 속도 변수들
static float ANIMATION_SPEED = 30.0f;
static float ROTATION_SPEED = 80.0f;

KunrealEngine::Boss::Boss()
	: _info(), _status(BossStatus::ENTER), _boss(nullptr), _player(nullptr), _patternIndex(-1), _distance(0.0f), _isCorePattern(false),
	_basicPattern(), _corePattern(), _subColliderList(), _maxColliderOnCount(1),
	_isStart(false), _isHit(false)
{

}

KunrealEngine::Boss::~Boss()
{
	for (auto index : _basicPattern)
	{
		_basicPattern.erase(std::remove(_basicPattern.begin(), _basicPattern.end(), index));
	}
}

void KunrealEngine::Boss::Initialize(GameObject* boss)
{
	_player = SceneManager::GetInstance().GetCurrentScene()->GetGameObject("Player");

	_boss = boss;

	_boss->SetTag("BOSS");

	CreateSubObject();
	SetParentToSubObject();

	SetMesh();

	SetBossTransform();
	
	SetCollider();
	RegisterCollider();
	SetColliderState(false);

	SortCorePattern();
}

void KunrealEngine::Boss::Update()
{
	Hit();
	Attack();

	switch (_status)
	{
		case BossStatus::ENTER:
		{
			Enter();
			break;
		}
		case BossStatus::IDLE:
		{
			Idle();
			break;
		}
		case BossStatus::CHASE:
		{
			Chase();
			break;
		}
		case BossStatus::STAGGERED:
		{
			Staggred();
			break;
		}
		case BossStatus::OFF_STAGGRED:
		{
			OffStaggred();
			break;
		}
		case BossStatus::DEAD:
		{
			Dead();
			break;
		}
		case BossStatus::BASIC_ATTACK:
		{
			BasicAttack();
			break;
		}
		case BossStatus::CORE_ATTACK:
		{
			CoreAttack();
			break;
		}
		case BossStatus::PATTERN_END:
		{
			PatternEnd();
			break;
		}
		default:
		{

			break;
		}
	}
}

void KunrealEngine::Boss::Enter()
{
	auto bossPosition = _boss->GetComponent<Transform>()->GetPosition();

	auto playerPosition = _player->GetComponent<Transform>()->GetPosition();

	auto distance = ToolBox::GetDistance(bossPosition, playerPosition);

	if (distance <= 1000.0f)
	{
		_isStart = true;
	}

	if (_isStart)
	{
		_boss->GetComponent<Animator>()->Play("Emergence", ANIMATION_SPEED, false);

		if (_boss->GetComponent<Animator>()->GetIsAnimationPlaying() == false)
		{
			_boss->GetComponent<Animator>()->Stop();
			_status = BossStatus::IDLE;
		}
	}
}

void KunrealEngine::Boss::Idle()
{
	if (!(_corePattern).empty())
	{
		if (_info._hp <= (_corePattern).back()->_triggerHp)
		{
			_isCorePattern = true;

			_status = BossStatus::CORE_ATTACK;
		}
	}

	// 다음 기본 패턴을 정함
	if (_patternIndex == -1 && !_isCorePattern)
	{
		_patternIndex = ToolBox::GetRandomNum(0, _basicPattern.size() - 1);

		while ((_basicPattern)[_patternIndex]->_isActive == false)
		{
			_patternIndex = ToolBox::GetRandomNum(0, _basicPattern.size() - 1);
		}
	}

	_maxColliderOnCount = _basicPattern[_patternIndex]->_maxColliderOnCount;

	// Chase로 상태 변환
	_status = BossStatus::CHASE;
}

void KunrealEngine::Boss::Chase()
{
	// 패턴의 사거리를 받아옴
	auto patternRange = (_basicPattern)[_patternIndex]->_range;

	auto bossPosition = _boss->GetComponent<Transform>()->GetPosition();

	auto playerPosition = _player->GetComponent<Transform>()->GetPosition();

	_distance = ToolBox::GetDistance(bossPosition, playerPosition);

	auto angle = CalculateAngle(bossPosition, playerPosition);

	auto isRotateFinish = LookAtPlayer(angle, ROTATION_SPEED);

	if (isRotateFinish)
	{
		// 패턴 사거리보다 플레이어와의 거리가 가까울 경우 공격 시행
		if (_distance <= 5 + patternRange)
		{
			_boss->GetComponent<Animator>()->Stop();

			_status = BossStatus::BASIC_ATTACK;
		}

		else
		{
			// 패턴 사거리보다 플레이어의 거리가 멀지만 일정 거리 이내일 경우 걸어서 이동
			if (_distance - patternRange <= 500)
			{
				auto isChasing = MoveToPlayer(playerPosition, _info._moveSpeed, patternRange);

				if (!isChasing)
				{
					_boss->GetComponent<Animator>()->Stop();

					_status = BossStatus::BASIC_ATTACK;
				}
			}

			// 패턴 사거리보다 플레이의 거리가 멀고 일정 거리를 벗어날 경우 텔레포트로 이동
			else
			{
				TeleportToPlayer();
			}
		}
	}
}

void KunrealEngine::Boss::Hit()
{
	auto bossCollider = _boss->GetComponent<BoxCollider>();

	if (_isHit == false)
	{
		if (bossCollider->IsCollided())
		{
			if (bossCollider->GetTargetObject() == _player)
			{
				if (_info._hp <= 0)
				{
					_status = BossStatus::DEAD;
				}

				// 무력화 게이지가 0이 되면
				if (_info._staggeredGauge <= 0)
				{
					_status = BossStatus::STAGGERED;
				}

				_isHit = true;

				// 보스의 콜라이더에 무언가가 충돌한다면
				// 플레이어의 최근 공격한 데미지를 받아와서 깍아야함
				// 
				// 보스가 플레이어를 알고 있는게 맞는가?
				_info._hp; // -= _player->GetComponent<Player>()->_playerInfo.
			}
		}
	}

	if (_isHit)
	{
		_boss->GetComponent<Animator>()->Play("Hit", ANIMATION_SPEED, false);

		if (_boss->GetComponent<Animator>()->GetIsAnimationPlaying() == false)
		{
			_isHit = false;
		}
	}
}


void KunrealEngine::Boss::Attack()
{
	for (auto collider : _subColliderList)
	{
		if (collider->IsCollided() && collider->GetTargetObject() == _player)
		{
			collider->SetActive(false);

			_maxColliderOnCount--;

			auto damage = _basicPattern[_patternIndex]->_damage;

			_player->GetComponent<Player>()->GetPlayerData()._hp -= damage;
		}
	}
}

void KunrealEngine::Boss::Staggred()
{
	// 무력화 애니메이션 실행
	_boss->GetComponent<Animator>()->Play("Staggred", ANIMATION_SPEED, false);

	if (_boss->GetComponent<Animator>()->GetIsAnimationPlaying() == false)
	{
		_boss->GetComponent<Animator>()->Stop();
		_status = BossStatus::OFF_STAGGRED;
	}
}

void KunrealEngine::Boss::OffStaggred()
{
	// 무력화 종료 애니메이션 실행
	_boss->GetComponent<Animator>()->Play("OffStaggred", ANIMATION_SPEED, false);

	if (_boss->GetComponent<Animator>()->GetIsAnimationPlaying() == false)
	{
		_boss->GetComponent<Animator>()->Stop();

		_info._staggeredGauge = _info.GetMaxStaggeredGauage();
		_status = BossStatus::IDLE;
	}
}

void KunrealEngine::Boss::Dead()
{
	// 보스가 죽을때 애니메이션 실행
	_boss->GetComponent<Animator>()->Play("Dead", ANIMATION_SPEED, false);
}

void KunrealEngine::Boss::BasicAttack()
{
	auto isPatternPlaying = (_basicPattern)[_patternIndex]->Play();

	if (isPatternPlaying == false)
	{
		_boss->GetComponent<Animator>()->Stop();
		_status = BossStatus::PATTERN_END;
	}
}

void KunrealEngine::Boss::CoreAttack()
{
	(_corePattern).back()->Play();
}

void KunrealEngine::Boss::PatternEnd()
{
	_boss->GetComponent<Animator>()->Play("Idle", ANIMATION_SPEED, false);

	SetColliderState(false);

	Startcoroutine(patternEnd);
}

void KunrealEngine::Boss::SetInfo(BossBasicInfo& info)
{
	_info = info;
}

void KunrealEngine::Boss::SetBasicPatternList(std::vector<BossPattern*>* basicPatternList)
{
}

void KunrealEngine::Boss::SetCorePatternList(std::vector<BossPattern*>* corePatternList)
{
	SortCorePattern();
}

const BossStatus& KunrealEngine::Boss::GetStatus()
{
	return _status;
}

void KunrealEngine::Boss::SetParentToSubObject()
{
	for (auto index : _subObjectList)
	{
		index->SetParent(_boss);
	}
}

bool KunrealEngine::Boss::CompareCorePattern(const BossPattern* pattern1, const BossPattern* pattern2)
{
	return pattern1->_triggerHp < pattern2->_triggerHp;
}

void KunrealEngine::Boss::SortCorePattern()
{
	std::sort((_corePattern).begin(), (_corePattern).end(), CompareCorePattern);
}

float KunrealEngine::Boss::CalculateAngle(const DirectX::XMFLOAT3& bossPosition, const DirectX::XMFLOAT3& playerPosition)
{
	auto angle = ToolBox::GetAngle(bossPosition, playerPosition);

	auto bossAngle = _boss->GetComponent<Transform>()->GetRotation().y;

	if (std::abs(bossAngle - angle) < 180.0f)
	{
		return angle;
	}
	else
	{
		if (angle < 0)
		{
			return 360.0f + angle;
		}
		else
		{
			return angle - 360.0f;
		}
	}
}

bool KunrealEngine::Boss::MoveToPlayer(DirectX::XMFLOAT3 targetPos, float speed, float patternRange)
{
	float moveSpeed = speed * TimeManager::GetInstance().GetDeltaTime();

	auto bossTransform = _boss->GetComponent<Transform>();

	DirectX::XMFLOAT3 bossPosition = bossTransform->GetPosition();

	// 패턴 거리 내에 플레이어가 존재하지 않을 경우
	if (_distance > patternRange - 5)
	{
		// 플레이어 이동
		_boss->GetComponent<Animator>()->Play("Run", speed, true);

		DirectX::XMVECTOR currentPosVec = DirectX::XMLoadFloat3(&bossPosition);

		DirectX::XMVECTOR direction = DirectX::XMVectorSubtract(DirectX::XMLoadFloat3(&targetPos), currentPosVec);

		direction = DirectX::XMVector3Normalize(direction);

		DirectX::XMVECTOR newPosition = DirectX::XMVectorAdd(currentPosVec, DirectX::XMVectorScale(direction, moveSpeed));
		bossTransform->SetPosition(newPosition.m128_f32[0], 0, newPosition.m128_f32[2]);

		return true;
	}

	else
	{
		return false;
	}
}

void KunrealEngine::Boss::TeleportToPlayer()
{
	auto bossTransform = _boss->GetComponent<Transform>();
	auto bossRotation = bossTransform->GetRotation();

	auto playerTransform = _player->GetComponent<Transform>();
	auto playerPosition = playerTransform->GetPosition();
	auto playerRotation = playerTransform->GetRotation();


	// 여기에 플레이어의 방향 벡터 * 일정 거리를 곱해 플레이어와 살짝 떨어지도록 해야 함
	// 플레이어의 방향 벡터를 가져와야함
	bossTransform->SetPosition(playerPosition.x, playerPosition.y, playerPosition.z);

	// 플레이어를 바라보도록 
	bossTransform->SetRotation(bossRotation.x, 180 + playerRotation.y, bossRotation.z);
}

bool KunrealEngine::Boss::LookAtPlayer(float angle, float rotateSpeed)
{
	auto bossTransform = _boss->GetComponent<Transform>();

	float errorRange = rotateSpeed / 50.0f;

	float speed = TimeManager::GetInstance().GetDeltaTime() * rotateSpeed;

	if (bossTransform->GetRotation().y < angle)
	{
		bossTransform->SetRotation(bossTransform->GetRotation().x, bossTransform->GetRotation().y + speed, bossTransform->GetRotation().z);
	}
	else
	{
		bossTransform->SetRotation(bossTransform->GetRotation().x, bossTransform->GetRotation().y - speed, bossTransform->GetRotation().z);
	}
	

	if (angle - errorRange <= bossTransform->GetRotation().y && bossTransform->GetRotation().y <= angle + errorRange)
	{
		if (_boss->GetComponent<Animator>()->GetNowAnimationName() == "Idle")
		{
			_boss->GetComponent<Animator>()->Stop();
		}
		return true;
	}
	else
	{
		_boss->GetComponent<Animator>()->Play("Idle", ANIMATION_SPEED, true);
		return false;
	}

}

void KunrealEngine::Boss::SetColliderState(bool tf)
{
	for (auto index : _subColliderList)
	{
		index->SetActive(tf);
	}
}

void KunrealEngine::Boss::SetBossTransform()
{

}

void KunrealEngine::Boss::RegisterCollider()
{
	_subColliderList.clear();

	for (auto index : _subObjectList)
	{
		auto collider = index->GetComponent<BoxCollider>();

		if (collider != nullptr)
		{
			_subColliderList.emplace_back(collider);
		}
	}
}