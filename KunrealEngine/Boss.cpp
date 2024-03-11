#include <random>
#include <algorithm>
#include <DirectXMath.h>
#include "GameObject.h"
#include "Animator.h"
#include "Transform.h"
#include "SceneManager.h"
#include "Scene.h"
#include "Player.h"
#include "Boss.h"

KunrealEngine::Boss::Boss()
	: _info(), _status(BossStatus::IDLE), _boss(nullptr), _player(nullptr), _patternIndex(-1), _distance(0.0f), _isCorePattern(false),
	_basicPattern(), _corePattern()
{

}

KunrealEngine::Boss::~Boss()
{
}

void KunrealEngine::Boss::Initialize(GameObject* boss)
{
	_player = SceneManager::GetInstance().GetCurrentScene()->GetGameObject("Player");

	_boss = boss;
}

void KunrealEngine::Boss::Update()
{
	// Hit는 항상 측정해야함
	Hit();

	switch (_status)
	{
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

void KunrealEngine::Boss::Idle()
{
	_boss->GetComponent<Animator>()->Play("Idle", 30.0f, true);

	if (!(*_corePattern).empty())
	{
		if (_info._hp <= (*_corePattern).back()->_triggerHp)
		{
			_isCorePattern = true;

			_status = BossStatus::CORE_ATTACK;
		}
	}

	// 다음 기본 패턴을 정함
	if (_patternIndex == -1 && ! _isCorePattern)
	{
		_patternIndex = GetRandomNumber(_basicPattern->size() - 1);

		while ((*_basicPattern)[_patternIndex]->_isActive == false)
		{
			_patternIndex = GetRandomNumber(_basicPattern->size() - 1);
		}
	}

	// Chase로 상태 변환
	_status = BossStatus::CHASE;
}

void KunrealEngine::Boss::Chase()
{
	// 패턴의 사거리를 받아옴
	auto patternRange = (*_basicPattern)[_patternIndex]->_range;

	auto bossPosition = _boss->GetComponent<Transform>()->GetPosition();

	auto playerPosition = _player->GetComponent<Transform>()->GetPosition();

	auto distance = CaculateDistance(bossPosition, playerPosition);

	// 패턴 사거리보다 플레이어와의 거리가 가까울 경우 공격 시행
	if (distance <= patternRange)
	{
		LookAtPlayer();

		_status = BossStatus::BASIC_ATTACK;
	}
	else
	{
		// 패턴 사거리보다 플레이어의 거리가 멀지만 일정 거리 이내일 경우 걸어서 이동
		if (distance - patternRange <= 500)
		{
			auto isChasing = MoveToPlayer(playerPosition, _info._moveSpeed, patternRange);

			if (isChasing == false)
			{
				_status = BossStatus::BASIC_ATTACK;
			}
		}

		// 패턴 사거리보다 플레이의 거리가 멀고 일정 거리를 벗어날 경우 텔레포트로 이동
		else
		{
			TeleportToPlayer();

			_status = BossStatus::BASIC_ATTACK;
		}
	}
}

void KunrealEngine::Boss::Hit()
{
	// 보스의 콜라이더에 무언가가 충돌한다면
	// 플레이어의 최근 공격한 데미지를 받아와서 깍아야함
	// 
	// 보스가 플레이어를 알고 있는게 맞는가?
	_info._hp; // -= _player->GetComponent<Player>()->_playerInfo.

	// 무력화 게이지가 0이 되면
	if (_info._staggeredGauge <= 0)
	{
		_status = BossStatus::STAGGERED;
	}

	if (_info._hp <= 0)
	{
		_status = BossStatus::DEAD;
	}
}

void KunrealEngine::Boss::Staggred()
{
	// 무력화 애니메이션 실행
	_boss->GetComponent<Animator>()->Play("Staggred", 30.0f, false);

	// 일정 시간 동안 가만히 있음

	// 무력화 종료 애니메이션 실행
	_boss->GetComponent<Animator>()->Play("StaggredFinish", 30.0f, false);

	_status = BossStatus::IDLE;
}

void KunrealEngine::Boss::Dead()
{
	// 보스가 죽을때 애니메이션 실행
	_boss->GetComponent<Animator>()->Play("Dead", 30.0f, false);
}

void KunrealEngine::Boss::BasicAttack()
{
	(*_basicPattern)[_patternIndex]->Play();
}

void KunrealEngine::Boss::CoreAttack()
{
	(*_corePattern).back()->Play();
}

void KunrealEngine::Boss::PatternEnd()
{
	if (_isCorePattern)
	{
		(*_corePattern).pop_back();

		_isCorePattern = false;
	}

	_status = BossStatus::IDLE;

	_patternIndex = -1;
}

void KunrealEngine::Boss::SetInfo(BossBasicInfo info)
{
	_info = info;
}

void KunrealEngine::Boss::SetBasicPatternList(std::vector<BossPattern*>* basicPatternList)
{
	_basicPattern = basicPatternList;
}

void KunrealEngine::Boss::SetCorePatternList(std::vector<BossPattern*>* corePatternList)
{
	_corePattern = corePatternList;

	PutCorePatternInQueue();
}

bool KunrealEngine::Boss::CompareCorePattern(const BossPattern* pattern1, const BossPattern* pattern2)
{
	return pattern1->_triggerHp < pattern2->_triggerHp;
}

void KunrealEngine::Boss::PutCorePatternInQueue()
{
	std::sort((*_corePattern).begin(), (*_corePattern).end(), CompareCorePattern);
}

float KunrealEngine::Boss::CaculateDistance(const DirectX::XMFLOAT3& bossPosition, const DirectX::XMFLOAT3& playerPosition)
{
	auto distX = bossPosition.x - playerPosition.x;
	auto distZ = bossPosition.z - playerPosition.z;

	_distance = std::sqrt(distX * distX + distZ * distZ);

	return _distance;
}

int KunrealEngine::Boss::GetRandomNumber(int maxNum)
{
	std::random_device randomDevice;
	std::mt19937 gen(randomDevice());
	std::uniform_int_distribution<int> distribution(0, maxNum);

	return distribution(gen);
}

bool KunrealEngine::Boss::MoveToPlayer(DirectX::XMFLOAT3 targetPos, float speed, float patternRange)
{
	auto bossTransform = _boss->GetComponent<Transform>();

	DirectX::XMFLOAT3 bossPosition = bossTransform->GetPosition();

	// 패턴 거리 내에 플레이어가 존재하지 않을 경우
	if (_distance > patternRange - 5)
	{
		LookAtPlayer();

		// 플레이어 이동
		_boss->GetComponent<Animator>()->Play("Run", 30.0f, true);

		DirectX::XMVECTOR currentPosVec = DirectX::XMLoadFloat3(&bossPosition);

		DirectX::XMVECTOR direction = DirectX::XMVectorSubtract(DirectX::XMLoadFloat3(&targetPos), currentPosVec);

		direction = DirectX::XMVector3Normalize(direction);

		DirectX::XMVECTOR newPosition = DirectX::XMVectorAdd(currentPosVec, DirectX::XMVectorScale(direction, speed));
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

void KunrealEngine::Boss::LookAtPlayer()
{
	auto bossTransform = _boss->GetComponent<Transform>();

	auto bossPosition = bossTransform->GetPosition();

	auto playerPosition = _player->GetComponent<Transform>()->GetPosition();

	DirectX::XMVECTOR currentPosVec = DirectX::XMLoadFloat3(&bossPosition);
	DirectX::XMVECTOR targetPosVec = DirectX::XMLoadFloat3(&playerPosition);

	DirectX::XMVECTOR currentForward = DirectX::XMVectorSet(0.0f, 0.0f, -1.0f, 0.0f);
	DirectX::XMVECTOR targetDirection = DirectX::XMVectorSubtract(targetPosVec, currentPosVec);
	targetDirection.m128_f32[1] = 0.0f;
	targetDirection = DirectX::XMVector3Normalize(targetDirection);

	// 두 벡터 간의 각도를 계산
	auto dotResult = DirectX::XMVector3Dot(currentForward, targetDirection);
	float dotProduct = DirectX::XMVectorGetX(dotResult);

	// 각도를 라디안에서 도로 변환
	float angle = acos(dotProduct);
	angle = DirectX::XMConvertToDegrees(angle);

	if (targetPosVec.m128_f32[0] > currentPosVec.m128_f32[0])
	{
		angle *= -1;
	}

	bossTransform->SetRotation(bossTransform->GetRotation().x, angle, bossTransform->GetRotation().z);
}
