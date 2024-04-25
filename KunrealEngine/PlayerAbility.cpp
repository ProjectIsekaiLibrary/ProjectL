#include "PlayerAbility.h"
#include "InputSystem.h"
#include "GraphicsSystem.h"
#include "MeshRenderer.h"
#include "BoxCollider.h"
#include "Ability.h"
#include "Projectile.h"
#include "Particle.h"
#include "TimeManager.h"

KunrealEngine::PlayerAbility::PlayerAbility()
	:_playerComp(nullptr), _meteor(nullptr)
{

}

KunrealEngine::PlayerAbility::~PlayerAbility()
{

}

void KunrealEngine::PlayerAbility::Initialize()
{
	_playerComp = GetOwner()->GetComponent<Player>();

	CreateAbility1();
	CreateAbility4();
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
	if (InputSystem::GetInstance()->KeyDown(KEY::R))
	{
		ResetMeteorPos();
		_meteor->SetActive(true);
	}

	_abilityContainer[0]->_abilityLogic();
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
	/// 이거 아님 참고용으로 냅둔것 나중에 고쳐야함
	Ability* shot = new Ability();
	shot->Initialize("Shot");

	shot->SetTotalData(
		"Meteor",			// 이름
		20.0f,			// 데미지
		10.0f,			// 마나
		5.0f,			// 무력화 피해량
		6.0f,			// 쿨타임
		15.0f			// 사거리
	);

	// 객체 저장
	_shot = shot->_projectile;

	// 투사체 컴포넌트 추가
	_shot->AddComponent<Projectile>();
	Projectile* shotProj = _shot->GetComponent<Projectile>();

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

	_shot->SetActive(false);

	shot->SetLogic([shot, shotProj, this]()
		{
			shot->_projectile->SetActive(true);

			shotProj->SetDirection(GetOwner());
		});

	AddToContanier(shot);
}

void KunrealEngine::PlayerAbility::ResetMeteorPos()
{
	// 마우스 3D좌표로부터 20.0f 위에 위치시키도록
	_meteor->GetComponent<Transform>()->SetPosition(GRAPHICS->ScreenToWorldPoint(InputSystem::GetInstance()->GetEditorMousePos().x, InputSystem::GetInstance()->GetEditorMousePos().y));
	_meteor->GetComponent<Transform>()->SetPosition
	(
		_meteor->GetComponent<Transform>()->GetPosition().x,
		_meteor->GetComponent<Transform>()->GetPosition().y + 40.0f,
		_meteor->GetComponent<Transform>()->GetPosition().z
	);

	// 우리 에셋으론 세워야 이쁘다
	_meteor->GetComponent<Transform>()->SetRotation(0.0f, 0.0f, -90.0f);
}

void KunrealEngine::PlayerAbility::CreateAbility4()
{
	Ability* meteor = new Ability();
	meteor->Initialize("Meteor");

	meteor->SetTotalData(
		"Meteor",			// 이름
		50.0f,			// 데미지
		10.0f,			// 마나
		15.0f,			// 무력화 피해량
		20.0f,			// 쿨타임
		15.0f			// 사거리
	);

	// 객체 저장
	_meteor = meteor->_projectile;

	// 투사체 컴포넌트 추가
	_meteor->AddComponent<Projectile>();
	Projectile* meteorProj = _meteor->GetComponent<Projectile>();

	meteorProj->SetMeshObject("Meteor/Meteor");
	meteorProj->GetCollider()->SetBoxSize(2.0f, 2.0f, 2.0f);

	// 파티클 추가	// 불타오르게
	Particle* meteorParticle = _meteor->AddComponent<Particle>();
	//Particle* meteorParticle = _meteor->GetComponent<Particle>();
	meteorParticle->SetParticleEffect("Fire", "Resources/Textures/Particles/flare.dds", 1000);
	meteorParticle->SetParticleSize(40.0f, 20.0f);
	meteorParticle->SetParticleVelocity(20.0f, true);
	meteorParticle->SetParticleDuration(2.0f, 2.0f);
	//meteorParticle->AddParticleColor(1.0f, 0.0f, 0.0f);
	meteorParticle->SetParticleDirection(0.0f, 20.0f, 0.0f);
	meteorParticle->SetActive(true);

	//Particle* meteorParticle2 = _meteor->AddComponent<Particle>();
	//meteorParticle2->SetParticleEffect("Fire2", "Resources/Textures/Particles/flare.dds", 1000);
	//meteorParticle2->SetParticleSize(20.0f, 1.0f);
	//meteorParticle2->SetParticleVelocity(50.0f, true);
	//meteorParticle2->SetParticleDuration(1.0f, 1.0f);
	////meteorParticle2->AddParticleColor(1.0f, 0.0f, 0.0f);
	//meteorParticle2->SetParticleDirection(0.0f, 20.0f, 0.0f);
	//meteorParticle2->SetActive(true);

	// 비활성화 조건		// 인게임에서는 소멸처럼
	meteorProj->SetDestoryCondition([meteorProj, this]()->bool
		{
			if (_meteor->GetComponent<Transform>()->GetPosition().y <= 2.0f)
			{
				return true;
			}
			else
			{
				return false;
			}
		});

	// 비활성화 된 상태로 시작
	_meteor->SetActive(false);

	// 전투 로직
	meteor->SetLogic([meteor, meteorProj, this]()
		{
			if (_meteor->GetActivated())
			{
				_meteor->GetComponent<Transform>()->SetPosition
				(
					_meteor->GetComponent<Transform>()->GetPosition().x,
					_meteor->GetComponent<Transform>()->GetPosition().y - 20.0f * TimeManager::GetInstance().GetDeltaTime(),
					_meteor->GetComponent<Transform>()->GetPosition().z
				);
			}
		});

	// 다 만들었으면 컨테이너에 추가
	AddToContanier(meteor);
}

void KunrealEngine::PlayerAbility::AddToContanier(Ability* abil)
{
	_abilityContainer.emplace_back(abil);
}
