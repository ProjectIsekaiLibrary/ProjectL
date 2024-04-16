#include "Player.h"
#include "Transform.h"
#include "PlayerAbility.h"
#include "PlayerMove.h"

KunrealEngine::Player::Player()
	:_transform(nullptr), _playerStatus(Status::IDLE), _tempStatus(Status::IDLE)
	, _playerInfo(
		100.0f,			// hp
		100.0f,			// stamina
		10.0f,			// movespeed
		70.0f,			// dashspeed
		40.0f,			// dashrange
		8.0f,			// dashcooldown
		1.0f,			// spellpower
		1.0f,			// damageReduce
		1.0f			// speedScale
	), _directionVector()
{
	
}

KunrealEngine::Player::~Player()
{
	
}

void KunrealEngine::Player::Initialize()
{
	// Player 컴포넌트 하나만으로 관련된 컴포넌트 모두 추가되도록
	_transform = GetOwner()->GetComponent<Transform>();
	GetOwner()->AddComponent<PlayerAbility>();
	GetOwner()->AddComponent<PlayerMove>();
}

void KunrealEngine::Player::Release()
{
	
}

void KunrealEngine::Player::FixedUpdate()
{
	
}

void KunrealEngine::Player::Update()
{
	AnimateByStatus();
}

void KunrealEngine::Player::LateUpdate()
{
	
}

void KunrealEngine::Player::OnTriggerEnter()
{
	
}

void KunrealEngine::Player::OnTriggerStay()
{
	
}

void KunrealEngine::Player::OnTriggerExit()
{
	
}

void KunrealEngine::Player::SetActive(bool active)
{
	
}

void KunrealEngine::Player::AnimateByStatus()
{
	// 상태가 변할 때 애니메이션 프레임을 0으로
	if (_tempStatus != _playerStatus)
	{
		GetOwner()->GetComponent<Animator>()->Stop();
		_tempStatus = _playerStatus;
	}

	// 상태에 따라 애니메이션 출력
	if (GetOwner()->GetComponent<MeshRenderer>() != nullptr)
	{
		switch (_playerStatus)
		{
			case KunrealEngine::Player::Status::IDLE:
				GetOwner()->GetComponent<Animator>()->Play("Idle", 30.0f * _playerInfo._speedScale, true);
				break;
			case KunrealEngine::Player::Status::WALK:
				GetOwner()->GetComponent<Animator>()->Play("Run", 30.0f * _playerInfo._speedScale, true);
				break;
			case KunrealEngine::Player::Status::DASH:
				GetOwner()->GetComponent<Animator>()->Play("Dash", 30.0f * _playerInfo._speedScale, true);
				break;
			case KunrealEngine::Player::Status::ABILITY:
				break;
			case KunrealEngine::Player::Status::STAGGERED:
				GetOwner()->GetComponent<Animator>()->Play("Staggered", 20.0f * _playerInfo._speedScale, true);
				break;
			case KunrealEngine::Player::Status::PARALYSIS:
				GetOwner()->GetComponent<Animator>()->Play("Paralysis2", 30.0f * _playerInfo._speedScale, false);
				break;
			case KunrealEngine::Player::Status::DEAD:
				GetOwner()->GetComponent<Animator>()->Play("Death", 30.0f * _playerInfo._speedScale, false);
				break;
			default:
				break;
		}
	}
}

const DirectX::XMVECTOR KunrealEngine::Player::GetDirectionVector()
{
	return this->_directionVector;
}

const KunrealEngine::Player::Status KunrealEngine::Player::GetPlayerStatus()
{
	return this->_playerStatus;
}

KunrealEngine::Player::PlayerInfo& KunrealEngine::Player::GetPlayerData()
{
	return this->_playerInfo;
}
