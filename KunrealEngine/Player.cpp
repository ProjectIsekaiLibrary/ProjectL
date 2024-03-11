#include "Player.h"
#include "Transform.h"
#include "PlayerAbility.h"
#include "PlayerMove.h"

KunrealEngine::Player::Player()
	:_transform(nullptr), _playerStatus(Status::IDLE)
	, _playerInfo(
		100.0f,			// hp
		100.0f,			// stamina
		10.0f,			// movespeed
		15.0f,			// dashspeed
		15.0f,			// dashrange
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
	throw std::logic_error("The method or operation is not implemented.");
}

void KunrealEngine::Player::SetActive(bool active)
{
	
}

void KunrealEngine::Player::AnimateByStatus()
{
	/// 현재 평상시, 달리기 애니메이션 밖에 없음
	/// 애니메이션 추가에 따라 업데이트 될 예정
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
				break;
			case KunrealEngine::Player::Status::ABILITY:
				break;
			case KunrealEngine::Player::Status::STAGGERED:
				break;
			case KunrealEngine::Player::Status::DEAD:
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
