#include "AbilityGenerator.h"
#include "Transform.h"
#include "SceneManager.h"
#include "Scene.h"
#include "GameObject.h"
#include "PlayerAbility.h"
#include "Projectile.h"

KunrealEngine::AbilityGenerator::AbilityGenerator()
	:_paComponent(nullptr)
{

}

KunrealEngine::AbilityGenerator::~AbilityGenerator()
{

}

void KunrealEngine::AbilityGenerator::SetPlayerAbility(PlayerAbility* pa)
{
	this->_paComponent = pa;
}

KunrealEngine::Ability* KunrealEngine::AbilityGenerator::Shot()
{
	/// 로직
	Ability* shot = new Ability();
	shot->SetTotalData(
		"Shot",				// 스킬 이름
		100.0f,				// 피해량
		10.0f,				// 소모 기력
		20.0f,				// 무력화 피해량
		5.0f,				// 재사용 대기시간
		10.f				// 사거리
	);

	

	_paComponent->AddToContanier(shot);
	return shot;
}

void KunrealEngine::AbilityGenerator::ShotLogic()
{
	GameObject* sp = SceneManager::GetInstance().GetCurrentScene()->CreateObject("ShotProjectile");
	GameObject* pla = SceneManager::GetInstance().GetCurrentScene()->GetGameObject("Player");
	Transform* playerTrans = pla->GetComponent<Transform>();

	sp->GetComponent<Transform>()->SetPosition(playerTrans->GetPosition().x, playerTrans->GetPosition().y, playerTrans->GetPosition().z);
	sp->AddComponent<Projectile>();

	sp->GetComponent<Projectile>()->CreateInfo(pla);
	sp->GetComponent<Projectile>()->SetMeshObject("cube/cube", "bricks.dds");


	
}
