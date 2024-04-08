#include "PlayerAbility.h"
#include "MeshRenderer.h"
#include "Ability.h"
#include "AbilityGenerator.h"

KunrealEngine::PlayerAbility::PlayerAbility()
	:_currentSkill(nullptr)
{

}

KunrealEngine::PlayerAbility::~PlayerAbility()
{

}

void KunrealEngine::PlayerAbility::Initialize()
{
	AbilityGenerator::GetInstance().SetPlayerAbility(this);
}

void KunrealEngine::PlayerAbility::Release()
{
	
}

void KunrealEngine::PlayerAbility::FixedUpdate()
{
	
}

void KunrealEngine::PlayerAbility::Update()
{
	
}

void KunrealEngine::PlayerAbility::LateUpdate()
{
	
}

void KunrealEngine::PlayerAbility::OnTriggerEnter()
{
	
}

void KunrealEngine::PlayerAbility::OnTriggerStay()
{
	
}

void KunrealEngine::PlayerAbility::OnTriggerExit()
{
	
}

void KunrealEngine::PlayerAbility::SetActive(bool active)
{
	
}

void KunrealEngine::PlayerAbility::AddToContanier(Ability* abil)
{
	_abilityContainer.push_back(abil);
}
