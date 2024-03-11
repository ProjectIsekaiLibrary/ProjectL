#include <DirectXMath.h>
#include <string>
#include "PointLight.h"

ArkEngine::ArkDX11::PointLight::PointLight(DirectX::XMFLOAT4 ambient, DirectX::XMFLOAT4 diffuse, DirectX::XMFLOAT4 specular, DirectX::XMFLOAT3 position, float radius)
	:BasicLight(ambient, diffuse, specular), _position(position), _radius(radius), _pad(0.0f)
{
	_att = DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f);

	SetPosition(position);
}

ArkEngine::ArkDX11::PointLight::~PointLight()
{
	
}

void ArkEngine::ArkDX11::PointLight::SetPosition(DirectX::XMFLOAT3 position)
{
	_position = position;
}

void ArkEngine::ArkDX11::PointLight::SetRange(float radius)
{
	_radius = radius;
}
