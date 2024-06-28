#include "Player.h"
#include "Transform.h"
#include "InputSystem.h"
#include "PlayerAbility.h"
#include "PlayerMove.h"
#include "MeshRenderer.h"
#include "BoxCollider.h"
#include "Transform.h"
#include "GraphicsSystem.h"
#include "Navigation.h"
#include "ToolBox.h"

#include "CylinderCollider.h"

KunrealEngine::Player::Player()
	:_transform(nullptr), _playerStatus(Status::IDLE), _tempStatus(Status::IDLE), _owner(nullptr)
	, _playerInfo(
		500.0f,			// hp
		100.0f,			// stamina
		30.0f,			// movespeed
		120.0f,			// dashspeed
		40.0f,			// dashrange
		8.0f,			// dashcooldown
		1.0f,			// spellpower
		1.0f,			// damageReduce
		3.0f			// speedScale
	), _directionVector(), _abilityAnimationIndex(0),
	_isSweep(false), _sweepRange(20.0f), _movedRange(0.0f), _sweepDuration(1.0f), _sweepNode(), _sweepAnimationSpeed(30.0f), _gravity(-5.81f), _nodeCount(0)
{
	_sweepNode.clear();
}

KunrealEngine::Player::~Player()
{

}

void KunrealEngine::Player::Initialize()
{
	// Player 컴포넌트 하나만으로 관련된 컴포넌트 모두 추가되도록
	this->_owner = GetOwner();

	this->_transform = _owner->GetComponent<Transform>();
	this->_transform->SetScale(0.1f, 0.1f, 0.1f);
	this->_transform->SetRotation(0.0f, 45.f, 0.0f);

	this->_owner->SetTag("Player");
	this->_owner->AddComponent<MeshRenderer>();
	this->_owner->GetComponent<MeshRenderer>()->SetMeshObject("PlayerWithCloak/PlayerWithCloak");
	this->_owner->GetComponent<MeshRenderer>()->SetActive(true);
	this->_owner->GetComponent<MeshRenderer>()->SetPickableState(true);
	this->_owner->GetComponent<MeshRenderer>()->SetNormalTexture(0, "PlayerWithCloak/M_Clothes_normal.png");
	this->_owner->GetComponent<MeshRenderer>()->SetNormalTexture(1, "PlayerWithCloak/M_Body_normal.png");
	this->_owner->GetComponent<MeshRenderer>()->SetNormalTexture(2, "PlayerWithCloak/M_Hair_normal.png");
	this->_owner->GetComponent<MeshRenderer>()->SetCartoonState(true);
	
	this->_owner->AddComponent<BoxCollider>();
	this->_owner->GetComponent<BoxCollider>()->SetColliderScale(5.0f, 12.0f, 5.0f);
	this->_owner->GetComponent<BoxCollider>()->SetOffset(0.0f, 8.0f, 0.0f);

	//this->_owner->AddComponent<CylinderCollider>();
	//this->_owner->GetComponent<CylinderCollider>()->SetColliderScale(5.0f, 12.0f, 5.0f);
	//this->_owner->GetComponent<CylinderCollider>()->SetOffset(0.0f, 8.0f, 0.0f);
	this->_owner->AddComponent<PlayerAbility>();
	this->_owner->AddComponent<PlayerMove>();

	
	// 잘들어가나
	// this->_owner->AddComponent<CylinderCollider>();
	//this->_owner->GetComponent<CylinderCollider>()->SetColliderScale(2.0f, 4.0f, 2.0f);
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
	CheckDeath();
	PlayerSweep();

	DebugFunc();

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
		this->_owner->GetComponent<Animator>()->Stop();
		_tempStatus = _playerStatus;
	}

	// 상태에 따라 애니메이션 출력
	if (this->_owner->GetComponent<MeshRenderer>() != nullptr)
	{
		switch (_playerStatus)
		{
			case KunrealEngine::Player::Status::IDLE:
				this->_owner->GetComponent<Animator>()->Play("Idle", 30.0f * _playerInfo._speedScale, true);
				break;
			case KunrealEngine::Player::Status::WALK:
				this->_owner->GetComponent<Animator>()->Play("Run", 30.0f * _playerInfo._speedScale, true);
				break;
			case KunrealEngine::Player::Status::DASH:
				this->_owner->GetComponent<Animator>()->Play("Dash", 30.0f * _playerInfo._speedScale, true);
				break;
			case KunrealEngine::Player::Status::ABILITY:
				if (this->_abilityAnimationIndex == 1)
				{
					this->_owner->GetComponent<Animator>()->Play("Shot", 40.0f * _playerInfo._speedScale, false);
				}
				else if (this->_abilityAnimationIndex == 2)
				{
					this->_owner->GetComponent<Animator>()->Play("Ice", 40.0f * _playerInfo._speedScale, false);
				}
				else if (this->_abilityAnimationIndex == 3)
				{
					this->_owner->GetComponent<Animator>()->Play("Area", 40.0f * _playerInfo._speedScale, false);
				}
				else if (this->_abilityAnimationIndex == 4)
				{
					this->_owner->GetComponent<Animator>()->Play("Meteor", 40.0f * _playerInfo._speedScale, false);
				}
				break;
			case KunrealEngine::Player::Status::STAGGERED:
				this->_owner->GetComponent<Animator>()->Play("Staggered", 20.0f * _playerInfo._speedScale, true);
				break;
			case KunrealEngine::Player::Status::PARALYSIS:
				this->_owner->GetComponent<Animator>()->Play("SmallHit", 60.0f * _playerInfo._speedScale, false);
				break;
			case KunrealEngine::Player::Status::SWEEP:
				this->_owner->GetComponent<Animator>()->Play("Sweep", _sweepAnimationSpeed * _playerInfo._speedScale, false);

				if (this->_owner->GetComponent<Animator>()->GetCurrentFrame() <= 10.0f)
				{
					this->_sweepAnimationSpeed = 70.0f;
				}
				else
				{
					this->_sweepAnimationSpeed = 30.0f;
				}

				//if (this->_owner->GetComponent<Animator>()->GetCurrentFrame() >= 30.f && this->_transform->GetPosition().y > 4.0f)
				//{
				//	this->_owner->GetComponent<Animator>()->SetCurrentFrame(36);
				//}

				if (this->_owner->GetComponent<Animator>()->GetCurrentFrame() == this->_owner->GetComponent<Animator>()->GetMaxFrame())
				{
					this->_owner->GetComponent<Animator>()->Pause();
				}
				break;
			case KunrealEngine::Player::Status::DEAD:
				this->_owner->GetComponent<Animator>()->Play("Death", 30.0f * _playerInfo._speedScale, false);
				break;
			default:
				break;
		}
	}
}

void KunrealEngine::Player::SetHitState(int patternType)
{
	if (patternType == 1)
	{
		_playerStatus = Status::PARALYSIS;
	}
	else if (patternType == 2)
	{
		_playerStatus = Status::SWEEP;
	}
}


void KunrealEngine::Player::CalculateSweep(DirectX::XMVECTOR direction)
{
	// 이미 누워있거나 날아가고 있으면 더 안날아가게
	if (this->_playerStatus == Status::SWEEP)
	{
		return;
	}

	// 노드를 비워준다
	this->_sweepNode.clear();
	this->_nodeCount = 0;

	this->_directionVector = DirectX::XMVectorNegate(direction);		// 바라보는 방향 반전

	// 플레이어 방향 회전
	DirectX::XMVECTOR currentForward = DirectX::XMVectorSet(0.0f, _transform->GetRotation().y, -1.0f, 0.0f);
	DirectX::XMVECTOR dotResult = DirectX::XMVector3Dot(currentForward, this->_directionVector);
	float dotX = DirectX::XMVectorGetX(dotResult);
	float angle = acos(dotX);
	angle = DirectX::XMConvertToDegrees(angle);
	
	// 플레이어 이동
	DirectX::XMFLOAT3 currentPoint = _transform->GetPosition();
	DirectX::XMVECTOR currentVector = DirectX::XMLoadFloat3(&currentPoint);

	// 이동 목표 지점
	DirectX::XMVECTOR targetVector = DirectX::XMVectorAdd(currentVector, DirectX::XMVectorScale(direction, this->_sweepRange));
	DirectX::XMFLOAT3 targetPoint;
	DirectX::XMStoreFloat3(&targetPoint, targetVector);

	// 각도가 반전되는 경우 처리
	if (targetVector.m128_f32[0] < currentVector.m128_f32[0])
	{
		angle *= -1;
	}
	this->_transform->SetRotation(0.0f, angle, 0.0f);

	// 직선으로 이동하니 Navigation을 통해 이동할 수 있는 위치 계산
	Navigation::GetInstance().SetSEpos(0, _transform->GetPosition().x, _transform->GetPosition().y, _transform->GetPosition().z,
		targetPoint.x, this->_owner->GetComponent<PlayerMove>()->GetPlayerY(), targetPoint.z);

	targetPoint = Navigation::GetInstance().FindRaycastPath(0);

	// 포물선 좌표 계산
	ToolBox::CalculateParabolaPath(_transform->GetPosition(), targetPoint, _sweepDuration, _gravity, _sweepNode);

	// 맵의 바닥 높이에 맞춰 마지막 노드 y값 재설정
	_sweepNode.back().y = this->_owner->GetComponent<PlayerMove>()->GetPlayerY();

	// 날아갈 준비 완료
	this->_isSweep = true;
}


void KunrealEngine::Player::CalculateSweep(DirectX::XMFLOAT3 direction)
{
	// 이미 누워있거나 날아가고 있으면 더 안날아가게
	if (this->_playerStatus == Status::SWEEP)
	{
		return;
	}

	// 노드를 비워준다
	this->_sweepNode.clear();
	this->_nodeCount = 0;
	DirectX::XMVECTOR directionVec = DirectX::XMLoadFloat3(&direction);

	this->_directionVector = DirectX::XMVectorNegate(directionVec);		// 바라보는 방향 반전

	// 플레이어 방향 회전
	DirectX::XMVECTOR currentForward = DirectX::XMVectorSet(0.0f, _transform->GetRotation().y, -1.0f, 0.0f);
	DirectX::XMVECTOR dotResult = DirectX::XMVector3Dot(currentForward, this->_directionVector);
	float dotX = DirectX::XMVectorGetX(dotResult);
	float angle = acos(dotX);
	angle = DirectX::XMConvertToDegrees(angle);

	// 플레이어 이동
	DirectX::XMFLOAT3 currentPoint = _transform->GetPosition();
	DirectX::XMVECTOR currentVector = DirectX::XMLoadFloat3(&currentPoint);

	// 이동 목표 지점
	DirectX::XMVECTOR targetVector = DirectX::XMVectorAdd(currentVector, DirectX::XMVectorScale(directionVec, this->_sweepRange));
	DirectX::XMFLOAT3 targetPoint;
	DirectX::XMStoreFloat3(&targetPoint, targetVector);

	// 각도가 반전되는 경우 처리
	if (targetVector.m128_f32[0] < currentVector.m128_f32[0])
	{
		angle *= -1;
	}
	this->_transform->SetRotation(0.0f, angle, 0.0f);

	// 직선으로 이동하니 Navigation을 통해 이동할 수 있는 위치 계산
	Navigation::GetInstance().SetSEpos(0, _transform->GetPosition().x, _transform->GetPosition().y, _transform->GetPosition().z,
		targetPoint.x, this->_owner->GetComponent<PlayerMove>()->GetPlayerY(), targetPoint.z);

	targetPoint = Navigation::GetInstance().FindRaycastPath(0);

	// 포물선 좌표 계산
	ToolBox::CalculateParabolaPath(_transform->GetPosition(), targetPoint, _sweepDuration, _gravity, _sweepNode);

	// 맵의 바닥 높이에 맞춰 마지막 노드 y값 재설정
	_sweepNode.back().y = this->_owner->GetComponent<PlayerMove>()->GetPlayerY();

	// 날아갈 준비 완료
	this->_isSweep = true;
}

void KunrealEngine::Player::PlayerSweep()
{
	/// 열심히 만들자
	/// 노드 수만큼 돌려야겠네
	if (this->_isSweep && this->_nodeCount < this->_sweepNode.size())
	{
		// 플레이어를 날아가는 상태로
		this->_playerStatus = Status::SWEEP;

		// 더 멀리 날아가는것 방지 안전장치
		//if (_movedRange >= _sweepRange)
		//{
		//	this->_isSweep = false;
		//	this->_playerStatus = Status::IDLE;
		//}

		//float absx = std::abs(_transform->GetPosition().x - _sweepNode[_nodeCount].x);
		//float absy = std::abs(_transform->GetPosition().y - _sweepNode[_nodeCount].y);
		//float absz = std::abs(_transform->GetPosition().z - _sweepNode[_nodeCount].z);

		if
			(
				std::abs(_transform->GetPosition().x - _sweepNode[_nodeCount].x) > 0.3f ||
				//std::abs(_transform->GetPosition().y - _sweepNode[_nodeCount].y) > 0.3f ||
				std::abs(_transform->GetPosition().z - _sweepNode[_nodeCount].z) > 0.3f
				)
		{
			this->_transform->SetPosition(_sweepNode[_nodeCount].x, _sweepNode[_nodeCount].y, _sweepNode[_nodeCount].z);
		}
		else
		{
			this->_nodeCount++;
		}

	}
	// 목표지점으로 도착하면 
	else if (this->_isSweep && this->_nodeCount == this->_sweepNode.size())
	{
// 		this->_playerStatus = Status::IDLE;
// 		this->_isSweep = false;
		Startcoroutine(afterSweep);
	}
}

void KunrealEngine::Player::MoveToScene(std::string sceneName)
{
	this->_owner->MoveToScene(sceneName);
	this->_owner->GetComponent<PlayerAbility>()->_shot->SetActive(false);
	this->_owner->GetComponent<PlayerAbility>()->_shot->MoveToScene(sceneName);
	this->_owner->GetComponent<PlayerAbility>()->_ice->SetActive(false);
	this->_owner->GetComponent<PlayerAbility>()->_ice->MoveToScene(sceneName);
	this->_owner->GetComponent<PlayerAbility>()->_meteor->SetActive(false);
	this->_owner->GetComponent<PlayerAbility>()->_meteor->MoveToScene(sceneName);
}

void KunrealEngine::Player::AfterHit()
{
	if (_playerStatus == Status::PARALYSIS)
	{
		if (this->_owner->GetComponent<Animator>()->GetCurrentFrame() >= this->_owner->GetComponent<Animator>()->GetMaxFrame())
		{
			_playerStatus = Status::IDLE;
		}

	}
	else if (_playerStatus == Status::SWEEP)
	{
		if (GetOwner()->GetComponent<Animator>()->GetCurrentFrame() >= GetOwner()->GetComponent<Animator>()->GetMaxFrame())
		{
			Startcoroutine(afterSweep);
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

void KunrealEngine::Player::CheckDeath()
{
	if (this->_playerInfo._hp <= 0.0f)
	{
		this->_playerStatus = Status::DEAD;
	}
}

void KunrealEngine::Player::DebugFunc()
{
	// O 누르면 하위객체들 비활성화 왜켜지는지 아무도 모름
	if (InputSystem::GetInstance()->KeyDown(KEY::O))
	{
		this->_owner->GetComponent<PlayerAbility>()->_shot->SetActive(false);
		this->_owner->GetComponent<PlayerAbility>()->_ice->SetActive(false);
		this->_owner->GetComponent<PlayerAbility>()->_meteor->SetActive(false);
	}

	// P누르면 부활
	if (this->_playerStatus == Status::DEAD && InputSystem::GetInstance()->KeyDown(KEY::P))
	{
		this->_playerInfo._hp = 500.0f;
		this->_playerStatus = Status::IDLE;
	}

	GRAPHICS->DrawDebugText(100, 400, 40, "%.3f", this->_playerInfo._hp);

	if (this->GetOwner()->GetComponent<BoxCollider>()->GetTargetObject() == nullptr)
	{
		GRAPHICS->DrawDebugText(100, 600, 40, "NULL");
	}
	else
	{
		GRAPHICS->DrawDebugText(100, 600, 40, this->GetOwner()->GetComponent<BoxCollider>()->GetTargetObject()->GetObjectName().c_str());
	}
	//GRAPHICS->DrawColorText()

}
