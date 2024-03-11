#include "BoxCollider.h"
#include "GameObject.h"
#include "Transform.h"
#include "PhysicsSystem.h"

KunrealEngine::BoxCollider::BoxCollider()
	:_isStatic(false), _boxSize(1.f, 1.f, 1.f), _position(0.f, 0.f, 0.f), _offset(0.f, 0.f, 0.f)
	, _debugObject(nullptr), _transform(nullptr), _isCollided(false), _targetObj(nullptr)
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
	
}

void KunrealEngine::BoxCollider::FixedUpdate()
{
	
}

void KunrealEngine::BoxCollider::Update()
{
	_position.x = this->GetOwner()->GetComponent<Transform>()->GetPosition().x + _offset.x;
	_position.y = this->GetOwner()->GetComponent<Transform>()->GetPosition().y + _offset.y;
	_position.z = this->GetOwner()->GetComponent<Transform>()->GetPosition().z + _offset.z;

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

void KunrealEngine::BoxCollider::SetDebugMeshData()
{
	_debugObject->SetPosition(_position.x, _position.y - (_boxSize.y / 2), _position.z);
	_debugObject->SetRotation(_transform->GetRotation().x, _transform->GetRotation().y, _transform->GetRotation().z);
	_debugObject->SetScale(_boxSize.x, _boxSize.y, _boxSize.z);

}

void KunrealEngine::BoxCollider::SetStatic()
{
	_isStatic = true;
}

void KunrealEngine::BoxCollider::SetDynamic()
{
	_isStatic = false;
}
