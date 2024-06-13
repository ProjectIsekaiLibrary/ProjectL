#include "PlayerAbility.h"
#include "InputSystem.h"
#include "GraphicsSystem.h"
#include "MeshRenderer.h"
#include "BoxCollider.h"
#include "Particle.h"
#include "TimeManager.h"
#include "ToolBox.h"
#include "Animator.h"
#include "Transform.h"

#include "PlayerMove.h"
#include "Boss.h"
#include "Kamen.h"
#include "Ent.h"
#include "Aracne.h"

#include "SceneManager.h"
#include "Scene.h"

KunrealEngine::PlayerAbility::PlayerAbility()
	:_playerComp(nullptr), _meteor(nullptr), _shot(nullptr), _ice(nullptr), _area(nullptr)
	, _isIceReady(true), _destroyIce(false), _isShotReady(true), _isMeteorReady(true), _isAreaReady(true)
	, _isShotHit(false), _isIceHit(false), _isAreaHit(false), _isMeteorHit(false)
	, _currentBoss(nullptr), _currentDamage(0.0f)
	, _isShotDetected(false), _isIceDetected(false), _isAreaDetected(false), _isMeteorDetected(false)
{

}

KunrealEngine::PlayerAbility::~PlayerAbility()
{

}

void KunrealEngine::PlayerAbility::Initialize()
{
	_playerComp = GetOwner()->GetComponent<Player>();

	CreateAbility1();
	CreateAbility2();
	CreateAbility3();
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
	/// 함수로 나누자 나중에
	/// 쿨타임 조건 필요

	if (InputSystem::GetInstance()->KeyDown(KEY::Q) && this->_isShotReady)
	{
		ResetShotPos();
		_isShotDetected = true;
		_isShotReady = false;
		_isShotHit = true;
		Startcoroutine(shotCoolDown);
		_shot->SetActive(true);
		_shot->GetComponent<Projectile>()->SetActive(true);
		_shot->GetComponent<Projectile>()->ResetCondition();

		_playerComp->_playerStatus = Player::Status::ABILITY;
		_playerComp->_abilityAnimationIndex = 1;				// 구체 투척 애니메이션
	}

	if (InputSystem::GetInstance()->KeyDown(KEY::W) && this->_isIceReady)
	{
		ResetIcePos();											// 투사체 위치 리셋
		_isIceDetected = true;
		_isIceHit = true;
		Startcoroutine(iceCoolDown);
		Startcoroutine(iceStandby);								// 얼음 출현 대기	
		_playerComp->_playerStatus = Player::Status::ABILITY;
		_playerComp->_abilityAnimationIndex = 2;				// 얼음 소환 애니메이션
		Startcoroutine(iceDestroy);
	}

	if (InputSystem::GetInstance()->KeyDown(KEY::E) && this->_isAreaReady)
	{
		ResetAreaPos();
		_isAreaDetected = true;
		_isAreaHit = true;
		Startcoroutine(AreaCoolDown);
		_area->GetComponent<BoxCollider>()->SetActive(true);
		_playerComp->_playerStatus = Player::Status::ABILITY;
		_playerComp->_abilityAnimationIndex = 3;				// 범위 공격 애니메이션
	}

	if (InputSystem::GetInstance()->KeyDown(KEY::R) && this->_isMeteorReady)
	{
		ResetMeteorPos();
		_isMeteorDetected = true;
		_isMeteorHit = true;
		_isMeteorReady = false;
		Startcoroutine(meteorCoolDown);
		_playerComp->_playerStatus = Player::Status::ABILITY;
		_playerComp->_abilityAnimationIndex = 4;
	}

	/// 교통정리 필요
	if (_playerComp->_playerStatus == Player::Status::ABILITY && _playerComp->_abilityAnimationIndex == 1 && GetOwner()->GetComponent<Animator>()->GetCurrentFrame() >= GetOwner()->GetComponent<Animator>()->GetMaxFrame())
	{
		_playerComp->_playerStatus = Player::Status::IDLE;
	}

	if (_playerComp->_playerStatus == Player::Status::ABILITY && _playerComp->_abilityAnimationIndex == 2 && GetOwner()->GetComponent<Animator>()->GetCurrentFrame() >= GetOwner()->GetComponent<Animator>()->GetMaxFrame())
	{
		_playerComp->_playerStatus = Player::Status::IDLE;
	}

	if (_playerComp->_playerStatus == Player::Status::ABILITY && _playerComp->_abilityAnimationIndex == 3 && GetOwner()->GetComponent<Animator>()->GetCurrentFrame() >= GetOwner()->GetComponent<Animator>()->GetMaxFrame())
	{
		_playerComp->_playerStatus = Player::Status::IDLE;
		this->_isAreaReady = false;
	}

	if (_playerComp->_playerStatus == Player::Status::ABILITY && _playerComp->_abilityAnimationIndex == 4 && GetOwner()->GetComponent<Animator>()->GetCurrentFrame() >= GetOwner()->GetComponent<Animator>()->GetMaxFrame())
	{
		_meteor->SetActive(true);
		_meteor->GetComponent<Particle>()->SetActive(true);
		_playerComp->_playerStatus = Player::Status::IDLE;
	}

	_abilityContainer[0]->_abilityLogic();
	_abilityContainer[1]->_abilityLogic();
	_abilityContainer[2]->_abilityLogic();
	_abilityContainer[3]->_abilityLogic();

	SetCurrentBossObject();

	if (this->_isShotReady)
	{
		GRAPHICS->DrawDebugText(100, 500, 40, "Q Ready");
	}
	else
	{
		GRAPHICS->DrawDebugText(100, 500, 40, "Q On CoolDown");
	}

	if (this->_isIceReady)
	{
		GRAPHICS->DrawDebugText(100, 600, 40, "W Ready");
	}
	else
	{
		GRAPHICS->DrawDebugText(100, 600, 40, "W On CoolDown");
	}

	if (this->_isAreaReady)
	{
		GRAPHICS->DrawDebugText(100, 700, 40, "E Ready");
	}
	else
	{
		GRAPHICS->DrawDebugText(100, 700, 40, "E On CoolDown");
	}

	if (this->_isMeteorReady)
	{
		GRAPHICS->DrawDebugText(100, 800, 40, "R Ready");
	}
	else
	{
		GRAPHICS->DrawDebugText(100, 800, 40, "Q On CoolDown");
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
	this->_isActivated = active;
}


void KunrealEngine::PlayerAbility::ResetShotPos()
{
	// 플레이어 위치에서 발사되도록
	_shot->GetComponent<Transform>()->SetPosition(this->GetOwner()->GetComponent<Transform>()->GetPosition());
	_shot->GetComponent<Transform>()->SetRotation(
		this->GetOwner()->GetComponent<Transform>()->GetRotation().x, 
		this->GetOwner()->GetComponent<Transform>()->GetRotation().y,
		this->GetOwner()->GetComponent<Transform>()->GetRotation().z
	);

	_shot->GetComponent<BoxCollider>()->Update();
	_shot->GetComponent<MeshRenderer>()->Update();
	_shot->GetComponent<Particle>()->Update();

	// 방향벡터
	DirectX::XMFLOAT3 currentPoint = _shot->GetComponent<Transform>()->GetPosition();
	DirectX::XMFLOAT3 targetPoint = GRAPHICS->ScreenToWorldPoint(InputSystem::GetInstance()->GetEditorMousePos().x, InputSystem::GetInstance()->GetEditorMousePos().y);

	DirectX::XMVECTOR direction = ToolBox::GetDirectionVec(currentPoint, targetPoint);

	DirectX::XMVECTOR currentPosVec = DirectX::XMLoadFloat3(&currentPoint);

	//플레이어 방향 돌리기
	DirectX::XMFLOAT3 playerPos = this->GetOwner()->GetComponent<Transform>()->GetPosition();
	DirectX::XMFLOAT3 playerRot = this->GetOwner()->GetComponent<Transform>()->GetRotation();
	DirectX::XMVECTOR playerPosVec = DirectX::XMLoadFloat3(&playerPos);
	DirectX::XMVECTOR rotPosVec = DirectX::XMLoadFloat3(&targetPoint);

	DirectX::XMVECTOR playerForward = DirectX::XMVectorSet(0.0f, playerRot.y, -1.0f, 0.0f);
	DirectX::XMVECTOR rotDirection = DirectX::XMVectorSubtract(rotPosVec, playerPosVec);
	rotDirection.m128_f32[1] = 0.0f;
	rotDirection = DirectX::XMVector3Normalize(rotDirection);

	DirectX::XMVECTOR dotResult = DirectX::XMVector3Dot(playerForward, rotDirection);
	float dotProduct = DirectX::XMVectorGetX(dotResult);

	float angle = acos(dotProduct);
	angle = DirectX::XMConvertToDegrees(angle);

	if (rotPosVec.m128_f32[0] > playerPosVec.m128_f32[0])
	{
		angle *= -1;
	}

	this->GetOwner()->GetComponent<Transform>()->SetRotation(0.0f, angle, 0.0f);

	_shot->GetComponent<Projectile>()->SetDirection(direction);
	_shot->GetComponent<Projectile>()->_movedRange = 0.0f;
}

void KunrealEngine::PlayerAbility::CreateAbility1()
{
	Ability* shot = new Ability();
	shot->Initialize("Shot");

	shot->SetTotalData(
		"Shot",			// 이름
		10.0f,			// 데미지
		10.0f,			// 마나
		5.0f,			// 무력화 피해량
		2.0f,			// 쿨타임
		10.0f			// 사거리
	);

	// 객체 저장
	_shot = shot->_projectile;

	// 크기 조정
	_shot->GetComponent<Transform>()->SetScale(0.3f, 0.3, 0.3f);

	// 투사체 컴포넌트 추가
	_shot->AddComponent<Projectile>();
	Projectile* shotProj = _shot->GetComponent<Projectile>();

	// 파티클 추가	// 번개공
	Particle* shotParticle = _shot->AddComponent<Particle>();
	shotParticle->SetParticleEffect("Shot", "Resources/Textures/Particles/fx_Flare6.dds", 1000);
	shotParticle->SetParticleSize(15.0f, 15.0f);
	shotParticle->SetParticleVelocity(24.0f, true);
	shotParticle->SetParticleDuration(1.2f, 0.1f);
	shotParticle->AddParticleColor(0.0f, 0.3f, 1.0f);
	//shotParticle->SetParticleRotation(90.0f, _shot->GetComponent<Transform>()->GetRotation().y, 0.0f);
	shotParticle->SetParticleDirection(17.1f, 0.0f, 1.0f);
	shotParticle->SetActive(true);

	shotProj->SetMeshObject("Meteor/Meteor");

	// Q 스킬은 메쉬가 안 보이게
	_shot->GetComponent<MeshRenderer>()->SetAlpha(0.0f);

	shotProj->GetCollider()->SetBoxSize(3.0f, 3.0f, 3.0f);
	shotProj->SetDestoryCondition([shot, shotProj, this]()->bool
		{
			if (shotProj->GetCollider()->IsCollided() && shotProj->GetCollider()->GetTargetObject() != this->GetOwner())		
			{
				if (shotProj->GetCollider()->GetTargetObject()->GetTag() == "BOSS" && _isShotHit)
				{
					_currentBoss->_info._hp -= 10.0f;
					_isShotHit = false;
				}

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
			if (_shot->GetActivated())
			{
				DirectX::XMFLOAT3 currentPoint = _shot->GetComponent<Transform>()->GetPosition();

				DirectX::XMVECTOR currentPosVec = DirectX::XMLoadFloat3(&currentPoint);
				DirectX::XMVECTOR newPosition = DirectX::XMVectorAdd(currentPosVec, DirectX::XMVectorScale(shotProj->GetDirection(), 50.0f * TimeManager::GetInstance().GetDeltaTime()));

				_shot->GetComponent<Transform>()->SetPosition(newPosition.m128_f32[0], 5.0f, newPosition.m128_f32[2]);
				_shot->GetComponent<Transform>()->SetRotation(0.0f, _shot->GetComponent<Transform>()->GetRotation().y + 10.0f, 0.0f);
				_shot->GetComponent<Particle>()->SetParticleRotation(_shot->GetComponent<Transform>()->GetRotation().y, 0.0f, 0.0f);
				shotProj->_movedRange += DirectX::XMVectorGetX(DirectX::XMVector3Length(DirectX::XMVectorSubtract(newPosition, currentPosVec))); 

			}
		});

	AddToContanier(shot);
}


void KunrealEngine::PlayerAbility::ResetIcePos()
{
	// 마우스 3D좌표로에 위치시키도록
	DirectX::XMFLOAT3 newPos = GRAPHICS->ScreenToWorldPoint(InputSystem::GetInstance()->GetEditorMousePos().x, InputSystem::GetInstance()->GetEditorMousePos().y);
	_ice->GetComponent<Transform>()->SetPosition
	(
		newPos.x,
		this->_playerComp->GetOwner()->GetComponent<Transform>()->GetPosition().y,
		newPos.z
	);

	//플레이어 방향 돌리기
	DirectX::XMFLOAT3 playerPos = this->GetOwner()->GetComponent<Transform>()->GetPosition();
	DirectX::XMFLOAT3 playerRot = this->GetOwner()->GetComponent<Transform>()->GetRotation();
	DirectX::XMVECTOR playerPosVec = DirectX::XMLoadFloat3(&playerPos);
	DirectX::XMVECTOR rotPosVec = DirectX::XMLoadFloat3(&newPos);

	DirectX::XMVECTOR playerForward = DirectX::XMVectorSet(0.0f, playerRot.y, -1.0f, 0.0f);
	DirectX::XMVECTOR rotDirection = DirectX::XMVectorSubtract(rotPosVec, playerPosVec);
	rotDirection.m128_f32[1] = 0.0f;
	rotDirection = DirectX::XMVector3Normalize(rotDirection);

	DirectX::XMVECTOR dotResult = DirectX::XMVector3Dot(playerForward, rotDirection);
	float dotProduct = DirectX::XMVectorGetX(dotResult);

	float angle = acos(dotProduct);
	angle = DirectX::XMConvertToDegrees(angle);

	if (rotPosVec.m128_f32[0] > playerPosVec.m128_f32[0])
	{
		angle *= -1;
	}

	this->GetOwner()->GetComponent<Transform>()->SetRotation(0.0f, angle, 0.0f);

	// 위치 조정
	_ice->GetComponent<BoxCollider>()->Update();
	_ice->GetComponent<MeshRenderer>()->Update();
}

void KunrealEngine::PlayerAbility::CreateAbility2()
{
	Ability* ice = new Ability();
	ice->Initialize("Ice");
	
	ice->SetTotalData(
		"Ice",			// 이름
		20.0f,			// 데미지
		15.0f,			// 마나
		10.0f,			// 무력화 피해량
		5.0f,			// 쿨타임
		12.0f			// 사거리
	);
	
	_ice = ice->_projectile;
	
	// 크기 조정
	_ice->GetComponent<Transform>()->SetScale(30.0f, 30.0f, 30.0f);
	_ice->GetComponent<Transform>()->SetRotation(90.0f, 0.0f, 0.0f);
	
	// 투사체 컴포넌트 추가
	_ice->AddComponent<Projectile>();
	Projectile* iceProj = _ice->GetComponent<Projectile>();
	
	iceProj->SetMeshObject("Ice/Ice", "Ice/Ice.png");
	iceProj->GetCollider()->SetBoxSize(15.0f, 15.0f, 15.0f);
	iceProj->SetDestoryCondition([iceProj, this]()->bool
		{
			if (this->_destroyIce)
			{
				return true;
			}
			else
			{
				return false;
			}
		});
	
	_ice->SetActive(false);
	
	ice->SetLogic([ice, iceProj, this]()
		{
			if (iceProj->GetCollider()->GetTargetObject() != nullptr && iceProj->GetCollider()->GetTargetObject()->GetTag() == "BOSS" && _isIceHit)
			{
				_currentBoss->_info._hp -= 30.0f;
				_isIceHit = false;
			}
		});
	
	AddToContanier(ice);
}


void KunrealEngine::PlayerAbility::ResetAreaPos()
{
	this->_area->GetComponent<Transform>()->SetPosition(this->GetOwner()->GetComponent<Transform>()->GetPosition());
}

void KunrealEngine::PlayerAbility::CreateAbility3()
{
	Ability* area = new Ability();
	area->Initialize("Area");

	area->SetTotalData(
		"Area",			// 이름
		40.0f,			// 데미지
		20.0f,			// 마나
		15.0f,			// 무력화 피해량
		7.0f,			// 쿨타임
		15.0f			// 사거리
	);

	_area = area->_projectile;

	// 크기 조정			/// 정밀 조정 필요
	_area->GetComponent<Transform>()->SetScale(20.0f, 20.0f, 20.0f);
	_area->GetComponent<Transform>()->SetRotation(90.0f, 0.0f, 0.0f);

	// 투사체 컴포넌트 추가
	_area->AddComponent<BoxCollider>();
	BoxCollider* areaCollider = _area->GetComponent<BoxCollider>();
	areaCollider->SetBoxSize(20.0f, 20.0f, 20.0f);
	
	_area->SetActive(false);

	area->SetLogic([area, areaCollider, this]()
		{
			if (areaCollider->GetTargetObject() != nullptr && areaCollider->GetActivated() && areaCollider->IsCollided() && areaCollider->GetTargetObject()->GetTag() == "BOSS" && this->_isAreaHit)
			{
				_currentBoss->_info._hp -= 20.0f;

				this->_isAreaHit = false;
			}
		});

	AddToContanier(area);
}

void KunrealEngine::PlayerAbility::ResetMeteorPos()
{
	// 마우스 3D좌표로부터 20.0f 위에 위치시키도록
	DirectX::XMFLOAT3 newPos = GRAPHICS->ScreenToWorldPoint(InputSystem::GetInstance()->GetEditorMousePos().x, InputSystem::GetInstance()->GetEditorMousePos().y);

	_meteor->GetComponent<Transform>()->SetPosition(newPos);
	_meteor->GetComponent<Transform>()->SetPosition
	(
		_meteor->GetComponent<Transform>()->GetPosition().x,
		_meteor->GetComponent<Transform>()->GetPosition().y + 40.0f,
		_meteor->GetComponent<Transform>()->GetPosition().z
	);

	//플레이어 방향 돌리기
	DirectX::XMFLOAT3 playerPos = this->GetOwner()->GetComponent<Transform>()->GetPosition();
	DirectX::XMFLOAT3 playerRot = this->GetOwner()->GetComponent<Transform>()->GetRotation();
	DirectX::XMVECTOR playerPosVec = DirectX::XMLoadFloat3(&playerPos);
	DirectX::XMVECTOR rotPosVec = DirectX::XMLoadFloat3(&newPos);

	DirectX::XMVECTOR playerForward = DirectX::XMVectorSet(0.0f, playerRot.y, -1.0f, 0.0f);
	DirectX::XMVECTOR rotDirection = DirectX::XMVectorSubtract(rotPosVec, playerPosVec);
	rotDirection.m128_f32[1] = 0.0f;
	rotDirection = DirectX::XMVector3Normalize(rotDirection);

	DirectX::XMVECTOR dotResult = DirectX::XMVector3Dot(playerForward, rotDirection);
	float dotProduct = DirectX::XMVectorGetX(dotResult);

	float angle = acos(dotProduct);
	angle = DirectX::XMConvertToDegrees(angle);

	if (rotPosVec.m128_f32[0] > playerPosVec.m128_f32[0])
	{
		angle *= -1;
	}

	this->GetOwner()->GetComponent<Transform>()->SetRotation(0.0f, angle, 0.0f);
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
		10.0f,			// 쿨타임
		15.0f			// 사거리
	);

	// 객체 저장
	_meteor = meteor->_projectile;

	// 우리 에셋으론 세워야 이쁘다
	_meteor->GetComponent<Transform>()->SetRotation(0.0f, 0.0f, -90.0f);

	// 투사체 컴포넌트 추가
	_meteor->AddComponent<Projectile>();
	Projectile* meteorProj = _meteor->GetComponent<Projectile>();

	meteorProj->SetMeshObject("Meteor/Meteor");
	meteorProj->GetCollider()->SetBoxSize(5.0f, 20.0f, 20.0f);

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

	// 비활성화 조건		// 인게임에서는 소멸처럼
	meteorProj->SetDestoryCondition([meteorProj, this]()->bool
		{
			if (meteorProj->GetCollider()->GetTargetObject() != nullptr && meteorProj->GetCollider()->IsCollided() && meteorProj->GetCollider()->GetTargetObject()->GetTag() == "BOSS" && this->_isMeteorHit)
			{
				_currentBoss->_info._hp -= 100.0f;
				_isMeteorHit = false;
			}

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

void KunrealEngine::PlayerAbility::SetCurrentBossObject()
{
	if (InputSystem::GetInstance()->KeyDown(KEY::I))
	{
		std::string sceneName = SceneManager::GetInstance().GetCurrentScene()->GetSceneName();

		if (sceneName == "mapTest3.json")
		{
			this->_currentBoss = SceneManager::GetInstance().GetCurrentScene()->GetObjectWithTag("BOSS")->GetComponent<Ent>()->GetBoss();
		}
		else if (sceneName == "mapTest4.json")
		{
			this->_currentBoss = SceneManager::GetInstance().GetCurrentScene()->GetObjectWithTag("BOSS")->GetComponent<Kamen>()->GetBoss();
		}
		else
		{
			int a = 10;
		}
	}
}

float KunrealEngine::PlayerAbility::GetDamage()
{
	return this->_currentDamage;
}
