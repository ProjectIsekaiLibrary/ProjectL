#include "Transform.h"

KunrealEngine::Transform::Transform()
	:_position({0.0f, 0.0f, 0.0f}), _rotation({ 0.0f, 0.0f, 0.0f }), _scale({ 0.0f, 0.0f, 0.0f }),
	_UIPosition({0.0f, 0.0f}), _UIScale({ 0.0f, 0.0f })
{

}

KunrealEngine::KunrealMath::Float2 float1{ 1.0f, 1.0f };

KunrealEngine::Transform::~Transform()
{

}

void KunrealEngine::Transform::Initialize()
{

}

void KunrealEngine::Transform::Finalize()
{

}

void KunrealEngine::Transform::FixedUpdate()
{

}

void KunrealEngine::Transform::Update()
{

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

/// 뭔가 필요한가..
void KunrealEngine::Transform::SetActive(bool active)
{

}

void KunrealEngine::Transform::SetRotation(float x, float y, float z)
{
	this->_rotation.x = x;
	this->_rotation.y = y;
	this->_rotation.z = z;

	CreateWorldTransformMatrix();
}

void KunrealEngine::Transform::SetPosition(float x, float y, float z)
{
	this->_position.x = x;
	this->_position.y = y;
	this->_position.z = z;

	CreateWorldTransformMatrix();
}

void KunrealEngine::Transform::SetScale(float x, float y, float z)
{
	this->_scale.x = x;
	this->_scale.y = y;
	this->_scale.z = z;

	CreateWorldTransformMatrix();
}

KunrealEngine::KunrealMath::Float3 KunrealEngine::Transform::GetPosition()
{
	return this->_position;
}

KunrealEngine::KunrealMath::Float3 KunrealEngine::Transform::GetRotation()
{
	return this->_rotation;
}

KunrealEngine::KunrealMath::Float3 KunrealEngine::Transform::GetScale()
{
	return this->_scale;
}

void KunrealEngine::Transform::SetUIPosition(float x, float y)
{
	_UIPosition.x = x;
	_UIPosition.y = y;
}

KunrealEngine::KunrealMath::Float2 KunrealEngine::Transform::GetUIPosition()
{
	return this->_UIPosition;
}

void KunrealEngine::Transform::SetUIScale(float x, float y)
{
	_UIScale.x = x;
	_UIScale.y = y;
}

KunrealEngine::KunrealMath::Float2 KunrealEngine::Transform::GetUIScale()
{
	return this->_UIScale;
}

void KunrealEngine::Transform::CreateWorldTransformMatrix()
{
	// 부모 오브젝트가 있을 때는?

	_worldTM = KunrealMath::Multiply4x4Matrix(CreateScaleMatrix(_scale), CreateQuaternionMatrix(_rotation), CreateTranslateMatrix(_position));

}

KunrealEngine::KunrealMath::Matrix4x4 KunrealEngine::Transform::GetWorldTM()
{
	return this->_worldTM;
}