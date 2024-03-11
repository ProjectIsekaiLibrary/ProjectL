#include <DirectXMath.h>
#include "DirectionalLight.h"
#include "LightManager.h"
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

void ArkEngine::IdirLight::SetAmbient(DirectX::XMFLOAT4 ambient)
{
	ArkEngine::LightManager::GetInstance()->GetAllDirLightList()[_index].SetAmbient(ambient);
	
	if (_activeIndex > -1)
	{
		ArkEngine::LightManager::GetInstance()->GetActiveDirLightList()[_activeIndex].SetAmbient(ambient);
	}
}

void ArkEngine::IdirLight::SetDiffuse(DirectX::XMFLOAT4 diffuse)
{
	ArkEngine::LightManager::GetInstance()->GetAllDirLightList()[_index].SetDiffuse(diffuse);
	
	if (_activeIndex > -1)
	{
		ArkEngine::LightManager::GetInstance()->GetActiveDirLightList()[_activeIndex].SetDiffuse(diffuse);
	}
}

void ArkEngine::IdirLight::SetSpecular(DirectX::XMFLOAT4 specular)
{
	ArkEngine::LightManager::GetInstance()->GetAllDirLightList()[_index].SetSpecular(specular);
	
	if (_activeIndex > -1)
	{
		ArkEngine::LightManager::GetInstance()->GetActiveDirLightList()[_activeIndex].SetSpecular(specular);
	}
}

void ArkEngine::IdirLight::SetDirection(DirectX::XMFLOAT3 direction)
{
	ArkEngine::LightManager::GetInstance()->GetAllDirLightList()[_index].SetDir(direction);
	
	if (_activeIndex > -1)
	{
		ArkEngine::LightManager::GetInstance()->GetActiveDirLightList()[_activeIndex].SetDir(direction);
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