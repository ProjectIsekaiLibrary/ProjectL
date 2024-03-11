#include "Transform.h"

KunrealEngine::Transform::Transform()
	:_position({ 0.0f, 0.0f, 0.0f }), _rotation({ 0.0f, 0.0f, 0.0f, 1.0f }), _scale({ 1.0f, 1.0f, 1.0f }),
	_worldTM(), _quaternion()
{

}

KunrealEngine::KunrealMath::Float2 float1{ 1.0f, 1.0f };

KunrealEngine::Transform::~Transform()
{

}

void KunrealEngine::Transform::Initialize()
{

}

void KunrealEngine::Transform::Release()
{

}

void KunrealEngine::Transform::FixedUpdate()
{

}

void KunrealEngine::Transform::Update()
{
	CreateWorldTransformMatrix();
}

void KunrealEngine::Transform::LateUpdate()
{

}

void KunrealEngine::Transform::OnTriggerEnter()
{

}

void KunrealEngine::Transform::OnTriggerStay()
{

}

void KunrealEngine::Transform::OnTriggerExit()
{

}

void KunrealEngine::Transform::SetActive(bool active)
{
	/// Transform은 active 여부와 상관없이 돌아가는게 맞을까
}

void KunrealEngine::Transform::SetRotation(float x, float y, float z)
{
	this->_rotation.x = x;
	this->_rotation.y = y;
	this->_rotation.z = z;

	// local의 회전값을 world로 변환해주기 위한 쿼터니언
	DirectX::XMVECTOR rotationPitchYaw = DirectX::XMQuaternionRotationRollPitchYaw(DirectX::XMConvertToRadians(_rotation.x), DirectX::XMConvertToRadians(_rotation.y), DirectX::XMConvertToRadians(_rotation.z));
	DirectX::XMStoreFloat4(&_quaternion, rotationPitchYaw);
}

void KunrealEngine::Transform::SetPosition(float x, float y, float z)
{
	this->_position.x = x;
	this->_position.y = y;
	this->_position.z = z;
}

void KunrealEngine::Transform::SetScale(float x, float y, float z)
{
	this->_scale.x = x;
	this->_scale.y = y;
	this->_scale.z = z;
}

DirectX::XMFLOAT3 KunrealEngine::Transform::GetPosition()
{
	return this->_position;
}

DirectX::XMFLOAT4 KunrealEngine::Transform::GetRotation()
{
	return this->_rotation;
}

DirectX::XMFLOAT3 KunrealEngine::Transform::GetScale()
{
	return this->_scale;
}

DirectX::XMFLOAT4X4 KunrealEngine::Transform::CreateWorldTransformMatrix()
{
	DirectX::XMStoreFloat4x4(&_worldTM, DirectX::XMMatrixScaling(_scale.x, _scale.y, _scale.z)
		* DirectX::XMMatrixRotationQuaternion(DirectX::XMLoadFloat4(&this->_quaternion))
		* DirectX::XMMatrixTranslation(_position.x, _position.y, _position.z));

	//DirectX::XMStoreFloat4x4(&_worldTM, DirectX::XMMatrixTranslation(_position.x, _position.y, _position.z)
	//	* DirectX::XMMatrixRotationQuaternion(DirectX::XMLoadFloat4(&this->_quaternion))
	//	* DirectX::XMMatrixScaling(_scale.x, _scale.y, _scale.z));

	return _worldTM; 
}

DirectX::XMFLOAT4X4 KunrealEngine::Transform::GetWorldTM()
{
	return this->_worldTM;
}

