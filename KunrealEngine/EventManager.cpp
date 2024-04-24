#include "EventManager.h"
#include "GameObject.h"
#include "Player.h"
#include "PlayerAbility.h"
#include "PlayerMove.h"

KunrealEngine::EventManager::EventManager()
	:_player(nullptr), _boss(nullptr)
{

}

KunrealEngine::EventManager::~EventManager()
{

}

void KunrealEngine::EventManager::Initialize()
{

}

void KunrealEngine::EventManager::Release()
{

}

void KunrealEngine::EventManager::Update()
{
	
}

void KunrealEngine::EventManager::SetBossObject()
{
	/// 스테이지에 따라서 보스 객체 정해주는거 필요
}

//void KunrealEngine::EventManager::CalculateDamageToBoss(Ability abil)
//{
//	//_boss->GetComponent<Kamen>()->GetInfo()._hp - abil._damage;
//}
