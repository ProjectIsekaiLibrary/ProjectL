#include "Player.h"
#include "Transform.h"
#include "InputSystem.h"
#include "PlayerAbility.h"
#include "PlayerMove.h"
#include "MeshRenderer.h"
#include "BoxCollider.h"
#include "Transform.h"

KunrealEngine::Player::Player()
	:_transform(nullptr), _playerStatus(Status::IDLE), _tempStatus(Status::IDLE)
	, _playerInfo(
		100.0f,			// hp
		100.0f,			// stamina
		15.0f,			// movespeed
		120.0f,			// dashspeed
		40.0f,			// dashrange
		8.0f,			// dashcooldown
		1.0f,			// spellpower
		1.0f,			// damageReduce
		1.0f			// speedScale
	), _directionVector(), _abilityAnimationIndex(0)
{

}

KunrealEngine::Player::~Player()
{

}

void KunrealEngine::Player::Initialize()
{
	// Player 컴포넌트 하나만으로 관련된 컴포넌트 모두 추가되도록
	_transform = GetOwner()->GetComponent<Transform>();
	_transform->SetScale(0.1f, 0.1f, 0.1f);
	_transform->SetRotation(0.0f, 45.f, 0.0f);

	GetOwner()->SetTag("Player");
	GetOwner()->AddComponent<MeshRenderer>();
	GetOwner()->GetComponent<MeshRenderer>()->SetMeshObject("PlayerWithCloak/PlayerWithCloak");
	GetOwner()->GetComponent<MeshRenderer>()->SetActive(true);
	GetOwner()->GetComponent<MeshRenderer>()->SetPickableState(true);
	GetOwner()->GetComponent<MeshRenderer>()->SetNormalTexture(0, "PlayerWithCloak/M_Clothes_normal.png");
	GetOwner()->GetComponent<MeshRenderer>()->SetNormalTexture(1, "PlayerWithCloak/M_Body_normal.png");
	GetOwner()->GetComponent<MeshRenderer>()->SetNormalTexture(2, "PlayerWithCloak/M_Hair_normal.png");
	GetOwner()->GetComponent<MeshRenderer>()->SetCartoonState(true);

	GetOwner()->AddComponent<BoxCollider>();
	GetOwner()->GetComponent<BoxCollider>()->SetBoxSize(5.0f, 12.0f, 5.0f);
	GetOwner()->GetComponent<BoxCollider>()->SetOffset(0.0f, 8.0f, 0.0f);
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
	AfterHit();
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
	this->_isActivated = active;
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
				if (this->_abilityAnimationIndex == 2)
				{
					GetOwner()->GetComponent<Animator>()->Play("Ice", 40.0f * _playerInfo._speedScale, false);
				}
				else if (this->_abilityAnimationIndex == 3)
				{
					GetOwner()->GetComponent<Animator>()->Play("Meteor", 40.0f * _playerInfo._speedScale, false);
				}
				break;
			case KunrealEngine::Player::Status::STAGGERED:
				GetOwner()->GetComponent<Animator>()->Play("Staggered", 20.0f * _playerInfo._speedScale, true);
				break;
			case KunrealEngine::Player::Status::PARALYSIS:
				GetOwner()->GetComponent<Animator>()->Play("Paralysis2", 30.0f * _playerInfo._speedScale, false);
				break;
			case KunrealEngine::Player::Status::SWEEP:
				GetOwner()->GetComponent<Animator>()->Play("Sweep", 30.0f * _playerInfo._speedScale, false);

				if (GetOwner()->GetComponent<Animator>()->GetCurrentFrame() == GetOwner()->GetComponent<Animator>()->GetMaxFrame())
				{
					GetOwner()->GetComponent<Animator>()->Pause();
				}
				break;
			case KunrealEngine::Player::Status::DEAD:
				GetOwner()->GetComponent<Animator>()->Play("Death", 30.0f * _playerInfo._speedScale, false);
				break;
			default:
				break;
		}
	}
}

void KunrealEngine::Player::SetHitState(int patternType)
{
	/// 보스쪽에서 타입을 ENUM으로 바꾸면 매개변수도 변경예정
	if (patternType == 0)
	{
		_playerStatus = Status::PARALYSIS;
	}
	else
	{
		_playerStatus = Status::SWEEP;
	}
}


void KunrealEngine::Player::MoveToScene(std::string sceneName)
{
	this->GetOwner()->MoveToScene(sceneName);
	this->GetOwner()->GetComponent<PlayerAbility>()->_shot->MoveToScene(sceneName);
	this->GetOwner()->GetComponent<PlayerAbility>()->_ice->MoveToScene(sceneName);
	this->GetOwner()->GetComponent<PlayerAbility>()->_meteor->MoveToScene(sceneName);
}

void KunrealEngine::Player::AfterHit()
{
	if (_playerStatus == Status::PARALYSIS)
	{
		if (GetOwner()->GetComponent<Animator>()->GetCurrentFrame() >= GetOwner()->GetComponent<Animator>()->GetMaxFrame())
		{
			_playerStatus = Status::IDLE;
		}

	}
	else if (_playerStatus == Status::SWEEP)
	{

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
