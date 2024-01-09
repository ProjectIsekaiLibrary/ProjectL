#include "DirectionalLight.h"
#include "LightManager.h"
#include "MathConverter.h"
#include "IdirLight.h"

ArkEngine::IdirLight::IdirLight(unsigned int lightIndex)
	: _index(lightIndex), _activeIndex(-1), _isActive(false)
{
	SetActive(true);
}

ArkEngine::IdirLight::~IdirLight()
{

}

void ArkEngine::IdirLight::Delete()
{
	auto& iLightList = ArkEngine::LightManager::GetInstance()->GetIDirLightList();

	if (_isActive == true)
	{
		auto& activeLightList = ArkEngine::LightManager::GetInstance()->GetActiveDirLightList();

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

	auto& lightList = ArkEngine::LightManager::GetInstance()->GetAllDirLightList();
	
	lightList.erase(lightList.begin() + _index);

	for (int i = _index+1; i < iLightList.size(); i++)
	{
		iLightList[i]->MinusIndex();
	}

	iLightList.erase(iLightList.begin() + _index);

	delete this;
}

void ArkEngine::IdirLight::SetAmbient(KunrealEngine::KunrealMath::Float4 ambient)
{
	auto amb = ArkEngine::ArkDX11::ConvertFloat4(ambient);
	ArkEngine::LightManager::GetInstance()->GetAllDirLightList()[_index].SetAmbient(amb);
	
	if (_activeIndex > -1)
	{
		ArkEngine::LightManager::GetInstance()->GetActiveDirLightList()[_activeIndex].SetAmbient(amb);
	}
}

void ArkEngine::IdirLight::SetDiffuse(KunrealEngine::KunrealMath::Float4 diffuse)
{
	auto dif = ArkEngine::ArkDX11::ConvertFloat4(diffuse);
	ArkEngine::LightManager::GetInstance()->GetAllDirLightList()[_index].SetDiffuse(dif);
	
	if (_activeIndex > -1)
	{
		ArkEngine::LightManager::GetInstance()->GetActiveDirLightList()[_activeIndex].SetDiffuse(dif);
	}
}

void ArkEngine::IdirLight::SetSpecular(KunrealEngine::KunrealMath::Float4 specular)
{
	auto spec = ArkEngine::ArkDX11::ConvertFloat4(specular);
	ArkEngine::LightManager::GetInstance()->GetAllDirLightList()[_index].SetSpecular(spec);
	
	if (_activeIndex > -1)
	{
		ArkEngine::LightManager::GetInstance()->GetActiveDirLightList()[_activeIndex].SetSpecular(spec);
	}
}

void ArkEngine::IdirLight::SetDirection(KunrealEngine::KunrealMath::Float3 direction)
{
	auto dir = ArkEngine::ArkDX11::ConvertFloat3(direction);
	ArkEngine::LightManager::GetInstance()->GetAllDirLightList()[_index].SetDir(dir);
	
	if (_activeIndex > -1)
	{
		ArkEngine::LightManager::GetInstance()->GetActiveDirLightList()[_activeIndex].SetDir(dir);
	}
}

bool ArkEngine::IdirLight::GetActive()
{
	return _isActive;
}

void ArkEngine::IdirLight::SetActive(bool ox)
{
	if (_isActive == false && ox == true)
	{
		_isActive = ox;

		auto dirLight = ArkEngine::LightManager::GetInstance()->GetAllDirLightList()[_index];
		
		ArkEngine::LightManager::GetInstance()->GetActiveDirLightList().emplace_back(dirLight);

		_activeIndex = (unsigned int)ArkEngine::LightManager::GetInstance()->GetActiveDirLightList().size() - 1;
	}

	else if (_isActive == true && ox == false)
	{
		_isActive = ox;

		auto& activeLightList = ArkEngine::LightManager::GetInstance()->GetActiveDirLightList();

		activeLightList.erase(activeLightList.begin() + _activeIndex);

		auto& iLightList = ArkEngine::LightManager::GetInstance()->GetIDirLightList();

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

int ArkEngine::IdirLight::GetActiveIndex()
{
	return _activeIndex;
}

void ArkEngine::IdirLight::MinusIndex()
{
	_index--;
}

void ArkEngine::IdirLight::MinusActiveIndex()
{
	_activeIndex--;
}