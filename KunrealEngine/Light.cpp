#include "Light.h"
#include "Transform.h"
#include "GameObject.h"

KunrealEngine::Light::Light()
	:_light(nullptr), _transform(nullptr), _type(LightType::None),
	_ambient(0.f, 0.f, 0.f, 0.f), _diffuse(0.f, 0.f, 0.f, 0.f), _specular(0.f, 0.f, 0.f, 0.f), _direction(0.f, 0.f, 0.f),
	_rangeP(0.f)
{

}

KunrealEngine::Light::~Light()
{

}

void KunrealEngine::Light::Initialize()
{
	this->_transform = this->GetOwner()->GetComponent<Transform>();
}

void KunrealEngine::Light::Finalize()
{
	if (_light != nullptr)
	{
		_light->Delete(); 
	}
}

void KunrealEngine::Light::FixedUpdate()
{

}

void KunrealEngine::Light::Update()
{
	SetPointPosition();
}

void KunrealEngine::Light::LateUpdate()
{

}

void KunrealEngine::Light::OnTriggerEnter()
{

}

void KunrealEngine::Light::OnTriggerStay()
{

}

void KunrealEngine::Light::OnTriggerExit()
{

}

void KunrealEngine::Light::SetActive(bool active)
{ 
	if (_light != nullptr)
	{
		_light->SetActive(active);
	}
}

KunrealEngine::LightType KunrealEngine::Light::GetLightType()
{
	if (_light != nullptr)
	{
		return this->_type;
	}
}

bool KunrealEngine::Light::GetLightStatus()
{
	if (_light == nullptr)
	{
		return false;
	}
	else
	{
		return true;
	}
}

void KunrealEngine::Light::CreateDirectionalLight(KunrealMath::Float4 ambient /*= { 0.f, 0.f, 0.f, 0.f }*/, KunrealMath::Float4 diffuse /*= { 0.f, 0.f, 0.f, 0.f }*/, KunrealMath::Float4 specular /*= { 0.f, 0.f, 0.f, 0.f }*/, KunrealMath::Float3 direction /*= { 0.f, 0.f, 0.f }*/)
{
	if (_light != nullptr)
	{
		_light->Delete();
	}

	_light = GRAPHICS->CreateDirectionalLight(ambient, diffuse, specular, direction);

	_ambient = ambient;
	_diffuse = diffuse;
	_specular = specular;
	_direction = direction;

	_type = LightType::DirectionalLight;

}

void KunrealEngine::Light::SetAmbient(float x, float y, float z, float w)
{
	if (_light == nullptr)
	{
		return;
	}

	KunrealMath::Float4 ambient(x, y, z, w);

	if (_type == LightType::DirectionalLight)
	{
		auto _castedLight = dynamic_cast<GInterface::GraphicsDirLight*>(_light);
		_castedLight->SetAmbient(ambient);
	}
	else if (_type == LightType::PointLight)
	{
		auto _castedLight = dynamic_cast<GInterface::GraphicsPointLight*>(_light);
		_castedLight->SetAmbient(ambient);
	}
	else if (_type == LightType::SpotLight)
	{
		// 현재 spotlight는 미구현
	}

	_ambient = ambient;
}

KunrealEngine::KunrealMath::Float4 KunrealEngine::Light::GetAmbient()
{
	if (_light != nullptr)
	{
		return this->_ambient;
	}
}

void KunrealEngine::Light::SetDiffuse(float x, float y, float z, float w)
{
	if (_light == nullptr)
	{
		return;
	}

	KunrealMath::Float4 diffuse(x, y, z, w);

	if (_type == LightType::DirectionalLight)
	{
		auto _castedLight = dynamic_cast<GInterface::GraphicsDirLight*>(_light);
		_castedLight->SetDiffuse(diffuse);
	}
	else if (_type == LightType::PointLight)
	{
		auto _castedLight = dynamic_cast<GInterface::GraphicsPointLight*>(_light);
		_castedLight->SetDiffuse(diffuse);
	}
	else if (_type == LightType::SpotLight)
	{
		// 현재 spotlight는 미구현
	}

	_diffuse = diffuse;
}

KunrealEngine::KunrealMath::Float4 KunrealEngine::Light::GetDiffuse()
{
	if (_light != nullptr)
	{
		return this->_diffuse;
	}


}

void KunrealEngine::Light::SetSpecular(float x, float y, float z, float w)
{
	if (_light == nullptr)
	{
		return;
	}

	KunrealMath::Float4 specular(x, y, z, w);

	if (_type == LightType::DirectionalLight)
	{
		auto _castedLight = dynamic_cast<GInterface::GraphicsDirLight*>(_light);
		_castedLight->SetSpecular(specular);
	}
	else if (_type == LightType::PointLight)
	{
		auto _castedLight = dynamic_cast<GInterface::GraphicsPointLight*>(_light);
		_castedLight->SetSpecular(specular);
	}
	else if (_type == LightType::SpotLight)
	{
		// 현재 spotlight는 미구현
	}

	_specular = specular;
}

KunrealEngine::KunrealMath::Float4 KunrealEngine::Light::GetSpecular()
{
	if (_light != nullptr)
	{
		return this->_specular;
	}
}

void KunrealEngine::Light::SetDirection(float x, float y, float z)
{
	if (_light != nullptr && _type == LightType::DirectionalLight)
	{
		KunrealMath::Float3 direction(x, y, z);

		auto _castedLight = dynamic_cast<GInterface::GraphicsDirLight*>(_light);
		_castedLight->SetDirection(direction);

		_direction = direction;
	}
}

KunrealEngine::KunrealMath::Float3 KunrealEngine::Light::GetDirection()
{
	if (_light != nullptr)
	{
		return this->_direction;
	}
}

void KunrealEngine::Light::CreatePointLight(KunrealEngine::KunrealMath::Float4 ambient /*= {0.f, 0.f, 0.f, 0.f}*/, KunrealEngine::KunrealMath::Float4 diffuse /*= { 0.f, 0.f, 0.f, 0.f }*/, KunrealEngine::KunrealMath::Float4 specular /*= { 0.f, 0.f, 0.f, 0.f }*/, float range /*= 0.f*/)
{
	if (_light != nullptr)
	{
		_light->Delete();
	}

	_light = GRAPHICS->CreatePointLight(ambient, diffuse, specular, _transform->GetPosition(), range);

	_ambient = ambient;
	_diffuse = diffuse;
	_specular = specular;
	_rangeP = range;

	_type = LightType::PointLight;
}

void KunrealEngine::Light::SetPointPosition()
{
	// 매 프레임 불리는게 맘에 안드는데..
	if (_light != nullptr && _type == LightType::PointLight)
	{
		GInterface::GraphicsPointLight* _castedLight = dynamic_cast<GInterface::GraphicsPointLight*>(_light);

		_castedLight->SetPosition(_transform->GetPosition());
	}
}

void KunrealEngine::Light::SetPointRange(float range)
{
	if (_light != nullptr && _type == LightType::PointLight)
	{
		GInterface::GraphicsPointLight* _castedLight = dynamic_cast<GInterface::GraphicsPointLight*>(_light);

		_castedLight->SetRange(range);

		_rangeP = range;
	}
}

float KunrealEngine::Light::GetPointRange()
{
	if (_light != nullptr)
	{
		return this->_rangeP;
	}
}
