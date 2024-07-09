#include "PlayerAbility.h"
#include "InputSystem.h"
#include "GraphicsSystem.h"
#include "MeshRenderer.h"
#include "BoxCollider.h"
#include "Particle.h"
#include "Animator.h"
#include "Transform.h"

#include "PlayerMove.h"
#include "Boss.h"
#include "Kamen.h"
#include "EventManager.h"

#include "SceneManager.h"
#include "EventManager.h"
#include "Scene.h"

KunrealEngine::PlayerAbility::PlayerAbility()
	:_playerComp(nullptr), _meteor(nullptr), _shot(nullptr), _ice(nullptr), _laser(nullptr)
	, _isIceReady(true), _destroyIce(false), _isShotReady(true), _isMeteorReady(true), _isLaserReady(true)
	, _isShotHit(false), _isIceHit(false), _isLaserHit(false), _isMeteorHit(false)
	, _isShotDetected(false), _isIceDetected(false), _isLaserDetected(false), _isMeteorDetected(false), _isShotEnded(false),
	_shotParticleTimer(0), _isMeteorEnded(false), _meteorParticleTimer(0), _isIceEnded(false), _iceParticleTimer(0), _isLaserEnded(false), _laserParticleTimer(0), _isLaserStarted(false)
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
		Startcoroutine(shotCoolDown);
		_isShotDetected = true;
		_isShotReady = false;
		_isShotHit = true;
		_shot->SetActive(true);
		_shot->GetComponent<Projectile>()->SetActive(true);
		_shotParticle2->GetComponent<Particle>()->SetActive(true);
		_shotParticle3->GetComponent<Particle>()->SetActive(true);
		_shotParticle4->GetComponent<Particle>()->SetActive(true);
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

		_iceParticle1->GetComponent<Particle>()->SetActive(true);
		_iceParticle2->GetComponent<Particle>()->SetActive(true);
		_iceParticle3->GetComponent<Particle>()->SetActive(true);
		_iceParticleHit1->GetComponent<Transform>()->SetPosition(_ice->GetComponent<Transform>()->GetPosition());
		_iceParticleHit2->GetComponent<Transform>()->SetPosition(_ice->GetComponent<Transform>()->GetPosition());

		_iceParticleHit1->GetComponent<Particle>()->Update();
		_iceParticleHit2->GetComponent<Particle>()->Update();
		_iceParticle1->GetComponent<Particle>()->Update();
		_iceParticle2->GetComponent<Particle>()->Update();
		_iceParticle3->GetComponent<Particle>()->Update();

		_playerComp->_playerStatus = Player::Status::ABILITY;
		_playerComp->_abilityAnimationIndex = 2;				// 얼음 소환 애니메이션	
		Startcoroutine(iceDestroy);
	}

	if (InputSystem::GetInstance()->KeyDown(KEY::E) && this->_isLaserReady)
	{
		ResetLaserPos();
		_isLaserDetected = true;
		_isLaserHit = true;
		_isLaserStarted = true;
		this->laserCount = 0;
		Startcoroutine(LaserCoolDown);
		Startcoroutine(LaserStandby);
		_playerComp->_playerStatus = Player::Status::ABILITY;
		_playerComp->_abilityAnimationIndex = 3;				// 범위 공격 애니메이션

		Startcoroutine(laserDestroy);
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
		this->_isLaserReady = false;
	}

	if (_playerComp->_playerStatus == Player::Status::ABILITY && _playerComp->_abilityAnimationIndex == 4 && GetOwner()->GetComponent<Animator>()->GetCurrentFrame() >= GetOwner()->GetComponent<Animator>()->GetMaxFrame())
	{
		_meteor->SetActive(true);
		_meteor->GetComponent<Particle>()->SetActive(true);
		_meteorParticle2->GetComponent<Particle>()->SetActive(true);
		_meteorParticle3->GetComponent<Particle>()->SetActive(true);
		_meteorParticle4->GetComponent<Particle>()->SetActive(true);
		_playerComp->_playerStatus = Player::Status::IDLE;
	}

	_abilityContainer[0]->_abilityLogic();
	_abilityContainer[1]->_abilityLogic();
	_abilityContainer[2]->_abilityLogic();
	_abilityContainer[3]->_abilityLogic();

	//if (this->_isShotReady)
	//{
	//	GRAPHICS->DrawDebugText(100, 500, 40, "Q Ready");
	//}
	//else
	//{
	//	GRAPHICS->DrawDebugText(100, 500, 40, "Q On CoolDown");
	//}
	//
	//if (this->_isIceReady)
	//{
	//	GRAPHICS->DrawDebugText(100, 600, 40, "W Ready");
	//}
	//else
	//{
	//	GRAPHICS->DrawDebugText(100, 600, 40, "W On CoolDown");
	//}
	//
	//if (this->_isLaserReady)
	//{
	//	GRAPHICS->DrawDebugText(100, 700, 40, "E Ready");
	//}
	//else
	//{
	//	GRAPHICS->DrawDebugText(100, 700, 40, "E On CoolDown");
	//}
	//
	//if (this->_isMeteorReady)
	//{
	//	GRAPHICS->DrawDebugText(100, 800, 40, "R Ready");
	//}
	//else
	//{
	//	GRAPHICS->DrawDebugText(100, 800, 40, "R On CoolDown");
	//}

	GRAPHICS->DrawDebugText(100, 500, 40, "LaserCount : %d", this->laserCount);

	if (this->_laser->GetCollider()->GetTargetObject() != nullptr)
	{
		GRAPHICS->DrawDebugText(100, 800, 40, this->_laser->GetCollider()->GetTargetObject()->GetObjectName().c_str());
	}
	else
	{
		GRAPHICS->DrawDebugText(100, 800, 40, "Laser Not Collided");
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

	_shot->GetComponent<BoxCollider>()->FixedUpdate();
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
		80.0f			// 사거리
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
	shotParticle->SetParticleEffect("BlastWave2", "Resources/Textures/Particles/fx_BlastWave2.dds", 1000);
	shotParticle->SetParticleSize(7.f, 7.0f);
	shotParticle->SetParticleVelocity(3.0f, true);
	shotParticle->SetParticleDirection(0.0f, 0.0f, 0.0f);
	shotParticle->AddParticleColor(0.5f, 0.5f, 2.0f);
	shotParticle->SetParticleDuration(0.7f, 0.05f);
	shotParticle->SetActive(true);

	_shotParticle2 = SceneManager::GetInstance().GetCurrentScene()->CreateObject("PlayerQ2");
	_shotParticle2->AddComponent<Particle>();
	_shotParticle2->GetComponent<Particle>()->SetParticleEffect("BlastWave3", "Resources/Textures/Particles/fx_BlastWave3.dds", 1000);
	_shotParticle2->GetComponent<Particle>()->SetParticleDuration(1.0f, 0.05f);
	_shotParticle2->GetComponent<Particle>()->SetParticleVelocity(7.0f, true);
	_shotParticle2->GetComponent<Particle>()->SetParticleSize(10.f, 10.0f);
	_shotParticle2->GetComponent<Particle>()->AddParticleColor(0.0f, 1.5f, 10.f);
	_shotParticle2->GetComponent<Particle>()->SetParticleDirection(0.0f, 0.0f, 0.0f);
	_shotParticle2->GetComponent<Particle>()->SetActive(true);
	_shotParticle2->SetActive(true);
	_shotParticle2->SetParent(_shot);

	_shotParticle3 = SceneManager::GetInstance().GetCurrentScene()->CreateObject("PlayerQ3");
	_shotParticle3->AddComponent<Particle>();
	_shotParticle3->GetComponent<Particle>()->SetParticleEffect("Cracks1", "Resources/Textures/Particles/fx_Cracks1.dds", 1000);
	_shotParticle3->GetComponent<Particle>()->SetParticleDuration(1.0f, 0.05f);
	_shotParticle3->GetComponent<Particle>()->SetParticleVelocity(200.0f, true);
	_shotParticle3->GetComponent<Particle>()->SetParticleSize(4.0f, 4.0f);
	_shotParticle3->GetComponent<Particle>()->AddParticleColor(0.0f, 1.5f, 10.f);
	_shotParticle3->GetComponent<Particle>()->SetParticleDirection(0.0f, 0.0f, 0.0f);
	_shotParticle3->GetComponent<Particle>()->SetActive(true);
	_shotParticle3->SetActive(true);
	_shotParticle3->SetParent(_shot);

	_shotParticle4 = SceneManager::GetInstance().GetCurrentScene()->CreateObject("PlayerQ4");
	_shotParticle4->AddComponent<Particle>();
	_shotParticle4->GetComponent<Particle>()->SetParticleEffect("EnergyBolt1", "Resources/Textures/Particles/fx_EnergyBolt1.dds", 1000);
	_shotParticle4->GetComponent<Particle>()->SetParticleDuration(1.0f, 0.05f);
	_shotParticle4->GetComponent<Particle>()->SetParticleVelocity(0.0f, true);
	_shotParticle4->GetComponent<Particle>()->SetParticleSize(1.0f, 1.0f);
	_shotParticle4->GetComponent<Particle>()->AddParticleColor(0.0f, 1.5f, 10.f);
	_shotParticle4->GetComponent<Particle>()->SetParticleDirection(0.0f, 0.0f, 0.0f);
	_shotParticle4->GetComponent<Particle>()->SetActive(true);
	_shotParticle4->SetActive(true);
	_shotParticle4->SetParent(_shot);

	_shotParticleHit1 = SceneManager::GetInstance().GetCurrentScene()->CreateObject("ShotParticleHit1");
	_shotParticleHit1->AddComponent<Particle>();
	_shotParticleHit1->GetComponent<Particle>()->SetParticleEffect("BlastWave1", "Resources/Textures/Particles/fx_BlastWave1.dds", 1000);
	_shotParticleHit1->GetComponent<Particle>()->SetParticleDuration(0.1f, 0.1f);
	_shotParticleHit1->GetComponent<Particle>()->SetParticleVelocity(100.0f, true);
	_shotParticleHit1->GetComponent<Particle>()->SetParticleSize(5.f, 5.0f);
	_shotParticleHit1->GetComponent<Particle>()->AddParticleColor(0.0f, 0.5f, 1.0f);
	_shotParticleHit1->GetComponent<Particle>()->SetParticleDirection(0.0f, 0.0f, 0.0f);
	_shotParticleHit1->GetComponent<Particle>()->SetActive(false);
	_shotParticleHit1->SetActive(true);

	_shotParticleHit2 = SceneManager::GetInstance().GetCurrentScene()->CreateObject("ShotParticleHit2");
	_shotParticleHit2->AddComponent<Particle>();
	_shotParticleHit2->GetComponent<Particle>()->SetParticleEffect("Cracks1", "Resources/Textures/Particles/fx_Cracks1.dds", 1000);
	_shotParticleHit2->GetComponent<Particle>()->SetParticleDuration(1.0f, 0.1f);
	_shotParticleHit2->GetComponent<Particle>()->SetParticleVelocity(150.0f, true);
	_shotParticleHit2->GetComponent<Particle>()->SetParticleSize(3.0f, 3.0f);
	_shotParticleHit2->GetComponent<Particle>()->AddParticleColor(0.0f, 0.5f, 2.0f);
	_shotParticleHit2->GetComponent<Particle>()->SetParticleDirection(0.0f, 0.0f, 0.0f);
	_shotParticleHit2->GetComponent<Particle>()->SetActive(false);
	_shotParticleHit2->SetActive(true);

	_shotParticleHit3 = SceneManager::GetInstance().GetCurrentScene()->CreateObject("ShotParticleHit3");
	_shotParticleHit3->AddComponent<Particle>();
	_shotParticleHit3->GetComponent<Particle>()->SetParticleEffect("Halo2", "Resources/Textures/Particles/fx_Halo2.dds", 1000);
	_shotParticleHit3->GetComponent<Particle>()->SetParticleDuration(0.1f, 0.1f);
	_shotParticleHit3->GetComponent<Particle>()->SetParticleVelocity(100.0f, true);
	_shotParticleHit3->GetComponent<Particle>()->SetParticleSize(15.f, 15.0f);
	_shotParticleHit3->GetComponent<Particle>()->AddParticleColor(0.0f, 1.0f, 5.0f);
	_shotParticleHit3->GetComponent<Particle>()->SetParticleDirection(0.0f, 0.0f, 0.0f);
	_shotParticleHit3->GetComponent<Particle>()->SetActive(false);
	_shotParticleHit3->SetActive(true);


	shotProj->SetMeshObject("Meteor/Meteor");

	// Q 스킬은 메쉬가 안 보이게
	_shot->GetComponent<MeshRenderer>()->SetAlpha(0.0f);

	shotProj->GetCollider()->SetColliderScale(5.0f, 5.0f, 5.0f);
	shotProj->SetDestoryCondition([shot, shotProj, this]()->bool
		{
			if (shotProj->GetCollider()->IsCollided() && shotProj->GetCollider()->GetTargetObject()->GetTag() == "Boss")
			{
				if (_isShotHit)
				{
					EventManager::GetInstance().CalculateDamageToBoss(shot);
					_isShotHit = false;
				}

				return true;
			}
			else if (shotProj->_movedRange > shot->GetRange())
			{
				return true;
			}
			else
			{
				return false;
			}
		});

	_shot->SetActive(false);

	shot->SetLogic([shot, shotProj, this]()  // 부모가 비활성화 되기전에 파티클을 켜줘야 위치를 받을 수 있음
		{
			if (_shot->GetActivated())
			{
				DirectX::XMFLOAT3 currentPoint = _shot->GetComponent<Transform>()->GetPosition();

				DirectX::XMVECTOR currentPosVec = DirectX::XMLoadFloat3(&currentPoint);
				DirectX::XMVECTOR newPosition = DirectX::XMVectorAdd(currentPosVec, DirectX::XMVectorScale(shotProj->GetDirection(), 50.0f * TimeManager::GetInstance().GetDeltaTime()));

				_shot->GetComponent<Transform>()->SetPosition(newPosition.m128_f32[0], 5.0f, newPosition.m128_f32[2]);
				shotProj->_movedRange += DirectX::XMVectorGetX(DirectX::XMVector3Length(DirectX::XMVectorSubtract(newPosition, currentPosVec)));

				if (shotProj->GetCollider()->IsCollided() && shotProj->GetCollider()->GetTargetObject()->GetTag() == "Boss")
				{
					_isShotEnded = true;
				}
			}


			if (!(_shot->GetActivated()) && _isShotEnded == true)
			{
				_shotParticleTimer += TimeManager::GetInstance().GetDeltaTime();
				_shotParticleHit1->GetComponent<Particle>()->SetActive(true);
				_shotParticleHit2->GetComponent<Particle>()->SetActive(true);
				_shotParticleHit3->GetComponent<Particle>()->SetActive(true);
				_shotParticleHit1->GetComponent<Transform>()->SetPosition(_shot->GetComponent<Transform>()->GetPosition());
				_shotParticleHit2->GetComponent<Transform>()->SetPosition(_shot->GetComponent<Transform>()->GetPosition());
				_shotParticleHit3->GetComponent<Transform>()->SetPosition(_shot->GetComponent<Transform>()->GetPosition());

				_shotParticleHit1->GetComponent<Particle>()->SetParticleSize(_shotParticleTimer * 40, _shotParticleTimer * 40);
				_shotParticleHit3->GetComponent<Particle>()->SetParticleSize(_shotParticleTimer * 120, _shotParticleTimer * 120);

				if (_shotParticleTimer > 0.2f)
				{
					_shotParticleHit1->GetComponent<Particle>()->SetActive(false);
					_shotParticleHit2->GetComponent<Particle>()->SetActive(false);
					_shotParticleHit3->GetComponent<Particle>()->SetActive(false);
					_shotParticleTimer = 0.0f;
					_isShotEnded = false;
				}
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

	_iceParticle1 = SceneManager::GetInstance().GetCurrentScene()->CreateObject("IceParticle1");
	_iceParticle1->AddComponent<Particle>();
	_iceParticle1->GetComponent<Particle>()->SetParticleEffect("Blast3", "Resources/Textures/Particles/fx_Blast3.dds", 1000);
	_iceParticle1->GetComponent<Particle>()->SetParticleDuration(3.0f, 2.0f);
	_iceParticle1->GetComponent<Particle>()->SetParticleVelocity(8.0f, true);
	_iceParticle1->GetComponent<Particle>()->SetParticleSize(5.f, 5.0f);
	_iceParticle1->GetComponent<Particle>()->AddParticleColor(0.5f, 3.0f, 5.0f);
	_iceParticle1->GetComponent<Particle>()->SetParticleDirection(0.0f, -1.0f, 0.0f);
	_iceParticle1->GetComponent<Particle>()->SetParticleAngle(339.0f, 0.0f, 30.0f);
	_iceParticle1->GetComponent<Particle>()->SetParticleCameraApply(true);
	_iceParticle1->GetComponent<Particle>()->SetActive(false);

	_iceParticle2 = SceneManager::GetInstance().GetCurrentScene()->CreateObject("IceParticle2");
	_iceParticle2->AddComponent<Particle>();
	_iceParticle2->GetComponent<Particle>()->SetParticleEffect("Dust3", "Resources/Textures/Particles/fx_Dust3.dds", 1000);
	_iceParticle2->GetComponent<Particle>()->SetParticleDuration(3.0f, 2.0f);
	_iceParticle2->GetComponent<Particle>()->SetParticleVelocity(8.0f, true);
	_iceParticle2->GetComponent<Particle>()->SetParticleSize(5.0f, 5.0f);
	_iceParticle2->GetComponent<Particle>()->AddParticleColor(0.5f, 3.0f, 5.0f);
	_iceParticle2->GetComponent<Particle>()->SetParticleDirection(0.0f, 0.0f, 0.0f);
	_iceParticle2->GetComponent<Particle>()->SetParticleAngle(339.0f, 0.0f, 30.0f);
	_iceParticle2->GetComponent<Particle>()->SetParticleCameraApply(true);
	_iceParticle2->GetComponent<Particle>()->SetActive(false);

	_iceParticle3 = SceneManager::GetInstance().GetCurrentScene()->CreateObject("IceParticle3");
	_iceParticle3->AddComponent<Particle>();
	_iceParticle3->GetComponent<Particle>()->SetParticleEffect("Sparks1", "Resources/Textures/Particles/fx_Sparks1.dds", 1000);
	_iceParticle3->GetComponent<Particle>()->SetParticleDuration(1.0f, 0.1f);
	_iceParticle3->GetComponent<Particle>()->SetParticleVelocity(1.0f, true);
	_iceParticle3->GetComponent<Particle>()->SetParticleSize(30.f, 30.0f);
	_iceParticle3->GetComponent<Particle>()->AddParticleColor(0.0f, 1.0f, 2.0f);
	_iceParticle3->GetComponent<Particle>()->SetParticleDirection(0.0f, 0.0f, 0.0f);
	_iceParticle3->GetComponent<Particle>()->SetParticleCameraApply(true);
	_iceParticle3->GetComponent<Particle>()->SetActive(false);

	_iceParticleHit1 = SceneManager::GetInstance().GetCurrentScene()->CreateObject("IceParticleHit1");
	_iceParticleHit1->AddComponent<Particle>();
	_iceParticleHit1->GetComponent<Particle>()->SetParticleEffect("Dust3", "Resources/Textures/Particles/fx_Dust3.dds", 100);
	_iceParticleHit1->GetComponent<Particle>()->SetParticleDuration(4.0f, 0.5f);
	_iceParticleHit1->GetComponent<Particle>()->SetParticleVelocity(10.0f, true);
	_iceParticleHit1->GetComponent<Particle>()->SetParticleSize(25.0f, 25.0f);
	_iceParticleHit1->GetComponent<Particle>()->AddParticleColor(0.5f, 3.0f, 5.0f);
	_iceParticleHit1->GetComponent<Particle>()->SetParticleDirection(0.0f, 0.0f, 0.0f);
	_iceParticleHit1->GetComponent<Particle>()->SetParticleAngle(0.0f, 0.0f, 0.0f);
	_iceParticleHit1->GetComponent<Particle>()->SetActive(false);

	_iceParticleHit2 = SceneManager::GetInstance().GetCurrentScene()->CreateObject("IceParticleHit2");
	_iceParticleHit2->AddComponent<Particle>();
	_iceParticleHit2->GetComponent<Particle>()->SetParticleEffect("Sparks1", "Resources/Textures/Particles/fx_Sparks1.dds", 100);
	_iceParticleHit2->GetComponent<Particle>()->SetParticleDuration(1.0f, 0.1f);
	_iceParticleHit2->GetComponent<Particle>()->SetParticleVelocity(1.0f, true);
	_iceParticleHit2->GetComponent<Particle>()->SetParticleSize(30.f, 30.0f);
	_iceParticleHit2->GetComponent<Particle>()->AddParticleColor(0.0f, 1.0f, 2.0f);
	_iceParticleHit2->GetComponent<Particle>()->SetParticleDirection(0.0f, 0.0f, 0.0f);
	_iceParticleHit2->GetComponent<Particle>()->SetParticleCameraApply(true);
	_iceParticleHit2->GetComponent<Particle>()->SetActive(false);

	// 투사체 컴포넌트 추가
	_ice->AddComponent<Projectile>();
	Projectile* iceProj = _ice->GetComponent<Projectile>();

	iceProj->SetMeshObject("Ice/Ice", "Ice/Ice.png");
	iceProj->GetCollider()->SetColliderScale(15.0f, 15.0f, 15.0f);
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
			_iceParticle1->GetComponent<Transform>()->SetPosition(_ice->GetComponent<Transform>()->GetPosition());
			_iceParticle2->GetComponent<Transform>()->SetPosition(_ice->GetComponent<Transform>()->GetPosition());
			_iceParticle3->GetComponent<Transform>()->SetPosition(_ice->GetComponent<Transform>()->GetPosition());

			if (iceProj->GetCollider()->GetTargetObject() != nullptr && iceProj->GetCollider()->GetTargetObject()->GetTag() == "Boss" && _isIceHit)
			{
				EventManager::GetInstance().CalculateDamageToBoss(ice);
				_isIceHit = false;
			}
		});

	AddToContanier(ice);
}


void KunrealEngine::PlayerAbility::ResetLaserPos()
{
	this->_laser->GetComponent<Transform>()->SetPosition(this->GetOwner()->GetComponent<Transform>()->GetPosition());

	// 플레이어 방향조절
	DirectX::XMFLOAT3 newPos = GRAPHICS->ScreenToWorldPoint(InputSystem::GetInstance()->GetEditorMousePos().x, InputSystem::GetInstance()->GetEditorMousePos().y);

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
	this->_laser->GetComponent<Transform>()->SetRotation(this->GetOwner()->GetComponent<Transform>()->GetRotation());

	DirectX::XMVECTOR newLaserPos = DirectX::XMVectorAdd(playerPosVec, DirectX::XMVectorScale(rotDirection, this->_laser->GetComponent<BoxCollider>()->GetColliderScale().z * 0.5f + 10.0f));

	this->_laser->GetComponent<Transform>()->SetPosition(newLaserPos.m128_f32[0], 5.0f, newLaserPos.m128_f32[2]);

	// 파티클 위치조정 부분

	float laserPosOffset = 15.0f;
	float laserScaleOffset = 80.0f;

	DirectX::XMFLOAT3 direction = ToolBox::RotateVector(DirectX::XMFLOAT3(0, 0, -1.0f), _playerComp->_transform->GetRotation().y);

	DirectX::XMVECTOR dirVec = DirectX::XMLoadFloat3(&direction);

	DirectX::XMVECTOR laserScaleDir = DirectX::XMVectorScale(dirVec, laserPosOffset);
	_laserParticle1->GetComponent<Transform>()->SetPosition(_playerComp->_transform->GetPosition().x + laserScaleDir.m128_f32[0], 16.0f, _playerComp->_transform->GetPosition().z + laserScaleDir.m128_f32[2]);
	_laserParticle2->GetComponent<Transform>()->SetPosition(_playerComp->_transform->GetPosition().x + laserScaleDir.m128_f32[0], 16.0f, _playerComp->_transform->GetPosition().z + laserScaleDir.m128_f32[2]);
	_laserParticle3->GetComponent<Transform>()->SetPosition(_playerComp->_transform->GetPosition().x + laserScaleDir.m128_f32[0], 16.0f, _playerComp->_transform->GetPosition().z + laserScaleDir.m128_f32[2]);
	_laserParticle4->GetComponent<Transform>()->SetPosition(_playerComp->_transform->GetPosition().x + laserScaleDir.m128_f32[0], 16.0f, _playerComp->_transform->GetPosition().z + laserScaleDir.m128_f32[2]);

	_laserParticle1->GetComponent<Particle>()->SetParticleRotation(90.0f, _playerComp->_transform->GetRotation().y, 0.0f);
	_laserParticle2->GetComponent<Particle>()->SetParticleRotation(90.0f, _playerComp->_transform->GetRotation().y, 0.0f);
	_laserParticle3->GetComponent<Particle>()->SetParticleRotation(90.0f, _playerComp->_transform->GetRotation().y, 0.0f);
	_laserParticle4->GetComponent<Particle>()->SetParticleRotation(0.0f, _playerComp->_transform->GetRotation().y, 0.0f);

	_laserParticle1->GetComponent<Particle>()->Update();
	_laserParticle2->GetComponent<Particle>()->Update();
	_laserParticle3->GetComponent<Particle>()->Update();
	_laserParticle4->GetComponent<Particle>()->Update();
}

void KunrealEngine::PlayerAbility::CreateAbility3()
{
	Ability* laser = new Ability();
	laser->Initialize("Laser");

	laser->SetTotalData(
		"Laser",			// 이름
		5.0f,			// 데미지
		20.0f,			// 마나
		15.0f,			// 무력화 피해량
		7.0f,			// 쿨타임
		15.0f			// 사거리
	);

	_laser = laser->_projectile;

	// 크기 조정			/// 정밀 조정 필요
	//_laser->GetComponent<Transform>()->SetScale(20.0f, 20.0f, 20.0f);
	_laser->GetComponent<Transform>()->SetRotation(90.0f, 0.0f, 0.0f);

	// 투사체 컴포넌트 추가
	_laser->AddComponent<BoxCollider>();
	BoxCollider* laserCollider = _laser->GetComponent<BoxCollider>();
	laserCollider->SetColliderScale(20.0f, 20.0f, 160.0f);

	_laserParticle1 = SceneManager::GetInstance().GetCurrentScene()->CreateObject("PlayerE1");
	_laserParticle1->AddComponent<Particle>();
	_laserParticle1->GetComponent<Particle>()->SetParticleEffect("fx_SmokeyHalo1", "Resources/Textures/Particles/fx_SmokeyHalo1.dds", 1000);
	_laserParticle1->GetComponent<Particle>()->SetParticleDuration(3.0f, 2.0f);
	_laserParticle1->GetComponent<Particle>()->SetParticleVelocity(84.f, false);
	_laserParticle1->GetComponent<Particle>()->SetParticleSize(50.f, 50.0f);
	_laserParticle1->GetComponent<Particle>()->AddParticleColor(0.0f, 0.5f, 1.0f);
	_laserParticle1->GetComponent<Particle>()->SetParticleRotation(90.0f, _playerComp->_transform->GetRotation().y, 0.0f);
	_laserParticle1->GetComponent<Particle>()->SetParticleCameraApply(true);
	_laserParticle1->GetComponent<Particle>()->SetParticleAngle(339, 0.0f, 30);
	_laserParticle1->SetTotalComponentState(false);
	_laserParticle1->SetActive(false);

	_laserParticle2 = SceneManager::GetInstance().GetCurrentScene()->CreateObject("PlayerE2");
	_laserParticle2->AddComponent<Particle>();
	_laserParticle2->GetComponent<Particle>()->SetParticleEffect("Laser", "Resources/Textures/Particles/fx_Dust2.dds", 1000);
	_laserParticle2->GetComponent<Particle>()->SetParticleDuration(3.0f, 2.0f);
	_laserParticle2->GetComponent<Particle>()->SetParticleVelocity(84.f, false);
	_laserParticle2->GetComponent<Particle>()->SetParticleSize(20.f, 20.f);
	_laserParticle2->GetComponent<Particle>()->AddParticleColor(1.0f, 1.0f, 0.0f);
	_laserParticle2->GetComponent<Particle>()->SetParticleRotation(90.0f, _playerComp->_transform->GetRotation().y, 0.0f);
	_laserParticle2->GetComponent<Particle>()->SetParticleCameraApply(true);
	_laserParticle2->GetComponent<Particle>()->SetParticleAngle(339, 0.0f, 30);
	_laserParticle2->SetTotalComponentState(false);
	_laserParticle2->SetActive(false);

	_laserParticle3 = SceneManager::GetInstance().GetCurrentScene()->CreateObject("PlayerE3");
	_laserParticle3->AddComponent<Particle>();
	_laserParticle3->GetComponent<Particle>()->SetParticleEffect("fx_BlastWave5", "Resources/Textures/Particles/fx_BlastWave5.dds", 1000);
	_laserParticle3->GetComponent<Particle>()->SetParticleDuration(3.0f, 2.0f);
	_laserParticle3->GetComponent<Particle>()->SetParticleVelocity(84.f, false);
	_laserParticle3->GetComponent<Particle>()->SetParticleSize(30.f, 30.f);
	_laserParticle3->GetComponent<Particle>()->AddParticleColor(1.0f, 1.0f, 0.3f);
	_laserParticle3->GetComponent<Particle>()->SetParticleRotation(90.0f, _playerComp->_transform->GetRotation().y, 0.0f);
	_laserParticle3->GetComponent<Particle>()->SetParticleCameraApply(true);
	_laserParticle3->GetComponent<Particle>()->SetParticleAngle(339, 0.0f, 30);
	_laserParticle3->SetTotalComponentState(false);
	_laserParticle3->SetActive(false);

	_laserParticle4 = SceneManager::GetInstance().GetCurrentScene()->CreateObject("PlayerE4");
	_laserParticle4->AddComponent<Particle>();
	_laserParticle4->GetComponent<Particle>()->SetParticleEffect("fx_Flare7", "Resources/Textures/Particles/fx_Flare7.dds", 1000);
	_laserParticle4->GetComponent<Particle>()->SetParticleDuration(3.0f, 0.1f);
	_laserParticle4->GetComponent<Particle>()->SetParticleVelocity(0.f, true);
	_laserParticle4->GetComponent<Particle>()->SetParticleSize(30.f, 30.f);
	_laserParticle4->GetComponent<Particle>()->AddParticleColor(0.0f, 0.5f, 1.0f);
	_laserParticle4->GetComponent<Particle>()->SetParticleRotation(90.0f, _playerComp->_transform->GetRotation().y, 0.0f);
	_laserParticle4->GetComponent<Particle>()->SetParticleCameraApply(true);
	_laserParticle4->GetComponent<Particle>()->SetParticleAngle(339, 0.0f, 30);
	_laserParticle4->SetTotalComponentState(false);
	_laserParticle4->SetActive(false);

	_laser->SetActive(false);

	laser->SetLogic([laser, laserCollider, this]()
		{
			if (_isLaserStarted == true)
			{
				_laserParticle1->GetComponent<Particle>()->SetParticleSize(50.f * ToolBox::GetRandomFloat(0.8f, 1.0f), 50.f * ToolBox::GetRandomFloat(0.8f, 1.0f));
				_laserParticle2->GetComponent<Particle>()->SetParticleSize(30.f * ToolBox::GetRandomFloat(0.8f, 1.0f), 30.f * ToolBox::GetRandomFloat(0.8f, 1.0f));
				_laserParticle3->GetComponent<Particle>()->SetParticleSize(60.f * ToolBox::GetRandomFloat(0.8f, 1.0f), 60.f * ToolBox::GetRandomFloat(0.8f, 1.0f));
				_laserParticle4->GetComponent<Particle>()->SetParticleSize(50.f * ToolBox::GetRandomFloat(0.8f, 1.0f), 50.f * ToolBox::GetRandomFloat(0.8f, 1.0f));
			}

			if (laserCollider->GetTargetObject() != nullptr && laserCollider->IsCollided() && laserCollider->GetTargetObject()->GetTag() == "Boss" && this->_isLaserHit)
			{
				EventManager::GetInstance().CalculateDamageToBoss(laser);
				this->_isLaserHit = false;
			}

			if (!this->_isLaserHit && this->_laser->GetActivated())
			{
				Startcoroutine(laserHit);
			}
		});

	AddToContanier(laser);
}

void KunrealEngine::PlayerAbility::ResetMeteorPos()
{
	// 마우스 3D좌표로부터 80.0f 위에 위치시키도록
	DirectX::XMFLOAT3 newPos = GRAPHICS->ScreenToWorldPoint(InputSystem::GetInstance()->GetEditorMousePos().x, InputSystem::GetInstance()->GetEditorMousePos().y);

	_meteor->GetComponent<Transform>()->SetPosition(newPos);
	_meteor->GetComponent<Transform>()->SetPosition
	(
		_meteor->GetComponent<Transform>()->GetPosition().x,
		_meteor->GetComponent<Transform>()->GetPosition().y + 80.0f,
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
		0.2f,			// 쿨타임
		15.0f			// 사거리
	);

	// 객체 저장
	_meteor = meteor->_projectile;

	// 우리 에셋으론 세워야 이쁘다
	_meteor->GetComponent<Transform>()->SetRotation(0.0f, 0.0f, 90.0f);

	// 투사체 컴포넌트 추가
	_meteor->AddComponent<Projectile>();
	Projectile* meteorProj = _meteor->GetComponent<Projectile>();

	// Mesh 및 크기 설정
	_meteor->GetComponent<Transform>()->SetScale(1.5f, 1.5f, 1.5f);

	meteorProj->SetMeshObject("Meteor/Meteor");
	meteorProj->GetCollider()->SetColliderScale(5.0f, 20.0f, 20.0f);

	// 파티클 추가	// 불타오르게
	Particle* meteorParticle = _meteor->AddComponent<Particle>();
	//Particle* meteorParticle = _meteor->GetComponent<Particle>();
	meteorParticle->SetParticleEffect("BlastWave2", "Resources/Textures/Particles/fx_BlastWave2.dds", 1000);
	//meteorParticle->SetParticleSize(17.f, 20.0f);
	meteorParticle->SetParticleSize(25.0f, 30.0f);
	meteorParticle->SetParticleVelocity(3.0f, true);
	meteorParticle->SetParticleDuration(1.5f, 1.5f);
	meteorParticle->AddParticleColor(6.0f, 0.2f, 0.1f);
	meteorParticle->SetParticleDirection(0.0f, 50.0f, 0.0f);
	meteorParticle->SetActive(true);

	_meteorParticle2 = SceneManager::GetInstance().GetCurrentScene()->CreateObject("PlayerR2");
	_meteorParticle2->AddComponent<Particle>();
	_meteorParticle2->GetComponent<Particle>()->SetParticleEffect("BlastWave3", "Resources/Textures/Particles/fx_BlastWave3.dds", 1000);
	_meteorParticle2->GetComponent<Particle>()->SetParticleDuration(2.0f, 1.4f);
	_meteorParticle2->GetComponent<Particle>()->SetParticleVelocity(6.5f, true);
	//_meteorParticle2->GetComponent<Particle>()->SetParticleSize(17.f, 20.0f);
	_meteorParticle2->GetComponent<Particle>()->SetParticleSize(25.0f, 30.0f);
	_meteorParticle2->GetComponent<Particle>()->AddParticleColor(2.0f, 1.0f, 0.0f);
	_meteorParticle2->GetComponent<Particle>()->SetParticleDirection(0.0f, 50.0f, 0.0f);
	_meteorParticle2->SetParent(_meteor);


	_meteorParticle3 = SceneManager::GetInstance().GetCurrentScene()->CreateObject("PlayerR3");
	_meteorParticle3->GetComponent<Transform>()->SetPosition(0, 10.0f, 0);
	_meteorParticle3->AddComponent<Particle>();
	_meteorParticle3->GetComponent<Particle>()->SetParticleEffect("Fire1", "Resources/Textures/Particles/fx_Fire1.dds", 1000);
	_meteorParticle3->GetComponent<Particle>()->SetParticleDuration(1.0f, 0.6f);
	_meteorParticle3->GetComponent<Particle>()->SetParticleVelocity(6.0f, true);
	//_meteorParticle3->GetComponent<Particle>()->SetParticleSize(13.f, 13.0f);
	_meteorParticle3->GetComponent<Particle>()->SetParticleSize(19.0f, 19.0f);
	_meteorParticle3->GetComponent<Particle>()->AddParticleColor(1.0f, 0.1f, 0.1f);
	_meteorParticle3->GetComponent<Particle>()->SetParticleDirection(0.0f, 100.0f, 0.0f);
	_meteorParticle3->SetParent(_meteor);

	_meteorParticle4 = SceneManager::GetInstance().GetCurrentScene()->CreateObject("PlayerR4");
	_meteorParticle4->GetComponent<Transform>()->SetPosition(0, 10.f, 0.f);
	_meteorParticle4->AddComponent<Particle>();
	_meteorParticle4->GetComponent<Particle>()->SetParticleEffect("Fire1", "Resources/Textures/Particles/fx_Fire1.dds", 1000);
	_meteorParticle4->GetComponent<Particle>()->SetParticleDuration(1.0f, 0.6f);
	_meteorParticle4->GetComponent<Particle>()->SetParticleVelocity(6.0f, true);
	//_meteorParticle4->GetComponent<Particle>()->SetParticleSize(1.f, 1.0f);
	_meteorParticle4->GetComponent<Particle>()->SetParticleSize(1.5f, 1.5f);
	_meteorParticle4->GetComponent<Particle>()->AddParticleColor(1.0f, 0.1f, 0.1f);
	_meteorParticle4->GetComponent<Particle>()->SetParticleDirection(0.0f, 100.0f, 0.0f);
	_meteorParticle4->SetParent(_meteor);

	// 착탄 파티클

	_meteorParticleHit1 = SceneManager::GetInstance().GetCurrentScene()->CreateObject("MeteorParticleHit1");
	_meteorParticleHit1->AddComponent<Particle>();
	_meteorParticleHit1->GetComponent<Particle>()->SetParticleEffect("Flare6", "Resources/Textures/Particles/fx_Twister3.dds", 1000);
	_meteorParticleHit1->GetComponent<Particle>()->SetParticleDuration(2.4f, 2.0f);
	_meteorParticleHit1->GetComponent<Particle>()->SetParticleVelocity(1.0f, true);
	_meteorParticleHit1->GetComponent<Particle>()->SetParticleSize(1.0f, 1.f);
	_meteorParticleHit1->GetComponent<Particle>()->AddParticleColor(1.f, 1.f, 1.f);
	_meteorParticleHit1->GetComponent<Particle>()->SetParticleDirection(0.0f, 400.0f, 0.0f);
	_meteorParticleHit1->GetComponent<Particle>()->SetActive(false);
	_meteorParticleHit1->SetActive(true);

	_meteorParticleHit2 = SceneManager::GetInstance().GetCurrentScene()->CreateObject("MeteorParticleHit2");
	_meteorParticleHit2->AddComponent<Particle>();
	_meteorParticleHit2->GetComponent<Particle>()->SetParticleEffect("Halo1", "Resources/Textures/Particles/fx_Halo1.dds", 1000);
	_meteorParticleHit2->GetComponent<Particle>()->SetParticleDuration(8.0f, 0.5f);
	_meteorParticleHit2->GetComponent<Particle>()->SetParticleVelocity(10.0f, true);
	_meteorParticleHit2->GetComponent<Particle>()->SetParticleSize(1.0f, 1.0f);
	_meteorParticleHit2->GetComponent<Particle>()->AddParticleColor(2.0f, 0.5f, 0.0f);
	_meteorParticleHit2->GetComponent<Particle>()->SetParticleDirection(0.0f, 0.0f, 0.0f);
	_meteorParticleHit2->GetComponent<Particle>()->SetParticleCameraApply(true);
	_meteorParticleHit2->GetComponent<Particle>()->SetActive(false);
	_meteorParticleHit2->SetActive(true);

	_meteorParticleHit3 = SceneManager::GetInstance().GetCurrentScene()->CreateObject("MeteorParticleHit3");
	_meteorParticleHit3->AddComponent<Particle>();
	_meteorParticleHit3->GetComponent<Particle>()->SetParticleEffect("Flare6", "Resources/Textures/Particles/fx_Twister3.dds", 1000);
	_meteorParticleHit3->GetComponent<Particle>()->SetParticleDuration(2.4f, 2.0f);
	_meteorParticleHit3->GetComponent<Particle>()->SetParticleVelocity(1.0f, true);
	_meteorParticleHit3->GetComponent<Particle>()->SetParticleSize(1.0f, 1.0f);
	_meteorParticleHit3->GetComponent<Particle>()->AddParticleColor(2.0f, 0.3f, 0.1f);
	_meteorParticleHit3->GetComponent<Particle>()->SetParticleDirection(0.0f, 600.0f, 0.0f);
	_meteorParticleHit3->GetComponent<Particle>()->SetParticleCameraApply(true);
	_meteorParticleHit3->GetComponent<Particle>()->SetActive(false);
	_meteorParticleHit3->SetActive(true);

	_meteorParticleHit4 = SceneManager::GetInstance().GetCurrentScene()->CreateObject("MeteorParticleHit4");
	_meteorParticleHit4->AddComponent<Particle>();
	_meteorParticleHit4->GetComponent<Particle>()->SetParticleEffect("BlastWave2", "Resources/Textures/Particles/fx_BlastWave2.dds", 1000);
	_meteorParticleHit4->GetComponent<Particle>()->SetParticleDuration(4.0f, 0.5f);
	_meteorParticleHit4->GetComponent<Particle>()->SetParticleVelocity(0.0f, true);
	_meteorParticleHit4->GetComponent<Particle>()->SetParticleSize(1.0f, 1.0f);
	_meteorParticleHit4->GetComponent<Particle>()->AddParticleColor(1.0f, 0.0f, 0.0f);
	_meteorParticleHit4->GetComponent<Particle>()->SetParticleDirection(0.0f, 0.0f, 0.0f);
	_meteorParticleHit4->GetComponent<Particle>()->SetParticleCameraApply(true);
	_meteorParticleHit4->GetComponent<Particle>()->SetActive(false);
	_meteorParticleHit4->SetActive(true);

	// 운석에 point light 추가
	DirectX::XMFLOAT4 ambient = { 1.0f, 1.0f, 1.0f, 1.0f };
	DirectX::XMFLOAT4 diffuse = { 1.0f, 0.0f, 0.0f, 1.0f };
	DirectX::XMFLOAT4 specular = { 1.0f, 1.0f, 1.0f, 1.0f };

	_meteor->AddComponent<Light>();
	_meteor->GetComponent<Light>()->CreatePointLight(ambient, diffuse, specular, 300.0f);

	// 비활성화 조건		// 인게임에서는 소멸처럼
	meteorProj->SetDestoryCondition([meteor, meteorProj, this]()->bool
		{
			if (_meteor->GetComponent<Transform>()->GetPosition().y <= 2.0f)
			{
				if (meteorProj->GetCollider()->GetTargetObject() != nullptr && meteorProj->GetCollider()->IsCollided() && meteorProj->GetCollider()->GetTargetObject()->GetTag() == "Boss" && this->_isMeteorHit)
				{
					EventManager::GetInstance().CalculateDamageToBoss(meteor);
					_isMeteorHit = false;
				}

				_isMeteorEnded = true;
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
					_meteor->GetComponent<Transform>()->GetPosition().y - 30.0f * TimeManager::GetInstance().GetDeltaTime(),
					_meteor->GetComponent<Transform>()->GetPosition().z
				);

				// 실제로 끝나지 않았지만 로직상 문제 X
				if (meteorProj->GetCollider()->IsCollided() && meteorProj->GetCollider()->GetTargetObject()->GetTag() == "Boss")
				{
					_isMeteorEnded = true;
				}
			}

			if (!(_meteor->GetActivated()) && _isMeteorEnded == true)
			{

				_meteorParticleTimer += TimeManager::GetInstance().GetDeltaTime();

				_meteorParticleHit1->GetComponent<Particle>()->SetActive(true);
				_meteorParticleHit2->GetComponent<Particle>()->SetActive(true);
				_meteorParticleHit3->GetComponent<Particle>()->SetActive(true);
				_meteorParticleHit4->GetComponent<Particle>()->SetActive(true);

				_meteorParticleHit1->GetComponent<Transform>()->SetPosition(_meteor->GetComponent<Transform>()->GetPosition());
				_meteorParticleHit2->GetComponent<Transform>()->SetPosition(_meteor->GetComponent<Transform>()->GetPosition());
				_meteorParticleHit3->GetComponent<Transform>()->SetPosition(_meteor->GetComponent<Transform>()->GetPosition());
				_meteorParticleHit4->GetComponent<Transform>()->SetPosition(_meteor->GetComponent<Transform>()->GetPosition());

				_meteorParticleHit1->GetComponent<Particle>()->SetParticleSize(30 - (_meteorParticleTimer * 40), 30 - (_meteorParticleTimer * 40));

				_meteorParticleHit3->GetComponent<Particle>()->SetParticleSize(120 - (_meteorParticleTimer * 150), 120 - (_meteorParticleTimer * 150));


				if (_meteorParticleTimer < 0.2f)
				{
					_meteorParticleHit2->GetComponent<Particle>()->SetParticleSize(_meteorParticleTimer * 300, _meteorParticleTimer * 300);
					_meteorParticleHit4->GetComponent<Particle>()->SetParticleSize(_meteorParticleTimer * 300, _meteorParticleTimer * 300);
				}
				else
				{
					_meteorParticleHit2->GetComponent<Particle>()->SetParticleSize(120 - (_meteorParticleTimer * 120), 120 - (_meteorParticleTimer * 120));
					_meteorParticleHit4->GetComponent<Particle>()->SetParticleSize(120 - (_meteorParticleTimer * 120), 120 - (_meteorParticleTimer * 120));
				}


				if (_meteorParticleTimer > 1.0f)
				{
					_meteorParticleTimer = 0.0f;
					_meteorParticleHit1->GetComponent<Particle>()->SetActive(false);
					_meteorParticleHit2->GetComponent<Particle>()->SetActive(false);
					_meteorParticleHit3->GetComponent<Particle>()->SetActive(false);
					_meteorParticleHit4->GetComponent<Particle>()->SetActive(false);
					_isMeteorEnded = false;
				}

				if (_meteorParticleTimer < 0.3f)
				{
					EventManager::GetInstance().CamShake();
				}
			}
		});

	// 다 만들었으면 컨테이너에 추가
	AddToContanier(meteor);
}

void KunrealEngine::PlayerAbility::AddToContanier(Ability* abil)
{
	_abilityContainer.emplace_back(abil);
}

