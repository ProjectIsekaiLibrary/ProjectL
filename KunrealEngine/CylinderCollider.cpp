#include "CylinderCollider.h"
#include "GameObject.h"
#include "Transform.h"
#include "PhysicsSystem.h"

KunrealEngine::CylinderCollider::CylinderCollider()
{

}

void KunrealEngine::CylinderCollider::Initialize()
{
	this->_ownerObj = this->GetOwner();
	this->_transform = this->_ownerObj->GetComponent<Transform>();
	this->_position = this->_transform->GetPosition();

	PhysicsSystem::GetInstance().CreateCylinderCollider(this);
	SetColliderScale(this->_transform->GetScale());
}

void KunrealEngine::CylinderCollider::Release()
{
	
}

void KunrealEngine::CylinderCollider::FixedUpdate()
{
	// 부모 오브젝트가 존재하는 경우
	if (this->GetOwner()->GetParent() != nullptr)
	{
		auto transform = this->_transform->GetWorldTM();
		DirectX::XMMATRIX worldTM = DirectX::XMLoadFloat4x4(&transform);

		DirectX::XMVECTOR scale;
		DirectX::XMVECTOR quat;
		DirectX::XMVECTOR translation;

		DirectX::XMMatrixDecompose(&scale, &quat, &translation, worldTM);

		DirectX::XMStoreFloat3(&this->_position, translation);
		this->_position.x += _offset.x;
		this->_position.y += _offset.y;
		this->_position.z += _offset.z;

		DirectX::XMStoreFloat4(&this->_quaternion, quat);
	}
	else
	{
		this->_position.x = this->GetOwner()->GetComponent<Transform>()->GetPosition().x + this->_offset.x;
		this->_position.y = this->GetOwner()->GetComponent<Transform>()->GetPosition().y + this->_offset.y;
		this->_position.z = this->GetOwner()->GetComponent<Transform>()->GetPosition().z + this->_offset.z;

		this->_quaternion = this->GetOwner()->GetComponent<Transform>()->GetQuaternion();
	}
}

void KunrealEngine::CylinderCollider::Update()
{
	
}

void KunrealEngine::CylinderCollider::LateUpdate()
{
	
}

void KunrealEngine::CylinderCollider::OnTriggerEnter()
{
	
}

void KunrealEngine::CylinderCollider::OnTriggerStay()
{
	
}

void KunrealEngine::CylinderCollider::OnTriggerExit()
{
	
}

void KunrealEngine::CylinderCollider::SetActive(bool active)
{
	this->_isActivated = active;

	if (!this->_isActivated)
	{
		this->_isCollided = false;
		this->_targetObj = nullptr;
	}

	PhysicsSystem::GetInstance().SetActorState(this, active);
}

KunrealEngine::CylinderCollider::~CylinderCollider()
{

}

void KunrealEngine::CylinderCollider::SetColliderScale(float x, float y, float z)
{
	this->_scale.x = x;
	this->_scale.y = y;
	this->_scale.z = z;

	PhysicsSystem::GetInstance().SetCylinderSize(this);
}

void KunrealEngine::CylinderCollider::SetColliderScale(const DirectX::XMFLOAT3& scale)
{
	this->_scale = scale;

	PhysicsSystem::GetInstance().SetCylinderSize(this);
}
