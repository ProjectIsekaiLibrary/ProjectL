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

KunrealEngine::PlayerMove::PlayerMove()
	:_transform(nullptr), _playerComp(nullptr), _targetPos(), _isDash(false), _isMoving(false)
	, _stopover(), _errorRange(0.5f), _nodeCount(0)
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
	if (InputSystem::GetInstance()->MouseButtonInput(1))
	{
		// 목표지점을 업데이트
		UpdateTargetPosition();

		// 이동상태로 만들어줌
		_playerComp->_playerStatus = Player::Status::WALK;
		_isMoving = true;
	}

	/// 여기에 쿨타임 조건도 추가해야함
	if (InputSystem::GetInstance()->KeyDown(KEY::SPACE))
	{
		UpdateTargetPosition();
		_isDash = true;
	}

	MoveToTarget(_targetPos, 15.f * TimeManager::GetInstance().GetDeltaTime());
	PlayerDash(_targetPos, _playerComp->_playerInfo._dashSpeed * TimeManager::GetInstance().GetDeltaTime());

	//NavigationMove(_targetPos, 15.f * TimeManager::GetInstance().GetDeltaTime());
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

	// 노드 숫자 업데이트
	_nodeCount = _stopover.size();
}

void KunrealEngine::PlayerMove::MoveToTarget(DirectX::XMFLOAT3 targetPos, float speed)
{
	// 평상시가 이동상태일 때
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
				_transform->SetPosition(newPosition.m128_f32[0], 0, newPosition.m128_f32[2]);

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

void KunrealEngine::PlayerMove::NavigationMove(DirectX::XMFLOAT3 targetPos, float speed)
{
	if (_playerComp->_playerStatus == Player::Status::IDLE || _playerComp->_playerStatus == Player::Status::WALK)
	{
		if (_isMoving)
		{
			// 네비게이션으로부터 이동목표 노드들을 받아옴
			Navigation::GetInstance().SetSEpos(0, _transform->GetPosition().x, _transform->GetPosition().y, _transform->GetPosition().z,
						_targetPos.x, _targetPos.y, _targetPos.z);
			_stopover = Navigation::GetInstance().FindStraightPath(0);
			
			for (int i = 0; i < _stopover.size(); i++)
			{
				DirectX::XMFLOAT3 trans(_transform->GetPosition().x, _transform->GetPosition().y, _transform->GetPosition().z);

				// 목표로 하는 좌표와 플레이어의 좌표가 다를 때
				if (std::abs(_transform->GetPosition().x - _stopover[i].second.x) > _errorRange ||
					std::abs(_transform->GetPosition().y - _stopover[i].second.y) > _errorRange ||
					std::abs(_transform->GetPosition().z - _stopover[i].second.z) > _errorRange)

				{
					DirectX::XMVECTOR currentPosVec = DirectX::XMLoadFloat3(&trans);
					DirectX::XMVECTOR targetPosVec = DirectX::XMLoadFloat3(&_stopover[i].second);

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

					DirectX::XMVECTOR direction = DirectX::XMVectorSubtract(DirectX::XMLoadFloat3(&_stopover[i].second), currentPosVec);
					direction = DirectX::XMVector3Normalize(direction);

					// 플레이어의 방향벡터 업데이트
					_playerComp->_directionVector = direction;

					// 플레이어 이동
					DirectX::XMVECTOR newPosition = DirectX::XMVectorAdd(currentPosVec, DirectX::XMVectorScale(direction, speed * _playerComp->_playerInfo._speedScale));
					_transform->SetPosition(newPosition.m128_f32[0], 0, newPosition.m128_f32[2]);

					// 카메라 이동
					DirectX::XMFLOAT3 cameraPos(_tempX + newPosition.m128_f32[0], _tempY, _tempZ + newPosition.m128_f32[2]);
					SceneManager::GetInstance().GetCurrentScene()->GetMainCamera()->GetComponent<Transform>()->SetPosition(cameraPos.x, cameraPos.y, cameraPos.z);
				}
				else
				{
					int a = 10;

				}
			}

			
		}
	}
}

//void KunrealEngine::PlayerMove::NavigationMove(DirectX::XMFLOAT3 targetPos, float speed)
//{
//	navi->SetSEpos(_transform->GetPosition().x, _transform->GetPosition().y, _transform->GetPosition().z,
//		_targetPos.x, _targetPos.y, _targetPos.z
//	);
//
//	_stopover = navi->FindPath();
//
//	if (_targetPos.x != 0 && _targetPos.z != 0)
//	{
//		int a = 10;
//	}
//}

void KunrealEngine::PlayerMove::PlayerDash(DirectX::XMFLOAT3 targetPos, float speed)
{

	if (_isDash)
	{
		// 평상시가 이동상태일 때
		// 스킬 사용중이거나 플레이어가 무력화 되었을 때는 작동 안함
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
			this->_transform->SetPosition(newPosition.m128_f32[0], 0, newPosition.m128_f32[2]);

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
