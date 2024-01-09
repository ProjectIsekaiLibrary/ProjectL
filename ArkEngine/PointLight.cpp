#include <DirectXMath.h>
#include <string>
#include "DebugObject.h"
#include "MathConverter.h" 
#include "PointLight.h"

ArkEngine::ArkDX11::PointLight::PointLight(DirectX::XMFLOAT4 ambient, DirectX::XMFLOAT4 diffuse, DirectX::XMFLOAT4 specular, DirectX::XMFLOAT3 position, float radius)
	:BasicLight(ambient, diffuse, specular), _position(position), _radius(radius), _pad(0.0f), _debugObject(nullptr)
{
	_att = DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f);

	std::string lightName = "Point" + std::to_string(_radius);
	_debugObject = new DebugObject(lightName, DebugObject::eDebugType::Shpere, _position.y, 2* _radius);
}

ArkEngine::ArkDX11::PointLight::~PointLight()
{
	
}

void ArkEngine::ArkDX11::PointLight::SetPosition(DirectX::XMFLOAT3 position)
{
	_position = position;

	_debugObject->SetPosition(_position.x, _position.y, _position.z);
}

void ArkEngine::ArkDX11::PointLight::SetRange(float radius)
{
	float size = 1 + (radius - _radius) / _radius;
	_debugObject->SetScale(_radius*size, _radius*size, _radius*size);

	_radius = radius;
}

void ArkEngine::ArkDX11::PointLight::DeleteDebugObject()
{
	delete _debugObject;
}
