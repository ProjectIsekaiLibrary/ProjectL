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
	:_transform(nullptr), _playerComp(nullptr), _targetPos(), _isDash(false), _isMoving(false)
	, _stopover(), _errorRange(0.5f), _nodeCount(0), _movedRange(0.0f), _posY(0.0f)
{
	_tempX = SceneManager::GetInstance().GetCurrentScene()->GetMainCamera()->GetComponent<Transform>()->GetPosition().x - 0;
	_tempY = SceneManager::GetInstance().GetCurrentScene()->GetMainCamera()->GetComponent<Transform>()->GetPosition().y - 0;
	_tempZ = SceneManager::GetInstance().GetCurrentScene()->GetMainCamera()->GetComponent<Transform>()->GetPosition().z - 0;


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
	if (InputSystem::GetInstance()->KeyDown(KEY::SPACE))
	{
		// 이동 상태 해제
		_isMoving = false;
		_movedRange = 0.0f;

		UpdateTargetPosition();
		UpdateDashNode();
		_isDash = true;
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
	_targetPos = GRAPHICS->ScreenToWorldPoint(InputSystem::GetInstance()->GetEditorMousePos().x, InputSystem::GetInstance()->GetEditorMousePos().y);
}

void KunrealEngine::PlayerMove::UpdateMoveNode()
{ 
	// 네비게이션으로부터 이동목표 노드들을 받아옴
	Navigation::GetInstance().SetSEpos(0, _transform->GetPosition().x, _transform->GetPosition().y, _transform->GetPosition().z,
		_targetPos.x, _targetPos.y, _targetPos.z);

	_stopover = Navigation::GetInstance().FindStraightPath(0);
	_nodeCount = 0;
}

void KunrealEngine::PlayerMove::UpdateDashNode()
{
	// 네비게이션으로부터 이동목표 노드들을 받아옴
	Navigation::GetInstance().SetSEpos(0, _transform->GetPosition().x, _transform->GetPosition().y, _transform->GetPosition().z,
		_targetPos.x, _targetPos.y, _targetPos.z);

	// 대시는 raycast로 현재 위치와 목표 위치 한점만을 비교
	// 마우스 위치에 지정된 targetPos와 대시 거리를 계산
	DirectX::XMFLOAT3 targetPoint = Navigation::GetInstance().FindRaycastPath(0);
	DirectX::XMFLOAT3 currentPoint = _transform->GetPosition();
	DirectX::XMVECTOR currentVector = DirectX::XMLoadFloat3(&currentPoint);

	float distance = ToolBox::GetDistance(currentPoint, targetPoint);

	// 방향 벡터 구하기
	DirectX::XMVECTOR direction = ToolBox::GetDirectionVec(currentPoint, targetPoint);

	// 플레이어의 방향벡터 변경
	// 대시하면서 다시 연산할바에 이미 연산된 김에 여기서 추가해줌
 	_playerComp->_directionVector = direction;


	// 목표 좌표가 대시 가능 거리보다 멀리 있을 경우
	if (distance > _playerComp->GetPlayerData()._dashRange)
	{
		// 대시 거리만큼 목표 좌표 이동
		direction = DirectX::XMVectorScale(direction, _playerComp->GetPlayerData()._dashRange);
		DirectX::XMVECTOR targetVector = DirectX::XMVectorAdd(currentVector, direction);
		
		// _targetPos에 연산값을 넣어줌
		DirectX::XMStoreFloat3(&_targetPos, targetVector);

	}
	else
	{
		_targetPos = targetPoint;
	}

	DirectX::XMVECTOR targetPosVec = DirectX::XMLoadFloat3(&_targetPos);

	// 두 벡터 간의 각도를 계산
	DirectX::XMVECTOR currentForward = DirectX::XMVectorSet(0.0f, _transform->GetRotation().y, -1.0f, 0.0f);

	DirectX::XMVECTOR dotResult = DirectX::XMVector3Dot(currentForward, _playerComp->_directionVector);
	float dotX = DirectX::XMVectorGetX(dotResult);

	// 각도를 라디안에서 원상태로 변환
	float angle = acos(dotX);
	angle = DirectX::XMConvertToDegrees(angle);

	// 각도가 반전되는 경우 처리
	if (targetPosVec.m128_f32[0] > currentVector.m128_f32[0])
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

				// 카메라 이동
				DirectX::XMFLOAT3 cameraPos(_tempX + newPosition.m128_f32[0], _tempY, _tempZ + newPosition.m128_f32[2]);
				SceneManager::GetInstance().GetCurrentScene()->GetMainCamera()->GetComponent<Transform>()->SetPosition(cameraPos.x, cameraPos.y, cameraPos.z);
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

				// 카메라 이동
				DirectX::XMFLOAT3 cameraPos(_tempX + newPosition.m128_f32[0], _tempY, _tempZ + newPosition.m128_f32[2]);
				SceneManager::GetInstance().GetCurrentScene()->GetMainCamera()->GetComponent<Transform>()->SetPosition(cameraPos.x, cameraPos.y, cameraPos.z);

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

		// 오브젝트의 transform
		DirectX::XMFLOAT3 trans(_transform->GetPosition().x, _transform->GetPosition().y, _transform->GetPosition().z);

		// 대시 가능 거리를 넘었는데도 움직이는 상황에 대한 예외처리
		if (_movedRange >= _playerComp->GetPlayerData()._dashRange)
		{
			_isDash = false;
			_playerComp->_playerStatus = Player::Status::IDLE;
		}
		// 목표로 하는 좌표와 플레이어의 좌표가 다를 때
		else if (std::abs(_transform->GetPosition().x - _targetPos.x) > _errorRange ||
			std::abs(_transform->GetPosition().y - _targetPos.y) > _errorRange + 100.0f ||
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

			// 카메라 이동
			DirectX::XMFLOAT3 cameraPos(_tempX + newPosition.m128_f32[0], _tempY, _tempZ + newPosition.m128_f32[2]);
			SceneManager::GetInstance().GetCurrentScene()->GetMainCamera()->GetComponent<Transform>()->SetPosition(cameraPos.x, cameraPos.y, cameraPos.z);
		
		}
		else
		{
			// 위치에 도달했다면 다시 평시 상태로
			_isDash = false;
			_playerComp->_playerStatus = Player::Status::IDLE;
		}

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

			// 카메라 이동
			DirectX::XMFLOAT3 cameraPos(_tempX + newPosition.m128_f32[0], _tempY, _tempZ + newPosition.m128_f32[2]);
			SceneManager::GetInstance().GetCurrentScene()->GetMainCamera()->GetComponent<Transform>()->SetPosition(cameraPos.x, cameraPos.y, cameraPos.z);

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

void KunrealEngine::PlayerMove::ShowPlayerInfo()
{
	//GRAPHICS->DrawDebugText(300, 300, 20, "%.3f", _targetPos.x);
	//GRAPHICS->DrawDebugText(360, 300, 20, "%.3f", _targetPos.y);
	//GRAPHICS->DrawDebugText(420, 300, 20, "%.3f", _targetPos.z);
	GRAPHICS->DrawDebugText(300, 200, 20, "%.3f", GetOwner()->GetComponent<Animator>()->GetMaxFrame());
	GRAPHICS->DrawDebugText(300, 100, 20, "%.3f", GetOwner()->GetComponent<Animator>()->GetCurrentFrame());
	
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
		default:
			GRAPHICS->DrawDebugText(360, 400, 20, "Player : IDK");
			break;
	}
}