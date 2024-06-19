#include "Collider.h"

KunrealEngine::Collider::Collider()
	: _transform(nullptr), _isCollided(false), _ownerObj(nullptr), _targetObj(nullptr), _shape(nullptr)
	, _position({ 0.0f, 0.0f, 0.0f }), _scale({ 1.0f, 1.0f, 1.0f }), _quaternion({ 0.0f, 0.0f, 0.0f, 0.0f })
	, _offset({ 0.0f, 0.0f, 0.0f })
{

}

KunrealEngine::Collider::~Collider()
{

}

bool KunrealEngine::Collider::IsCollided()
{
	return this->_isCollided;
}

KunrealEngine::GameObject* KunrealEngine::Collider::GetOwnerObject()
{
	return this->_ownerObj;
}

KunrealEngine::GameObject* KunrealEngine::Collider::GetTargetObject()
{
	return this->_targetObj;
}

void KunrealEngine::Collider::SetOffset(float x, float y, float z)
{
	this->_offset.x = x;
	this->_offset.y = y;
	this->_offset.z = z;
}

void KunrealEngine::Collider::SetOffset(const DirectX::XMFLOAT3& offset)
{
	this->_offset = offset;
}

DirectX::XMFLOAT3 KunrealEngine::Collider::GetOffset()
{
	return this->_offset;
}

DirectX::XMFLOAT3 KunrealEngine::Collider::GetColliderPos()
{
	return this->_position;
}

DirectX::XMFLOAT3 KunrealEngine::Collider::GetColliderScale()
{
	return this->_scale;
}

DirectX::XMFLOAT4 KunrealEngine::Collider::GetColliderQuaternion()
{
	return this->_quaternion;
}
