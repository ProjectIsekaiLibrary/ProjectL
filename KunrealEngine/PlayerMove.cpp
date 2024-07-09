#include "PlayerMove.h"
#include "MeshRenderer.h"
#include "Animator.h"
#include "InputSystem.h"
#include "GraphicsSystem.h"
#include "TimeManager.h"
#include "SceneManager.h"
#include "Scene.h"
#include "Transform.h"
#include "Camera.h"
#include "Navigation.h"
#include "ToolBox.h"

KunrealEngine::PlayerMove::PlayerMove()
	:_transform(nullptr), _playerComp(nullptr), _targetPos(), _isDash(false), _isMoving(false), _isDashReady(true)
	, _stopover(), _errorRange(0.5f), _nodeCount(0), _movedRange(0.0f), _movableRange(0.0f), _posY(2.0f)
{

}

KunrealEngine::PlayerMove::~PlayerMove()
{

}

void KunrealEngine::PlayerMove::Initialize()
{
	_transform = GetOwner()->GetComponent<Transform>();
	_playerComp = GetOwner()->GetComponent<Player>();
}

void KunrealEngine::PlayerMove::Release()
{

}

void KunrealEngine::PlayerMove::FixedUpdate()
{

}

void KunrealEngine::PlayerMove::Update()
{
	// 마우스 우클릭시	// 홀드도 적용
	if (InputSystem::GetInstance()->MouseButtonUp(1))
	{
		if (_playerComp->_playerStatus == Player::Status::IDLE || _playerComp->_playerStatus == Player::Status::WALK)
		{
			GRAPHICS->DeleteAllLine();

			// 목표지점을 업데이트
			UpdateTargetPosition();
			UpdateMoveNode();

			// 이동상태로 만들어줌
			_playerComp->_playerStatus = Player::Status::WALK;
			_isMoving = true;
		}
	}

	/// 여기에 쿨타임 조건 및 플레이어 상태 조건 추가해야함
	if (InputSystem::GetInstance()->KeyDown(KEY::SPACE) && this->_isDashReady)
	{
		if (_playerComp->_playerStatus == Player::Status::IDLE || _playerComp->_playerStatus == Player::Status::WALK
			|| _playerComp->_playerStatus == Player::Status::DASH || _playerComp->_playerStatus == Player::Status::ABILITY
			)
		{
			this->_isDashReady = false;
			Startcoroutine(dashReady);

			// 이동 상태 해제
			_isMoving = false;
			_movedRange = 0.0f;
			_movableRange = 0.0f;

			UpdateTargetPosition();
			UpdateDashNode();
			_isDash = true;
		}
	}

	/// 디버깅용
	//if (_stopover.size() > 0)
	//{
	//	for (const auto& path : _stopover)
	//	{
	//		GRAPHICS->CreateDebugLine(path.first, path.second, DirectX::XMFLOAT4(1.0f, 1.0f, 0.0f, 1.0f));
	//	}
	//}

	//MoveToTarget(_targetPos, 15.f * TimeManager::GetInstance().GetDeltaTime());
	//PlayerDash(_targetPos, _playerComp->_playerInfo._dashSpeed * TimeManager::GetInstance().GetDeltaTime());

	NavigationMove(15.f * TimeManager::GetInstance().GetDeltaTime());
	NavigationDash(15.f * TimeManager::GetInstance().GetDeltaTime());

	ShowPlayerInfo();
}

void KunrealEngine::PlayerMove::LateUpdate()
{

}

void KunrealEngine::PlayerMove::OnTriggerEnter()
{

}

void KunrealEngine::PlayerMove::OnTriggerStay()
{

}

void KunrealEngine::PlayerMove::OnTriggerExit()
{

}

void KunrealEngine::PlayerMove::SetActive(bool active)
{
	this->_isActivated = active;
}

void KunrealEngine::PlayerMove::UpdateTargetPosition()
{
	/// 에디터 마우스 기준
	if (this->GetOwner()->GetObjectScene()->GetSceneName() == "Title")
	{
		_targetPos = GRAPHICS->ScreenToWorldPoint(InputSystem::GetInstance()->GetEditorMousePos().x, InputSystem::GetInstance()->GetEditorMousePos().y, -66.0f);

	}
	else
	{
		_targetPos = GRAPHICS->ScreenToWorldPoint(InputSystem::GetInstance()->GetEditorMousePos().x, InputSystem::GetInstance()->GetEditorMousePos().y);
	}

	//_targetPos = GRAPHICS->ScreenToWorldPoint(InputSystem::GetInstance()->GetEditorMousePos().x, InputSystem::GetInstance()->GetEditorMousePos().y);
}

void KunrealEngine::PlayerMove::UpdateMoveNode()
{
	// 네비게이션으로부터 이동목표 노드들을 받아옴
	Navigation::GetInstance().SetSEpos(0, _transform->GetPosition().x, _transform->GetPosition().y, _transform->GetPosition().z,
		_targetPos.x, _transform->GetPosition().y, _targetPos.z);

	_stopover = Navigation::GetInstance().FindStraightPath(0);

	// 반환 된 노드가 없으면 == 움직일 수 없는 공간을 클릭했으면
	while (_stopover.size() == 0)
	{
		if (this->_transform->GetPosition().y > this->_posY)
		{
			this->_transform->SetPosition(this->_transform->GetPosition().x, this->_posY, this->_transform->GetPosition().z);
		}

		// 플레이어 위치에서 마우스 위치로의 방향벡터
		DirectX::XMFLOAT3 currentPoint = _transform->GetPosition();
		DirectX::XMVECTOR currentVec = DirectX::XMLoadFloat3(&currentPoint);
		DirectX::XMFLOAT3 targetPoint = _targetPos;
		//DirectX::XMFLOAT3 targetWithY = { targetPoint.x, targetPoint.y + _posY, targetPoint.z };
		DirectX::XMFLOAT3 targetWithY = { targetPoint.x, _posY, targetPoint.z };
		DirectX::XMVECTOR targetPosVec = DirectX::XMLoadFloat3(&targetWithY);

		// 목표지점으로부터 플레이어 위치로의 방향벡터
		DirectX::XMVECTOR direction = ToolBox::GetDirectionVec(targetWithY, currentPoint);

		// 플레이어 방향으로 조금씩 이동
		direction = DirectX::XMVectorScale(direction, 1.0f);
		DirectX::XMVECTOR targetVector = DirectX::XMVectorAdd(targetPosVec, direction);

		// 구한 값을 목표지점에
		DirectX::XMStoreFloat3(&_targetPos, targetVector);

		Navigation::GetInstance().SetSEpos(0, _transform->GetPosition().x, _transform->GetPosition().y, _transform->GetPosition().z,
			_targetPos.x, _transform->GetPosition().y, _targetPos.z);

		_stopover = Navigation::GetInstance().FindStraightPath(0);
	}

	_nodeCount = 0;
}

void KunrealEngine::PlayerMove::UpdateDashNode()
{
	// 플레이어 위치에서 마우스 위치로의 방향벡터
	DirectX::XMFLOAT3 currentPoint = _transform->GetPosition();
	DirectX::XMVECTOR currentVec = DirectX::XMLoadFloat3(&currentPoint);
	DirectX::XMFLOAT3 targetPoint = _targetPos;
	DirectX::XMFLOAT3 targetWithY = { targetPoint.x, targetPoint.y + _posY, targetPoint.z };

	DirectX::XMVECTOR direction = ToolBox::GetDirectionVec(currentPoint, targetWithY);
	_playerComp->_directionVector = direction;

	///
	//_playerComp->CalculateSweep(direction);
	///

	// 플레이어 위치에서 방향벡터 방향으로 대시 거리만큼의 좌표
	direction = DirectX::XMVectorScale(direction, _playerComp->GetPlayerData()._dashRange);
	DirectX::XMVECTOR targetVector = DirectX::XMVectorAdd(currentVec, direction);

	// 목표지점 멤버변수에 결과값을 넣어주고
	DirectX::XMStoreFloat3(&_targetPos, targetVector);

	// 구한 좌표를 네비게이션의 SetEPos 함수에 매개변수로 넣는다
	Navigation::GetInstance().SetSEpos(0, _transform->GetPosition().x, _transform->GetPosition().y, _transform->GetPosition().z,
		_targetPos.x, _targetPos.y + _posY, _targetPos.z);

	// 목표 지점을 네비게이션의 RayCast 함수의 반환값으로 업데이트
	_targetPos = Navigation::GetInstance().FindRaycastPath(0);
	DirectX::XMVECTOR targetPosVec = DirectX::XMLoadFloat3(&_targetPos);

	// 반환받은 노드를 바탕으로 대시 가능 거리 계산
	_movableRange = ToolBox::GetDistance(currentPoint, _targetPos);

	// 두 벡터 간의 각도를 계산
	DirectX::XMVECTOR currentForward = DirectX::XMVectorSet(0.0f, _transform->GetRotation().y, -1.0f, 0.0f);

	DirectX::XMVECTOR dotResult = DirectX::XMVector3Dot(currentForward, _playerComp->_directionVector);
	float dotX = DirectX::XMVectorGetX(dotResult);

	// 각도를 라디안에서 원상태로 변환
	float angle = acos(dotX);
	angle = DirectX::XMConvertToDegrees(angle);

	// 각도가 반전되는 경우 처리
	if (targetPosVec.m128_f32[0] > currentVec.m128_f32[0])
	{
		angle *= -1;
	}

	// 플레이어의 각도를 변경
	this->_transform->SetRotation(0.0f, angle, 0.0f);

	_nodeCount = 0;
}

void KunrealEngine::PlayerMove::MoveToTarget(DirectX::XMFLOAT3 targetPos, float speed)
{
	// 평상시나 이동상태일 때
	// 스킬 사용중이거나 플레이어가 무력화 되었을 때는 작동 안함
	if (_playerComp->_playerStatus == Player::Status::IDLE || _playerComp->_playerStatus == Player::Status::WALK)
	{
		if (_isMoving)
		{
			// 이동상태로 만들어줌
			_playerComp->_playerStatus = Player::Status::WALK;

			DirectX::XMFLOAT3 trans(_transform->GetPosition().x, _transform->GetPosition().y, _transform->GetPosition().z);

			// 목표로 하는 좌표와 플레이어의 좌표가 다를 때
			if (std::abs(_transform->GetPosition().x - targetPos.x) > _errorRange ||
				std::abs(_transform->GetPosition().y - targetPos.y) > _errorRange ||
				std::abs(_transform->GetPosition().z - targetPos.z) > _errorRange)

			{
				DirectX::XMVECTOR currentPosVec = DirectX::XMLoadFloat3(&trans);
				DirectX::XMVECTOR targetPosVec = DirectX::XMLoadFloat3(&targetPos);

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

				_transform->SetRotation(0.0f, angle, 0.0f);

				DirectX::XMVECTOR direction = DirectX::XMVectorSubtract(DirectX::XMLoadFloat3(&targetPos), currentPosVec);
				direction = DirectX::XMVector3Normalize(direction);

				// 플레이어의 방향벡터 업데이트
				_playerComp->_directionVector = direction;

				// 플레이어 이동
				DirectX::XMVECTOR newPosition = DirectX::XMVectorAdd(currentPosVec, DirectX::XMVectorScale(direction, speed * _playerComp->_playerInfo._speedScale));
				_transform->SetPosition(newPosition.m128_f32[0], _posY, newPosition.m128_f32[2]);
			}
			else
			{
				// 이동이 끝나면 평상시 상태로
				_playerComp->_playerStatus = Player::Status::IDLE;
				_isMoving = false;
			}
		}
	}
}

void KunrealEngine::PlayerMove::NavigationMove(float speed)
{
	// 평상시나 이동상태일 때
	// 스킬 사용중이거나 플레이어가 무력화, 경직 상태가 되었을 때는 작동 안함
	if (_playerComp->_playerStatus == Player::Status::IDLE || _playerComp->_playerStatus == Player::Status::WALK)
	{
		// 움직임이 가능한 상태이고 노드 수만큼 이동하지 않았을 때
		if (_isMoving && _nodeCount < _stopover.size())
		{
			// 이동상태로 만들어줌
			_playerComp->_playerStatus = Player::Status::WALK;

			DirectX::XMFLOAT3 trans(_transform->GetPosition().x, _transform->GetPosition().y, _transform->GetPosition().z);

			// 목표로 하는 노드 좌표와 플레이어의 좌표가 다를 때
			if (std::abs(trans.x - _stopover[_nodeCount].second.x) > _errorRange ||
				std::abs(trans.y - _stopover[_nodeCount].second.y) > _errorRange + 100.0f ||
				std::abs(trans.z - _stopover[_nodeCount].second.z) > _errorRange)
			{
				DirectX::XMVECTOR currentPosVec = DirectX::XMLoadFloat3(&trans);
				DirectX::XMVECTOR targetPosVec = DirectX::XMLoadFloat3(&_stopover[_nodeCount].second);

				DirectX::XMVECTOR currentForward = DirectX::XMVectorSet(0.0f, 0.0f, -1.0f, 0.0f);
				DirectX::XMVECTOR targetDirection = DirectX::XMVectorSubtract(targetPosVec, currentPosVec);
				targetDirection.m128_f32[1] = 0.0f;
				targetDirection = DirectX::XMVector3Normalize(targetDirection);

				// 두 벡터 간의 각도를 계산
				DirectX::XMVECTOR dotResult = DirectX::XMVector3Dot(currentForward, targetDirection);
				float dotProduct = DirectX::XMVectorGetX(dotResult);

				// 각도를 라디안에서 도로 변환
				float angle = acos(dotProduct);
				angle = DirectX::XMConvertToDegrees(angle);

				if (targetPosVec.m128_f32[0] > currentPosVec.m128_f32[0])
				{
					angle *= -1;
				}

				// 계산한 각도로 플레이어의 rotation 변경
				_transform->SetRotation(0.0f, angle, 0.0f);

				// 방향벡터 계산
				DirectX::XMVECTOR direction = DirectX::XMVectorSubtract(DirectX::XMLoadFloat3(&_stopover[_nodeCount].second), currentPosVec);
				direction = DirectX::XMVector3Normalize(direction);

				// 플레이어의 방향벡터 업데이트
				_playerComp->_directionVector = direction;

				// 플레이어 이동
				DirectX::XMVECTOR newPosition = DirectX::XMVectorAdd(currentPosVec, DirectX::XMVectorScale(direction, speed * _playerComp->_playerInfo._speedScale));
				_transform->SetPosition(newPosition.m128_f32[0], _posY, newPosition.m128_f32[2]);
			}
			else
			{
				_nodeCount++;
			}
		}
		else if (_isMoving && _nodeCount == _stopover.size())
		{
			// 이동이 끝나면 평상시 상태로
			_playerComp->_playerStatus = Player::Status::IDLE;
			_isMoving = false;
		}
		else
		{
			// 이동 상태가 아니라면 아무 처리도 안함
		}
	}
}

void KunrealEngine::PlayerMove::NavigationDash(float speed)
{
	// 대시는 일직선으로 움직임 => 노드가 필요없다
	if (_isDash)
	{
		// 플레이어의 상태를 대시로
		this->_playerComp->_playerStatus = Player::Status::DASH;
		this->GetOwner()->GetComponent<MeshRenderer>()->SetActive(false);

		// 오브젝트의 transform
		DirectX::XMFLOAT3 trans(_transform->GetPosition().x, _transform->GetPosition().y, _transform->GetPosition().z);

		// 대시 가능 거리를 넘었는데도 움직이는 상황에 대한 예외처리
		//if (_movedRange >= _playerComp->GetPlayerData()._dashRange)
		if (_movedRange >= _movableRange)
		{
			_isDash = false;
			_playerComp->_playerStatus = Player::Status::IDLE;
		}
		// 목표로 하는 좌표와 플레이어의 좌표가 다를 때
		else if (std::abs(_transform->GetPosition().x - _targetPos.x) > _errorRange ||
			//std::abs(_transform->GetPosition().y - _targetPos.y) > _errorRange + _posY ||
			std::abs(_transform->GetPosition().z - _targetPos.z) > _errorRange)
		{

			// 플레이어의 위치와 목표 위치와의 좌표 비교과정
			DirectX::XMVECTOR currentPosVec = DirectX::XMLoadFloat3(&trans);
			DirectX::XMVECTOR targetPosVec = DirectX::XMLoadFloat3(&_targetPos);

			// 플레이어 이동
			DirectX::XMVECTOR newPosition = DirectX::XMVectorAdd(currentPosVec, DirectX::XMVectorScale(_playerComp->_directionVector, _playerComp->_playerInfo._dashSpeed * _playerComp->_playerInfo._speedScale * TimeManager::GetInstance().GetDeltaTime()));
			_transform->SetPosition(newPosition.m128_f32[0], _posY, newPosition.m128_f32[2]);

			// 이동한 거리 계산
			_movedRange += DirectX::XMVectorGetX(DirectX::XMVector3Length(DirectX::XMVectorSubtract(newPosition, currentPosVec)));
		}
		else
		{
			// 위치에 도달했다면 다시 평시 상태로
			_isDash = false;
			_playerComp->_playerStatus = Player::Status::IDLE;
		}

	}
	else
	{
		this->GetOwner()->GetComponent<MeshRenderer>()->SetActive(true);
	}
}

void KunrealEngine::PlayerMove::PlayerDash(DirectX::XMFLOAT3 targetPos, float speed)
{

	if (_isDash)
	{
		// 평상시가 이동상태일 때
		// 플레이어가 무력화 되었을 때는 작동 안함
		//if (_playerComp->_playerStatus == Player::Status::IDLE || _playerComp->_playerStatus == Player::Status::WALK)
		{
			// 플레이어의 상태를 대시로
			this->_playerComp->_playerStatus = Player::Status::DASH;

			// 오브젝트의 transform
			DirectX::XMFLOAT3 trans(_transform->GetPosition().x, _transform->GetPosition().y, _transform->GetPosition().z);

			// 플레이어의 위치와 목표 위치와의 좌표 비교과정
			DirectX::XMVECTOR currentPosVec = DirectX::XMLoadFloat3(&trans);
			DirectX::XMVECTOR targetPosVec = DirectX::XMLoadFloat3(&targetPos);

			DirectX::XMVECTOR currentForward = DirectX::XMVectorSet(0.0f, _transform->GetRotation().y, -1.0f, 0.0f);
			DirectX::XMVECTOR targetDirection = DirectX::XMVectorSubtract(targetPosVec, currentPosVec);
			//targetDirection.m128_f32[1] = 0.0f;
			targetDirection = DirectX::XMVector3Normalize(targetDirection);

			// 두 벡터 간의 각도를 계산
			auto dotResult = DirectX::XMVector3Dot(currentForward, targetDirection);
			float dotProduct = DirectX::XMVectorGetX(dotResult);

			// 각도를 라디안에서 원상태로 변환
			float angle = acos(dotProduct);
			angle = DirectX::XMConvertToDegrees(angle);

			// 각도가 반전되는 경우 처리
			if (targetPosVec.m128_f32[0] > currentPosVec.m128_f32[0])
			{
				angle *= -1;
			}

			// 오브젝트의 각도를 변경
			this->_transform->SetRotation(0.0f, angle, 0.0f);

			// 방향 벡터
			DirectX::XMVECTOR direction = DirectX::XMVectorSubtract(DirectX::XMLoadFloat3(&targetPos), currentPosVec);
			direction = DirectX::XMVector3Normalize(direction);

			// 플레이어의 방향벡터 업데이트
			_playerComp->_directionVector = direction;

			// 대시 거리만큼 이동 좌표 계산
			DirectX::XMVECTOR dashTo = DirectX::XMVectorScale(direction, _playerComp->_playerInfo._dashRange);

			// 플레이어 이동
			DirectX::XMVECTOR newPosition = DirectX::XMVectorAdd(currentPosVec, DirectX::XMVectorScale(dashTo, speed * _playerComp->_playerInfo._speedScale));
			this->_transform->SetPosition(newPosition.m128_f32[0], _posY, newPosition.m128_f32[2]);

			// 목표 좌표에 도달했을 때 대시 여부를 다시 false로
			if (_targetPos.x - _transform->GetPosition().x <= _errorRange
				&& _targetPos.y - _transform->GetPosition().y <= _errorRange
				&& _targetPos.z - _transform->GetPosition().z <= _errorRange
				)
			{
				_isDash = false;

				// 이동 후 플레이어의 상태를 평상시 상태로
				this->_playerComp->_playerStatus = Player::Status::IDLE;

			}

			// 목표 좌표가 마우스 위치로 되어있으므로 플레이어 위치에서 멈추도록
			this->_targetPos = this->_transform->GetPosition();
		}
	}
}


void KunrealEngine::PlayerMove::SetPlayerY(float y)
{
	this->_posY = y;
}


float KunrealEngine::PlayerMove::GetPlayerY()
{
	return this->_posY;
}


void KunrealEngine::PlayerMove::RecalculateNavigation()
{
	// 반환 된 노드가 없으면 == 움직일 수 없는 공간을 클릭했으면
	if (_stopover.size() == 0)
	{
		// 플레이어 위치에서 마우스 위치로의 방향벡터
		DirectX::XMFLOAT3 currentPoint = _transform->GetPosition();
		DirectX::XMVECTOR currentVec = DirectX::XMLoadFloat3(&currentPoint);
		DirectX::XMFLOAT3 targetPoint = _targetPos;
		DirectX::XMFLOAT3 targetWithY = { targetPoint.x, targetPoint.y + _posY, targetPoint.z };
		DirectX::XMVECTOR targetPosVec = DirectX::XMLoadFloat3(&targetWithY);

		// 목표지점으로부터 플레이어 위치로의 방향벡터
		DirectX::XMVECTOR direction = ToolBox::GetDirectionVec(targetWithY, currentPoint);

		// 플레이어 방향으로 조금씩 이동
		direction = DirectX::XMVectorScale(direction, 1.0f);
		DirectX::XMVECTOR targetVector = DirectX::XMVectorAdd(targetPosVec, direction);

		// 구한 값을 목표지점에
		DirectX::XMStoreFloat3(&_targetPos, targetVector);
	}
	else
	{
		return;
	}
}

void KunrealEngine::PlayerMove::ShowPlayerInfo()
{
	GRAPHICS->DrawDebugText(300, 300, 20, "%.3f", _targetPos.x);
	GRAPHICS->DrawDebugText(360, 300, 20, "%.3f", _targetPos.y);
	GRAPHICS->DrawDebugText(420, 300, 20, "%.3f", _targetPos.z);
	//GRAPHICS->DrawDebugText(300, 200, 20, "%.3f", GetOwner()->GetComponent<Animator>()->GetMaxFrame());
	//GRAPHICS->DrawDebugText(300, 100, 20, "%.3f", GetOwner()->GetComponent<Animator>()->GetCurrentFrame());

	switch (_playerComp->_playerStatus)
	{
		case Player::Status::IDLE:
			GRAPHICS->DrawDebugText(360, 400, 20, "Player : IDLE");
			break;
		case Player::Status::WALK:
			GRAPHICS->DrawDebugText(360, 400, 20, "Player : WALK");
			break;
		case Player::Status::DASH:
			GRAPHICS->DrawDebugText(360, 400, 20, "Player : DASH");
			break;
		case Player::Status::PARALYSIS:
			GRAPHICS->DrawDebugText(360, 400, 20, "Player : PARALYSIS");
			break;
		case Player::Status::ABILITY:
			GRAPHICS->DrawDebugText(360, 400, 20, "Player : ABILITY");
			break;
		case Player::Status::DEAD:
			GRAPHICS->DrawDebugText(360, 400, 20, "Player : DEAD");
			break;
		default:
			GRAPHICS->DrawDebugText(360, 400, 20, "Player : IDK");
			break;
	}
}