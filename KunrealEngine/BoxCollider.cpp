#include "BoxCollider.h"
#include "GameObject.h"
#include "Transform.h"
#include "MeshRenderer.h"
#include "PhysicsSystem.h"

KunrealEngine::BoxCollider::BoxCollider()
	:_isStatic(false), _boxSize(1.0f, 1.0f, 1.0f), _position(0.0f, 0.0f, 0.0f), _offset(0.0f, 0.0f, 0.0f), _quaternion()
	, _debugObject(nullptr), _transform(nullptr), _isCollided(false), _targetObj(nullptr),
	_parentObject(nullptr), _parentBoneName(), _shape(nullptr)
{

}

KunrealEngine::BoxCollider::~BoxCollider()
{

}

void KunrealEngine::BoxCollider::Initialize()
{
	_transform = this->GetOwner()->GetComponent<Transform>();
	_position = this->_transform->GetPosition();
	PhysicsSystem::GetInstance().CreateDynamicBoxCollider(this);		// 기본은 Dynamic으로

	_debugObject = GRAPHICS->CreateDebugCube(this->GetOwner()->GetObjectName().c_str(), _boxSize.x, _boxSize.y, _boxSize.z);
}

void KunrealEngine::BoxCollider::Release()
{
	_debugObject->Delete();
}

void KunrealEngine::BoxCollider::FixedUpdate()
{

}

void KunrealEngine::BoxCollider::Update()
{
	// 메쉬가 존재하지 않고 콜라이더만 존재하는 경우
	if (_parentObject != nullptr)
	{
		CalculateParentBone();
	}

	// 특정 본을 부모로 하지 않을 경우
	if (!_transform->_haveParentBone)
	{
		// 부모 오브젝트는 존재하는 경우
		if (this->GetOwner()->GetParent() != nullptr)
		{
			auto transform = _transform->GetWorldTM();
			DirectX::XMMATRIX worldTM = DirectX::XMLoadFloat4x4(&transform);

			DirectX::XMVECTOR scale;
			DirectX::XMVECTOR quat;
			DirectX::XMVECTOR translation;

			DirectX::XMMatrixDecompose(&scale, &quat, &translation, worldTM);

			DirectX::XMStoreFloat3(&this->_position, translation);
			_position.x	+= _offset.x;
			_position.y	+= _offset.y;
			_position.z	+= _offset.z;

			DirectX::XMStoreFloat4(&this->_quaternion, quat);
		}
		else
		{
			_position.x = this->GetOwner()->GetComponent<Transform>()->GetPosition().x + _offset.x;
			_position.y = this->GetOwner()->GetComponent<Transform>()->GetPosition().y + _offset.y;
			_position.z = this->GetOwner()->GetComponent<Transform>()->GetPosition().z + _offset.z;

			_quaternion = this->GetOwner()->GetComponent<Transform>()->GetQuaternion();
		}
	}

	// 특정 본을 부모로 받는 경우
	else
	{
 		_position = this->GetOwner()->GetComponent<Transform>()->_posForBone;
		_position.x += _offset.x;
		_position.y += _offset.y;
		_position.z += _offset.z;

		_quaternion = this->GetOwner()->GetComponent<Transform>()->_quatForBone;
	}

	SetDebugMeshData();
}

void KunrealEngine::BoxCollider::LateUpdate()
{

}

void KunrealEngine::BoxCollider::OnTriggerEnter()
{

}

void KunrealEngine::BoxCollider::OnTriggerStay()
{

}

void KunrealEngine::BoxCollider::OnTriggerExit()
{

}

void KunrealEngine::BoxCollider::SetActive(bool active)
{
	this->_isActivated = active;
	
	if (!this->_isActivated)
	{
		this->_isCollided = false;
	}

	_debugObject->SetActvie(active);
}

bool KunrealEngine::BoxCollider::IsCollided()
{
	return this->_isCollided;
}

KunrealEngine::GameObject* KunrealEngine::BoxCollider::GetTargetObject()
{
	return this->_targetObj;
}

void KunrealEngine::BoxCollider::SetOffset(float x, float y, float z)
{
	_offset.x = x;
	_offset.y = y;
	_offset.z = z;
}

DirectX::XMFLOAT3 KunrealEngine::BoxCollider::GetOffset()
{
	return this->_offset;
}

void KunrealEngine::BoxCollider::SetBoxSize(float x, float y, float z)
{
	_boxSize.x = x;
	_boxSize.y = y;
	_boxSize.z = z;

	//_debugObject->SetPosition(_transform->GetPosition().x, _transform->GetPosition().y, _transform->GetPosition().z);
	_debugObject->SetScale(x, y, z);

	PhysicsSystem::GetInstance().SetBoxSize(this);
}

DirectX::XMFLOAT3 KunrealEngine::BoxCollider::GetBoxSize()
{
	return this->_boxSize;
}

DirectX::XMFLOAT3 KunrealEngine::BoxCollider::GetColliderPos()
{
	return this->_position;
}

DirectX::XMFLOAT4 KunrealEngine::BoxCollider::GetColliderQuat()
{
	return this->_quaternion;
}

void KunrealEngine::BoxCollider::SetDebugMeshData()
{
	if (!_transform->_haveParentBone)
	{
		_debugObject->SetPosition(_position.x, _position.y, _position.z);
		_debugObject->SetScale(_boxSize.x, _boxSize.y, _boxSize.z);
		_debugObject->SetRotation(_transform->GetRotation().x, _transform->GetRotation().y, _transform->GetRotation().z);
	}
	else
	{
		DirectX::XMFLOAT4X4 worldToDebug;

		DirectX::XMStoreFloat4x4(&worldToDebug, DirectX::XMMatrixScaling(_boxSize.x, _boxSize.y, _boxSize.z)
			* DirectX::XMMatrixRotationQuaternion(DirectX::XMLoadFloat4(&this->_quaternion))
			* DirectX::XMMatrixTranslation(_position.x, _position.y, _position.z));
		
		_debugObject->SetTransform(worldToDebug);
		
		
	}
}

void KunrealEngine::BoxCollider::SetStatic()
{
	_isStatic = true;
}

void KunrealEngine::BoxCollider::SetDynamic()
{
	_isStatic = false;
}

void KunrealEngine::BoxCollider::SetTransform(GameObject* renderable, std::string boneName)
{
	_parentObject = renderable;
	_parentBoneName = boneName;

	_transform->_haveParentBone = true;

	if (renderable != this->GetOwner())
	{
		this->GetOwner()->SetParent(renderable);
	}
}

void KunrealEngine::BoxCollider::SeperateFromParent()
{
	if (_parentObject != nullptr)
	{
		_parentObject = nullptr;
		_parentBoneName = "";

		_transform->_haveParentBone = false;
	}
}

void KunrealEngine::BoxCollider::CalculateParentBone()
{
	auto mat = GRAPHICS->GetTransform(_parentObject->GetComponent<MeshRenderer>()->GetMeshObject(), _parentBoneName);

	if (_transform->_haveParentBone)
	{
		DirectX::XMMATRIX worldTM = DirectX::XMLoadFloat4x4(&mat);

		DirectX::XMVECTOR scale;
		DirectX::XMVECTOR quat;
		DirectX::XMVECTOR translation;

		DirectX::XMMatrixDecompose(&scale, &quat, &translation, worldTM);

		DirectX::XMStoreFloat3(&this->_transform->_posForBone, translation);
		DirectX::XMStoreFloat4(&this->_transform->_quatForBone, quat);
	}
}
