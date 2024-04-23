#include "PlayerAbility.h"
#include "MeshRenderer.h"
#include "Ability.h"

KunrealEngine::PlayerAbility::PlayerAbility()
	:_playerComp(nullptr)
{

}

KunrealEngine::PlayerAbility::~PlayerAbility()
{

}

void KunrealEngine::PlayerAbility::Initialize()
{
	_playerComp = GetOwner()->GetComponent<Player>();

	CreateAbility1();
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

void KunrealEngine::PlayerAbility::CreateAbility1()
{
	Ability* shot = new Ability();
	shot->SetTotalData(
		"Shot",			// 이름
		20.0f,			// 데미지
		10.0f,			// 마나
		5.0f,			// 무력화 피해량
		6.0f,			// 쿨타임
		15.0f			// 사거리
	);

	shot->SetLogic([]() 
		{
			//Projectile* projectile = new Projectile();
		});
}

void KunrealEngine::PlayerAbility::AddToContanier(Ability* abil)
{
	_abilityContainer.push_back(abil);
}
