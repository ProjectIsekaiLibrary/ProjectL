#include "PlayerAbility.h"
#include "MeshRenderer.h"
#include "BoxCollider.h"
#include "Ability.h"
#include "Projectile.h"
#include "InputSystem.h"

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
	for (auto abils : _abilityContainer)
	{
		abils->Release();
	}
}

void KunrealEngine::PlayerAbility::FixedUpdate()
{
	
}

void KunrealEngine::PlayerAbility::Update()
{
	/// 쿨타임 조건 필요
	if (InputSystem::GetInstance()->KeyInput(KEY::Q))
	{
		_abilityContainer[0]->_abilityLogic();
	}
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
	shot->Initialize("Shot");

	shot->SetTotalData(
		"Shot",			// 이름
		20.0f,			// 데미지
		10.0f,			// 마나
		5.0f,			// 무력화 피해량
		6.0f,			// 쿨타임
		15.0f			// 사거리
	);

	shot->_projectile->AddComponent<Projectile>();
	Projectile* shotProj = shot->_projectile->GetComponent<Projectile>();

	shotProj->SetMeshObject("Meteor/Meteor");
	shotProj->GetCollider()->SetBoxSize(2.0f, 2.0f, 2.0f);
	shotProj->SetDestoryCondition([shotProj, this]()->bool 
		{
			if (shotProj->GetCollider()->IsCollided() && shotProj->GetCollider()->GetTargetObject() != this->GetOwner())
			{
				return true;
			}
			else
			{
				return false;
			}
		});

	shot->_projectile->SetActive(false);

	shot->SetLogic([shot, shotProj, this]() 
		{
			shot->_projectile->SetActive(true);
			shot->_projectile->GetComponent<Transform>()->SetPosition(
				this->GetOwner()->GetComponent<Transform>()->GetPosition().x,
				this->GetOwner()->GetComponent<Transform>()->GetPosition().y,
				this->GetOwner()->GetComponent<Transform>()->GetPosition().z
				);

			shotProj->SetDirection(GetOwner());
		});

	AddToContanier(shot);
}

void KunrealEngine::PlayerAbility::AddToContanier(Ability* abil)
{
	_abilityContainer.emplace_back(abil);
}
