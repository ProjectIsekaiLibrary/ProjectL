#include "PointLight.h"
#include "LightManager.h"
#include "MathConverter.h"
#include "IPointLight.h"

ArkEngine::IPointLight::IPointLight(unsigned int lightIndex)
	: _index(lightIndex), _activeIndex(-1), _isActive(false)
{
	SetActive(true);
}

ArkEngine::IPointLight::~IPointLight()
{
	
}

void ArkEngine::IPointLight::Delete()
{
	auto& iLightList = ArkEngine::LightManager::GetInstance()->GetIPointLightList();

	if (_isActive == true)
	{
		auto& activeLightList = ArkEngine::LightManager::GetInstance()->GetActivePointLightList();
		activeLightList[_activeIndex].DeleteDebugObject();
		activeLightList.erase(activeLightList.begin() + _activeIndex);

		for (auto index : iLightList)
		{
			if (index->GetActiveIndex() > _activeIndex)
			{
				index->MinusActiveIndex();
			}
		}

		_activeIndex = -1;
	}

	auto& lightList = ArkEngine::LightManager::GetInstance()->GetAllPointLightList();

	lightList.erase(lightList.begin() + _index);

	for (int i = _index + 1; i < iLightList.size(); i++)
	{
		iLightList[i]->MinusIndex();
	}

	iLightList.erase(iLightList.begin() + _index);

	delete this;
}

void ArkEngine::IPointLight::SetAmbient(KunrealEngine::KunrealMath::Float4 ambient)
{
	auto amb = ArkEngine::ArkDX11::ConvertFloat4(ambient);
	ArkEngine::LightManager::GetInstance()->GetAllPointLightList()[_index].SetAmbient(amb);

	if (_activeIndex > -1)
	{
		ArkEngine::LightManager::GetInstance()->GetActivePointLightList()[_activeIndex].SetAmbient(amb);
	}
}

void ArkEngine::IPointLight::SetDiffuse(KunrealEngine::KunrealMath::Float4 diffuse)
{
	auto dif = ArkEngine::ArkDX11::ConvertFloat4(diffuse);
	ArkEngine::LightManager::GetInstance()->GetAllPointLightList()[_index].SetDiffuse(dif);
	
	if (_activeIndex > -1)
	{
		ArkEngine::LightManager::GetInstance()->GetActivePointLightList()[_activeIndex].SetDiffuse(dif);
	}
}

void ArkEngine::IPointLight::SetSpecular(KunrealEngine::KunrealMath::Float4 specular)
{
	auto spec = ArkEngine::ArkDX11::ConvertFloat4(specular);
	ArkEngine::LightManager::GetInstance()->GetAllPointLightList()[_index].SetSpecular(spec);
	
	if (_activeIndex > -1)
	{
		ArkEngine::LightManager::GetInstance()->GetActivePointLightList()[_activeIndex].SetSpecular(spec);
	}
}

void ArkEngine::IPointLight::SetPosition(KunrealEngine::KunrealMath::Float3 position)
{
	auto pos = ArkEngine::ArkDX11::ConvertFloat3(position);
	ArkEngine::LightManager::GetInstance()->GetAllPointLightList()[_index].SetPosition(pos);
	
	if (_activeIndex > -1)
	{
		ArkEngine::LightManager::GetInstance()->GetActivePointLightList()[_activeIndex].SetPosition(pos);
	}
}

void ArkEngine::IPointLight::SetRange(float range)
{
	ArkEngine::LightManager::GetInstance()->GetAllPointLightList()[_index].SetRange(range/2);

	if (_activeIndex > -1)
	{
		ArkEngine::LightManager::GetInstance()->GetActivePointLightList()[_activeIndex].SetRange(range/2);
	}
}

bool ArkEngine::IPointLight::GetActive()
{
	return _isActive;
}

void ArkEngine::IPointLight::SetActive(bool ox)
{
	if (_isActive == false && ox == true)
	{
		_isActive = ox;

		auto pointLight = ArkEngine::LightManager::GetInstance()->GetAllPointLightList()[_index];

		ArkEngine::LightManager::GetInstance()->GetActivePointLightList().emplace_back(pointLight);

		_activeIndex = (unsigned int)ArkEngine::LightManager::GetInstance()->GetActivePointLightList().size() - 1;
	}

	else if (_isActive == true && ox == false)
	{
		_isActive = ox;

		auto& activeLightList = ArkEngine::LightManager::GetInstance()->GetActivePointLightList();

		activeLightList.erase(activeLightList.begin() + _activeIndex);

		auto& iLightList = ArkEngine::LightManager::GetInstance()->GetIPointLightList();

		for (auto index : iLightList)
		{
			if (index->GetActiveIndex() > _activeIndex)
			{
				index->MinusActiveIndex();
			}
		}

		_activeIndex = -1;
	}
}

int ArkEngine::IPointLight::GetActiveIndex()
{
	return _activeIndex;
}

void ArkEngine::IPointLight::MinusIndex()
{
	_index--;
}

void ArkEngine::IPointLight::MinusActiveIndex()
{
	_activeIndex--;
}
