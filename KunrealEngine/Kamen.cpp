#include <functional>
#include "ToolBox.h"
#include "TimeManager.h"
#include "SceneManager.h"
#include "Kamen.h"

KunrealEngine::Kamen::Kamen()
	: Boss(), _leftHand(nullptr), _rightHand(nullptr), _call(nullptr), _call2(nullptr), _lazer(nullptr),
	_callMoveDistance(0.0f), _isRotateFinish(false), _isCoreStart(false), _isRandomStart(false),
	_leftAttack(nullptr), _rightAttack(nullptr), _spellAttack(nullptr), _callAttack(nullptr), _call2Attack(nullptr),
	_turn180(nullptr), _backStep(nullptr), _teleport(nullptr), _teleportWithLook(nullptr),
	_turnClockWise(nullptr), _turnAntiClockWise(nullptr),
	_emergence9Lich(nullptr), _targetIndex(0),
	_call2PrevStep(1), _lazerCollider(nullptr),
	_swordInsideAttack(nullptr), _swordInsideWarning(nullptr), _outsideSafe(nullptr),
	_basicSwordAttack(nullptr), _freeSword(nullptr), _freeSwordCollider(nullptr), _swordHide(nullptr),
	_swordTurnClockWise(nullptr), _swordTurnAntiClockWise(nullptr), _swordRotateAngle(0.0f),
	_swordStartPos(), _swordOriginPos(), _circleWarningSize(0.0f), _swordEmmergence(nullptr),
	_insideSafe(nullptr), _swordOutsideWarning(nullptr), _swordLinearAtack(nullptr), _swordLinearDistance(0.0f),
	_swordLinearReady(nullptr), _swordLookPlayer(nullptr), _swordDirection(), _timer(0.0f), _swordPath(nullptr),
	_swordChopAttack(nullptr), _donutSafe(nullptr), _swordDonutWarning1(nullptr), _swordDonutWarning2(nullptr), _swordDonutWarning3(nullptr),
	_swordRotation(), _swordChopSpeed(0.0f), _warningMaxTimer(0.0f),
	_leftHandBone(nullptr), _rightFireAttack(nullptr), _rightHandBone(nullptr), _leftFireAttack(nullptr),
	_alterEgo(nullptr), _isSpecial2Ready(false),
	_isSpecial2Playing(false), _egoTimer(0.0f), _isEgoAppearInit(false), _isEgoAppearFinish(false), _isEgoAttackReady(false),
	_isEgoAttack(false), _egoLeftHandBone(nullptr), _egoRightHandBone(nullptr),
	_egoCall2PrevStep(0), _egoCall2(nullptr), _egoLazer(nullptr), _egoLazerCollider(nullptr), _reverseEmergence(nullptr),
	_emergence(nullptr), _emergencePos(), _bossInsideWarning(nullptr), _bossInsideAttack(nullptr), _bossRandomInsideWarning(nullptr),
	_egoInsideWarning(nullptr), _egoInsideAttack(nullptr)
{
	BossBasicInfo info;

	info.SetHp(100).SetPhase(3).SetArmor(10).SetDamage(100).SetMoveSpeed(20.0f).SetsStaggeredGauge(100.0f);
	info.SetAttackRange(5.0f);

	SetInfo(info);

	_centerPos.x = 6.5f;
	_centerPos.y = 2.0f;
	_centerPos.z = -34.0f;
}

KunrealEngine::Kamen::~Kamen()
{

}

void KunrealEngine::Kamen::Initialize()
{
	// 반드시 해야함
	Boss::Initialize(this->GetOwner());

	// 보스 타이머 설정
	SetStartTime(0.0f);
}

void KunrealEngine::Kamen::Release()
{
}

void KunrealEngine::Kamen::FixedUpdate()
{

}

void KunrealEngine::Kamen::Update()
{
	// 반드시 해야함
	Boss::Update();
}

void KunrealEngine::Kamen::LateUpdate()
{

}

void KunrealEngine::Kamen::OnTriggerEnter()
{

}

void KunrealEngine::Kamen::OnTriggerStay()
{

}

void KunrealEngine::Kamen::OnTriggerExit()
{

}

void KunrealEngine::Kamen::SetActive(bool active)
{
	this->_isActivated = active;
}

void KunrealEngine::Kamen::SetMesh()
{
	_boss->AddComponent<MeshRenderer>();
	_boss->GetComponent<MeshRenderer>()->SetMeshObject("Lich/Lich");
}

void KunrealEngine::Kamen::SetTexture()
{
	auto texSize = _boss->GetComponent<MeshRenderer>()->GetTextures().size();
	for (int i = 0; i < texSize; i++)
	{
		_boss->GetComponent<MeshRenderer>()->SetDiffuseTexture(i, "Lich/T_Lich_1_D.tga");
		_boss->GetComponent<MeshRenderer>()->SetNormalTexture(i, "Lich/T_Lich_N.tga");
		_boss->GetComponent<MeshRenderer>()->SetEmissiveTexture(i, "Lich/T_Lich_01_E.tga");
	}

	_boss->GetComponent<MeshRenderer>()->SetCartoonState(true);
}

void KunrealEngine::Kamen::SetBossTransform()
{
	_boss->GetComponent<Transform>()->SetPosition(_centerPos.x, _centerPos.y, _centerPos.z);
}


void KunrealEngine::Kamen::SetBossCollider()
{
	_boss->AddComponent<BoxCollider>();
	_boss->GetComponent<BoxCollider>()->SetTransform(_boss, "Spine1_M");
	_boss->GetComponent<BoxCollider>()->SetColliderScale(6.0f, 18.0f, 10.0f);
	_boss->GetComponent<BoxCollider>()->SetOffset(0.0f, -1.5f, 0.0f);
}

void KunrealEngine::Kamen::CreatePattern()
{
	CreateSubObject();
	CreateParticleObject();

	CreateLeftAttack();
	CreateRightAttack();
	CreateTurn180();
	CreateSpellAttack();
	CreateLeftAttackThrowingFire();
	CreateRightAttackThrowingFire();
	CreateBossRandomInsideWarning();

	CreateSwordAttack();

	CreateCallAttack();
	CreateCall2Attack();
	CreateBackStep();
	CreateTeleportToCenter();
	CreateTeleportToCenterWithLook();
	CreateTurnClockWise();
	CreateTurnAntiClockWise();
	CreateReverseEmergence();
	CreateEmergence();

	CreateSwordEmergence();
	CreateSwordHide();

	CraeteSwordTurnClockWise();
	CreateSwordTurnAntiClock();
	CreateOutsideSafe();
	CreateInsideSafe();
	CreateDonutSafe();

	CreateSwordLookPlayer();
	CreateSwordLinearReady();
	CreateSwordLinearAttack();

	CreateSwordChopAttack();


	CreateEmergenceAttack();

	// 실제 사용중인 패턴들 모아놓음
	GamePattern();
}

void KunrealEngine::Kamen::GamePattern()
{
	BasicPattern();						// 기본 spell, call
	//
	//LeftRightPattern();					// 전방 좌, 우 어택
	//RightLeftPattern();					// 전방 좌, 후방 우 어택
	//BackStepCallPattern();				// 백스탭 뒤 콜 어택
	//TeleportSpellPattern();				// 텔포 후 spell	

	//EmergenceAttackPattern();				// 사라졌다가 등장 후 보스 주변 원으로 터지는 공격

	SwordTurnAntiClockPattern();		// 텔포 후 반시계 -> 외부 안전
	//SwordTurnClockPattern();			// 텔포 후 시계 -> 내부 안전
	//SwordLinearAttackPattern();
	//SwordChopPattern();

	//BasicSwordAttackPattern();

	//CoreEmmergencePattern();

	_basicPattern.emplace_back(_leftFireAttack);
	//_basicPattern.emplace_back(_rightFireAttack);
}


void KunrealEngine::Kamen::Idle()
{
	_boss->GetComponent<Animator>()->Play("Idle", _info._baseAnimSpeed, true);

	// 핵심 패턴이 구현되어있다면
	if (!(_corePattern).empty())
	{
		// 핵심 패턴은 back에서부터 hp가 높은 순서로 되어있기에
		// 보스의 체력이 핵심 패턴의 발동 체력 이하가 된다면
		if (_info._hp <= (_corePattern).back()->_triggerHp)
		{
			// 코어패턴인지 여부 확인
			_isCorePattern = true;

			_nowTitlePattern = _corePattern.back();

			// 패턴 준비 상태로 변경
			_status = BossStatus::PATTERN_READY;

			// 함수 종료
			return;
		}
	}

	// 핵심 패턴이 아닌 기본 공격일 경우에 다음 기본 패턴을 정함
	if (_patternIndex == -1 && !_isCorePattern)
	{
		// 랜덤 패턴을 위한 랜덤 인덱스를 가져옴
		_patternIndex = ToolBox::GetRandomNum(0, _basicPattern.size() - 1);

		if (_basicPattern.size() > 1)
		{
			while (_patternIndex == _exPatternIndex)
			{
				// 랜덤 패턴을 위한 랜덤 인덱스를 가져옴
				_patternIndex = ToolBox::GetRandomNum(0, _basicPattern.size() - 1);
			}

			// 가져온 랜덤 패턴이 활성화되어있지 않다면
			while ((_basicPattern)[_patternIndex]->_isActive == false)
			{
				// 가져온 랜덤 패턴이 활성화 되어 있을때까지 다시 랜덤 인덱스를 추출
				_patternIndex = ToolBox::GetRandomNum(0, _basicPattern.size() - 1);
			}
		}

		// 여기서 patternindex가 1,2,3,4,5라면 분신을 소환
		if (_isSpecial2Ready && (_basicPattern)[_patternIndex]->_withEgo)
		{
			_isSpecial2Playing = true;

			_boss->GetComponent<Animator>()->Stop();
		}
		else
		{
			_isSpecial2Playing = false;
		}
	}

	_nowTitlePattern = _basicPattern[_patternIndex];

	// Chase로 상태 변환
	if (!_isSpecial2Playing)
	{
		_status = BossStatus::CHASE;
	}
	else
	{
		if (_isEgoAppearFinish)
		{
			_status = BossStatus::CHASE;
		}
	}

	if (_nowTitlePattern->_skipChase)
	{
		_status = BossStatus::PATTERN_READY;
	}
}

void KunrealEngine::Kamen::SpecialAttack2()
{
	if (!_isSpecial2Playing)
	{
		_egoTimer += TimeManager::GetInstance().GetDeltaTime();

		if (_egoTimer >= 5.0f)
		{
			_isSpecial2Ready = true;
		}
	}

	else
	{
		if (!_isEgoAppearInit)
		{
			_alterEgo->SetActive(true);
			_alterEgo->GetComponent<MeshRenderer>()->SetActive(true);

			auto egoTransform = _alterEgo->GetComponent<Transform>();

			auto angle = ToolBox::GetAngleWithDirection(egoTransform->GetPosition(), _playerTransform->GetPosition(), egoTransform->GetRotation().y);

			egoTransform->SetRotation(egoTransform->GetRotation().x, angle, egoTransform->GetRotation().z);

			_isEgoAppearInit = true;
		}

		if (_isEgoAppearInit && !_isEgoAppearFinish)
		{
			auto isPlaying = _alterEgo->GetComponent<Animator>()->Play("Emergence", 30.0f);

			if (isPlaying == false)
			{
				_alterEgo->GetComponent<Animator>()->Stop();

				_isEgoAppearFinish = true;

				_isEgoAttackReady = true;
			}
		}

		if (_isEgoAppearFinish && _status == BossStatus::CHASE)
		{
			_alterEgo->GetComponent<Animator>()->Play("Idle", 20.0f);

			auto egoTransform = _alterEgo->GetComponent<Transform>();
			auto goalAngle = ToolBox::GetAngleWithDirection(egoTransform->GetPosition(), _playerTransform->GetPosition(), 0.0f);

			auto rotation = egoTransform->GetRotation();

			// 회전 속도
			float speed = TimeManager::GetInstance().GetDeltaTime() * 50.0f;

			// 현재 각도가 목표로 하는 각도보다 작을 경우

			auto errorRange = std::abs(goalAngle) - std::abs(egoTransform->GetRotation().y);

			errorRange = std::abs(errorRange);

			if (errorRange > 1.0f)
			{
				if (goalAngle < rotation.y)
				{
					// 회전 속도만큼 회전
					egoTransform->SetRotation(egoTransform->GetRotation().x, egoTransform->GetRotation().y - speed, egoTransform->GetRotation().z);
				}
				else
				{
					// 회전 속도만큼 회전
					egoTransform->SetRotation(egoTransform->GetRotation().x, egoTransform->GetRotation().y + speed, egoTransform->GetRotation().z);
				}
			}
		}

		if (_isEgoAttackReady && _status == BossStatus::BASIC_ATTACK)
		{
			_alterEgo->GetComponent<Animator>()->Play("Idle", 20.0f);

			_isEgoAttack = false;
		}


		if (_isEgoAppearFinish && _isEgoAttackReady == false)
		{
			if (!_isEgoAttack)
			{
				_alterEgo->GetComponent<Animator>()->Stop();
				_isEgoAttack = true;
			}
		}

		if ((_isEgoAttack || _isEgoAttackReady) && _status == BossStatus::PATTERN_END)
		{
			_alterEgo->GetComponent<MeshRenderer>()->SetActive(false);
			_alterEgo->SetActive(false);

			_isSpecial2Ready = false;
			_isSpecial2Playing = false;
			_isEgoAppearInit = false;
			_isEgoAppearFinish = false;
			_isEgoAttackReady = false;
			_isEgoAttack = false;
			_egoTimer = false;

			auto egoTransform = _alterEgo->GetComponent<Transform>();

			egoTransform->SetRotation(egoTransform->GetRotation().x, 0.0f, egoTransform->GetRotation().z);

			_alterEgo->GetComponent<Animator>()->Stop();
		}
	}
}


void KunrealEngine::Kamen::CreateParticleObject()
{
	auto mapOffsetY = 5.0f;

	// hand fire1
	for (int j = 0; j < 2; j++)
	{
		for (int i = 0; i < 5; i++)
		{
			{
				std::string index;
				if (j == 0)
				{
					index = "ParticleHandFire1_" + std::to_string(i + 1);
				}
				else
				{
					index = "ParticleEgoHandFire1_" + std::to_string(i + 1);
				}
				auto handFire = _boss->GetObjectScene()->CreateObject(index);
				handFire->GetComponent<Transform>()->SetPosition(0.0f, mapOffsetY, 0.0f);
				handFire->AddComponent<Particle>();
				handFire->GetComponent<Particle>()->SetParticleEffect("BlastWave2", "Resources/Textures/Particles/fx_BlastWave2.dds", 1000);
				handFire->GetComponent<Particle>()->SetParticleDuration(1.0f, 4.0f);
				handFire->GetComponent<Particle>()->SetParticleVelocity(5.0f, true);
				handFire->GetComponent<Particle>()->SetParticleSize(7.f, 7.0f);
				handFire->GetComponent<Particle>()->AddParticleColor(1.2f, 7.5f, 0.6f);
				handFire->GetComponent<Particle>()->SetActive(false);
				handFire->SetActive(false);

				if (j == 0)
				{
					handFire->SetParent(_handFire[i]);

					_particleBossfire1.emplace_back(handFire);
				}
				else
				{
					handFire->SetParent(_egoHandFire[i]);

					_particleEgofire2.emplace_back(handFire);
				}
			}
		}
	}

	for (int i = 0; i < 5; i++)
	{
		//본체 hand fire2
		{
			std::string index = "ParticleHandFire2_" + std::to_string(i + 1);
			auto BossHandFire = _boss->GetObjectScene()->CreateObject(index);
			BossHandFire->GetComponent<Transform>()->SetPosition(0.0f, mapOffsetY, 0.0f);
			BossHandFire->AddComponent<Particle>();
			BossHandFire->GetComponent<Particle>()->SetParticleEffect("BlastWave3", "Resources/Textures/Particles/fx_BlastWave3.dds", 1000);
			BossHandFire->GetComponent<Particle>()->SetParticleDuration(1.0f, 0.7f);
			BossHandFire->GetComponent<Particle>()->SetParticleVelocity(10.0f, true);
			BossHandFire->GetComponent<Particle>()->SetParticleSize(10.f, 10.0f);
			BossHandFire->GetComponent<Particle>()->AddParticleColor(1.5f, 7.5f, 0.4f);
			BossHandFire->GetComponent<Particle>()->SetActive(false);
			BossHandFire->SetActive(false);

			BossHandFire->SetParent(_handFire[i]);

			_particleBossfire2.emplace_back(BossHandFire);
		}

		{
			// 분신용 hand fire2
			std::string index = "ParticleEgoHandFire2_" + std::to_string(i + 1);
			auto EgoHandFire = _boss->GetObjectScene()->CreateObject(index);
			EgoHandFire->GetComponent<Transform>()->SetPosition(0.0f, mapOffsetY, 0.0f);
			EgoHandFire->AddComponent<Particle>();
			EgoHandFire->GetComponent<Particle>()->SetParticleEffect("BlastWave3", "Resources/Textures/Particles/fx_BlastWave3.dds", 1000);
			EgoHandFire->GetComponent<Particle>()->SetParticleDuration(1.0f, 0.7f);
			EgoHandFire->GetComponent<Particle>()->SetParticleVelocity(10.0f, true);
			EgoHandFire->GetComponent<Particle>()->SetParticleSize(10.f, 10.0f);
			EgoHandFire->GetComponent<Particle>()->AddParticleColor(1.5f, 7.5f, 0.4f);
			EgoHandFire->GetComponent<Particle>()->SetActive(false);
			EgoHandFire->SetActive(false);

			EgoHandFire->SetParent(_egoHandFire[i]);

			_particleEgofire2.emplace_back(EgoHandFire);
		}
	}
	{
		for (int i = 0; i < 2; i++)
		{
			std::string name;
			if (i == 0)
			{
				name = "ParticleCll2_1";
			}
			else
			{
				name = "ParticleEgoCll2_1";
			}
			auto call2_1 = _boss->GetObjectScene()->CreateObject(name);
			call2_1->GetComponent<Transform>()->SetPosition(0, mapOffsetY, 0);
			call2_1->AddComponent<Particle>();
			call2_1->GetComponent<Particle>()->SetParticleEffect("fx_Halo3", "Resources/Textures/Particles/fx_Halo3.dds", 1000);
			call2_1->GetComponent<Particle>()->SetParticleDuration(1.2f, 2.0f);
			call2_1->GetComponent<Particle>()->SetParticleVelocity(1.0f, true);
			call2_1->GetComponent<Particle>()->SetParticleSize(15.f, 9.0f);
			call2_1->GetComponent<Particle>()->AddParticleColor(1.0f, 5.f, 0.0f);
			call2_1->GetComponent<Particle>()->SetParticleDirection(0.0f, 50.0f, 0.0f);
			call2_1->GetComponent<Particle>()->SetActive(false);
			call2_1->SetActive(false);

			if (i == 0)
			{
				call2_1->SetParent(_call2);
				_particleCall2.emplace_back(call2_1);
			}
			else
			{
				call2_1->SetParent(_egoCall2);
				_particleEgoCall2.emplace_back(call2_1);
			}
		}

		for (int i = 0; i < 2; i++)
		{
			std::string name;
			if (i == 0)
			{
				name = "ParticleCll2_2";
			}
			else
			{
				name = "ParticleEgoCll2_2";
			}

			auto call2_2 = _boss->GetObjectScene()->CreateObject(name);
			call2_2->GetComponent<Transform>()->SetPosition(0, mapOffsetY, 0.f);
			call2_2->AddComponent<Particle>();
			call2_2->GetComponent<Particle>()->SetParticleEffect("fx_Sparks1", "Resources/Textures/Particles/fx_Sparks1.dds", 1000);
			call2_2->GetComponent<Particle>()->SetParticleDuration(1.3f, 1.6f);
			call2_2->GetComponent<Particle>()->SetParticleVelocity(3.0f, true);
			call2_2->GetComponent<Particle>()->SetParticleSize(14.f, 14.f);
			call2_2->GetComponent<Particle>()->AddParticleColor(0.0f, 5.0f, 0.0f);
			call2_2->GetComponent<Particle>()->SetParticleDirection(0.0f, 50.0f, 0.0f);
			call2_2->GetComponent<Particle>()->SetActive(false);
			call2_2->SetActive(false);

			if (i == 0)
			{
				call2_2->SetParent(_call2);
				_particleCall2.emplace_back(call2_2);
			}
			else
			{
				call2_2->SetParent(_egoCall2);
				_particleEgoCall2.emplace_back(call2_2);
			}
		}

		for (int i = 0; i < 2; i++)
		{
			std::string name;
			if (i == 0)
			{
				name = "ParticleCll2_3";
			}
			else
			{
				name = "ParticleEgoCll2_3";
			}

			auto call2_3 = _boss->GetObjectScene()->CreateObject(name);
			call2_3->GetComponent<Transform>()->SetPosition(0.0f, mapOffsetY, 0.0f);
			call2_3->AddComponent<Particle>();
			call2_3->GetComponent<Particle>()->SetParticleEffect("fx_Sparks1", "Resources/Textures/Particles/fx_Sparks1.dds", 1000);
			call2_3->GetComponent<Particle>()->SetParticleDuration(2.0f, 1.0f);
			call2_3->GetComponent<Particle>()->SetParticleVelocity(3.f, true);
			call2_3->GetComponent<Particle>()->SetParticleSize(12.f, 12.0f);
			call2_3->GetComponent<Particle>()->AddParticleColor(1.0f, 1.0f, 1.0f);
			call2_3->GetComponent<Particle>()->SetParticleDirection(0.0f, 50.0f, 0.0f);
			call2_3->GetComponent<Particle>()->SetActive(false);
			call2_3->SetActive(false);

			if (i == 0)
			{
				call2_3->SetParent(_call2);
				_particleCall2.emplace_back(call2_3);
			}
			else
			{
				call2_3->SetParent(_egoCall2);
				_particleEgoCall2.emplace_back(call2_3);
			}
		}
	}
	{
		for (int i = 0; i < 11; i++)
		{
			std::string name = "swordCyllinder" + std::to_string(i+1);
			auto swordCylinder = _boss->GetObjectScene()->CreateObject(name);
			swordCylinder->SetParent(_swordInsideAttack);
			swordCylinder->AddComponent<Particle>();
			swordCylinder->GetComponent<Particle>()->SetParticleEffect("BlastWave5", "Resources/Textures/Particles/fx_BlastWave5.dds", 1000);
			swordCylinder->GetComponent<Particle>()->SetParticleDuration(1.5f, 0.1f);
			swordCylinder->GetComponent<Particle>()->SetParticleVelocity(0.0f, true);
			swordCylinder->GetComponent<Particle>()->SetParticleSize(30.f, 30.0f);
			swordCylinder->GetComponent<Particle>()->SetParticleDirection(0.0f, 300.0f, 0.0f);
			swordCylinder->GetComponent<Particle>()->SetParticleCameraApply(true);

			swordCylinder->GetComponent<Particle>()->SetActive(false);

			if (i == 0)
			{
				swordCylinder->GetComponent<Particle>()->AddParticleColor(1.0f, 1.0f, 1.0f);
			}
			else
			{
				swordCylinder->GetComponent<Particle>()->AddParticleColor(0.3f, 1.0f, 0.0f);
			}

			_particleSwordOutsideAttack.emplace_back(swordCylinder);
		}
	}

	{
		for (int i = 0; i < 3; i++)
		{
			std::string name = "donutParticle" + std::to_string(i + 1);
			auto donutParticle = _boss->GetObjectScene()->CreateObject(name);

			donutParticle->AddComponent<Particle>();
			donutParticle->GetComponent<Particle>()->SetParticleEffect("BlastWave5", "Resources/Textures/Particles/fx_BlastWave5.dds", 1000);
			donutParticle->GetComponent<Particle>()->SetParticleDuration(0.8f, 1.0f);
			donutParticle->GetComponent<Particle>()->SetParticleVelocity(0.0f, true);
			donutParticle->GetComponent<Particle>()->AddParticleColor(0.0f, 1.0f, 0.3f);
			donutParticle->GetComponent<Particle>()->SetParticleDirection(0.0f, 300.0f, 0.0f);
			//donutParticle->GetComponent<Particle>()->SetParticleAngle(55.f);
			donutParticle->GetComponent<Particle>()->SetParticleCameraApply(true);
			donutParticle->GetComponent<Particle>()->SetActive(false);
			_particleSwordDonutAttack.emplace_back(donutParticle);

			if (i == 0)
			{
				donutParticle->SetParent(_swordDonutWarning1);
				donutParticle->GetComponent<Particle>()->SetParticleSize(50.f, 50.0f);
			}
			else if (i == 1)
			{
				donutParticle->SetParent(_swordDonutWarning2);
				donutParticle->GetComponent<Particle>()->SetParticleSize(130.f, 130.f);
			}
			else
			{
				donutParticle->SetParent(_swordDonutWarning3);
				donutParticle->GetComponent<Particle>()->SetParticleSize(220.f, 220.f);
			}
		}
	}
	{
		{
			std::string name = "teleportParticle";
			auto teleportParticle = _boss->GetObjectScene()->CreateObject(name);
			teleportParticle->SetParent(_bossInsideAttack);

			teleportParticle->AddComponent<Particle>();
			teleportParticle->GetComponent<Particle>()->SetParticleEffect("BlastWave1", "Resources/Textures/Particles/fx_BlastWave1.dds", 1000);
			teleportParticle->GetComponent<Particle>()->SetParticleDuration(1.0f, 0.2f);
			teleportParticle->GetComponent<Particle>()->SetParticleVelocity(10.0f, true);
			teleportParticle->GetComponent<Particle>()->SetParticleSize(15.f, 15.0f);
			teleportParticle->GetComponent<Particle>()->SetParticleDirection(0.0f, 0.0f, 0.0f);
			teleportParticle->GetComponent<Particle>()->AddParticleColor(0.05f, 0.1f, 0.0f);
			teleportParticle->GetComponent<Particle>()->SetActive(false);
			_particleEmergenceAttack.emplace_back(teleportParticle);
		}

		for (int i = 0; i < 2; i++)
		{
			std::string name = "teleportParticle" + std::to_string(i + 1);
			auto teleportParticle = _boss->GetObjectScene()->CreateObject(name);
			teleportParticle->SetParent(_bossInsideAttack);

			teleportParticle->AddComponent<Particle>();
			teleportParticle->GetComponent<Particle>()->SetParticleEffect("fx_BlastWave3", "Resources/Textures/Particles/fx_BlastWave3.dds", 1000);
			teleportParticle->GetComponent<Particle>()->SetParticleDuration(1.0f, 0.2f);
			teleportParticle->GetComponent<Particle>()->SetParticleVelocity(10.0f, true);
			teleportParticle->GetComponent<Particle>()->AddParticleColor(0.1f, 0.1f, 0.04f);
			teleportParticle->GetComponent<Particle>()->SetParticleDirection(0.0f, 0.0f, 0.0f);
			teleportParticle->GetComponent<Particle>()->SetParticleSize(15.f, 15.0f);
			teleportParticle->GetComponent<Particle>()->SetActive(false);

			if (i == 0)
			{
				teleportParticle->GetComponent<Particle>()->SetParticleAngle(80.0f, 0.0f, 0.0f);
			}
			else if (i == 1)
			{
				teleportParticle->GetComponent<Particle>()->SetParticleAngle(260.0f, 0.0f, 0.0f);
			}
			_particleEmergenceAttack.emplace_back(teleportParticle);
		}
		{
			std::string name = "teleportParticle3";
			auto teleportParticle = _boss->GetObjectScene()->CreateObject(name);
			teleportParticle->SetParent(_bossInsideAttack);

			teleportParticle->AddComponent<Particle>();
			teleportParticle->GetComponent<Particle>()->SetParticleEffect("Halo2", "Resources/Textures/Particles/fx_Halo2.dds", 1000);
			teleportParticle->GetComponent<Particle>()->SetParticleDuration(1.0f, 0.2f);
			teleportParticle->GetComponent<Particle>()->SetParticleVelocity(10.0f, true);
			teleportParticle->GetComponent<Particle>()->SetParticleSize(15.f, 15.0f);
			teleportParticle->GetComponent<Particle>()->SetParticleDirection(0.0f, 0.0f, 0.0f);
			teleportParticle->GetComponent<Particle>()->AddParticleColor(0.1f, 0.1f, 0.0f);
			teleportParticle->GetComponent<Particle>()->SetActive(false);
			_particleEmergenceAttack.emplace_back(teleportParticle);
		}
	}
	{
		{
			std::string name = "insidePillarBoss";
			auto particleBossPillar1 = _boss->GetObjectScene()->CreateObject(name);
			particleBossPillar1->SetParent(_swordInsideAttack);

			particleBossPillar1->AddComponent<Particle>();
			particleBossPillar1->GetComponent<Particle>()->SetParticleEffect("Halo1", "Resources/Textures/Particles/fx_Halo1.dds", 1000);
			particleBossPillar1->GetComponent<Particle>()->SetParticleDuration(2.1f, 1.f);
			particleBossPillar1->GetComponent<Particle>()->SetParticleVelocity(10.0f, true);
			particleBossPillar1->GetComponent<Particle>()->SetParticleSize(80.f, 80.0f);
			particleBossPillar1->GetComponent<Particle>()->AddParticleColor(0.6f, 0.6f, 0.6f);
			particleBossPillar1->GetComponent<Particle>()->SetParticleDirection(0.0f, 160.0f, 0.0f);
			particleBossPillar1->GetComponent<Particle>()->SetParticleCameraApply(true);
			particleBossPillar1->GetComponent<Particle>()->SetActive(false);
		}
		{
			std::string name = "insidePillarBoss";
			auto particleBossPillar2 = _boss->GetObjectScene()->CreateObject(name);
			particleBossPillar2->SetParent(_swordInsideAttack);

			particleBossPillar2->AddComponent<Particle>();
			particleBossPillar2->GetComponent<Particle>()->SetParticleEffect("Sparks1", "Resources/Textures/Particles/fx_Sparks1.dds", 1000);
			particleBossPillar2->GetComponent<Particle>()->SetParticleDuration(2.f, 1.2f);
			particleBossPillar2->GetComponent<Particle>()->SetParticleVelocity(1.0f, true);
			particleBossPillar2->GetComponent<Particle>()->SetParticleSize(120.f, 120.0f);
			particleBossPillar2->GetComponent<Particle>()->AddParticleColor(0.3f, 1.5f, 0.0f);
			particleBossPillar2->GetComponent<Particle>()->SetParticleDirection(0.0f, 160.0f, 0.0f);
			particleBossPillar2->GetComponent<Particle>()->SetParticleCameraApply(true);
			particleBossPillar2->GetComponent<Particle>()->SetActive(false);
		}
		{
			std::string name = "insidePillarBoss";
			auto particleBossPillar3 = _boss->GetObjectScene()->CreateObject(name);
			particleBossPillar3->SetParent(_swordInsideAttack);

			particleBossPillar3->AddComponent<Particle>();
			particleBossPillar3->GetComponent<Particle>()->SetParticleEffect("Blaster2", "Resources/Textures/Particles/fx_Blaster2.dds", 1000);
			particleBossPillar3->GetComponent<Particle>()->SetParticleDuration(2.0f, 1.15f);
			particleBossPillar3->GetComponent<Particle>()->SetParticleVelocity(0.4f, true);
			particleBossPillar3->GetComponent<Particle>()->SetParticleSize(52.f, 40.0f);
			particleBossPillar3->GetComponent<Particle>()->AddParticleColor(0.0f, 0.0f, 2.0f);
			particleBossPillar3->GetComponent<Particle>()->SetParticleDirection(0.0f, 160.0f, 0.0f);
			particleBossPillar3->GetComponent<Particle>()->SetParticleCameraApply(true);
			particleBossPillar3->GetComponent<Particle>()->SetActive(false);
		}
	}
	{
		std::string name = "lazerParticle";
		auto lazerParticle = _boss->GetObjectScene()->CreateObject(name);
		lazerParticle->SetParent(_lazer);

		lazerParticle->AddComponent<Particle>();
		lazerParticle->GetComponent<Particle>()->SetParticleEffect("fx_BlastWave2", "Resources/Textures/Particles/fx_BlastWave2.dds", 1000);
		lazerParticle->GetComponent<Particle>()->SetParticleDuration(2.0f, 1.15f);
		lazerParticle->GetComponent<Particle>()->SetParticleVelocity(84.f, true);
		lazerParticle->GetComponent<Particle>()->SetParticleSize(40.f, 40.0f);
		lazerParticle->GetComponent<Particle>()->AddParticleColor(0.0f, 0.0f, 2.0f);
		lazerParticle->GetComponent<Particle>()->SetParticleCameraApply(true);
		lazerParticle->GetComponent<Particle>()->SetActive(false);
	}
}

void KunrealEngine::Kamen::CreateSubObject()
{
	// 왼손
	_leftHand = _boss->GetObjectScene()->CreateObject("LeftHand");
	_leftHand->AddComponent<BoxCollider>();
	_leftHand->GetComponent<BoxCollider>()->SetTransform(_boss, "MiddleFinger1_L");
	_leftHand->GetComponent<BoxCollider>()->SetColliderScale(2.0f, 3.0f, 2.0f);
	_leftHand->GetComponent<BoxCollider>()->SetActive(false);
	_leftHand->SetTotalComponentState(false);
	_leftHand->SetActive(false);

	// 왼손 불 위치 체크용 메쉬
	_leftHandBone = _boss->GetObjectScene()->CreateObject("leftHandBone");
	_leftHandBone->AddComponent<MeshRenderer>();
	_leftHandBone->GetComponent<MeshRenderer>()->SetMeshObject("cube/cube");
	_leftHandBone->GetComponent<MeshRenderer>()->SetParentBone(_boss, "MiddleFinger1_L");
	_leftHandBone->GetComponent<Transform>()->SetScale(10.0f, 10.f, 10.f);
	_leftHandBone->GetComponent<MeshRenderer>()->SetActive(false);
	_leftHandBone->SetTotalComponentState(false);
	_leftHandBone->SetActive(false);

	// 오른손
	_rightHand = _boss->GetObjectScene()->CreateObject("rightHand");
	_rightHand->AddComponent<BoxCollider>();
	_rightHand->GetComponent<BoxCollider>()->SetTransform(_boss, "MiddleFinger1_R");
	_rightHand->GetComponent<BoxCollider>()->SetColliderScale(2.0f, 3.0f, 2.0f);
	_rightHand->GetComponent<BoxCollider>()->SetActive(false);
	_rightHand->SetTotalComponentState(false);
	_rightHand->SetActive(false);

	// 오른손 불 위치 체크용 메쉬
	_rightHandBone = _boss->GetObjectScene()->CreateObject("RightHandBone");
	_rightHandBone->AddComponent<MeshRenderer>();
	_rightHandBone->GetComponent<MeshRenderer>()->SetMeshObject("cube/cube");
	_rightHandBone->GetComponent<MeshRenderer>()->SetParentBone(_boss, "MiddleFinger1_R");
	_rightHandBone->GetComponent<Transform>()->SetScale(10.0f, 10.f, 10.f);
	_rightHandBone->GetComponent<MeshRenderer>()->SetActive(false);
	_rightHandBone->SetTotalComponentState(false);
	_rightHandBone->SetActive(false);

	// 5개 불 
	for (int i = 0; i < 5; i++)
	{
		std::string index = "HandFire" + std::to_string(i + 1);
		auto handFire = _boss->GetObjectScene()->CreateObject(index);
		handFire->AddComponent<BoxCollider>();
		handFire->GetComponent<BoxCollider>()->SetColliderScale(10.0f, 10.0f, 10.0f);
		handFire->SetTotalComponentState(false);
		handFire->SetActive(false);
		_handFire.emplace_back(handFire);
	}

	// 내부 장판
	_bossInsideWarning = _boss->GetObjectScene()->CreateObject("BossInsideWarning");
	_bossInsideWarning->AddComponent<TransparentMesh>();
	_bossInsideWarning->GetComponent<TransparentMesh>()->CreateTMesh("BossInsideWarning", "Resources/Textures/Warning/Warning.dds", 0.6f, true);
	_bossInsideWarning->GetComponent<Transform>()->SetPosition(_bossTransform->GetPosition().x, _bossTransform->GetPosition().y, _bossTransform->GetPosition().z);
	_bossInsideWarning->GetComponent<TransparentMesh>()->SetRenderType(3);
	_bossInsideWarning->GetComponent<TransparentMesh>()->SetTimer(2.0f);
	_bossInsideWarning->GetComponent<TransparentMesh>()->SetActive(false);
	_bossInsideWarning->SetTotalComponentState(false);
	_bossInsideWarning->SetActive(false);


	// 내부 장판 공격
	_bossInsideAttack = _boss->GetObjectScene()->CreateObject("BossInsideAttack");
	//_bossInsideAttack->AddComponent<Particle>();
	//_bossInsideAttack->GetComponent<Particle>()->SetParticleEffect("fire", "Resources/Textures/Particles/flare.dds", 1000);
	//_bossInsideAttack->GetComponent<Particle>()->SetParticleDuration(1.5f, 2.0f);
	//_bossInsideAttack->GetComponent<Particle>()->SetParticleVelocity(80.f, false);
	//_bossInsideAttack->GetComponent<Particle>()->AddParticleColor(1.2f, 7.5f, 0.6f);
	//_bossInsideAttack->SetTotalComponentState(false);
	//_bossInsideAttack->GetComponent<Particle>()->SetActive(false);


	// 내부 장판
	_swordInsideWarning = _boss->GetObjectScene()->CreateObject("OutsideSafe");
	_swordInsideWarning->AddComponent<TransparentMesh>();
	_swordInsideWarning->GetComponent<TransparentMesh>()->CreateTMesh("OutsideSafe", "Resources/Textures/Warning/Warning.dds", 0.6f, true);
	_swordInsideWarning->GetComponent<Transform>()->SetScale(30.0f, 30.0f, 30.0f);
	_swordInsideWarning->GetComponent<Transform>()->SetPosition(_bossTransform->GetPosition().x, _bossTransform->GetPosition().y, _bossTransform->GetPosition().z);
	_swordInsideWarning->GetComponent<TransparentMesh>()->SetRenderType(3);
	_swordInsideWarning->GetComponent<TransparentMesh>()->SetTimer(1.0f);
	_swordInsideWarning->GetComponent<TransparentMesh>()->SetActive(false);
	_swordInsideWarning->SetTotalComponentState(false);
	_swordInsideWarning->SetActive(false);

	// 내부 장판 공격
	_swordInsideAttack = _boss->GetObjectScene()->CreateObject("InsideAttack");
	_swordInsideAttack->AddComponent<BoxCollider>();
	_swordInsideAttack->GetComponent<BoxCollider>()->SetColliderScale(30.0f, 30.0f, 30.0f);
	_swordInsideAttack->GetComponent<BoxCollider>()->SetActive(false);
	_swordInsideAttack->AddComponent<Particle>();
	_swordInsideAttack->GetComponent<Particle>()->SetParticleEffect("fire", "Resources/Textures/Particles/flare.dds", 1000);
	_swordInsideAttack->GetComponent<Particle>()->SetParticleDuration(1.5f, 2.0f);
	_swordInsideAttack->GetComponent<Particle>()->SetParticleVelocity(80.f, false);
	_swordInsideAttack->GetComponent<Particle>()->AddParticleColor(1.2f, 7.5f, 0.6f);
	_swordInsideAttack->GetComponent<Particle>()->SetActive(false);
	_swordInsideAttack->SetTotalComponentState(false);
	_swordInsideAttack->SetActive(false);

	// 외부 장판
	_swordOutsideWarning = _boss->GetObjectScene()->CreateObject("InsideSafe");
	_swordOutsideWarning->AddComponent<TransparentMesh>();
	_swordOutsideWarning->GetComponent<TransparentMesh>()->CreateTMesh("InsideSafe", "Resources/Textures/Warning/Warning.dds", 0.6f, false);
	_swordOutsideWarning->GetComponent<Transform>()->SetScale(200.0f, 200.0f, 200.0f);
	_swordOutsideWarning->GetComponent<Transform>()->SetPosition(_centerPos.x, _centerPos.y, _centerPos.z);
	_swordOutsideWarning->GetComponent<TransparentMesh>()->SetTimer(2.0f);
	_swordOutsideWarning->GetComponent<TransparentMesh>()->SetRenderType(5);
	_swordOutsideWarning->GetComponent<TransparentMesh>()->SetExceptRange(DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f), 30.0f);
	_swordOutsideWarning->GetComponent<TransparentMesh>()->SetActive(false);
	_swordOutsideWarning->SetTotalComponentState(false);
	_swordOutsideWarning->SetActive(false);

	// 도넛 장판 1번
	_swordDonutWarning1 = _boss->GetObjectScene()->CreateObject("donutSafe1");
	_swordDonutWarning1->AddComponent<TransparentMesh>();
	_swordDonutWarning1->GetComponent<TransparentMesh>()->CreateTMesh("DonutSafe1", "Resources/Textures/Warning/Warning.dds", 0.6f, true);
	_swordDonutWarning1->GetComponent<Transform>()->SetScale(20.0f, 20.0f, 20.0f);
	_swordDonutWarning1->GetComponent<TransparentMesh>()->SetTimer(1.0f);
	_swordDonutWarning1->GetComponent<TransparentMesh>()->SetActive(false);

	_swordDonutWarning1->AddComponent<Particle>();
	_swordDonutWarning1->GetComponent<Particle>()->SetParticleEffect("BlastWave5", "Resources/Textures/Particles/fx_BlastWave5.dds", 1000);
	_swordDonutWarning1->GetComponent<Particle>()->SetParticleDuration(0.8f, 1.0f);
	_swordDonutWarning1->GetComponent<Particle>()->SetParticleVelocity(0.0f, true);
	_swordDonutWarning1->GetComponent<Particle>()->SetParticleSize(45.0f, 45.0f);
	_swordDonutWarning1->GetComponent<Particle>()->AddParticleColor(0.3f, 1.0f, 0.0f);
	_swordDonutWarning1->GetComponent<Particle>()->SetParticleDirection(0.0f, 300.0f, 0.0f);
	_swordDonutWarning1->GetComponent<Particle>()->SetParticleCameraApply(true);
	_swordDonutWarning1->SetTotalComponentState(false);
	_swordDonutWarning1->SetActive(false);

	// 도넛 장판 2번
	_swordDonutWarning2 = _boss->GetObjectScene()->CreateObject("donutSafe2");
	_swordDonutWarning2->AddComponent<TransparentMesh>();
	_swordDonutWarning2->GetComponent<TransparentMesh>()->CreateTMesh("DonutSafe2", "Resources/Textures/Warning/Warning.dds", 0.6f, true);
	_swordDonutWarning2->GetComponent<Transform>()->SetScale(60.0f, 60.0f, 60.0f);
	_swordDonutWarning2->GetComponent<TransparentMesh>()->SetTimer(1.0f);
	_swordDonutWarning2->GetComponent<TransparentMesh>()->SetRenderType(6);
	_swordDonutWarning2->GetComponent<TransparentMesh>()->SetActive(false);
	_swordDonutWarning2->AddComponent<Particle>();
	_swordDonutWarning2->GetComponent<Particle>()->SetParticleEffect("BlastWave3", "Resources/Textures/Particles/fx_BlastWave3.dds", 1000);
	_swordDonutWarning2->GetComponent<Particle>()->SetParticleDuration(0.8f, 1.0f);
	_swordDonutWarning2->GetComponent<Particle>()->SetParticleVelocity(0.0f, true);
	_swordDonutWarning2->GetComponent<Particle>()->SetParticleSize(120.f, 120.0f);
	_swordDonutWarning2->GetComponent<Particle>()->AddParticleColor(0.3f, 1.0f, 0.0f);
	_swordDonutWarning2->GetComponent<Particle>()->SetParticleDirection(0.0f, 300.0f, 0.0f);
	_swordDonutWarning2->GetComponent<Particle>()->SetParticleCameraApply(true);
	_swordDonutWarning2->SetTotalComponentState(false);
	_swordDonutWarning2->SetActive(false);

	// 도넛 장판 3번
	_swordDonutWarning3 = _boss->GetObjectScene()->CreateObject("donutSafe3");
	_swordDonutWarning3->AddComponent<TransparentMesh>();
	_swordDonutWarning3->GetComponent<TransparentMesh>()->CreateTMesh("DonutSafe3", "Resources/Textures/Warning/Warning.dds", 0.6f, true);
	_swordDonutWarning3->GetComponent<Transform>()->SetScale(100.0f, 100.0f, 100.0f);
	_swordDonutWarning3->GetComponent<TransparentMesh>()->SetTimer(1.0f);
	_swordDonutWarning3->GetComponent<TransparentMesh>()->SetRenderType(6);
	_swordDonutWarning3->GetComponent<TransparentMesh>()->SetActive(false);
	_swordDonutWarning3->AddComponent<Particle>();
	_swordDonutWarning3->GetComponent<Particle>()->SetParticleEffect("BlastWave3", "Resources/Textures/Particles/fx_BlastWave3.dds", 1000);
	_swordDonutWarning3->GetComponent<Particle>()->SetParticleDuration(0.8f, 1.0f);
	_swordDonutWarning3->GetComponent<Particle>()->SetParticleVelocity(0.0f, true);
	_swordDonutWarning3->GetComponent<Particle>()->SetParticleSize(215.f, 215.0f);
	_swordDonutWarning3->GetComponent<Particle>()->AddParticleColor(0.3f, 1.0f, 0.0f);
	_swordDonutWarning3->GetComponent<Particle>()->SetParticleDirection(0.0f, 300.0f, 0.0f);
	_swordDonutWarning3->GetComponent<Particle>()->SetParticleCameraApply(true);
	_swordDonutWarning3->SetTotalComponentState(false);
	_swordDonutWarning3->SetActive(false);

	// 레이저 
	_lazer = _boss->GetObjectScene()->CreateObject("lazer");
	_lazer->AddComponent<Particle>();
	_lazer->GetComponent<Particle>()->SetParticleEffect("Laser", "Resources/Textures/Particles/fx_LightFlash1.dds", 1000);
	_lazer->GetComponent<Particle>()->SetParticleSize(15.f, 15.f);
	_lazer->GetComponent<Particle>()->SetParticleDuration(1.7f, 2.0f);
	_lazer->GetComponent<Particle>()->SetParticleVelocity(84.f, false);
	_lazer->GetComponent<Particle>()->SetParticleRotation(90.0f, _bossTransform->GetRotation().y, 0.0f);
	_lazer->GetComponent<Particle>()->AddParticleColor(0.2f, 1.0f, 0.0f);
	_lazer->GetComponent<Particle>()->SetParticleCameraApply(true);
	_lazer->GetComponent<Particle>()->SetActive(false);
	_lazer->SetTotalComponentState(false);
	_lazer->SetActive(false);

	// 레이저 콜라이더
	_lazerCollider = _boss->GetObjectScene()->CreateObject("lazerCollider");
	_lazerCollider->AddComponent<BoxCollider>();
	_lazerCollider->GetComponent<BoxCollider>()->SetColliderScale(140.0f, 10.0f, 10.0f);
	_lazerCollider->GetComponent<BoxCollider>()->SetActive(false);
	_lazerCollider->SetTotalComponentState(false);
	_lazerCollider->SetActive(false);

	// call 투사체
	_call = _boss->GetObjectScene()->CreateObject("call");
	_call->AddComponent<BoxCollider>();
	_call->GetComponent<BoxCollider>()->SetColliderScale(10.0f, 10.0f, 10.0f);
	_call->GetComponent<BoxCollider>()->SetActive(false);
	_call->AddComponent<Particle>();
	_call->GetComponent<Particle>()->SetParticleEffect("Flame", "Resources/Textures/Particles/flare.dds", 1000);
	_call->GetComponent<Particle>()->SetParticleDuration(2.0f, 2.0f);
	_call->GetComponent<Particle>()->SetParticleVelocity(3.f, true);
	_call->GetComponent<Particle>()->SetParticleSize(10.f, 30.0f);
	_call->GetComponent<Particle>()->SetParticleDirection(0.0f, 7.0f, 0.0f);
	_call->GetComponent<Particle>()->AddParticleColor(1.2f, 7.5f, 0.6f);
	_call->GetComponent<Particle>()->SetActive(false);
	DirectX::XMFLOAT4 diffuse = { 0.0f, 0.6f, 0.0f, 1.0f };
	DirectX::XMFLOAT4 ambient = { 0.0f, 0.15f, 0.0f, 1.0f };
	DirectX::XMFLOAT4 specular = { 0.0f, 0.15f, 0.0f, 1.0f };
	_call->AddComponent<Light>();
	_call->GetComponent<Light>()->CreatePointLight(ambient, diffuse, specular, 1.0f);
	_call->GetComponent<Light>()->SetActive(false);
	_call->SetTotalComponentState(false);
	_call->SetActive(false);

	// call2 투사체
	_call2 = _boss->GetObjectScene()->CreateObject("call2");
	_call2->AddComponent<BoxCollider>();
	_call2->GetComponent<BoxCollider>()->SetColliderScale(15.0f, 30.0f, 15.0f);
	_call2->GetComponent<BoxCollider>()->SetActive(false);
	_call2->AddComponent<Light>();
	_call2->GetComponent<Light>()->CreatePointLight(ambient, diffuse, specular, 1.0f);
	_call2->GetComponent<Light>()->SetActive(false);
	_call2->SetTotalComponentState(false);
	_call2->SetActive(false);

	// 따로 패턴하는 검
	_freeSword = _boss->GetObjectScene()->CreateObject("KamenSword");
	_freeSword->AddComponent<MeshRenderer>();
	_freeSword->GetComponent<MeshRenderer>()->SetMeshObject("KamenSword/KamenSword");
	_freeSword->GetComponent<MeshRenderer>()->SetActive(false);
	_freeSword->AddComponent<BoxCollider>();
	_freeSword->GetComponent<BoxCollider>()->SetColliderScale(5.0f, 20.0f, 5.0f);
	_freeSword->GetComponent<BoxCollider>()->SetActive(false);
	auto texSize = _freeSword->GetComponent<MeshRenderer>()->GetTextures().size();
	for (int i = 0; i < texSize; i++)
	{
		_freeSword->GetComponent<MeshRenderer>()->SetDiffuseTexture(i, "KamenSword/KamenSword_BaseColor.png");
		_freeSword->GetComponent<MeshRenderer>()->SetNormalTexture(i, "KamenSword/KamenSword_Normal.png");
		_freeSword->GetComponent<MeshRenderer>()->SetEmissiveTexture(i, "KamenSword/KamenSword_Emissive.png");
	}
	_freeSword->SetTotalComponentState(false);
	_freeSword->SetActive(false);

	// 검 경로 장판
	_swordPath = _boss->GetObjectScene()->CreateObject("swordPath");
	_swordPath->AddComponent<TransparentMesh>();
	_swordPath->GetComponent<TransparentMesh>()->CreateTMesh("SwordPath", "Resources/Textures/Warning/Warning.dds", 0.6f, false);
	_swordPath->GetComponent<Transform>()->SetScale(100.0f, 10.0f, 10.0f);
	_swordPath->GetComponent<TransparentMesh>()->SetTimer(4.5f);
	_swordPath->GetComponent<TransparentMesh>()->SetActive(false);
	_swordPath->SetTotalComponentState(false);
	_swordPath->SetActive(false);

	// 핵심패턴 9마리 보스
	for (int i = 0; i < 9; i++)
	{
		std::string index = "fake" + std::to_string(i + 1);
		auto boss = _boss->GetObjectScene()->CreateObject(index);

		boss->AddComponent<MeshRenderer>();
		boss->GetComponent<MeshRenderer>()->SetMeshObject("FakeLich/FakeLich");

		auto texSize = boss->GetComponent<MeshRenderer>()->GetTextures().size();
		for (int i = 0; i < texSize; i++)
		{
			boss->GetComponent<MeshRenderer>()->SetDiffuseTexture(i, "Lich/T_Lich_1_D.tga");
			boss->GetComponent<MeshRenderer>()->SetNormalTexture(i, "Lich/T_Lich_N.tga");
			boss->GetComponent<MeshRenderer>()->SetEmissiveTexture(i, "Lich/T_Lich_01_E.tga");
		}

		boss->AddComponent<BoxCollider>();
		boss->GetComponent<BoxCollider>()->SetTransform(boss, "Spine1_M");
		boss->GetComponent<BoxCollider>()->SetColliderScale(6.0f, 18.0f, 10.0f);
		boss->GetComponent<BoxCollider>()->SetOffset(0.0f, -1.5f, 0.0f);

		boss->SetTotalComponentState(false);
		boss->SetActive(false);

		_fakeBoss.emplace_back(boss);
	}

	/// 분신 서브 오브젝트들

	// 분신 소환
	_alterEgo = _boss->GetObjectScene()->CreateObject("Ego");

	_alterEgo->AddComponent<MeshRenderer>();
	_alterEgo->GetComponent<MeshRenderer>()->SetMeshObject("FakeLich/FakeLich");
	_alterEgo->GetComponent<Transform>()->SetPosition(0.0f, 2.0f, 0.0f);

	auto egoTexSize = _alterEgo->GetComponent<MeshRenderer>()->GetTextures().size();
	for (int i = 0; i < egoTexSize; i++)
	{
		_alterEgo->GetComponent<MeshRenderer>()->SetDiffuseTexture(i, "Lich/T_Lich_1_D.tga");
		_alterEgo->GetComponent<MeshRenderer>()->SetNormalTexture(i, "Lich/T_Lich_N.tga");
		_alterEgo->GetComponent<MeshRenderer>()->SetEmissiveTexture(i, "Lich/T_Lich_01_E.tga");
	}
	
	_alterEgo->SetTotalComponentState(false);
	_alterEgo->SetActive(false);

	// 왼손 불 위치 체크용 메쉬
	_egoLeftHandBone = _boss->GetObjectScene()->CreateObject("EgoLeftHandBone");
	_egoLeftHandBone->AddComponent<MeshRenderer>();
	_egoLeftHandBone->GetComponent<MeshRenderer>()->SetMeshObject("cube/cube");
	_egoLeftHandBone->GetComponent<MeshRenderer>()->SetParentBone(_alterEgo, "MiddleFinger1_L");
	_egoLeftHandBone->GetComponent<MeshRenderer>()->SetActive(false);
	_egoLeftHandBone->GetComponent<Transform>()->SetScale(10.0f, 10.f, 10.f);
	_egoLeftHandBone->SetTotalComponentState(false);
	_egoLeftHandBone->SetActive(false);

	// 오른손 불 위치 체크용 메쉬
	_egoRightHandBone = _boss->GetObjectScene()->CreateObject("EgoRightHandBone");
	_egoRightHandBone->AddComponent<MeshRenderer>();
	_egoRightHandBone->GetComponent<MeshRenderer>()->SetMeshObject("cube/cube");
	_egoRightHandBone->GetComponent<MeshRenderer>()->SetParentBone(_alterEgo, "MiddleFinger1_R");
	_egoRightHandBone->GetComponent<MeshRenderer>()->SetActive(false);
	_egoRightHandBone->GetComponent<Transform>()->SetScale(10.0f, 10.f, 10.f);
	_egoRightHandBone->SetTotalComponentState(false);
	_egoRightHandBone->SetActive(false);

	// 5개 불 
	for (int i = 0; i < 5; i++)
	{
		std::string index = "EgoHandFire" + std::to_string(i + 1);
		auto handFire = _boss->GetObjectScene()->CreateObject(index);
		handFire->AddComponent<BoxCollider>();
		handFire->GetComponent<BoxCollider>()->SetColliderScale(10.0f, 10.0f, 10.0f);

		_egoHandFire.emplace_back(handFire);

		handFire->SetTotalComponentState(false);
		handFire->SetActive(false);
	}

	// call2 투사체
	_egoCall2 = _boss->GetObjectScene()->CreateObject("EgoCall2");
	_egoCall2->AddComponent<BoxCollider>();
	_egoCall2->GetComponent<BoxCollider>()->SetColliderScale(15.0f, 30.0f, 15.0f);
	_egoCall2->GetComponent<BoxCollider>()->SetActive(false);
	_egoCall2->AddComponent<Light>();
	_egoCall2->GetComponent<Light>()->CreatePointLight(ambient, diffuse, specular, 1.0f);
	_egoCall2->SetTotalComponentState(false);
	_egoCall2->SetActive(false);

	// 레이저 
	_egoLazer = _boss->GetObjectScene()->CreateObject("EgoLazer");
	_egoLazer->AddComponent<Particle>();
	_egoLazer->GetComponent<Particle>()->SetParticleEffect("Laser", "Resources/Textures/Particles/RailGun_64.dds", 1000);
	_egoLazer->GetComponent<Particle>()->SetParticleDuration(1.7f, 2.0f);
	_egoLazer->GetComponent<Particle>()->SetParticleVelocity(84.f, false);
	_egoLazer->GetComponent<Particle>()->SetParticleRotation(90.0f, _bossTransform->GetRotation().y, 0.0f);
	_egoLazer->GetComponent<Particle>()->AddParticleColor(0.4f, 1.0f, 0.0f);
	_egoLazer->GetComponent<Particle>()->SetActive(false);
	_egoLazer->SetTotalComponentState(false);
	_egoLazer->SetActive(false);

	// 레이저 콜라이더
	_egoLazerCollider = _boss->GetObjectScene()->CreateObject("EgoLazerCollider");
	_egoLazerCollider->AddComponent<BoxCollider>();
	_egoLazerCollider->GetComponent<BoxCollider>()->SetColliderScale(140.0f, 10.0f, 10.0f);
	_egoLazerCollider->SetTotalComponentState(false);
	_egoLazerCollider->SetActive(false);

	// 내부 장판
	_egoInsideWarning = _boss->GetObjectScene()->CreateObject("EgoInsideWarning");
	_egoInsideWarning->AddComponent<TransparentMesh>();
	_egoInsideWarning->GetComponent<TransparentMesh>()->CreateTMesh("EgoInsideWarning", "Resources/Textures/Warning/Warning.dds", 0.6f, true);
	_egoInsideWarning->GetComponent<Transform>()->SetPosition(_alterEgo->GetComponent<Transform>()->GetPosition().x, _alterEgo->GetComponent<Transform>()->GetPosition().y, _alterEgo->GetComponent<Transform>()->GetPosition().z);
	_egoInsideWarning->GetComponent<TransparentMesh>()->SetRenderType(3);
	_egoInsideWarning->GetComponent<TransparentMesh>()->SetTimer(2.0f);
	_egoInsideWarning->SetTotalComponentState(false);
	_egoInsideWarning->SetActive(false);

	// 내부 장판 공격
	_egoInsideAttack = _boss->GetObjectScene()->CreateObject("EgoInsideAttack");
	_egoInsideAttack->AddComponent<BoxCollider>();
	_egoInsideAttack->GetComponent<BoxCollider>()->SetActive(false);
	_egoInsideAttack->AddComponent<Particle>();
	_egoInsideAttack->GetComponent<Particle>()->SetParticleEffect("fire", "Resources/Textures/Particles/flare.dds", 1000);
	_egoInsideAttack->GetComponent<Particle>()->SetParticleDuration(1.5f, 2.0f);
	_egoInsideAttack->GetComponent<Particle>()->SetParticleVelocity(80.f, false);
	_egoInsideAttack->GetComponent<Particle>()->AddParticleColor(1.2f, 7.5f, 0.6f);
	_egoInsideAttack->SetTotalComponentState(false);
	_egoInsideAttack->SetActive(false);
}


void KunrealEngine::Kamen::LeftRightPattern()
{
	BossPattern* leftRightPattern = new BossPattern();

	leftRightPattern->SetNextPatternForcePlay(false);

	leftRightPattern->SetMaxColliderCount(0);

	leftRightPattern->SetRange(_info._attackRange);

	leftRightPattern->SetPattern(_leftAttack);

	leftRightPattern->SetPattern(_rightAttack);

	_basicPattern.emplace_back(leftRightPattern);
}


void KunrealEngine::Kamen::RightLeftPattern()
{
	BossPattern* rightLeftPattern = new BossPattern();

	rightLeftPattern->SetNextPatternForcePlay(true);

	rightLeftPattern->SetMaxColliderCount(0);

	rightLeftPattern->SetRange(_info._attackRange);

	rightLeftPattern->SetPattern(_rightAttack);

	rightLeftPattern->SetPattern(_turn180);

	rightLeftPattern->SetPattern(_leftAttack);

	_basicPattern.emplace_back(rightLeftPattern);
}


void KunrealEngine::Kamen::BasicPattern()
{
	_basicPattern.emplace_back(_spellAttack);
	//_basicPattern.emplace_back(_callAttack);
}

void KunrealEngine::Kamen::CreateLeftAttack()
{
	BossPattern* pattern = new BossPattern();

	pattern->SetPatternName("Left_Attack_Once");

	pattern->SetAnimName("Left_Attack").SetDamage(100.0f).SetSpeed(20.0f).SetRange(_info._attackRange).SetAfterDelay(0.5);
	pattern->SetAttackState(BossPattern::eAttackState::ePush).SetMaxColliderCount(1);
	pattern->SetSubObject(_leftHand);
	auto leftHandLogic = CreateBasicAttackLogic(pattern, _leftHand, 10);

	pattern->SetLogic(leftHandLogic);

	_leftAttack = pattern;
}


void KunrealEngine::Kamen::CreateLeftAttackThrowingFire()
{
	BossPattern* pattern = new BossPattern();

	pattern->SetPatternName("Left_Attack_Fire");
	pattern->SetAnimName("Left_Attack").SetDamage(10.0f).SetSpeed(20.0f).SetRange(_info._attackRange + 50.0f).SetAfterDelay(0.5);
	pattern->SetAttackState(BossPattern::eAttackState::eParalysis).SetMaxColliderCount(1);
	pattern->SetWithEgo(true).SetColliderType(BossPattern::eColliderType::eBox);

	for (int i = 0; i < 5; i++)
	{
		_handFireReady.emplace_back(true);

		_handFireDir.emplace_back();

		pattern->SetSubObject(_handFire[i]);

		//분신용
		_egoHandFireReady.emplace_back(true);

		_egoHandFireDir.emplace_back();
	}

	auto initLogic = [pattern, this]()
		{
			for (int i = 0; i < 5; i++)
			{
				for (int i = 0; i < 5; i++)
				{
					pattern->DeleteSubObject(_egoHandFire[i]);
				}

				_handFireReady[i] = true;
			}

			if (_isEgoAttackReady)
			{
				for (int i = 0; i < 5; i++)
				{
					pattern->SetSubObject(_egoHandFire[i]);

					_egoHandFireReady[i] = true;
				}

				_isEgoAttackReady = false;
			}
		};

	pattern->SetInitializeLogic(initLogic);

	auto attackLogic = [pattern, this]()
		{
			auto animator = _boss->GetComponent<Animator>();
			auto isAnimationPlaying = animator->Play(pattern->_animName, pattern->_speed, false);

			auto bossPos = DirectX::XMFLOAT3(_bossTransform->GetPosition().x, 20.0f, _bossTransform->GetPosition().z);
			auto fireSpeed = 0.05f;

			// 분신용
			if (_isEgoAttack)
			{
				auto egoAnimator = _alterEgo->GetComponent<Animator>();
				egoAnimator->Play(pattern->_animName, pattern->_speed, false);
			}

			for (int i = 0; i < 5; i++)
			{
				if (animator->GetCurrentFrame() >= 14 + i)
				{
					if (_handFireReady[i] == true)
					{
						_handFireReady[i] = false;

						auto mat = _leftHandBone->GetComponent<MeshRenderer>()->GetParentBoneOriginalTransform("MiddleFinger1_L");
						auto firePos = DirectX::XMFLOAT3(mat._41, mat._42, mat._43);

						auto direction = ToolBox::GetDirectionVec(bossPos, firePos);

						_handFireDir[i] = DirectX::XMFLOAT3(direction.m128_f32[0], direction.m128_f32[1], direction.m128_f32[2]);

						_handFire[i]->GetComponent<Transform>()->SetPosition(firePos);

						auto childs = _handFire[i]->GetChilds();

						for (auto& index : childs)
						{
							index->GetComponent<Particle>()->SetActive(true);
						}

						// 콜라이더 키기
						auto objectIndex = pattern->GetSubObjectIndex(_handFire[i]);
						pattern->_isColliderActive[objectIndex] = true;
					}
					else
					{
						auto firePos = _handFire[i]->GetComponent<Transform>()->GetPosition();
						DirectX::XMVECTOR newPosition = DirectX::XMVectorAdd(DirectX::XMLoadFloat3(&firePos), DirectX::XMVectorScale(DirectX::XMLoadFloat3(&_handFireDir[i]), pattern->_speed * fireSpeed));

						_handFire[i]->GetComponent<Transform>()->SetPosition(newPosition.m128_f32[0], newPosition.m128_f32[1], newPosition.m128_f32[2]);
					}

					// 분신용
					if (_isEgoAttack)
					{
						if (_egoHandFireReady[i] == true)
						{
							_egoHandFireReady[i] = false;

							auto egoPosTransform = _alterEgo->GetComponent<Transform>()->GetPosition();
							auto egoPos = DirectX::XMFLOAT3(egoPosTransform.x, 20.0f, egoPosTransform.z);

							auto mat = _egoLeftHandBone->GetComponent<MeshRenderer>()->GetParentBoneOriginalTransform("MiddleFinger1_L");
							auto firePos = DirectX::XMFLOAT3(mat._41, mat._42, mat._43);

							auto direction = ToolBox::GetDirectionVec(egoPos, firePos);

							_egoHandFireDir[i] = DirectX::XMFLOAT3(direction.m128_f32[0], direction.m128_f32[1], direction.m128_f32[2]);

							_egoHandFire[i]->GetComponent<Transform>()->SetPosition(firePos);

							auto childs = _egoHandFire[i]->GetChilds();

							for (auto& index : childs)
							{
								index->GetComponent<Particle>()->SetActive(true);
							}

							// 콜라이더 키기
							auto objectIndex = pattern->GetSubObjectIndex(_egoHandFire[i]);
							pattern->_isColliderActive[objectIndex] = true;
						}
						else
						{
							auto firePos = _egoHandFire[i]->GetComponent<Transform>()->GetPosition();
							DirectX::XMVECTOR newPosition = DirectX::XMVectorAdd(DirectX::XMLoadFloat3(&firePos), DirectX::XMVectorScale(DirectX::XMLoadFloat3(&_egoHandFireDir[i]), pattern->_speed * fireSpeed));

							_egoHandFire[i]->GetComponent<Transform>()->SetPosition(newPosition.m128_f32[0], newPosition.m128_f32[1], newPosition.m128_f32[2]);
						}
					}
				}
			}

			// 일정 프레임이 넘어가면 데미지 체크용 콜라이더를 키기

			if (pattern->_colliderOnCount > 0)
			{
				if (animator->GetCurrentFrame() >= 10)
				{

				}
			}

			if (isAnimationPlaying == false)
			{
				return false;
			}

			return true;
		};

	pattern->SetLogic(attackLogic);

	_leftFireAttack = pattern;
}

void KunrealEngine::Kamen::CreateRightAttack()
{
	BossPattern* pattern = new BossPattern();

	pattern->SetPatternName("Right_Attack_Once");

	pattern->SetAnimName("Right_Attack").SetDamage(100.0f).SetSpeed(20.0f).SetRange(_info._attackRange).SetAfterDelay(0.5f);
	pattern->SetAttackState(BossPattern::eAttackState::ePush).SetMaxColliderCount(1);
	pattern->SetSubObject(_rightHand);

	auto rightHandLogic = CreateBasicAttackLogic(pattern, _rightHand, 10);

	pattern->SetLogic(rightHandLogic);

	_rightAttack = pattern;
}


void KunrealEngine::Kamen::CreateRightAttackThrowingFire()
{
	BossPattern* pattern = new BossPattern();

	pattern->SetPatternName("Right_Attack_Fire");
	pattern->SetAnimName("Right_Attack").SetDamage(10.0f).SetSpeed(20.0f).SetRange(_info._attackRange + 50.0f).SetAfterDelay(0.5);
	pattern->SetAttackState(BossPattern::eAttackState::eParalysis).SetMaxColliderCount(1);
	pattern->SetWithEgo(true).SetColliderType(BossPattern::eColliderType::eBox);

	for (int i = 0; i < 5; i++)
	{
		_handFireReady.emplace_back(true);

		_handFireDir.emplace_back();

		pattern->SetSubObject(_handFire[i]);

		//분신용
		_egoHandFireReady.emplace_back(true);

		_egoHandFireDir.emplace_back();
	}

	auto initLogic = [pattern, this]()
		{
			for (int i = 0; i < 5; i++)
			{
				_handFireReady[i] = true;

				auto childs = _handFire[i]->GetChilds();

				for (auto& index : childs)
				{
					index->GetComponent<Particle>()->SetActive(false);
				}

				pattern->DeleteSubObject(_egoHandFire[i]);
			}

			if (_isEgoAttackReady)
			{
				for (int i = 0; i < 5; i++)
				{
					_egoHandFireReady[i] = true;

					auto childs = _egoHandFire[i]->GetChilds();

					for (auto& index : childs)
					{
						index->GetComponent<Particle>()->SetActive(false);
					}

					pattern->SetSubObject(_egoHandFire[i]);
				}

				_isEgoAttackReady = false;
			}
		};

	pattern->SetInitializeLogic(initLogic);

	auto attackLogic = [pattern, this]()
		{
			auto animator = _boss->GetComponent<Animator>();
			auto isAnimationPlaying = animator->Play(pattern->_animName, pattern->_speed, false);

			auto bossPos = DirectX::XMFLOAT3(_bossTransform->GetPosition().x, 20.0f, _bossTransform->GetPosition().z);
			auto fireSpeed = 0.05f;

			// 분신용
			if (_isEgoAttack)
			{
				auto egoAnimator = _alterEgo->GetComponent<Animator>();
				egoAnimator->Play(pattern->_animName, pattern->_speed, false);
			}

			for (int i = 0; i < 5; i++)
			{
				int index = i;
				if (i == 3)
				{
					index++;
				}
				else if (i == 4)
				{
					index += 2;
				}
				if (animator->GetCurrentFrame() >= 24 + index)
				{
					if (_handFireReady[i] == true)
					{
						_handFireReady[i] = false;

						auto mat = _rightHandBone->GetComponent<MeshRenderer>()->GetParentBoneOriginalTransform("MiddleFinger1_R");
						auto firePos = DirectX::XMFLOAT3(mat._41, mat._42, mat._43);

						auto direction = ToolBox::GetDirectionVec(bossPos, firePos);

						_handFireDir[i] = DirectX::XMFLOAT3(direction.m128_f32[0], direction.m128_f32[1], direction.m128_f32[2]);

						_handFire[i]->GetComponent<Transform>()->SetPosition(firePos);

						auto childs = _handFire[i]->GetChilds();

						for (auto& index : childs)
						{
							index->GetComponent<Particle>()->SetActive(true);
						}

						// 콜라이더 키기
						auto objectIndex = pattern->GetSubObjectIndex(_handFire[i]);
						pattern->_isColliderActive[objectIndex] = true;
					}
					else
					{
						auto firePos = _handFire[i]->GetComponent<Transform>()->GetPosition();
						DirectX::XMVECTOR newPosition = DirectX::XMVectorAdd(DirectX::XMLoadFloat3(&firePos), DirectX::XMVectorScale(DirectX::XMLoadFloat3(&_handFireDir[i]), pattern->_speed * fireSpeed));

						_handFire[i]->GetComponent<Transform>()->SetPosition(newPosition.m128_f32[0], newPosition.m128_f32[1], newPosition.m128_f32[2]);
					}

					// 분신용
					if (_isEgoAttack)
					{
						if (_egoHandFireReady[i] == true)
						{
							_egoHandFireReady[i] = false;

							auto egoPosTransform = _alterEgo->GetComponent<Transform>()->GetPosition();
							auto egoPos = DirectX::XMFLOAT3(egoPosTransform.x, 20.0f, egoPosTransform.z);

							auto mat = _egoRightHandBone->GetComponent<MeshRenderer>()->GetParentBoneOriginalTransform("MiddleFinger1_R");
							auto firePos = DirectX::XMFLOAT3(mat._41, mat._42, mat._43);

							auto direction = ToolBox::GetDirectionVec(egoPos, firePos);

							_egoHandFireDir[i] = DirectX::XMFLOAT3(direction.m128_f32[0], direction.m128_f32[1], direction.m128_f32[2]);

							_egoHandFire[i]->GetComponent<Transform>()->SetPosition(firePos);

							auto childs = _egoHandFire[i]->GetChilds();

							for (auto& index : childs)
							{
								index->GetComponent<Particle>()->SetActive(true);
							}

							// 콜라이더 키기
							auto objectIndex = pattern->GetSubObjectIndex(_egoHandFire[i]);
							pattern->_isColliderActive[objectIndex] = true;
						}
						else
						{
							auto firePos = _egoHandFire[i]->GetComponent<Transform>()->GetPosition();
							DirectX::XMVECTOR newPosition = DirectX::XMVectorAdd(DirectX::XMLoadFloat3(&firePos), DirectX::XMVectorScale(DirectX::XMLoadFloat3(&_egoHandFireDir[i]), pattern->_speed * fireSpeed));

							_egoHandFire[i]->GetComponent<Transform>()->SetPosition(newPosition.m128_f32[0], newPosition.m128_f32[1], newPosition.m128_f32[2]);
						}
					}
				}
			}


			// 일정 프레임이 넘어가면 데미지 체크용 콜라이더를 키기

			if (pattern->_colliderOnCount > 0)
			{
				if (animator->GetCurrentFrame() >= 10)
				{

				}
			}

			if (isAnimationPlaying == false)
			{
				return false;
			}

			return true;
		};

	pattern->SetLogic(attackLogic);

	_rightFireAttack = pattern;
}

void KunrealEngine::Kamen::CreateTurn180()
{
	BossPattern* pattern = new BossPattern();

	pattern->SetPatternName("Turn180");

	pattern->SetAnimName("Idle").SetMaxColliderCount(0).SetSpeed(200.0f);

	auto turn180 = [pattern, this]()
		{
			auto animator = _boss->GetComponent<Animator>();

			animator->Play("Idle", pattern->_speed, false);

			// 회전 시킴
			auto rotateFinish = Rotate(180, pattern->_speed);

			// 회전 끝나지 않았다면
			if (rotateFinish == false)
			{
				// 계속 회전시키기 위함
				return true;
			}
			else
			{
				return false;
			}
		};


	pattern->SetLogic(turn180);

	_turn180 = pattern;
}

void KunrealEngine::Kamen::CreateBackStep()
{
	BossPattern* pattern = new BossPattern();

	pattern->SetPatternName("BackStep");

	pattern->SetAnimName("Run").SetSpeed(50.0f).SetRange(30.0f).SetMaxColliderCount(0);

	// 로직 함수 실행 가능하도록 넣어주기
	pattern->SetLogic(CreateBackStepLogic(pattern, pattern->_speed, pattern->_range));

	_backStep = pattern;
}

void KunrealEngine::Kamen::CreateTeleportToCenter()
{
	BossPattern* pattern = new BossPattern();

	pattern->SetPatternName("Teleport");

	pattern->SetAnimName("Idle").SetRange(0.0f).SetMaxColliderCount(0);

	auto teleport = [pattern, this]()
		{
			auto isTeleportFinish = Teleport(DirectX::XMFLOAT3(_centerPos.x, _centerPos.y, _centerPos.z), true, 1.0f);

			if (isTeleportFinish)
			{
				RotateByAngle(0.0f);

				return false;
			}

			return true;
		};

	// 로직 함수 실행 가능하도록 넣어주기
	pattern->SetLogic(teleport);

	_teleport = pattern;
}


void KunrealEngine::Kamen::CreateTeleportToCenterWithLook()
{
	BossPattern* pattern = new BossPattern();

	pattern->SetPatternName("Teleport");

	pattern->SetAnimName("Idle").SetRange(0.0f).SetMaxColliderCount(0);

	auto teleport = [pattern, this]()
		{
			auto isTeleportFinish = Teleport(DirectX::XMFLOAT3(_centerPos.x, _centerPos.y, _centerPos.z), true, 1.0f);

			if (isTeleportFinish)
			{
				return false;
			}

			return true;
		};

	// 로직 함수 실행 가능하도록 넣어주기
	pattern->SetLogic(teleport);

	_teleportWithLook = pattern;
}

void KunrealEngine::Kamen::CreateTurnClockWise()
{
	BossPattern* pattern = new BossPattern();

	pattern->SetPatternName("TurnClockWise");

	pattern->SetAnimName("Idle").SetMaxColliderCount(0).SetSpeed(50.0f);

	auto initializeLogic = [pattern, this]()
		{
			_boss->GetComponent<BoxCollider>()->SetActive(true);
			_boss->GetComponent<MeshRenderer>()->SetActive(true);
		};

	pattern->SetInitializeLogic(initializeLogic);

	auto turn = [pattern, this]()
		{
			auto animator = _boss->GetComponent<Animator>();

			// 회전 진행 안됐다면
			if (_isRotateFinish == false)
			{
				animator->Play("Idle", pattern->_speed, true);

				// 회전 시킴
				auto rotateFinish = RotateClockWise(4 * pattern->_speed, false);

				// 회전 끝나지 않았다면
				if (rotateFinish == false)
				{
					// 계속 회전시키기 위함
					return true;
				}
				else
				{
					_isRotateFinish = false;
					return false;
				}
			}
		};

	pattern->SetLogic(turn);

	_turnClockWise = pattern;
}


void KunrealEngine::Kamen::CreateTurnAntiClockWise()
{
	BossPattern* pattern = new BossPattern();

	pattern->SetPatternName("TurnAntiClockWise");

	pattern->SetAnimName("Idle").SetMaxColliderCount(0).SetSpeed(50.0f);

	auto initializeLogic = [pattern, this]()
		{
			_boss->GetComponent<BoxCollider>()->SetActive(true);
			_boss->GetComponent<MeshRenderer>()->SetActive(true);
		};

	pattern->SetInitializeLogic(initializeLogic);

	auto turn = [pattern, this]()
		{
			auto animator = _boss->GetComponent<Animator>();

			// 회전 진행 안됐다면
			if (_isRotateFinish == false)
			{
				animator->Play("Idle", pattern->_speed, true);

				// 회전 시킴
				auto rotateFinish = RotateClockWise(4 * pattern->_speed, true);

				// 회전 끝나지 않았다면
				if (rotateFinish == false)
				{
					// 계속 회전시키기 위함
					return true;
				}
				else
				{
					_isRotateFinish = false;
					return false;
				}
			}
		};

	pattern->SetLogic(turn);

	_turnAntiClockWise = pattern;
}


void KunrealEngine::Kamen::CreateReverseEmergence()
{
	BossPattern* pattern = new BossPattern();

	pattern->SetPatternName("ReverseEmergence");

	pattern->SetAnimName("ReverseEmergence").SetMaxColliderCount(0).SetSpeed(20.0f);

	auto initializeLogic = [pattern, this]()
		{
			_boss->GetComponent<BoxCollider>()->SetActive(true);
			_boss->GetComponent<MeshRenderer>()->SetActive(true);
		};

	pattern->SetInitializeLogic(initializeLogic);

	auto reverseEmergence = [pattern, this]()
		{
			auto animator = _boss->GetComponent<Animator>();

			auto isPlaying = animator->Play(pattern->_animName, pattern->_speed);

			if (isPlaying)
			{
				return true;
			}
			else
			{
				_boss->GetComponent<BoxCollider>()->SetActive(false);
				_boss->GetComponent<MeshRenderer>()->SetActive(false);

				return false;
			}
		};

	pattern->SetLogic(reverseEmergence);

	_reverseEmergence = pattern;
}


void KunrealEngine::Kamen::CreateEmergence()
{
	BossPattern* pattern = new BossPattern();

	pattern->SetPatternName("Emergence");

	pattern->SetAnimName("Emergence").SetMaxColliderCount(1).SetSpeed(20.0f).SetDamage(10.0f).SetAttackState(BossPattern::eAttackState::ePush);
	pattern->SetColliderType(BossPattern::eColliderType::eCylinder);
	pattern->SetSubObject(_bossInsideAttack);
	pattern->SetSubObject(_egoInsideAttack);

	auto initializeLogic = [pattern, this]()
		{
			_boss->GetComponent<BoxCollider>()->SetActive(true);
			_boss->GetComponent<MeshRenderer>()->SetActive(true);

			_boss->GetComponent<Transform>()->SetPosition(_emergencePos);

			auto angle = CalculateAngle(_bossTransform->GetPosition(), _playerTransform->GetPosition());
			_bossTransform->SetRotation(_bossTransform->GetRotation().x, angle, _bossTransform->GetRotation().z);

			_bossInsideAttack->GetComponent<Transform>()->SetPosition(_emergencePos.x, _bossTransform->GetPosition().y + 1.0f, _emergencePos.z);

			_timer = 0.0f;
		};

	pattern->SetInitializeLogic(initializeLogic);

	auto emergence = [pattern, this]()
		{
			auto animator = _boss->GetComponent<Animator>();

			auto isPlaying = animator->Play(pattern->_animName, pattern->_speed);

			if (_isEgoAttack)
			{
				_egoInsideAttack->GetComponent<Particle>()->SetActive(true);

				auto particleScaleUp = (_circleWarningSize) / 100.0f;
				particleScaleUp += 1.0f;

				_egoInsideAttack->GetComponent<Particle>()->SetParticleSize(100.f * particleScaleUp * ToolBox::GetRandomFloat(0.3f, 1.0f), 40.0f * particleScaleUp * ToolBox::GetRandomFloat(0.1f, 1.0f));
			}

			if (isPlaying)
			{
				_timer += TimeManager::GetInstance().GetDeltaTime();
				for (int i = 0; i < _particleEmergenceAttack.size(); i++)
				{
					if (_timer < 1.3f)
					{
						_particleEmergenceAttack[i]->GetComponent<Particle>()->SetActive(true);

						if (i == 1 || i == 2)
						{
							_particleEmergenceAttack[i]->GetComponent<Particle>()->SetParticleSize(20 + 100 * _timer * 1.2f, 20 + 100 * _timer * 1.2f);
						}
						else
						{
							_particleEmergenceAttack[i]->GetComponent<Particle>()->SetParticleSize(10 + 100 * _timer, 10 + 100 * _timer);
						}

					}
					else
					{
						if (i == 0 || i == 3)
						{
							_particleEmergenceAttack[i]->GetComponent<Particle>()->SetActive(false);						
						}
					}
				}
					return true;
				//_bossInsideAttack->GetComponent<Particle>()->SetActive(true);
				//
				//auto particleScaleUp = (_circleWarningSize) / 100.0f;
				//particleScaleUp += 1.0f;
				//
				//_bossInsideAttack->GetComponent<Particle>()->SetParticleSize(100.f * particleScaleUp * ToolBox::GetRandomFloat(0.3f, 1.0f), 40.0f * particleScaleUp * ToolBox::GetRandomFloat(0.1f, 1.0f));
				//
				//return true;
			}
			else
			{
				_timer = 0.0f;
				return false;
			}
		};

	pattern->SetLogic(emergence);

	_emergence = pattern;
}

void KunrealEngine::Kamen::CreateOutsideSafe()
{
	BossPattern* pattern = new BossPattern();

	pattern->SetPatternName("OutSideSafe");

	pattern->SetAnimName("Idle").SetRange(0.0f).SetMaxColliderCount(1).SetSpeed(20.0f).SetDamage(1.0f);
	pattern->SetAttackState(BossPattern::eAttackState::ePush);
	pattern->SetColliderType(BossPattern::eColliderType::eCylinder);

	// 패턴 시작전에 초기화, 장판 켜줌
	auto initializeLogic = [pattern, this]()
		{
			pattern->SetSubObject(_swordInsideWarning);
			pattern->SetSubObject(_swordInsideAttack);

			_swordInsideWarning->GetComponent<TransparentMesh>()->Reset();
			_swordInsideWarning->GetComponent<TransparentMesh>()->SetActive(true);
			_swordInsideWarning->GetComponent<Transform>()->SetPosition(_swordOriginPos.x, _bossTransform->GetPosition().y + 1.0f, _swordOriginPos.z);
			_swordInsideAttack->GetComponent<Transform>()->SetPosition(_swordOriginPos.x, _bossTransform->GetPosition().y + 1.0f, _swordOriginPos.z);
			
			for (auto& pillarParticle : _swordInsideAttack->GetChilds())
			{
				pillarParticle->GetComponent<Transform>()->SetPosition(_centerPos.x - 5, _centerPos.y, _centerPos.z + 34.0f);
			}

			_timer = 0.0f;

			_swordInsideWarning->GetComponent<Transform>()->SetScale(_circleWarningSize, _circleWarningSize, _circleWarningSize);
			_swordInsideAttack->GetComponent<BoxCollider>()->SetColliderScale(_circleWarningSize, _circleWarningSize, _circleWarningSize);
		};

	pattern->SetInitializeLogic(initializeLogic);

	auto outsideSafe = [pattern, this]()
		{
			// 장판 실행
			auto isPlayed = _swordInsideWarning->GetComponent<TransparentMesh>()->PlayOnce();

			// 장판 실행이 완료되면
			if (isPlayed)
			{
				// 콜라이더 키기
				auto objectIndex = pattern->GetSubObjectIndex(_swordInsideAttack);
				pattern->_isColliderActive[objectIndex] = true;

				// n초동안 콜라이더 실행
				_timer += TimeManager::GetInstance().GetDeltaTime();
				//_swordInsideAttack->GetComponent<Particle>()->SetActive(true);

				auto particleScaleUp = (_circleWarningSize) / 100.0f;
				particleScaleUp += 1.0f;

				//_swordInsideAttack->GetComponent<Particle>()->SetParticleSize(100.f * particleScaleUp * ToolBox::GetRandomFloat(0.3f, 1.0f), 40.0f * particleScaleUp * ToolBox::GetRandomFloat(0.1f, 1.0f));

				for (auto& pillarParticle : _swordInsideAttack->GetChilds())
				{
					pillarParticle->GetComponent<Particle>()->SetActive(true);
				}

				if (_timer >= 2.0f)
				{
					return false;
				}
			}

			return true;
		};

	// 로직 함수 실행 가능하도록 넣어주기
	pattern->SetLogic(outsideSafe);

	_outsideSafe = pattern;
}


void KunrealEngine::Kamen::CreateInsideSafe()
{
	BossPattern* pattern = new BossPattern();

	pattern->SetPatternName("InSideSafe");

	pattern->SetAnimName("Idle").SetRange(0.0f).SetMaxColliderCount(1).SetSpeed(20.0f).SetDamage(3.0f);
	pattern->SetAttackState(BossPattern::eAttackState::ePush);
	pattern->SetColliderType(BossPattern::eColliderType::eCircleSafe);

	// 패턴 시작전에 초기화, 장판 켜줌
	auto initializeLogic = [pattern, this]()
		{
			pattern->SetSubObject(_swordOutsideWarning);
			pattern->SetSubObject(_swordInsideAttack);

			_swordOutsideWarning->GetComponent<TransparentMesh>()->SetExceptRange(_swordOriginPos, _circleWarningSize);
			_swordOutsideWarning->GetComponent<TransparentMesh>()->Reset();
			_swordOutsideWarning->GetComponent<TransparentMesh>()->SetActive(true);
			_swordOutsideWarning->GetComponent<Transform>()->SetPosition(_centerPos.x, _centerPos.y, _centerPos.z);
			_swordOutsideWarning->GetComponent<Transform>()->SetScale(100.0f, 100.0f, 100.0f);
			_swordInsideAttack->GetComponent<Transform>()->SetPosition(_swordOriginPos.x, _bossTransform->GetPosition().y + 1.0f, _swordOriginPos.z);

			_timer = 0.0f;

			//for (auto& object : _particleSwordOutsideAttack)
			//{
			//	object->GetComponent<Particle>()->SetParticleSize(60.0f, 60.0f);
			//	//object->GetComponent<Particle>()->SetActive(true);
			//}

			_particleSwordOutsideAttack[0]->GetComponent<Particle>()->SetParticleSize(60.0f, 60.0f);
			_particleSwordOutsideAttack[0]->GetComponent<Particle>()->SetActive(true);
		};

	pattern->SetInitializeLogic(initializeLogic);

	auto outsideSafe = [pattern, this]()
		{
			// 장판 실행
			auto isPlayed = _swordOutsideWarning->GetComponent<TransparentMesh>()->PlayOnce();

			// 장판 실행이 완료되면
			if (isPlayed)
			{
				// 콜라이더 키기
				auto objectIndex = pattern->GetSubObjectIndex(_swordInsideAttack);
				pattern->_isColliderActive[objectIndex] = true;

				_timer += TimeManager::GetInstance().GetDeltaTime();

				const float delays[] = { 0.1f, 0.15f, 0.2f, 0.25f, 0.3f, 0.35f, 0.4f, 0.45f, 0.5f, 0.55f };

				for (int i = 0; i < 10; ++i) {
					if (_timer > delays[i]) 
					{
						auto particle = _particleSwordOutsideAttack[i + 1]->GetComponent<Particle>();
						particle->SetActive(true);
						float adjustedTime = _timer - delays[i];
						float newSize = 60.0f + adjustedTime * 600.0f;
						particle->SetParticleSize(newSize, newSize);
					}
				}


				//for (int i = 1; i < _particleSwordOutsideAttack.size(); i++)
				//{
				//	_particleSwordOutsideAttack[i]->GetComponent<Particle>()->SetParticleSize(60 + _timer * 300 *i, 60 + _timer * 300*i);
				//}

				if (_timer >= 3.0f)
				{
					return false;
				}
			}

			return true;
		};

	// 로직 함수 실행 가능하도록 넣어주기
	pattern->SetLogic(outsideSafe);

	_insideSafe = pattern;
}


void KunrealEngine::Kamen::CreateDonutSafe()
{
	BossPattern* pattern = new BossPattern();

	pattern->SetPatternName("DonutSafe");

	pattern->SetAnimName("Idle").SetRange(0.0f).SetMaxColliderCount(1).SetSpeed(20.0f);
	pattern->SetAttackState(BossPattern::eAttackState::ePush);

	auto initializeLogic = [pattern, this]()
		{
			pattern->SetSubObject(_swordDonutWarning1);
			pattern->SetSubObject(_swordDonutWarning2);
			pattern->SetSubObject(_swordDonutWarning3);

			pattern->SetSubObject(_freeSword);

			auto swordPos = _freeSword->GetComponent<Transform>()->GetPosition();

			_swordDonutWarning1->GetComponent<TransparentMesh>()->Reset();
			_swordDonutWarning1->GetComponent<TransparentMesh>()->SetActive(true);
			_swordDonutWarning1->GetComponent<Transform>()->SetPosition(swordPos.x, _centerPos.y, swordPos.z);

			_swordDonutWarning2->GetComponent<TransparentMesh>()->Reset();
			_swordDonutWarning2->GetComponent<TransparentMesh>()->SetActive(true);
			_swordDonutWarning2->GetComponent<Transform>()->SetPosition(swordPos.x, _centerPos.y, swordPos.z);
			_swordDonutWarning2->GetComponent<TransparentMesh>()->SetExceptRange(swordPos, _circleWarningSize);

			_swordDonutWarning3->GetComponent<TransparentMesh>()->Reset();
			_swordDonutWarning3->GetComponent<TransparentMesh>()->SetActive(true);
			_swordDonutWarning3->GetComponent<Transform>()->SetPosition(swordPos.x, _centerPos.y, swordPos.z);
			_swordDonutWarning3->GetComponent<TransparentMesh>()->SetExceptRange(swordPos, _circleWarningSize * 2);

			_timer = 0.0f;
		};

	pattern->SetInitializeLogic(initializeLogic);

	auto donutSafe = [pattern, this]()
		{
			_timer += TimeManager::GetInstance().GetDeltaTime();

			auto warningInterval = 0.3f;
			auto playSecondTime = warningInterval;
			auto playThirdTime = playSecondTime + warningInterval;

			auto particleInterval = 0.5f;

			// 첫번째 장판 실행
			auto isPlayed1 = _swordDonutWarning1->GetComponent<TransparentMesh>()->PlayOnce();

			// 첫번째 장판 실행 후 0.5초 후에 
			if (_timer >= particleInterval)
			{
				// 첫번째 장판 위치에 파티클 실행
				_swordDonutWarning1->GetComponent<Particle>()->SetActive(true);
				GameObject* swordDonutChild1 = _swordDonutWarning1->GetChilds()[0];
				swordDonutChild1->GetComponent<Particle>()->SetActive(true);
			
				// 첫번째 장판 실행이 끝난 후
				if (isPlayed1)
				{
					_swordDonutWarning1->GetComponent<Particle>()->SetActive(false);
					swordDonutChild1->GetComponent<Particle>()->SetActive(false);
				}
			}

			if (_timer >= playSecondTime)
			{
				// 두번째 장판 실행
				auto isPlayed2 = _swordDonutWarning2->GetComponent<TransparentMesh>()->PlayOnce();

				// 두번째 장판 실행 후 0.5초 후에
				if (_timer > playSecondTime + particleInterval)
				{
					// 두번째 장판 위치에 파티클 실행
					_swordDonutWarning2->GetComponent<Particle>()->SetActive(true);
					GameObject* swordDonutChild2 = _swordDonutWarning2->GetChilds()[0];
					swordDonutChild2->GetComponent<Particle>()->SetActive(true);
				
					// 두번째 장판 실행이 끝난 후
					if (isPlayed2)
					{
						_swordDonutWarning2->GetComponent<Particle>()->SetActive(false);
						swordDonutChild2->GetComponent<Particle>()->SetActive(false);
					}
				}
			}

			if (_timer >= playThirdTime)
			{
				auto isPlayed3 = _swordDonutWarning3->GetComponent<TransparentMesh>()->PlayOnce();

				if (_timer >= 1.3)
				{
					_swordDonutWarning3->GetComponent<Particle>()->SetActive(true);
					GameObject* swordDonutChild3 = _swordDonutWarning3->GetChilds()[0];
					swordDonutChild3->GetComponent<Particle>()->SetActive(true);
				
					// 장판 실행이 완료되면
					if (isPlayed3 && _timer > 2.0f)
					{
						{
							_swordDonutWarning3->GetComponent<Particle>()->SetActive(false);
							swordDonutChild3->GetComponent<Particle>()->SetActive(false);
				
							pattern->DeleteSubObject(_freeSword);
							return false;
						}
					}
				}
			}
			return true;
		};

	// 로직 함수 실행 가능하도록 넣어주기
	pattern->SetLogic(donutSafe);

	_donutSafe = pattern;
}

void KunrealEngine::Kamen::CreateSpellAttack()
{
	BossPattern* pattern = new BossPattern();

	pattern->SetPatternName("Spell");

	pattern->SetAnimName("Spell").SetDamage(10.0f).SetSpeed(20.0f).SetRange(_info._attackRange + 80.0f).SetAfterDelay(2.0f);
	pattern->SetMaxColliderCount(1).SetAttackState(BossPattern::eAttackState::eParalysis).SetColliderType(BossPattern::eColliderType::eBox);
	pattern->SetSubObject(_lazer);
	pattern->SetSubObject(_lazerCollider);
	pattern->SetWithEgo(true);

	auto initLogic = [pattern, this]()
		{
			_boss->GetComponent<BoxCollider>()->SetActive(true);
			_boss->GetComponent<MeshRenderer>()->SetActive(true);

			auto lazerPosOffset = 15.0f;
			auto lazerScaleOffset = 80.0f;

			auto direction = GetDirection();

			auto lazerScaleDir = DirectX::XMVectorScale(direction, lazerPosOffset);
			_lazer->GetComponent<Transform>()->SetPosition(_bossTransform->GetPosition().x + lazerScaleDir.m128_f32[0], 16.0f, _bossTransform->GetPosition().z + lazerScaleDir.m128_f32[2]);
			_lazer->GetComponent<Particle>()->SetParticleRotation(90.0f, _bossTransform->GetRotation().y, 0.0f);
			_lazer->GetChilds()[0]->GetComponent<Transform>()->SetPosition(_bossTransform->GetPosition().x + lazerScaleDir.m128_f32[0], 16.0f, _bossTransform->GetPosition().z + lazerScaleDir.m128_f32[2]);
			_lazer->GetChilds()[0]->GetComponent<Particle>()->SetParticleRotation(90.0f, _bossTransform->GetRotation().y, 0.0f);

			pattern->SetSpeed(20.0f);

			auto lazerColliderScaleDir = DirectX::XMVectorScale(direction, lazerScaleOffset);

			_lazerCollider->GetComponent<Transform>()->SetPosition(_bossTransform->GetPosition().x + lazerColliderScaleDir.m128_f32[0], 16.0f, _bossTransform->GetPosition().z + lazerColliderScaleDir.m128_f32[2]);

			_lazerCollider->GetComponent<Transform>()->SetRotation(_bossTransform->GetRotation().x, _bossTransform->GetRotation().y + 90.0f, _bossTransform->GetRotation().z);

			if (_isEgoAttackReady)
			{
				auto egoTransform = _alterEgo->GetComponent<Transform>();
				auto egoPos = egoTransform->GetPosition();
				auto egoLazerDirection = GetEgoDirection();
				auto egoLazerScaleDir = DirectX::XMVectorScale(egoLazerDirection, lazerPosOffset);

				_egoLazer->GetComponent<Transform>()->SetPosition(egoPos.x + egoLazerScaleDir.m128_f32[0], 16.0f, egoPos.z + egoLazerScaleDir.m128_f32[2]);
				_egoLazer->GetComponent<Particle>()->SetParticleRotation(90.0f, egoTransform->GetRotation().y, 0.0f);

				auto egolazerColliderScaleDir = DirectX::XMVectorScale(egoLazerDirection, lazerScaleOffset);

				_egoLazerCollider->GetComponent<Transform>()->SetPosition(_bossTransform->GetPosition().x + egolazerColliderScaleDir.m128_f32[0], 16.0f, _bossTransform->GetPosition().z + egolazerColliderScaleDir.m128_f32[2]);

				_egoLazerCollider->GetComponent<Transform>()->SetRotation(_bossTransform->GetRotation().x, _bossTransform->GetRotation().y + 90.0f, _bossTransform->GetRotation().z);

				_isEgoAttackReady = false;

				pattern->SetSubObject(_egoLazer);
				pattern->SetSubObject(_egoLazerCollider);
			}
			else
			{
				pattern->DeleteSubObject(_egoLazer);
				pattern->DeleteSubObject(_egoLazerCollider);
			}
		};

	pattern->SetInitializeLogic(initLogic);
	auto spellLogic = [pattern, this]()
		{
			auto animator = _boss->GetComponent<Animator>();
			auto isAnimationPlaying = animator->Play(pattern->_animName, pattern->_speed, false);

			// 분신
			if (_isEgoAttack)
			{
				auto egoAnimator = _alterEgo->GetComponent<Animator>();
				egoAnimator->Play(pattern->_animName, pattern->_speed, false);
			}

			// 일정 프레임이 넘어가면 데미지 체크용 콜라이더를 키기

			if (animator->GetCurrentFrame() >= 30.0f)
			{
				if (animator->GetCurrentFrame() >= 55.0f)
				{
					_lazer->GetComponent<Particle>()->SetActive(false);
					_lazer->GetChilds()[0]->GetComponent<Particle>()->SetActive(false);

					if (pattern->_colliderOnCount > 0)
					{
						_lazerCollider->GetComponent<BoxCollider>()->SetActive(false);
					}

					if (_isEgoAttack)
					{
						_egoLazer->GetComponent<Particle>()->SetActive(false);
						//if (pattern->_colliderOnCount > 0)
						//{
						//	_egoLazerCollider->GetComponent<BoxCollider>()->SetActive(false);
						//}
					}

				}
				else
				{
					pattern->SetSpeed(10.0f);
					_lazer->GetComponent<Particle>()->SetActive(true);
					_lazer->GetComponent<Particle>()->SetParticleSize(40.f,40.f);
					
					_lazer->GetChilds()[0]->GetComponent<Particle>()->SetActive(true);

					if (pattern->_colliderOnCount > 0)
					{
						_lazerCollider->GetComponent<BoxCollider>()->SetActive(true);
					}

					if (_isEgoAttack)
					{
						_egoLazer->GetComponent<Particle>()->SetActive(true);
						_egoLazer->GetComponent<Particle>()->SetParticleSize(40.f * ToolBox::GetRandomFloat(0.3f, 1.0f), 30.0f * ToolBox::GetRandomFloat(0.1f, 1.0f));
						//if (pattern->_colliderOnCount > 0)
						//{
						//	_egoLazerCollider->GetComponent<BoxCollider>()->SetActive(true);
						//}
					}
				}
			}

			if (isAnimationPlaying == false)
			{
				return false;
			}

			return true;
		};


	pattern->SetLogic(spellLogic);

	_spellAttack = pattern;
}


void KunrealEngine::Kamen::CreateCallAttack()
{
	BossPattern* pattern = new BossPattern();

	pattern->SetPatternName("Call");

	pattern->SetAnimName("Call").SetDamage(100.0f).SetSpeed(20.0f).SetRange(_info._attackRange + 50.0f).SetAfterDelay(1.0f);
	pattern->SetMaxColliderCount(1).SetAttackState(BossPattern::eAttackState::ePush);
	pattern->SetSubObject(_call);

	auto callLogic = [pattern, this]()
		{
			auto animator = _boss->GetComponent<Animator>();
			auto isAnimationPlaying = animator->Play(pattern->_animName, pattern->_speed, false);

			// 일정 프레임이 넘어가면 데미지 체크용 콜라이더를 키기
			if (animator->GetCurrentFrame() >= 20)
			{
				if (pattern->_colliderOnCount > 0)
				{
					// 콜라이더 키기
					_call->GetComponent<BoxCollider>()->SetActive(true);
					// 파티클 키기
					_call->GetComponent<Particle>()->SetActive(true);
					// 라이트 키기
					_call->GetComponent<Light>()->SetActive(true);
				}

				// 보스가 바라보는 방향 가져옴
				auto direction = GetDirection();

				// 현재 보스의 포지션
				auto nowPos = _boss->GetComponent<Transform>()->GetPosition();

				auto nowPosVec = DirectX::XMLoadFloat3(&nowPos);

				auto callNowPos = _call->GetComponent<Transform>()->GetPosition();
				auto distance = ToolBox::GetDistance(nowPos, callNowPos);

				if (distance < pattern->_range)
				{
					_callMoveDistance += (pattern->_speed * 1.5) * TimeManager::GetInstance().GetDeltaTime();
				}
				else
				{
					animator->Stop();
					isAnimationPlaying = false;
				}


				// 현재 보스의 포지션에서 바라보는 백터 방향으로 더해줌
				DirectX::XMVECTOR newPosition = DirectX::XMVectorAdd(nowPosVec, DirectX::XMVectorScale(direction, _callMoveDistance));

				_call->GetComponent<Transform>()->SetPosition(newPosition.m128_f32[0], newPosition.m128_f32[1], newPosition.m128_f32[2]);
			}

			if (isAnimationPlaying == false)
			{
				return false;
			}

			return true;
		};

	pattern->SetLogic(callLogic, false);

	_callInitLogic = [pattern, this]()
		{
			_call->GetComponent<Transform>()->SetPosition(_boss->GetComponent<Transform>()->GetPosition().x, _boss->GetComponent<Transform>()->GetPosition().y, _boss->GetComponent<Transform>()->GetPosition().z);
			_callMoveDistance = 0.0f;
		};

	// 이니셜라이즈 로직 함수 넣어주기
	pattern->SetInitializeLogic(_callInitLogic);

	_callAttack = pattern;
}


void KunrealEngine::Kamen::CreateCall2Attack()
{
	BossPattern* pattern = new BossPattern();

	pattern->SetPatternName("Call2");

	pattern->SetAnimName("Call").SetDamage(10.0f).SetSpeed(20.0f).SetRange(_info._attackRange + 50.0f).SetAfterDelay(1.0f);
	pattern->SetMaxColliderCount(1).SetAttackState(BossPattern::eAttackState::ePush).SetColliderType(BossPattern::eColliderType::eBox);
	pattern->SetWithEgo(true);
	pattern->SetSubObject(_call2);

	auto initLogic = [pattern, this]()
		{
			if (!_isEgoAttackReady)
			{
				pattern->DeleteSubObject(_egoCall2);
			}
			else
			{
				pattern->SetSubObject(_egoCall2);
				_isEgoAttackReady = false;
			}
		};

	pattern->SetInitializeLogic(initLogic);

	auto callLogic = [pattern, this]()
		{
			auto animator = _boss->GetComponent<Animator>();
			auto isAnimationPlaying = animator->Play(pattern->_animName, pattern->_speed, false);

			if (_isEgoAttack)
			{
				auto egoAnimator = _alterEgo->GetComponent<Animator>();
				egoAnimator->Play(pattern->_animName, pattern->_speed, false);
			}

			// 일정 프레임이 넘어가면 데미지 체크용 콜라이더를 키기
			if (animator->GetCurrentFrame() >= 20)
			{
				if (pattern->_colliderOnCount > 0)
				{
					// 라이트도 키기
					_call2->GetComponent<Light>()->SetActive(true);

					// 파티클 키기
					for (auto& index : _call2->GetChilds())
					{
						index->GetComponent<Particle>()->SetActive(true);
					}

					// 콜라이더 키기
					auto objectIndex = pattern->GetSubObjectIndex(_call2);
					pattern->_isColliderActive[objectIndex] = true;
				}

				// 보스가 바라보는 방향 가져옴
				auto direction = GetDirection();

				// 현재 보스의 포지션
				auto nowPos = _boss->GetComponent<Transform>()->GetPosition();

				auto nowPosVec = DirectX::XMLoadFloat3(&nowPos);

				int step = (animator->GetCurrentFrame() - 20.0f) / 10.0f + 1;

				if (step != _call2PrevStep)
				{
					pattern->_colliderOnCount = pattern->_maxColliderOnCount;
				}

				_call2PrevStep = step;


				// 현재 보스의 포지션에서 바라보는 백터 방향으로 더해줌
				DirectX::XMVECTOR newPosition = DirectX::XMVectorAdd(nowPosVec, DirectX::XMVectorScale(direction, 20.0f * step));

				auto call2Pos = _call2->GetComponent<Transform>()->GetPosition();
				_call2->GetComponent<Transform>()->SetPosition(newPosition.m128_f32[0], call2Pos.y, newPosition.m128_f32[2]);

				if (_isEgoAttack)
				{
					// 라이트도 키기
					_egoCall2->GetComponent<Light>()->SetActive(true);

					// 파티클 키기
					for (auto& index : _egoCall2->GetChilds())
					{
						index->GetComponent<Particle>()->SetActive(true);
					}

					// 콜라이더 키기
					auto objectIndex = pattern->GetSubObjectIndex(_egoCall2);
					pattern->_isColliderActive[objectIndex] = true;

					// 현재 보스의 포지션
					auto egoNowPos = _alterEgo->GetComponent<Transform>()->GetPosition();
					auto playerPos = _playerTransform->GetPosition();

					// 보스가 바라보는 방향 가져옴
					auto egoDirection = GetEgoDirection();

					auto egoNowPosVec = DirectX::XMLoadFloat3(&egoNowPos);

					//if (step != _egoCall2PrevStep)
					//{
					//	pattern->_colliderOnCount = pattern->_maxColliderOnCount;
					//}

					//_egoCall2PrevStep = step;


					// 현재 보스의 포지션에서 바라보는 백터 방향으로 더해줌
					DirectX::XMVECTOR egoNewPosition = DirectX::XMVectorAdd(egoNowPosVec, DirectX::XMVectorScale(egoDirection, 20.0f * step));

					auto egopCall2Pos = _call2->GetComponent<Transform>()->GetPosition();
					_egoCall2->GetComponent<Transform>()->SetPosition(egoNewPosition.m128_f32[0], egopCall2Pos.y, egoNewPosition.m128_f32[2]);
				}
			}

			if (isAnimationPlaying == false)
			{
				return false;
			}

			return true;
		};

	pattern->SetLogic(callLogic, false);

	_call2Attack = pattern;
}


void KunrealEngine::Kamen::CreateBossRandomInsideWarning()
{
	BossPattern* pattern = new BossPattern();

	pattern->SetPatternName("BossRandomInsideWarning");

	pattern->SetSubObject(_bossInsideWarning);

	// 패턴 시작전에 초기화, 장판 켜줌
	auto initializeLogic = [pattern, this]()
		{
			_circleWarningSize = 40.0f;

			auto ranX = ToolBox::GetRandomFloat(-50.0f, 50.0f);
			auto ranY = ToolBox::GetRandomFloat(-90.0f, 10.0f);

			_emergencePos = { ranX , _bossTransform->GetPosition().y, ranY };

			_bossInsideWarning->GetComponent<TransparentMesh>()->Reset();
			_bossInsideWarning->GetComponent<TransparentMesh>()->SetActive(true);
			_bossInsideWarning->GetComponent<Transform>()->SetPosition(_emergencePos.x, _bossTransform->GetPosition().y + 1.0f, _emergencePos.z);
			_timer = 0.0f;

			_bossInsideWarning->GetComponent<Transform>()->SetScale(_circleWarningSize, _circleWarningSize, _circleWarningSize);

			if (_isEgoAttackReady)
			{
				_egoInsideWarning->GetComponent<TransparentMesh>()->Reset();
				_egoInsideWarning->GetComponent<TransparentMesh>()->SetActive(true);
				_egoInsideWarning->GetComponent<Transform>()->SetPosition(_alterEgo->GetComponent<Transform>()->GetPosition().x, _alterEgo->GetComponent<Transform>()->GetPosition().y + 1.0f, _alterEgo->GetComponent<Transform>()->GetPosition().z);

				_egoInsideWarning->GetComponent<Transform>()->SetScale(_circleWarningSize, _circleWarningSize, _circleWarningSize);

				_isEgoAttackReady = false;

				pattern->SetSubObject(_egoInsideWarning);
			}
			else
			{
				pattern->DeleteSubObject(_egoInsideWarning);
			}
		};

	pattern->SetInitializeLogic(initializeLogic);

	auto outsideSafe = [pattern, this]()
		{
			// 장판 실행
			auto isPlayed = _bossInsideWarning->GetComponent<TransparentMesh>()->PlayOnce();

			if (_isEgoAttack)
			{
				_egoInsideWarning->GetComponent<TransparentMesh>()->PlayOnce();
			}

			// 장판 실행이 완료되면
			if (isPlayed)
			{
				return false;
			}

			return true;
		};

	pattern->SetLogic(outsideSafe);

	_bossRandomInsideWarning = pattern;
}

void KunrealEngine::Kamen::CreateSwordAttack()
{
	BossPattern* pattern = new BossPattern();

	pattern->SetPatternName("Sword_Attack");

	pattern->SetAnimName("Right_Attack").SetDamage(100.0f).SetSpeed(15.0f).SetRange(_info._attackRange + 25.0f).SetAfterDelay(1.0f);
	pattern->SetAttackState(BossPattern::eAttackState::ePush).SetMaxColliderCount(1);
	pattern->SetSubObject(_freeSword);

	auto attackLogic = [pattern, this]()
		{
			auto animator = _boss->GetComponent<Animator>();
			auto isAnimationPlaying = animator->Play(pattern->_animName, pattern->_speed, false);

			// 일정 프레임이 넘어가면 데미지 체크용 콜라이더를 키기
			if (pattern->_colliderOnCount > 0)
			{
				if (animator->GetCurrentFrame() >= 10.0f)
				{
					pattern->_speed = 15.0f;
					_freeSword->GetComponent<BoxCollider>()->SetActive(true);
				}
				else
				{
					pattern->_speed = 15 * 0.3f;
				}
			}

			if (_timer < 0.5f)
			{
				_timer += TimeManager::GetInstance().GetDeltaTime() * 0.2f;
				_freeSword->GetComponent<MeshRenderer>()->SetDissolve(_timer);
			}

			if (isAnimationPlaying == false)
			{
				_freeSword->SetActive(true);
				pattern->DeleteSubObject(_freeSword);
				return false;
			}

			return true;
		};

	pattern->SetLogic(attackLogic);

	auto swordInitLogic = [pattern, this]()
		{
			_freeSword->GetComponent<MeshRenderer>()->SetParentBone(_boss, "Wrist_L");
			_freeSword->GetComponent<Transform>()->SetPosition(0.0f, 0.0f, 0.0f);
			_freeSword->GetComponent<Transform>()->SetRotation(0.0f, 0.0f, 0.0f);
			_freeSword->GetComponent<BoxCollider>()->SetColliderScale(5.0f, 20.0f, 5.0f);
			_freeSword->GetComponent<BoxCollider>()->SetTransform(_boss, "Wrist_L");

			pattern->SetSubObject(_freeSword);

			// 디졸브 이펙트 키기
			_freeSword->SetActive(true);
			_freeSword->GetComponent<MeshRenderer>()->SetActive(true);
			_freeSword->GetComponent<MeshRenderer>()->SetIsDissolve(true);

			_timer = 0.0f;
			_freeSword->GetComponent<MeshRenderer>()->SetDissolve(_timer);

		};

	// 이니셜라이즈 로직 함수 넣어주기
	pattern->SetInitializeLogic(swordInitLogic);

	_basicSwordAttack = pattern;
}


void KunrealEngine::Kamen::CreateSwordEmergence()
{
	BossPattern* pattern = new BossPattern();

	pattern->SetPatternName("Sword_Emmergence");

	pattern->SetAnimName("Idle").SetSpeed(20.0f).SetAfterDelay(0.5f).SetMaxColliderCount(0);

	auto swordInitLogic = [pattern, this]()
		{
			pattern->SetSubObject(_freeSword);

			auto swordTransform = _freeSword->GetComponent<Transform>();
			swordTransform->SetPosition(_swordStartPos.x, _swordStartPos.y, _swordStartPos.z);

			// 디졸브 이펙트 키기
			_freeSword->GetComponent<MeshRenderer>()->SetActive(true);
			_freeSword->GetComponent<MeshRenderer>()->SetIsDissolve(true);

			_timer = 0.0f;
			_freeSword->GetComponent<MeshRenderer>()->SetDissolve(_timer);

			_freeSword->GetComponent<BoxCollider>()->SetActive(false);

			_freeSword->GetComponent<Transform>()->SetRotation(180.0f, _freeSword->GetComponent<Transform>()->GetRotation().y, 0.0f);
		};

	pattern->SetInitializeLogic(swordInitLogic);

	auto emmergenceLogic = [pattern, this]()
		{
			if (_timer < 0.5f)
			{
				_timer += TimeManager::GetInstance().GetDeltaTime() * 0.3f;
				_freeSword->GetComponent<MeshRenderer>()->SetDissolve(_timer);
			}
			else
			{
				pattern->DeleteSubObject(_freeSword);
				return false;
			}

			return true;
		};

	pattern->SetLogic(emmergenceLogic);

	_swordEmmergence = pattern;
}

void KunrealEngine::Kamen::CreateSwordHide()
{
	BossPattern* pattern = new BossPattern();

	pattern->SetPatternName("Sword_Hide");

	pattern->SetAnimName("Idle").SetSpeed(20.0f).SetAfterDelay(0.5f).SetMaxColliderCount(0);

	auto swordInitLogic = [pattern, this]()
		{
			pattern->SetSubObject(_freeSword);

			// 디졸브 이펙트 키기
			_freeSword->GetComponent<MeshRenderer>()->SetActive(true);
			_freeSword->GetComponent<MeshRenderer>()->SetIsDissolve(true);

			_timer = 0.5f;
			_freeSword->GetComponent<MeshRenderer>()->SetDissolve(_timer);

			_freeSword->GetComponent<BoxCollider>()->SetActive(false);
		};

	pattern->SetInitializeLogic(swordInitLogic);

	auto hideLogic = [pattern, this]()
		{
			if (_timer > 0)
			{
				_timer -= TimeManager::GetInstance().GetDeltaTime() * 0.3f;
				_freeSword->GetComponent<MeshRenderer>()->SetDissolve(_timer);
			}
			else
			{
				return false;
			}

			return true;
		};

	pattern->SetLogic(hideLogic);

	_swordHide = pattern;
}


void KunrealEngine::Kamen::CraeteSwordTurnClockWise()
{
	BossPattern* pattern = new BossPattern();

	pattern->SetPatternName("SwordTurnClockWise");

	pattern->SetAnimName("Idle").SetSpeed(3.0f);
	pattern->SetMaxColliderCount(0);

	auto swordInitLogic = [pattern, this]()
		{
			pattern->SetSubObject(_freeSword);

			_freeSword->GetComponent<MeshRenderer>()->DeleteParentBone();

			_freeSword->GetComponent<MeshRenderer>()->SetActive(true);

			_freeSword->GetComponent<MeshRenderer>()->SetIsDissolve(false);

			_freeSword->GetComponent<BoxCollider>()->SetActive(false);

			_freeSword->GetComponent<Transform>()->SetPosition(_swordStartPos.x, _freeSword->GetComponent<Transform>()->GetPosition().y, _swordStartPos.z);

			_swordRotation = _freeSword->GetComponent<Transform>()->GetRotation();

			_swordRotateAngle = 0.0f;
		};

	pattern->SetInitializeLogic(swordInitLogic);

	auto swordTurnLogic = [pattern, this]()
		{
			_swordRotateAngle += TimeManager::GetInstance().GetDeltaTime() * pattern->_speed;

			float x = _swordOriginPos.x - _circleWarningSize * cos(_swordRotateAngle);
			float z = _swordOriginPos.z + _circleWarningSize * sin(_swordRotateAngle);

			_freeSword->GetComponent<Transform>()->SetPosition(x, _freeSword->GetComponent<Transform>()->GetPosition().y, z);

			auto goalAngle = ToolBox::GetAngleWithDirection(_swordOriginPos, _freeSword->GetComponent<Transform>()->GetPosition(), 0.0f) - 90.0f;

			_freeSword->GetComponent<Transform>()->SetRotation(_swordRotation.x, _swordRotation.y + goalAngle, _swordRotation.z);

			if (_swordRotateAngle >= 2 * DirectX::XM_PI)
			{
				pattern->DeleteSubObject(_freeSword);
				return false;
			}

			return true;
		};

	pattern->SetLogic(swordTurnLogic);

	_swordTurnClockWise = pattern;
}


void KunrealEngine::Kamen::CreateSwordTurnAntiClock()
{
	BossPattern* pattern = new BossPattern();

	pattern->SetPatternName("SwordTurnClockWise");

	pattern->SetAnimName("Idle").SetSpeed(3.0f);
	pattern->SetMaxColliderCount(0);

	auto swordInitLogic = [pattern, this]()
		{
			pattern->SetSubObject(_freeSword);

			_freeSword->GetComponent<MeshRenderer>()->DeleteParentBone();

			_freeSword->GetComponent<MeshRenderer>()->SetActive(true);

			_freeSword->GetComponent<MeshRenderer>()->SetIsDissolve(false);

			_freeSword->GetComponent<BoxCollider>()->SetActive(false);

			_freeSword->GetComponent<Transform>()->SetPosition(_swordStartPos.x, _freeSword->GetComponent<Transform>()->GetPosition().y, _swordStartPos.z);

			_swordRotation = _freeSword->GetComponent<Transform>()->GetRotation();

			_swordRotateAngle = 0.0f;
		};

	pattern->SetInitializeLogic(swordInitLogic);

	auto swordTurnLogic = [pattern, this]()
		{
			_swordRotateAngle += TimeManager::GetInstance().GetDeltaTime() * pattern->_speed;

			float x = _swordOriginPos.x - _circleWarningSize * cos(_swordRotateAngle);
			float z = _swordOriginPos.z - _circleWarningSize * sin(_swordRotateAngle);

			_freeSword->GetComponent<Transform>()->SetPosition(x, _freeSword->GetComponent<Transform>()->GetPosition().y, z);

			auto goalAngle = ToolBox::GetAngleWithDirection(_swordOriginPos, _freeSword->GetComponent<Transform>()->GetPosition(), 0.0f) - 90.0f;

			_freeSword->GetComponent<Transform>()->SetRotation(_swordRotation.x, _swordRotation.y + goalAngle, _swordRotation.z);

			if (_swordRotateAngle >= 2 * DirectX::XM_PI)
			{
				pattern->DeleteSubObject(_freeSword);
				return false;
			}

			return true;
		};

	pattern->SetLogic(swordTurnLogic);

	_swordTurnAntiClockWise = pattern;
}


void KunrealEngine::Kamen::CreateSwordLinearReady()
{
	BossPattern* pattern = new BossPattern();

	pattern->SetPatternName("SwordLinearReady");

	pattern->SetAnimName("Idle").SetSpeed(5.0f);
	pattern->SetMaxColliderCount(0);

	auto swordInitLogic = [pattern, this]()
		{
			pattern->SetSubObject(_freeSword);

			_freeSword->GetComponent<MeshRenderer>()->SetActive(true);

			_freeSword->GetComponent<MeshRenderer>()->SetIsDissolve(false);

			_freeSword->GetComponent<MeshRenderer>()->DeleteParentBone();

			// 테스트용
			_freeSword->GetComponent<BoxCollider>()->SetActive(true);

			_swordRotateAngle = 0.0f;
		};

	auto swordLinearReadyLogic = [pattern, this]()
		{
			auto translationSpeed = TimeManager::GetInstance().GetDeltaTime() * pattern->_speed;

			_swordRotateAngle -= TimeManager::GetInstance().GetDeltaTime() * pattern->_speed * 5;

			auto position = _freeSword->GetComponent<Transform>()->GetPosition();
			auto rotation = _freeSword->GetComponent<Transform>()->GetRotation();

			_freeSword->GetComponent<Transform>()->SetPosition(position.x, position.y - translationSpeed, position.z);
			_freeSword->GetComponent<Transform>()->SetRotation(rotation.x, rotation.y, _swordRotateAngle);

			if (_swordRotateAngle <= -90.0f)
			{
				pattern->DeleteSubObject(_freeSword);
				return false;
			}

			return true;
		};

	pattern->SetLogic(swordLinearReadyLogic);

	pattern->SetInitializeLogic(swordInitLogic);

	_swordLinearReady = pattern;
}

void KunrealEngine::Kamen::CreateSwordLinearAttack()
{
	BossPattern* pattern = new BossPattern();

	pattern->SetPatternName("SwordLinearAttack");

	pattern->SetAnimName("Idle").SetSpeed(30.0f);
	pattern->SetAttackState(BossPattern::eAttackState::ePush);
	pattern->SetMaxColliderCount(1);
	pattern->SetColliderType(BossPattern::eColliderType::eBox);

	auto lenearAttackInitLogic = [pattern, this]()
		{
			pattern->SetSubObject(_freeSword);

			_freeSword->GetComponent<MeshRenderer>()->DeleteParentBone();

			_freeSword->GetComponent<MeshRenderer>()->SetActive(true);

			_freeSword->GetComponent<MeshRenderer>()->SetIsDissolve(false);

			_freeSword->GetComponent<BoxCollider>()->SetActive(true);

			_swordLinearDistance = 0.0f;

			_swordDirection = DirectX::XMFLOAT3(1.0f, 0.0f, 0.0f);
			_swordDirection = ToolBox::RotateVector(_swordDirection, _freeSword->GetComponent<Transform>()->GetRotation().y);

			auto direction = DirectX::XMVector3Normalize(DirectX::XMLoadFloat3(&_swordDirection));

			DirectX::XMStoreFloat3(&_swordDirection, direction);
		};

	pattern->SetInitializeLogic(lenearAttackInitLogic);

	auto swordLinearAttackLogic = [pattern, this]()
		{
			float moveSpeed = pattern->_speed * TimeManager::GetInstance().GetDeltaTime();

			auto swordTransform = _freeSword->GetComponent<Transform>()->GetPosition();

			DirectX::XMVECTOR newPosition = DirectX::XMVectorAdd(DirectX::XMLoadFloat3(&swordTransform), DirectX::XMVectorScale(DirectX::XMLoadFloat3(&_swordDirection), moveSpeed));

			_swordLinearDistance += moveSpeed;

			_freeSword->GetComponent<Transform>()->SetPosition(newPosition.m128_f32[0], newPosition.m128_f32[1], newPosition.m128_f32[2]);

			if (_swordLinearDistance > 100.0f)
			{
				pattern->DeleteSubObject(_freeSword);
				return false;
			}

			return true;
		};

	pattern->SetLogic(swordLinearAttackLogic);

	_swordLinearAtack = pattern;
}

void KunrealEngine::Kamen::CreateSwordChopAttack()
{
	BossPattern* pattern = new BossPattern();

	pattern->SetPatternName("SwordChopAttack");

	pattern->SetAnimName("Idle").SetSpeed(15.0f);
	pattern->SetMaxColliderCount(0);

	auto swordInitLogic = [pattern, this]()
		{
			pattern->SetSubObject(_freeSword);

			_freeSword->GetComponent<MeshRenderer>()->SetActive(true);

			_freeSword->GetComponent<MeshRenderer>()->SetIsDissolve(false);

			_freeSword->GetComponent<MeshRenderer>()->DeleteParentBone();
		};

	auto swordChopAttackLogic = [pattern, this]()
		{
			auto position = _freeSword->GetComponent<Transform>()->GetPosition();

			if (position.y > 15.0f)
			{
				auto chopSpeed = TimeManager::GetInstance().GetDeltaTime() * _swordChopSpeed;

				_freeSword->GetComponent<Transform>()->SetPosition(position.x, position.y - chopSpeed, position.z);
			}

			else
			{
				pattern->DeleteSubObject(_freeSword);
				return false;
			}

			return true;
		};

	pattern->SetLogic(swordChopAttackLogic);

	pattern->SetInitializeLogic(swordInitLogic);

	_swordChopAttack = pattern;
}

void KunrealEngine::Kamen::CreateSwordLookPlayer()
{
	BossPattern* pattern = new BossPattern();

	pattern->SetPatternName("SwordLookPlayer");

	pattern->SetAnimName("Idle").SetSpeed(20.0f);
	pattern->SetMaxColliderCount(0);

	auto swordLookInitLogic = [pattern, this]()
		{
			pattern->SetSubObject(_freeSword);
			pattern->SetSubObject(_swordPath);

			_freeSword->GetComponent<MeshRenderer>()->DeleteParentBone();

			_freeSword->GetComponent<MeshRenderer>()->SetActive(true);

			_freeSword->GetComponent<MeshRenderer>()->SetIsDissolve(false);

			_swordPath->GetComponent<TransparentMesh>()->SetActive(true);

			_swordPath->GetComponent<TransparentMesh>()->Reset();

			auto swordTransform = _freeSword->GetComponent<Transform>()->GetPosition();
			_swordPath->GetComponent<Transform>()->SetPosition(swordTransform.x, _bossTransform->GetPosition().y, swordTransform.z);

			auto swordPathTransform = _swordPath->GetComponent<Transform>();
			swordPathTransform->SetRotation(swordPathTransform->GetRotation().x, _freeSword->GetComponent<Transform>()->GetRotation().y, swordPathTransform->GetRotation().z);

			_timer = 0.0f;
		};

	pattern->SetInitializeLogic(swordLookInitLogic);

	auto swordLookPlayerLogic = [pattern, this]()
		{
			_swordPath->GetComponent<TransparentMesh>()->PlayOnce();

			// 일정 시간 까지 플레이어 따라다니다가 헤드 고정시킬 예정

			auto goalAngle = ToolBox::GetAngleWithDirection(_playerTransform->GetPosition(), _swordStartPos, 0.0f) - 90.0f;

			auto swordTransform = _freeSword->GetComponent<Transform>();
			auto rotation = swordTransform->GetRotation();

			auto swordPathTransform = _swordPath->GetComponent<Transform>();
			// 회전 속도
			float speed = TimeManager::GetInstance().GetDeltaTime() * pattern->_speed;

			_timer += TimeManager::GetInstance().GetDeltaTime();

			// 현재 각도가 목표로 하는 각도보다 작을 경우
			if (_timer < _warningMaxTimer)
			{
				auto errorRange = std::abs(goalAngle) - std::abs(rotation.y);
				errorRange = std::abs(errorRange);
				if (errorRange > 1.0f)
				{
					if (goalAngle < rotation.y)
					{
						// 회전 속도만큼 회전
						swordTransform->SetRotation(swordTransform->GetRotation().x, swordTransform->GetRotation().y - speed, swordTransform->GetRotation().z);
						swordPathTransform->SetRotation(swordPathTransform->GetRotation().x, swordTransform->GetRotation().y, swordPathTransform->GetRotation().z);

					}
					else
					{
						// 회전 속도만큼 회전
						swordTransform->SetRotation(swordTransform->GetRotation().x, swordTransform->GetRotation().y + speed, swordTransform->GetRotation().z);
						swordPathTransform->SetRotation(swordPathTransform->GetRotation().x, swordTransform->GetRotation().y, swordPathTransform->GetRotation().z);
					}
				}
			}

			if (_timer > _warningMaxTimer + 1.0f)
			{
				// 회전이 완료되었다고 반환
				pattern->DeleteSubObject(_freeSword);
				return false;
			}
			else
			{
				return true;
			}
		};

	pattern->SetLogic(swordLookPlayerLogic);

	_swordLookPlayer = pattern;
}

void KunrealEngine::Kamen::CreateEmergenceAttack()
{
	BossPattern* pattern = new BossPattern();

	pattern->SetPatternName("Emergence9Lich");

	pattern->SetAnimName("Emergence").SetSpeed(15.0f);
	pattern->SetMaxColliderCount(0);

	for (int i = 0; i < 9; i++)
	{
		pattern->SetSubObject(_fakeBoss[i]);
	}

	int index = 0;
	for (int j = 0; j < 3; j++)
	{
		for (int i = 0; i < 3; i++)
		{
			_fakeBoss[index]->GetComponent<Transform>()->SetPosition(40.0f - (40.0 * i), 0.0f, 40.0f - (40.0 * j));
			index++;
		}
	}

	auto core1Logic = [pattern, this]()
		{
			// 9명의 분신 중 무엇이 사라질 지 인덱스 랜덤 설정
			if (!_isRandomStart)
			{
				_targetIndex = ToolBox::GetRandomNum(_fakeBoss.size() - 1);

				_isRandomStart = true;
			}

			// 패턴 시작 위해 중앙으로 텔레포트
			if (!_isRotateFinish)
			{
				auto newPos = _fakeBoss[_targetIndex]->GetComponent<Transform>()->GetPosition();
				auto isTeleportFinish = Teleport(newPos, false, 3.0f);

				// 텔포 끝나면 패턴 시작
				if (!isTeleportFinish)
				{
					return true;
				}
				else
				{
					// 텔포 끝나면 다시 돌아오지 않도록
					RotateByAngle(0.0f);

					_isRotateFinish = true;
				}
			}

			/// emergence 역애니메이션 한번 재생시켜줌 아직 미구현

			// 패턴 시작 전 초기화해줄것들
			if (!_isCoreStart)
			{
				// 페이크 보스들 켜줌
				for (int i = 0; i < _fakeBoss.size(); i++)
				{
					if (i != _targetIndex)
					{
						_fakeBoss[i]->SetActive(true);
						_fakeBoss[i]->SetTotalComponentState(true);
					}

					else
					{
						_fakeBoss[i]->SetActive(false);
					}
				}

				_isCoreStart = true;
			}

			auto animator = _boss->GetComponent<Animator>();
			auto isAnimationPlaying = animator->Play(pattern->_animName, pattern->_speed, false);

			if (isAnimationPlaying == false)
			{
				_isCoreStart = false;
				_isRotateFinish = false;

				_targetIndex = 0;
				return false;
			}

			return true;
		};

	pattern->SetLogic(core1Logic, false);

	_emergence9Lich = pattern;
}


DirectX::XMVECTOR KunrealEngine::Kamen::GetEgoDirection()
{
	auto direction = ToolBox::RotateVector(DirectX::XMFLOAT3(0, 0, -1.0f), _alterEgo->GetComponent<Transform>()->GetRotation().y);

	DirectX::XMVECTOR dirVec = DirectX::XMLoadFloat3(&direction);

	return dirVec;
}


const DirectX::XMFLOAT3& KunrealEngine::Kamen::GetSwordDirection()
{
	return _swordDirection;
}

void KunrealEngine::Kamen::BackStepCallPattern()
{
	BossPattern* backStepCallPattern = new BossPattern();

	backStepCallPattern->SetSkipMove(true);

	backStepCallPattern->SetNextPatternForcePlay(true);

	backStepCallPattern->SetMaxColliderCount(0).SetWithEgo(true);

	backStepCallPattern->SetPattern(_backStep);

	backStepCallPattern->SetPattern(_call2Attack);

	backStepCallPattern->SetRange(backStepCallPattern->_patternList[2]->_range - backStepCallPattern->_patternList[1]->_range);

	_basicPattern.emplace_back(backStepCallPattern);
}

void KunrealEngine::Kamen::TeleportSpellPattern()
{
	BossPattern* teleportSpellPattern = new BossPattern();

	teleportSpellPattern->SetNextPatternForcePlay(true);
	teleportSpellPattern->SetSkipChase(true);
	teleportSpellPattern->SetRange(100.0f);

	teleportSpellPattern->SetPattern(_teleportWithLook);

	teleportSpellPattern->SetPattern(_spellAttack);


	_basicPattern.emplace_back(teleportSpellPattern);
}


void KunrealEngine::Kamen::EmergenceAttackPattern()
{
	BossPattern* emergenceAttackPattern = new BossPattern();

	emergenceAttackPattern->SetNextPatternForcePlay(true);
	emergenceAttackPattern->SetSkipChase(true);
	emergenceAttackPattern->SetRange(30.0f);
	emergenceAttackPattern->SetWithEgo(true);

	emergenceAttackPattern->SetPattern(_reverseEmergence);
	emergenceAttackPattern->SetPattern(_bossRandomInsideWarning);
	emergenceAttackPattern->SetPattern(_emergence);

	_basicPattern.emplace_back(emergenceAttackPattern);
}

void KunrealEngine::Kamen::SwordTurnClockPattern()
{
	BossPattern* swordTurnClockPattern = new BossPattern();

	swordTurnClockPattern->SetSkipChase(true);

	swordTurnClockPattern->SetMaxColliderCount(0);

	swordTurnClockPattern->SetPattern(_swordEmmergence);

	swordTurnClockPattern->SetPattern(_swordChopAttack);

	swordTurnClockPattern->SetPattern(_swordTurnClockWise);

	swordTurnClockPattern->SetPattern(_swordHide);

	swordTurnClockPattern->SetPattern(_insideSafe);

	swordTurnClockPattern->SetRange(swordTurnClockPattern->_patternList[1]->_range);

	auto swordInitLogic = [swordTurnClockPattern, this]()
		{
			_circleWarningSize = 30.0f;

			auto ranX = ToolBox::GetRandomFloat(-50.0f, 50.0f);
			auto ranZ = ToolBox::GetRandomFloat(-50.0f, 50.0f);

			_swordOriginPos = DirectX::XMFLOAT3{ ranX, _freeSword->GetComponent<Transform>()->GetPosition().y + 30.0f, ranZ };

			float x = _swordOriginPos.x - _circleWarningSize * cos(0.0f);
			float z = _swordOriginPos.z - _circleWarningSize * sin(0.0f);

			_swordStartPos = DirectX::XMFLOAT3{ x , _freeSword->GetComponent<Transform>()->GetPosition().y + 30.0f, z };

			_swordChopSpeed = 20.0f;

			_warningMaxTimer = 0.5f;
		};

	swordTurnClockPattern->SetInitializeLogic(swordInitLogic);

	_specialPattern.emplace_back(swordTurnClockPattern);
}

void KunrealEngine::Kamen::SwordTurnAntiClockPattern()
{
	BossPattern* swordTurnAntiClockPattern = new BossPattern();

	swordTurnAntiClockPattern->SetSkipChase(true);

	swordTurnAntiClockPattern->SetMaxColliderCount(0);

	swordTurnAntiClockPattern->SetPattern(_swordEmmergence);

	swordTurnAntiClockPattern->SetPattern(_swordChopAttack);

	swordTurnAntiClockPattern->SetPattern(_swordTurnAntiClockWise);

	swordTurnAntiClockPattern->SetPattern(_swordHide);

	swordTurnAntiClockPattern->SetPattern(_outsideSafe);

	swordTurnAntiClockPattern->SetRange(swordTurnAntiClockPattern->_patternList[1]->_range);

	auto swordInitLogic = [swordTurnAntiClockPattern, this]()
		{
			_circleWarningSize = 40.0f;

			auto ranX = ToolBox::GetRandomFloat(-50.0f, 50.0f);
			auto ranZ = ToolBox::GetRandomFloat(-50.0f, 50.0f);

			_swordOriginPos = DirectX::XMFLOAT3{ ranX, _freeSword->GetComponent<Transform>()->GetPosition().y + 30.0f, ranZ };

			float x = _swordOriginPos.x - _circleWarningSize * cos(0.0f);
			float z = _swordOriginPos.z - _circleWarningSize * sin(0.0f);

			_swordStartPos = DirectX::XMFLOAT3{ x , _freeSword->GetComponent<Transform>()->GetPosition().y + 30.0f, z };

			_swordChopSpeed = 20.0f;

			_warningMaxTimer = 0.5f;
		};

	swordTurnAntiClockPattern->SetInitializeLogic(swordInitLogic);

	_specialPattern.emplace_back(swordTurnAntiClockPattern);
}


void KunrealEngine::Kamen::SwordLinearAttackPattern()
{
	BossPattern* swordLinearAttack = new BossPattern();

	swordLinearAttack->SetSkipChase(true);

	swordLinearAttack->SetMaxColliderCount(0);

	swordLinearAttack->SetPattern(_swordEmmergence);

	swordLinearAttack->SetPattern(_swordLinearReady);

	swordLinearAttack->SetPattern(_swordLookPlayer);

	swordLinearAttack->SetPattern(_swordLinearAtack);

	swordLinearAttack->SetPattern(_swordHide);

	auto swordLinearAttackInitLogic = [swordLinearAttack, this]()
		{
			auto random = ToolBox::GetRandomNum(3);

			switch (random)
			{
				case 0: _swordStartPos = DirectX::XMFLOAT3{ 50.0f , 30.0f, 50.0f };
					  break;
				case 1: _swordStartPos = DirectX::XMFLOAT3{ 50.0f , 30.0f, -50.0f };
					  break;
				case 2: _swordStartPos = DirectX::XMFLOAT3{ -50.0f , 30.0f, 50.0f };
					  break;
				case 3: _swordStartPos = DirectX::XMFLOAT3{ -50.0f , 30.0f, -50.0f };
					  break;
				default:
					break;
			}

			auto swordTransform = _freeSword->GetComponent<Transform>();

			auto angle = ToolBox::GetAngleWithDirection(_playerTransform->GetPosition(), _swordStartPos, 0.0f);

			swordTransform->SetRotation(swordTransform->GetRotation().x, angle - 90.0f, swordTransform->GetRotation().z);

			_warningMaxTimer = 4.0f;
		};

	swordLinearAttack->SetInitializeLogic(swordLinearAttackInitLogic);

	_specialPattern.emplace_back(swordLinearAttack);
}


void KunrealEngine::Kamen::SwordChopPattern()
{
	BossPattern* swordChopAttack = new BossPattern();

	swordChopAttack->SetSkipChase(true);

	swordChopAttack->SetMaxColliderCount(0);

	swordChopAttack->SetPattern(_swordEmmergence);

	swordChopAttack->SetPattern(_swordChopAttack);

	swordChopAttack->SetPattern(_donutSafe);

	swordChopAttack->SetPattern(_swordHide);

	auto swordChopAttackInitLogic = [swordChopAttack, this]()
		{
			_circleWarningSize = 40.0f;

			auto ranX = ToolBox::GetRandomFloat(-50.0f, 50.0f);
			auto ranZ = ToolBox::GetRandomFloat(-50.0f, 50.0f);

			_swordOriginPos = DirectX::XMFLOAT3{ ranX, 0.0f, ranZ };

			float x = _swordOriginPos.x - _circleWarningSize * cos(0.0f);
			float z = _swordOriginPos.z - _circleWarningSize * sin(0.0f);

			_swordStartPos = DirectX::XMFLOAT3{ x , 30.0f + 70.0f, z };

			auto swordTransform = _freeSword->GetComponent<Transform>();

			auto angle = ToolBox::GetAngleWithDirection(_playerTransform->GetPosition(), _swordStartPos, 0.0f);

			swordTransform->SetRotation(swordTransform->GetRotation().x, angle - 90.0f, swordTransform->GetRotation().z);

			_swordChopSpeed = 40.0f;

			_warningMaxTimer = 1.0f;
		};

	swordChopAttack->SetInitializeLogic(swordChopAttackInitLogic);

	_specialPattern.emplace_back(swordChopAttack);
}

void KunrealEngine::Kamen::BasicSwordAttackPattern()
{
	BossPattern* basicSwordAttack = new BossPattern();

	basicSwordAttack->SetNextPatternForcePlay(true);
	basicSwordAttack->SetMaxColliderCount(0);
	basicSwordAttack->SetPattern(_basicSwordAttack);
	basicSwordAttack->SetPattern(_swordHide);

	basicSwordAttack->SetRange(basicSwordAttack->_patternList[1]->_range);

	_basicPattern.emplace_back(basicSwordAttack);
}

void KunrealEngine::Kamen::CoreEmmergencePattern()
{
	BossPattern* coreEmmergence = new BossPattern();

	coreEmmergence->SetNextPatternForcePlay(true);

	coreEmmergence->SetTriggerHp(100.0f);

	coreEmmergence->SetMaxColliderCount(0);

	coreEmmergence->SetPattern(_teleport);

	coreEmmergence->SetPattern(_emergence9Lich);

	coreEmmergence->SetPattern(_leftAttack);

	_basicPattern.emplace_back(coreEmmergence);
}
