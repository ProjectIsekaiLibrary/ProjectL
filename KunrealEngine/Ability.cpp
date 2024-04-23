#include "Ability.h"
#include "SceneManager.h"
#include "Scene.h"
#include "MeshRenderer.h"
#include "ImageRenderer.h"

KunrealEngine::Ability::Ability()
	:_damage(0), _stamina(0), _staggerDamage(0), _cooldown(0), _range(0), _isActivated(false), _isUsable(true), _isHit(false)
	, _effect(nullptr), _icon(nullptr)
{

}

KunrealEngine::Ability::~Ability()
{

}

void KunrealEngine::Ability::Initialize()
{

}

void KunrealEngine::Ability::Release()
{
	// 이펙트와 아이콘을 담고 있는 게임오브젝트 삭제
	if (_effect != nullptr)
	{
		SceneManager::GetInstance().GetCurrentScene()->DeleteGameObject(_effect->GetOwner());
	}

	if (_icon != nullptr)
	{
		SceneManager::GetInstance().GetCurrentScene()->DeleteGameObject(_icon->GetOwner());
	}
}

void KunrealEngine::Ability::Update()
{

}

void KunrealEngine::Ability::SetActive(bool active)
{
	
}

void KunrealEngine::Ability::SetTotalData(std::string name, float damage, float stamina, float sDamage, float cooldown, float range)
{
	this->_abilityName = name;
	this->_damage = damage;
	this->_stamina = stamina;
	this->_staggerDamage = sDamage;
	this->_cooldown = cooldown;
	this->_range = range;
}

void KunrealEngine::Ability::SetAbilityName(std::string name)
{
	this->_abilityName = name;
}

void KunrealEngine::Ability::SetDamage(float damage)
{
	this->_damage = damage;
}

void KunrealEngine::Ability::SetRequireStamina(float stamina)
{
	this->_stamina = stamina;
}

void KunrealEngine::Ability::SetStaggerDamage(float stag)
{
	this->_staggerDamage = stag;
}

void KunrealEngine::Ability::SetCoolDown(float cd)
{
	this->_cooldown = cd;
}

void KunrealEngine::Ability::SetRange(float range)
{
	this->_range = range;
}

void KunrealEngine::Ability::SetLogic(std::function<void()> logic)
{
	this->_abilityLogic = logic;
}
