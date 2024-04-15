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
#include "InputSystem.h"
#include "Navigation.h"
#include "Boss.h"

KunrealEngine::Boss::Boss()
	: _info(), _status(BossStatus::ENTER), _boss(nullptr), _player(nullptr), _patternIndex(-1), _exPatternIndex(-1),
	_distance(0.0f), _isCorePattern(false),
	_basicPattern(), _corePattern(), _subColliderList(), _maxColliderOnCount(1),
	_isStart(false), _isHit(false), _isRotateFinish(false),
	_bossTransform(nullptr), _playerTransform(nullptr),
	_stopover(), _nodeCount(0)
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

	_bossTransform = _boss->GetComponent<Transform>();
	_playerTransform = _player->GetComponent<Transform>();

	CreateSubObject();
	SetParentToSubObject();

	SetMesh();

	SetTexture();

	SetBossTransform();
	
	SetCollider();
	RegisterCollider();
	SetColliderState(false);

	SortCorePattern();
}

void KunrealEngine::Boss::Update()
{
	GRAPHICS->DrawDebugText(500, 0, 50, "%s", _boss->GetComponent<Animator>()->GetNowAnimationName().c_str());

	// 캐릭터가 이동시
	if (InputSystem::GetInstance()->MouseButtonUp(1))
	{
		_isRotateFinish = false;
	}

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
			GRAPHICS->DeleteAllLine();
			break;
		}
		case BossStatus::CHASE:
		{
			if (_stopover.size() > 0)
			{
				for (const auto& path : _stopover)
				{
					GRAPHICS->CreateDebugLine(path.first, path.second, DirectX::XMFLOAT4(0.0f, 1.0f, 1.0f, 1.0f));
				}
			}

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

	// 일정 거리 이내가 되면 시작
	if (distance <= 1000.0f)
	{
		_isStart = true;
	}

	// 시작하면 
	if (_isStart)
	{
		// 보스 등장 애니메이션 실행
		_boss->GetComponent<Animator>()->Play("Emergence", _info._baseAnimSpeed, false);

		// 보스 등장 애니메이션 실행이 끝난다면
		if (_boss->GetComponent<Animator>()->GetIsAnimationPlaying() == false)
		{
			// 다음 애니메이션 실행을 위해 애니메이션 프레임을 0으로 되돌림
			_boss->GetComponent<Animator>()->Stop();
			// IDLE로 상태 변경
			_status = BossStatus::IDLE;
		}
	}
}

void KunrealEngine::Boss::Idle()
{
	_boss->GetComponent<Animator>()->Play("Idle", _info._baseAnimSpeed, true);

	// 핵심 패턴이 구현되어있다면
	if (!(_corePattern).empty())
	{
		// 핵심 패턴은 back에서부터 hp가 높은 순서로 되어있기에
		// 보스의 체력이 핵심 패턴의 발동 체력 이하가 된다면
		if (_info._hp <= (_corePattern).back()->_triggerHp)
		{
			// 코어패턴인지 여부 확인
			_isCorePattern = true;

			// Core Attack으로 상태 변경
			_status = BossStatus::CORE_ATTACK;

			// 현재 실행 중인 Idle 애니메이션 종료
			_boss->GetComponent<Animator>()->Stop();

			// 함수 종료
			return;
		}
	}

	// 핵심 패턴이 아닌 기본 공격일 경우에 다음 기본 패턴을 정함
	if (_patternIndex == -1 && !_isCorePattern)
	{
		// 랜덤 패턴을 위한 랜덤 인덱스를 가져옴
		_patternIndex = ToolBox::GetRandomNum(0, _basicPattern.size() - 1);

		while (_patternIndex == _exPatternIndex)
		{
			// 랜덤 패턴을 위한 랜덤 인덱스를 가져옴
			_patternIndex = ToolBox::GetRandomNum(0, _basicPattern.size() - 1);
		}

		// 가져온 랜덤 패턴이 활성화되어있지 않다면
		while ((_basicPattern)[_patternIndex]->_isActive == false)
		{
			// 가져온 랜덤 패턴이 활성화 되어 있을때까지 다시 랜덤 인덱스를 추출
			_patternIndex = ToolBox::GetRandomNum(0, _basicPattern.size() - 1);
		}
	}

	// Attack 처리를 위해 패턴 중 최대 충돌이 몇번 일어나는지를 지니고 있음  
	_maxColliderOnCount = _basicPattern[_patternIndex]->_maxColliderOnCount;

	// Chase로 상태 변환
	_status = BossStatus::CHASE;	
}

void KunrealEngine::Boss::Chase()
{
	// 현재 실행 중인 애니메이션이 Idle 이라면
	if (_boss->GetComponent<Animator>()->GetNowAnimationName() == "Run")
	{
		_boss->GetComponent<Animator>()->Play("Run", _info._baseAnimSpeed, true);
	}
	else
	{
		_boss->GetComponent<Animator>()->Play("Idle", _info._baseAnimSpeed, true);
	}

	auto patternRange = (_basicPattern)[_patternIndex]->_range;

	auto bossPosition = _bossTransform->GetPosition();

	auto playerPosition = _playerTransform->GetPosition();

	// 보스와 플레이어 사이의 거리를 측정
	_distance = ToolBox::GetDistance(bossPosition, playerPosition);

	// 패턴의 사거리를 받아옴
	if (!_isRotateFinish)
	{
		// 보스와 플레이어 사이의 각도를 측정
		auto angle = CalculateAngle(bossPosition, playerPosition);

		// 보스가 플레이어를 바라보게 함
		_isRotateFinish = LookAtPlayer(angle, _info._rotationSpeed);
	}

	// 보스가 플레이어를 바라보게 되었을 때
	if (_isRotateFinish)
	{
		// 패턴 사거리보다 플레이어와의 거리가 가까울 경우 공격 시행
		if (_distance <= 5 + patternRange)
		{
			// 현재 실행 중인 Idle 애니메이션을 종료
			_boss->GetComponent<Animator>()->Stop();

			// Basic Attack으로 상태 변경
			_status = BossStatus::BASIC_ATTACK;
			return;
		}

		// 패턴 사거리보다 플레이어와의 거리가 멀 경우
		else
		{
			// 패턴 사거리보다 플레이어의 거리가 멀지만 일정 거리 이내일 경우 걸어서 이동
			if (_distance - patternRange <= 30.0f)
			{
				// 보스와 플레이어의 갈 수 있는 길을 노드로 계산
				UpdateMoveNode();

				// 계산한 노드 백터가 존재한다면
				if (_stopover.size() > 0)
				{
					// 플레이어를 향하여 이동
					auto isChasing = MoveToPlayer(_stopover[_nodeCount].first, _stopover[_nodeCount].second, _info._moveSpeed, patternRange);

					// 첫 노드만큼 쫓아갔다면
					if (!isChasing)
					{
						// 다음 노드를 기반으로 이동
						_nodeCount++;
					}
				}

				// 모든 노드를 읽어 보스가 플레이어를 다 쫓아갔다면
				if (_nodeCount >= _stopover.size()+1)
				{
					// 현재 실행 중인 Idle 애니메이션을 종료
					_boss->GetComponent<Animator>()->Stop();
					
					// Baisc Attack으로 상태 변경
					_status = BossStatus::BASIC_ATTACK;
					return;
				}

				//
				//auto isChasing = MoveToPlayer(playerPosition, _info._moveSpeed, patternRange);

				//if (!isChasing)
				//{
				//	_boss->GetComponent<Animator>()->Stop();
				//
				//	_status = BossStatus::BASIC_ATTACK;
				//}
			}

			// 패턴 사거리보다 플레이의 거리가 멀고 일정 거리를 벗어날 경우 텔레포트로 이동
			else
			{
				TeleportToPlayer();

				// 현재 실행 중인 Idle 애니메이션을 종료
				_boss->GetComponent<Animator>()->Stop();

				// Baisc Attack으로 상태 변경
				_status = BossStatus::BASIC_ATTACK;
				return;
			}
		}
	}
}

void KunrealEngine::Boss::Hit()
{
	auto bossCollider = _boss->GetComponent<BoxCollider>();

	// 맞지 않은 상태일 때
	if (_isHit == false)
	{
		// 보스의 콜라이더가 켜져있다면
		if (bossCollider->IsCollided())
		{
			// 콜라이더가 플레이어와 접촉했다면
			if (bossCollider->GetTargetObject() == _player)
			{
				// 체력이 0이 되면
				if (_info._hp <= 0)
				{
					// 현재 실행 중인 애니메이션을 멈춤
					_boss->GetComponent<Animator>()->Stop();

					_status = BossStatus::DEAD;
				}

				// 무력화 게이지가 0이 되면
				if (_info._staggeredGauge <= 0)
				{
					// 현재 실행 중인 애니메이션을 멈춤
					_boss->GetComponent<Animator>()->Stop();

					_status = BossStatus::STAGGERED;
				}

				// 여러번 hit 되는거를 방지하기 위해 Hit이 되었다는 상태를 체크
				_isHit = true;

				// 보스의 콜라이더에 무언가가 충돌한다면
				// 플레이어의 최근 공격한 데미지를 받아와서 깍아야함
				// 
				// 보스가 플레이어를 알고 있는게 맞는가?
				_info._hp; // -= _player->GetComponent<Player>()->_playerInfo.
			}
		}
	}

	// Hit시 애니메이션 실행
	//if (_isHit)
	//{
	//	_boss->GetComponent<Animator>()->Play("Hit", ANIMATION_SPEED, false);
	//
	//	// Hit 애니메이션이 끝날때
	//	if (_boss->GetComponent<Animator>()->GetIsAnimationPlaying() == false)
	//	{
	//		// 다음 애니메이션 실행을 위해 애니메이션 프레임을 0으로 되돌림
	//		_boss->GetComponent<Animator>()->Stop();
	//		// 다시 Hit 가능하도록 맞은 상태를 false로 변경
	//		_isHit = false;
	//	}
	//}
}


void KunrealEngine::Boss::Attack()
{
	// 보스의 하위 콜라이더를 돌면서
	for (const auto& collider : _subColliderList)
	{
		// 콜라이더와 충돌하였고 그 대상이 플레이어라면
		if (collider->IsCollided() && collider->GetTargetObject() == _player)
		{
			// 여러번 공격판정이 되는거를 막기 위해 콜라이더를 끄고
			collider->SetActive(false);

			// 패턴의 최대 타격 횟수에서 하나를 감소시킴
			_maxColliderOnCount--;

			// 패턴의 데미지를 가져옴
			auto damage = _basicPattern[_patternIndex]->_damage;

			// 플레이어의 hp에서 패턴의 데미지만큼 차감시킴
			_player->GetComponent<Player>()->GetPlayerData()._hp -= damage;
		}
	}
}

void KunrealEngine::Boss::Staggred()
{
	// 무력화 애니메이션 실행
	_boss->GetComponent<Animator>()->Play("Staggred", _info._baseAnimSpeed, false);

	// 무력화 애니메이션이 끝났다면
	if (_boss->GetComponent<Animator>()->GetIsAnimationPlaying() == false)
	{
		_boss->GetComponent<Animator>()->Stop();
		_status = BossStatus::OFF_STAGGRED;
	}
}

void KunrealEngine::Boss::OffStaggred()
{
	// 무력화 종료 애니메이션 실행
	_boss->GetComponent<Animator>()->Play("OffStaggred", _info._baseAnimSpeed, false);

	// 무력화 종료 애니메이션이 끝났다면
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
	_boss->GetComponent<Animator>()->Play("Dead", _info._baseAnimSpeed, false);
}

void KunrealEngine::Boss::BasicAttack()
{
	// 패턴을 실행
	auto isPatternPlaying = (_basicPattern)[_patternIndex]->Play();

	// 패턴 실행이 끝났다면
	if (isPatternPlaying == false)
	{
		_boss->GetComponent<Animator>()->Stop();
		_status = BossStatus::PATTERN_END;
	}
}

void KunrealEngine::Boss::CoreAttack()
{
	// 패턴을 실행
	auto isPatternPlaying = (_corePattern).back()->Play();

	// 패턴 실행이 끝났다면
	if (isPatternPlaying == false)
	{
		_boss->GetComponent<Animator>()->Stop();
		_status = BossStatus::PATTERN_END;
	}
}

void KunrealEngine::Boss::PatternEnd()
{
	// 이전 패턴 인덱스 저장
	_exPatternIndex = _patternIndex;

	// Idle 애니메이션 실행
	_boss->GetComponent<Animator>()->Play("Idle", _info._baseAnimSpeed, true);

	// 모든 하위 오브젝트의 충돌 처리를 끔
	SetColliderState(false);

	// 1초동안 대기한 뒤 Idle 상태로 전환
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
	// 보스와 플레이어 사이의 각도를 계산
	auto angle = ToolBox::GetAngle(bossPosition, playerPosition);

	auto bossAngle = _boss->GetComponent<Transform>()->GetRotation().y;

	// 현재보스의 각도와 계산한 플레이어와의 각도간의 차이가 180 이하일경우
	// angle만큼 그대로 움직이면 됨 (이렇게 안하면 90, 270도 중 270도로 돌아버려 비효율적)
	if (std::abs(bossAngle - angle) < 180.0f)
	{
		return angle;
	}
	// 각도간의 차이가 180도를 넘을경우
	else
	{
		// angle이 음수일 경우
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

	auto distance = ToolBox::GetDistance(bossPosition, targetPos);

	// 패턴 거리 내에 플레이어가 존재하지 않을 경우
	if (distance > 1.0f)
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


bool KunrealEngine::Boss::MoveToPlayer(DirectX::XMFLOAT3 startPos, DirectX::XMFLOAT3 targetPos, float speed, float patternRange)
{
	float moveSpeed = speed * TimeManager::GetInstance().GetDeltaTime();

	// 패턴 거리 내에 플레이어가 존재하지 않을 경우
	if (_distance > patternRange - 5)
	{
		// 플레이어 이동
		_boss->GetComponent<Animator>()->Play("Run", speed, true);

		DirectX::XMVECTOR currentPosVec = DirectX::XMLoadFloat3(&startPos);

		DirectX::XMVECTOR direction = DirectX::XMVectorSubtract(DirectX::XMLoadFloat3(&targetPos), currentPosVec);

		direction = DirectX::XMVector3Normalize(direction);

		DirectX::XMVECTOR newPosition = DirectX::XMVectorAdd(currentPosVec, DirectX::XMVectorScale(direction, moveSpeed));
		_bossTransform->SetPosition(newPosition.m128_f32[0], 0.0f, newPosition.m128_f32[2]);

		return true;
	}

	else
	{
		return false;
	}
}

void KunrealEngine::Boss::TeleportToPlayer()
{
	auto playerDirection = _player->GetComponent<Player>()->GetDirectionVector();
	auto originPlayerPos = _playerTransform->GetPosition();
	auto currentPosVec = DirectX::XMLoadFloat3(&originPlayerPos);

	auto patternRange = (_basicPattern)[_patternIndex]->_range;
	DirectX::XMVECTOR newPlayerPosition = DirectX::XMVectorAdd(currentPosVec, DirectX::XMVectorScale(playerDirection, 10.0f + patternRange));

	// 보스와 플레이어 까지의 경로 계산
	Navigation::GetInstance().SetSEpos(1, _bossTransform->GetPosition().x, _bossTransform->GetPosition().y, _bossTransform->GetPosition().z,
		newPlayerPosition.m128_f32[0], 0.0f, newPlayerPosition.m128_f32[2]);

	_stopover = Navigation::GetInstance().FindStraightPath(1);

	if (_stopover.empty())
	{
		return;
	}

	auto bossRotation = _bossTransform->GetRotation();
	auto playerRotation = _playerTransform->GetRotation();

	// 여기에 플레이어의 방향 벡터 * 일정 거리를 곱해 플레이어와 살짝 떨어지도록 해야 함
	// 플레이어의 방향 벡터를 가져와야함

	auto target = _stopover.back().second;
	_bossTransform->SetPosition(target.x, 0.0f, target.z);

	// 플레이어를 바라보도록 
	_bossTransform->SetRotation(bossRotation.x, 180 + playerRotation.y, bossRotation.z);
}

bool KunrealEngine::Boss::LookAtPlayer(float angle, float rotateSpeed)
{
	// 회전 속도
	float speed = TimeManager::GetInstance().GetDeltaTime() * rotateSpeed;

	// 에러 범위
	float errorRange = speed * 2.0f;

	// 현재 각도가 목표로 하는 각도보다 작을 경우
	if (_bossTransform->GetRotation().y < angle)
	{
		// 회전 속도만큼 회전
		_bossTransform->SetRotation(_bossTransform->GetRotation().x, _bossTransform->GetRotation().y + speed, _bossTransform->GetRotation().z);
	}
	// 현재 각도가 목표로 하는 각도보다 클 경우
	else
	{
		// 회전 속도만큼 회전
		_bossTransform->SetRotation(_bossTransform->GetRotation().x, _bossTransform->GetRotation().y - speed, _bossTransform->GetRotation().z);
	}
	
	// 현재의 각도가 목표로 하는 각도의 오차범위 내에 있을 경우
	if (angle - errorRange <= _bossTransform->GetRotation().y && _bossTransform->GetRotation().y <= angle + errorRange)
	{
		// 현재 실행 중인 애니메이션이 Idle 이라면
		if (_boss->GetComponent<Animator>()->GetNowAnimationName() == "Idle")
		{
			// Idle 애니메이션을 멈추고
			_boss->GetComponent<Animator>()->Stop();
		}
		// 회전이 완료되었다고 반환
		return true;
	}
	else
	{
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


void KunrealEngine::Boss::SetMesh()
{

}


void KunrealEngine::Boss::SetTexture()
{

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

void KunrealEngine::Boss::UpdateMoveNode()
{
 	Navigation::GetInstance().SetSEpos(1, _bossTransform->GetPosition().x, _bossTransform->GetPosition().y, _bossTransform->GetPosition().z,
		_playerTransform->GetPosition().x, _playerTransform->GetPosition().y, _playerTransform->GetPosition().z);
 
	_stopover = Navigation::GetInstance().FindStraightPath(1);
 	_nodeCount = 0;
}

void KunrealEngine::Boss::UpdateMoveNode(DirectX::XMFLOAT3 targetPos)
{
	Navigation::GetInstance().SetSEpos(1, _bossTransform->GetPosition().x, _bossTransform->GetPosition().y, _bossTransform->GetPosition().z,
		targetPos.x, targetPos.y, targetPos.z);

	_stopover = Navigation::GetInstance().FindStraightPath(1);
	_nodeCount = 0;
}
