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
	_swordStartPos(), _swordOriginPos(), _swordCircleWarningSize(0.0f), _swordEmmergence(nullptr),
	_insideSafe(nullptr), _swordOutsideWarning(nullptr), _swordLinearAtack(nullptr),
	_swordLinearReady(nullptr), _swordLookPlayer(nullptr), _swordDirection(), _timer(0.0f), _swordTimer(0.0f), _swordPath(nullptr),
	_swordChopAttack(nullptr), _donutSafe(nullptr), _swordDonutWarning1(nullptr), _swordDonutWarning2(nullptr), _swordDonutWarning3(nullptr),
	_swordRotation(), _swordChopSpeed(0.0f), _warningMaxTimer(0.0f),
	_leftHandBone(nullptr), _rightFireAttack(nullptr), _rightHandBone(nullptr), _leftFireAttack(nullptr),
	_alterEgo(nullptr), _isSpecial2Ready(false),
	_isSpecial2Playing(false), _egoTimer(0.0f), _isEgoAppearInit(false), _isEgoAppearFinish(false), _isEgoAttackReady(false),
	_isEgoAttack(false), _egoLeftHandBone(nullptr), _egoRightHandBone(nullptr),
	_egoCall2PrevStep(0), _egoCall2(nullptr), _egoLazer(nullptr), _egoLazerCollider(nullptr), _reverseEmergence(nullptr),
	_emergence(nullptr), _emergencePos(), _bossInsideWarning(nullptr), _bossInsideAttack(nullptr), _bossRandomInsideWarning(nullptr),
	_egoInsideWarning(nullptr), _egoInsideAttack(nullptr), _donutAttack(nullptr), _donutSize(0.0f), _bossCircleWarningSize(0.0f), _randomPos(),
	_timer2(0.0f), _timer3(0.0f), _fiveWayAttack(nullptr),
	_swordSwingVertical(nullptr), _kamenSword(nullptr), _swordSwingTwice(nullptr), _isSwordSecondAttackStart(false), _swordSwingTwiceHard(nullptr), _swordDirection2(), _swordMoveDistance2(0.0f), _swordSwingHorizontal(nullptr), _swordSwingTeleport(nullptr),
	_kamenSwordParticle(0), _kamenSwordAfterImageParticle(0), _largeBlade(nullptr),
	_swordRotationAttack(nullptr), _swordMultipleAttack(nullptr), _battleCry(nullptr)
{
	BossBasicInfo info;

	info.SetHp(1000).SetArmor(1).SetDamage(100).SetMoveSpeed(20.0f).SetsStaggeredGauge(100.0f);
	info.SetAttackRange(5.0f).SetMaxPhase(3);

	SetInfo(info);

	_centerPos.x = 0.0f;
	_centerPos.y = 2.0f;
	_centerPos.z = 0.0f;
}

KunrealEngine::Kamen::~Kamen()
{

}

void KunrealEngine::Kamen::Initialize()
{
	// 반드시 해야함
	Boss::Initialize(this->GetOwner());
	mainLight = SceneManager::GetInstance().GetScene("Main")->GetGameObject("DirectionalLight")->GetComponent<Light>();
	// 보스 타이머 설정
	SetStartTime(0.0f);

	SetSpecialPatternPlayPhase(2);
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
	HoldKamenSword();

	// 특정 패턴에 따라 Directional Light를 바꾼다

	for (auto& kamenLastphaseParticle : _kamenLastphaseParticle)
	{
		kamenLastphaseParticle->GetComponent<Particle>()->SetActive(true);
		kamenLastphaseParticle->GetComponent<Particle>()->SetParticleAngle(270, (_boss->GetComponent<Transform>()->GetRotation().y), 0);
	}
	for (auto& kamenLastphaseParticleBack : _kamenLastphaseParticleBack)
	{
		kamenLastphaseParticleBack->GetComponent<Particle>()->SetActive(true);
		kamenLastphaseParticleBack->GetComponent<Particle>()->SetParticleAngle(270, -(_boss->GetComponent<Transform>()->GetRotation().y), 0);
	}
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
	_boss->GetComponent<Transform>()->SetScale(1.5f, 1.5f, 1.5f);
}


void KunrealEngine::Kamen::SetBossCollider()
{
	_boss->AddComponent<BoxCollider>();
	_boss->GetComponent<BoxCollider>()->SetTransform(_boss, "Spine1_M");
	_boss->GetComponent<BoxCollider>()->SetColliderScale(10.0f, 30.0f, 20.0f);
	_boss->GetComponent<BoxCollider>()->SetOffset(0.0f, -6.0f, 0.0f);
}

void KunrealEngine::Kamen::CreatePattern()
{
	CreateSubObject();
	CreateParticleObject();

	//CreateLeftAttack();
	//CreateRightAttack();
	//CreateCallAttack();
	//CreateEmergenceAttack();

	CreateTurn180();
	CreateSpellAttack();
	CreateLeftAttackThrowingFire();
	CreateRightAttackThrowingFire();
	CreateBossRandomInsideWarning();

	CreateSwordAttack();

	CreateCall2Attack();
	CreateBackStep();
	CreateTeleportToCenter();
	CreateTeleportToCenterWithLook();
	CreateTeleportRandomWithLook();
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
	CreateDonutAttack();
	CreateFiveWayAttack();

	CreateSwordSwingVertical();
	CreateSwordSwingTwice();
	CreateSwordSwingTwiceHard();
	CreateSwordSwingHorizontal();
	CreateSwordMultipleAttack();

	CreateSwordLookPlayer();
	CreateSwordLinearReady();
	CreateSwordLinearAttack();

	CreateSwordChopAttack();

	CreateBattleCry();
	CreateKamenHoldSword();

	// 코어
	CreateSwordMultipleAttack();

	// 실제 사용중인 패턴들 모아놓음
	GamePattern();
}

void KunrealEngine::Kamen::GamePattern()
{
	//_basicPattern[0].emplace_back(_leftFireAttack);	// 왼손으로 투사체 5개 발사
	//_basicPattern[0].emplace_back(_rightFireAttack);	// 오른손으로 투사체 5개 발사
	//TeleportSpellPattern();							// 텔포 후 spell	
	//BackStepCallPattern();							// 투사체 4번 터지는 패턴
	//EmergenceAttackPattern();						// 사라졌다가 등장 후 보스 주변 원으로 터지는 공격
	//_basicPattern[0].emplace_back(_fiveWayAttack);		// 5갈래 분신 발사
	//
	//_basicPattern[1] = _basicPattern[0];
	//
	//_basicPattern[2].emplace_back(_swordSwingTwice);
	//_basicPattern[2].emplace_back(_swordSwingTwiceHard);
	//_basicPattern[2].emplace_back(_swordSwingHorizontal);
	//_basicPattern[2].emplace_back(_swordSwingVertical);

	SwordTurnAntiClockPattern();					// 텔포 후 반시계 -> 외부 안전
	SwordTurnClockPattern();						// 텔포 후 시계 -> 내부 안전
	SwordLinearAttackPattern();						// 칼 직선 공격
	SwordChopPattern();								// 도넛

	CoreSwordMutipleAttackPattern();

	CreateDecalTest();
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
		_patternIndex = ToolBox::GetRandomNum(0, _basicPattern[_info._phase - 1].size() - 1);

		if (_basicPattern[_info._phase - 1].size() > 1)
		{
			while (_patternIndex == _exPatternIndex)
			{
				// 랜덤 패턴을 위한 랜덤 인덱스를 가져옴
				_patternIndex = ToolBox::GetRandomNum(0, _basicPattern[_info._phase - 1].size() - 1);
			}

			// 가져온 랜덤 패턴이 활성화되어있지 않다면
			while ((_basicPattern[_info._phase - 1])[_patternIndex]->_isActive == false)
			{
				// 가져온 랜덤 패턴이 활성화 되어 있을때까지 다시 랜덤 인덱스를 추출
				_patternIndex = ToolBox::GetRandomNum(0, _basicPattern[_info._phase - 1].size() - 1);
			}
		}

		// 여기서 patternindex가 1,2,3,4,5라면 분신을 소환
		if (_isSpecial2Ready && (_basicPattern[_info._phase - 1])[_patternIndex]->_withEgo)
		{
			_isSpecial2Playing = true;

			_boss->GetComponent<Animator>()->Stop();
		}
		else
		{
			_isSpecial2Playing = false;
		}
	}

	_nowTitlePattern = _basicPattern[_info._phase - 1][_patternIndex];

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

			auto ranX = ToolBox::GetRandomFloat(-80.0f, 90.0f);
			auto ranZ = ToolBox::GetRandomFloat(-100.0f, 80.0f);

			egoTransform->SetPosition(ranX, 2.0F, ranZ);

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
			_alterEgo->GetComponent<Animator>()->Play("Idle", 20.0f, true);

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
	DirectX::XMFLOAT4 Ambient = { 1.0f, 1.0f, 1.0f, 1.0f };
	DirectX::XMFLOAT4 Diffuse = { 0.0f, 1.0f, 0.0f, 1.0f };
	DirectX::XMFLOAT4 Specular = { 1.0f, 1.0f, 1.0f, 1.0f };

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
				handFire->_autoAwake = true;;
				handFire->GetComponent<Transform>()->SetPosition(0.0f, mapOffsetY, 0.0f);
				handFire->AddComponent<Particle>();
				handFire->GetComponent<Particle>()->SetParticleEffect("BlastWave2", "Resources/Textures/Particles/fx_BlastWave2.dds", 1000);
				handFire->GetComponent<Particle>()->SetParticleDuration(1.0f, 4.0f);
				handFire->GetComponent<Particle>()->SetParticleVelocity(5.0f, true);
				handFire->GetComponent<Particle>()->SetParticleSize(7.f, 7.0f);
				handFire->GetComponent<Particle>()->AddParticleColor(1.2f, 7.5f, 0.6f);
				handFire->GetComponent<Particle>()->SetActive(false);


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
			BossHandFire->_autoAwake = true;
			BossHandFire->GetComponent<Transform>()->SetPosition(0.0f, mapOffsetY, 0.0f);
			BossHandFire->AddComponent<Particle>();
			BossHandFire->GetComponent<Particle>()->SetParticleEffect("BlastWave3", "Resources/Textures/Particles/fx_BlastWave3.dds", 1000);
			BossHandFire->GetComponent<Particle>()->SetParticleDuration(1.0f, 0.7f);
			BossHandFire->GetComponent<Particle>()->SetParticleVelocity(10.0f, true);
			BossHandFire->GetComponent<Particle>()->SetParticleSize(10.f, 10.0f);
			BossHandFire->GetComponent<Particle>()->AddParticleColor(1.5f, 7.5f, 0.4f);
			BossHandFire->GetComponent<Particle>()->SetActive(false);

			BossHandFire->SetParent(_handFire[i]);

			_particleBossfire2.emplace_back(BossHandFire);
		}

		{
			// 분신용 hand fire2
			std::string index = "ParticleEgoHandFire2_" + std::to_string(i + 1);
			auto EgoHandFire = _boss->GetObjectScene()->CreateObject(index);
			EgoHandFire->_autoAwake = true;
			EgoHandFire->GetComponent<Transform>()->SetPosition(0.0f, mapOffsetY, 0.0f);
			EgoHandFire->AddComponent<Particle>();
			EgoHandFire->GetComponent<Particle>()->SetParticleEffect("BlastWave3", "Resources/Textures/Particles/fx_BlastWave3.dds", 1000);
			EgoHandFire->GetComponent<Particle>()->SetParticleDuration(1.0f, 0.7f);
			EgoHandFire->GetComponent<Particle>()->SetParticleVelocity(10.0f, true);
			EgoHandFire->GetComponent<Particle>()->SetParticleSize(10.f, 10.0f);
			EgoHandFire->GetComponent<Particle>()->AddParticleColor(1.5f, 7.5f, 0.4f);
			EgoHandFire->GetComponent<Particle>()->SetActive(false);

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
			call2_1->_autoAwake = true;
			call2_1->GetComponent<Transform>()->SetPosition(0, mapOffsetY, 0);
			call2_1->AddComponent<Particle>();
			call2_1->GetComponent<Particle>()->SetParticleEffect("fx_Halo3", "Resources/Textures/Particles/fx_Halo3.dds", 1000);
			call2_1->GetComponent<Particle>()->SetParticleDuration(1.2f, 2.0f);
			call2_1->GetComponent<Particle>()->SetParticleVelocity(3.0f, true);
			call2_1->GetComponent<Particle>()->SetParticleSize(22.f, 15.0f);
			call2_1->GetComponent<Particle>()->AddParticleColor(1.0f, 5.f, 0.0f);
			call2_1->GetComponent<Particle>()->SetParticleDirection(0.0f, 90.0f, 0.0f);
			call2_1->GetComponent<Particle>()->SetActive(false);

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
			call2_2->_autoAwake = true;
			call2_2->GetComponent<Transform>()->SetPosition(0, mapOffsetY, 0.f);
			call2_2->AddComponent<Particle>();
			call2_2->GetComponent<Particle>()->SetParticleEffect("fx_Sparks1", "Resources/Textures/Particles/fx_Sparks1.dds", 1000);
			call2_2->GetComponent<Particle>()->SetParticleDuration(1.3f, 1.6f);
			call2_2->GetComponent<Particle>()->SetParticleVelocity(3.0f, true);
			call2_2->GetComponent<Particle>()->SetParticleSize(20.f, 20.f);
			call2_2->GetComponent<Particle>()->AddParticleColor(0.0f, 5.0f, 0.0f);
			call2_2->GetComponent<Particle>()->SetParticleDirection(0.0f, 82.0f, 0.0f);
			call2_2->GetComponent<Particle>()->SetActive(false);

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
			call2_3->_autoAwake = true;
			call2_3->GetComponent<Transform>()->SetPosition(0.0f, mapOffsetY, 0.0f);
			call2_3->AddComponent<Particle>();
			call2_3->GetComponent<Particle>()->SetParticleEffect("fx_Sparks1", "Resources/Textures/Particles/fx_Sparks1.dds", 1000);
			call2_3->GetComponent<Particle>()->SetParticleDuration(2.0f, 1.0f);
			call2_3->GetComponent<Particle>()->SetParticleVelocity(3.f, true);
			call2_3->GetComponent<Particle>()->SetParticleSize(18.f, 18.0f);
			call2_3->GetComponent<Particle>()->AddParticleColor(1.0f, 1.0f, 1.0f);
			call2_3->GetComponent<Particle>()->SetParticleDirection(0.0f, 81.0f, 0.0f);
			call2_3->GetComponent<Particle>()->SetActive(false);

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
		for (int i = 0; i < 21; i++)
		{
			std::string name = "swordCyllinder" + std::to_string(i + 1);
			auto swordCylinder = _boss->GetObjectScene()->CreateObject(name);
			swordCylinder->_autoAwake = true;
			swordCylinder->SetParent(_swordInsideAttack);
			swordCylinder->GetComponent<Transform>()->SetPosition(0.0f, 2.0f, 0.0f);
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

			donutParticle->_autoAwake = true;
			donutParticle->AddComponent<Particle>();
			donutParticle->GetComponent<Particle>()->SetParticleEffect("BlastWave5", "Resources/Textures/Particles/fx_BlastWave5.dds", 1000);
			donutParticle->GetComponent<Particle>()->SetParticleDuration(0.8f, 1.0f);
			donutParticle->GetComponent<Particle>()->SetParticleVelocity(0.0f, true);
			donutParticle->GetComponent<Particle>()->AddParticleColor(0.0f, 1.0f, 0.3f);
			donutParticle->GetComponent<Particle>()->SetParticleDirection(0.0f, 300.0f, 0.0f);
			donutParticle->GetComponent<Particle>()->SetParticleCameraApply(true);
			donutParticle->GetComponent<Particle>()->SetActive(false);
			donutParticle->SetTotalComponentState(false);


			if (i == 0)
			{
				//donutParticle->SetParent(_swordDonutWarning1);
				donutParticle->GetComponent<Particle>()->SetParticleSize(50.f, 50.0f);
			}
			else if (i == 1)
			{
				//donutParticle->SetParent(_swordDonutWarning2);
				donutParticle->GetComponent<Particle>()->SetParticleSize(120.f, 120.f);
			}
			else
			{
				//donutParticle->SetParent(_swordDonutWarning3);
				donutParticle->GetComponent<Particle>()->SetParticleSize(220.f, 220.f);
			}

			_particleSwordDonut1.emplace_back(donutParticle);
		}
	}
	{
		for (int i = 0; i < 3; i++)
		{
			std::string name = "donutParticle2" + std::to_string(i + 1);
			auto donutParticle2 = _boss->GetObjectScene()->CreateObject(name);

			donutParticle2->_autoAwake = true;
			donutParticle2->AddComponent<Particle>();
			donutParticle2->GetComponent<Particle>()->SetParticleEffect("BlastWave5", "Resources/Textures/Particles/fx_BlastWave5.dds", 1000);
			donutParticle2->GetComponent<Particle>()->SetParticleDuration(0.8f, 1.0f);
			donutParticle2->GetComponent<Particle>()->SetParticleVelocity(0.0f, true);
			donutParticle2->GetComponent<Particle>()->AddParticleColor(0.0f, 1.0f, 0.3f);
			donutParticle2->GetComponent<Particle>()->SetParticleDirection(0.0f, 300.0f, 0.0f);
			donutParticle2->GetComponent<Particle>()->SetParticleCameraApply(true);
			donutParticle2->GetComponent<Particle>()->SetActive(false);
			donutParticle2->SetTotalComponentState(false);

			if (i == 0)
			{
				donutParticle2->GetComponent<Particle>()->SetParticleSize(50.f, 50.0f);
			}
			else if (i == 1)
			{
				donutParticle2->GetComponent<Particle>()->SetParticleSize(100.f, 100.f);
			}
			else
			{
				donutParticle2->GetComponent<Particle>()->SetParticleSize(190.f, 190.f);
			}

			_particleSwordDonut2.emplace_back(donutParticle2);
		}


		for (int i = 0; i < 3; i++)
		{
			std::string name = "donutParticle3" + std::to_string(i + 1);
			auto donutParticle3 = _boss->GetObjectScene()->CreateObject(name);

			donutParticle3->_autoAwake = true;
			donutParticle3->AddComponent<Particle>();
			donutParticle3->GetComponent<Particle>()->SetParticleEffect("BlastWave5", "Resources/Textures/Particles/fx_BlastWave5.dds", 1000);
			donutParticle3->GetComponent<Particle>()->SetParticleDuration(0.8f, 1.0f);
			donutParticle3->GetComponent<Particle>()->SetParticleVelocity(0.0f, true);
			donutParticle3->GetComponent<Particle>()->AddParticleColor(0.3f, 1.0f, 0.0f);
			donutParticle3->GetComponent<Particle>()->SetParticleDirection(0.0f, 300.0f, 0.0f);
			donutParticle3->GetComponent<Particle>()->SetParticleCameraApply(true);
			donutParticle3->SetTotalComponentState(false);

			if (i == 0)
			{
				donutParticle3->GetComponent<Particle>()->SetParticleSize(45.0f, 45.0f);
			}
			else if (i == 1)
			{
				donutParticle3->GetComponent<Particle>()->SetParticleSize(110.f, 110.0f);
			}
			else
			{
				donutParticle3->GetComponent<Particle>()->SetParticleSize(205.f, 205.0f);
			}

			_particleSwordDonut3.emplace_back(donutParticle3);
		}

	}
	{
		{
			std::string name = "teleportParticle";
			auto teleportParticle = _boss->GetObjectScene()->CreateObject(name);
			teleportParticle->SetParent(_bossInsideAttack);

			teleportParticle->_autoAwake = true;
			teleportParticle->AddComponent<Particle>();
			teleportParticle->GetComponent<Particle>()->SetParticleEffect("BlastWave1", "Resources/Textures/Particles/fx_BlastWave1.dds", 1000);
			teleportParticle->GetComponent<Particle>()->SetParticleDuration(1.0f, 1.5f);
			teleportParticle->GetComponent<Particle>()->SetParticleVelocity(10.0f, true);
			teleportParticle->GetComponent<Particle>()->SetParticleSize(15.f, 15.0f);
			teleportParticle->GetComponent<Particle>()->SetParticleDirection(0.0f, 300.0f, 0.0f);
			teleportParticle->GetComponent<Particle>()->AddParticleColor(0.05f, 0.1f, 0.0f);
			teleportParticle->GetComponent<Particle>()->SetParticleCameraApply(true);
			teleportParticle->SetTotalComponentState(false);
		}

		for (int i = 0; i < 2; i++)
		{
			std::string name = "teleportParticle" + std::to_string(i + 1);
			auto teleportParticle = _boss->GetObjectScene()->CreateObject(name);
			teleportParticle->SetParent(_bossInsideAttack);

			teleportParticle->_autoAwake = true;
			teleportParticle->AddComponent<Particle>();
			teleportParticle->GetComponent<Particle>()->SetParticleEffect("fx_BlastWave3", "Resources/Textures/Particles/fx_BlastWave3.dds", 1000);
			teleportParticle->GetComponent<Particle>()->SetParticleDuration(1.0f, 0.2f);
			teleportParticle->GetComponent<Particle>()->SetParticleVelocity(10.0f, true);
			teleportParticle->GetComponent<Particle>()->AddParticleColor(0.1f, 0.1f, 0.04f);
			teleportParticle->GetComponent<Particle>()->SetParticleDirection(0.0f, 0.0f, 0.0f);
			teleportParticle->GetComponent<Particle>()->SetParticleSize(15.f, 15.0f);
			teleportParticle->GetComponent<Particle>()->SetParticleCameraApply(true);
			teleportParticle->SetTotalComponentState(false);

			//if (i == 0)
			//{
			//	teleportParticle->GetComponent<Particle>()->SetParticleAngle(80.0f, 0.0f, 0.0f);
			//}
			//else if (i == 1)
			//{
			//	teleportParticle->GetComponent<Particle>()->SetParticleAngle(260.0f, 0.0f, 0.0f);
			//}
			_particleEmergenceAttack.emplace_back(teleportParticle);
		}
		{
			std::string name = "teleportParticle3";
			auto teleportParticle = _boss->GetObjectScene()->CreateObject(name);
			teleportParticle->SetParent(_bossInsideAttack);

			teleportParticle->_autoAwake = true;
			teleportParticle->AddComponent<Particle>();
			teleportParticle->GetComponent<Particle>()->SetParticleEffect("Halo2", "Resources/Textures/Particles/fx_Halo2.dds", 1000);
			teleportParticle->GetComponent<Particle>()->SetParticleDuration(1.0f, 1.5f);
			teleportParticle->GetComponent<Particle>()->SetParticleVelocity(10.0f, true);
			teleportParticle->GetComponent<Particle>()->SetParticleSize(15.f, 15.0f);
			teleportParticle->GetComponent<Particle>()->SetParticleDirection(0.0f, 300.0f, 0.0f);
			teleportParticle->GetComponent<Particle>()->AddParticleColor(1.0f, 1.0f, 0.2f);
			teleportParticle->GetComponent<Particle>()->SetParticleCameraApply(true);
			teleportParticle->SetTotalComponentState(false);
			_particleEmergenceAttack.emplace_back(teleportParticle);
		}
	}
	{
		{
			std::string name = "teleportEgoParticle";
			auto teleportEgoParticle = _boss->GetObjectScene()->CreateObject(name);
			teleportEgoParticle->SetParent(_egoInsideAttack);

			teleportEgoParticle->_autoAwake = true;
			teleportEgoParticle->AddComponent<Particle>();
			teleportEgoParticle->GetComponent<Particle>()->SetParticleEffect("BlastWave1", "Resources/Textures/Particles/fx_BlastWave1.dds", 1000);
			teleportEgoParticle->GetComponent<Particle>()->SetParticleDuration(1.0f, 1.5f);
			teleportEgoParticle->GetComponent<Particle>()->SetParticleVelocity(10.0f, true);
			teleportEgoParticle->GetComponent<Particle>()->SetParticleSize(15.f, 15.0f);
			teleportEgoParticle->GetComponent<Particle>()->SetParticleDirection(0.0f, 300.0f, 0.0f);
			teleportEgoParticle->GetComponent<Particle>()->AddParticleColor(0.05f, 0.1f, 0.0f);
			teleportEgoParticle->GetComponent<Particle>()->SetParticleCameraApply(true);
			teleportEgoParticle->SetTotalComponentState(false);
			_particleEgoEmergenceAttack.emplace_back(teleportEgoParticle);
		}

		for (int i = 0; i < 2; i++)
		{
			std::string name = "teleportEgoParticle" + std::to_string(i + 1);
			auto teleportEgoParticle = _boss->GetObjectScene()->CreateObject(name);
			teleportEgoParticle->SetParent(_egoInsideAttack);

			teleportEgoParticle->_autoAwake = true;
			teleportEgoParticle->AddComponent<Particle>();
			teleportEgoParticle->GetComponent<Particle>()->SetParticleEffect("fx_BlastWave3", "Resources/Textures/Particles/fx_BlastWave3.dds", 1000);
			teleportEgoParticle->GetComponent<Particle>()->SetParticleDuration(1.0f, 0.2f);
			teleportEgoParticle->GetComponent<Particle>()->SetParticleVelocity(10.0f, true);
			teleportEgoParticle->GetComponent<Particle>()->AddParticleColor(0.1f, 0.1f, 0.04f);
			teleportEgoParticle->GetComponent<Particle>()->SetParticleDirection(0.0f, 0.0f, 0.0f);
			teleportEgoParticle->GetComponent<Particle>()->SetParticleSize(15.f, 15.0f);
			teleportEgoParticle->GetComponent<Particle>()->SetParticleCameraApply(true);
			teleportEgoParticle->SetTotalComponentState(false);
			_particleEgoEmergenceAttack.emplace_back(teleportEgoParticle);
		}
		{
			std::string name = "teleportEgoParticle3";
			auto teleportEgoParticle = _boss->GetObjectScene()->CreateObject(name);
			teleportEgoParticle->SetParent(_egoInsideAttack);

			teleportEgoParticle->_autoAwake = true;
			teleportEgoParticle->AddComponent<Particle>();
			teleportEgoParticle->GetComponent<Particle>()->SetParticleEffect("Halo2", "Resources/Textures/Particles/fx_Halo2.dds", 1000);
			teleportEgoParticle->GetComponent<Particle>()->SetParticleDuration(1.0f, 1.5f);
			teleportEgoParticle->GetComponent<Particle>()->SetParticleVelocity(10.0f, true);
			teleportEgoParticle->GetComponent<Particle>()->SetParticleSize(15.f, 15.0f);
			teleportEgoParticle->GetComponent<Particle>()->SetParticleDirection(0.0f, 300.0f, 0.0f);
			teleportEgoParticle->GetComponent<Particle>()->AddParticleColor(1.0f, 1.0f, 0.2f);
			teleportEgoParticle->GetComponent<Particle>()->SetParticleCameraApply(true);
			teleportEgoParticle->SetTotalComponentState(false);
			_particleEgoEmergenceAttack.emplace_back(teleportEgoParticle);
		}
	}
	{
		{
			std::string name = "insidePillarBoss";
			auto particleBossPillar1 = _boss->GetObjectScene()->CreateObject(name);
			particleBossPillar1->SetParent(_swordInsideAttack);

			particleBossPillar1->_autoAwake = true;
			particleBossPillar1->AddComponent<Particle>();
			particleBossPillar1->GetComponent<Particle>()->SetParticleEffect("Halo1", "Resources/Textures/Particles/fx_Halo1.dds", 1000);
			particleBossPillar1->GetComponent<Particle>()->SetParticleDuration(2.1f, 1.f);
			particleBossPillar1->GetComponent<Particle>()->SetParticleVelocity(10.0f, true);
			particleBossPillar1->GetComponent<Particle>()->SetParticleSize(80.f, 80.0f);
			particleBossPillar1->GetComponent<Particle>()->AddParticleColor(0.6f, 0.6f, 0.6f);
			particleBossPillar1->GetComponent<Particle>()->SetParticleDirection(0.0f, 160.0f, 0.0f);
			particleBossPillar1->GetComponent<Particle>()->SetParticleCameraApply(true);
			particleBossPillar1->SetTotalComponentState(false);
		}
		{
			std::string name = "insidePillarBoss";
			auto particleBossPillar2 = _boss->GetObjectScene()->CreateObject(name);
			particleBossPillar2->SetParent(_swordInsideAttack);

			particleBossPillar2->_autoAwake = true;
			particleBossPillar2->AddComponent<Particle>();
			particleBossPillar2->GetComponent<Particle>()->SetParticleEffect("Sparks1", "Resources/Textures/Particles/fx_Sparks1.dds", 1000);
			particleBossPillar2->GetComponent<Particle>()->SetParticleDuration(2.f, 1.2f);
			particleBossPillar2->GetComponent<Particle>()->SetParticleVelocity(1.0f, true);
			particleBossPillar2->GetComponent<Particle>()->SetParticleSize(120.f, 120.0f);
			particleBossPillar2->GetComponent<Particle>()->AddParticleColor(0.3f, 1.5f, 0.0f);
			particleBossPillar2->GetComponent<Particle>()->SetParticleDirection(0.0f, 160.0f, 0.0f);
			particleBossPillar2->GetComponent<Particle>()->SetParticleCameraApply(true);
			particleBossPillar2->SetTotalComponentState(false);
		}
		{
			std::string name = "insidePillarBoss";
			auto particleBossPillar3 = _boss->GetObjectScene()->CreateObject(name);
			particleBossPillar3->SetParent(_swordInsideAttack);

			particleBossPillar3->_autoAwake = true;
			particleBossPillar3->AddComponent<Particle>();
			particleBossPillar3->GetComponent<Particle>()->SetParticleEffect("Blaster2", "Resources/Textures/Particles/fx_Blaster2.dds", 1000);
			particleBossPillar3->GetComponent<Particle>()->SetParticleDuration(2.0f, 1.15f);
			particleBossPillar3->GetComponent<Particle>()->SetParticleVelocity(0.4f, true);
			particleBossPillar3->GetComponent<Particle>()->SetParticleSize(52.f, 40.0f);
			particleBossPillar3->GetComponent<Particle>()->AddParticleColor(0.0f, 0.0f, 2.0f);
			particleBossPillar3->GetComponent<Particle>()->SetParticleDirection(0.0f, 160.0f, 0.0f);
			particleBossPillar3->GetComponent<Particle>()->SetParticleCameraApply(true);
			particleBossPillar3->SetTotalComponentState(false);
		}
	}
	{
		std::string name = "lazerParticle";
		auto lazerParticle = _boss->GetObjectScene()->CreateObject(name);
		lazerParticle->SetParent(_lazer);

		lazerParticle->_autoAwake = true;
		lazerParticle->AddComponent<Particle>();
		lazerParticle->GetComponent<Particle>()->SetParticleEffect("fx_BlastWave3", "Resources/Textures/Particles/fx_BlastWave3.dds", 1000);
		lazerParticle->GetComponent<Particle>()->SetParticleDuration(2.0f, 1.15f);
		lazerParticle->GetComponent<Particle>()->SetParticleVelocity(84.f, false);
		lazerParticle->GetComponent<Particle>()->SetParticleSize(40.f, 40.0f);
		lazerParticle->GetComponent<Particle>()->AddParticleColor(0.8f, 1.0f, 0.0f);
		lazerParticle->GetComponent<Particle>()->SetParticleRotation(90.0f, _bossTransform->GetRotation().y, 0.0f);
		//lazerParticle->GetComponent<Particle>()->SetParticleCameraApply(true);
		lazerParticle->SetTotalComponentState(false);
	}
	{
		std::string name = "egoLazerParticle";
		auto lazerParticle = _boss->GetObjectScene()->CreateObject(name);
		lazerParticle->SetParent(_egoLazer);

		lazerParticle->_autoAwake = true;
		lazerParticle->AddComponent<Particle>();
		lazerParticle->GetComponent<Particle>()->SetParticleEffect("fx_BlastWave3", "Resources/Textures/Particles/fx_BlastWave3.dds", 1000);
		lazerParticle->GetComponent<Particle>()->SetParticleDuration(2.0f, 1.15f);
		lazerParticle->GetComponent<Particle>()->SetParticleVelocity(84.f, false);
		lazerParticle->GetComponent<Particle>()->SetParticleSize(40.f, 40.0f);
		lazerParticle->GetComponent<Particle>()->AddParticleColor(0.8f, 1.0f, 0.0f);
		lazerParticle->GetComponent<Particle>()->SetParticleRotation(90.0f, _bossTransform->GetRotation().y, 0.0f);
		//lazerParticle->GetComponent<Particle>()->SetParticleCameraApply(true);
		lazerParticle->SetTotalComponentState(false);
	}
	{
		for (int i = 0; i < _fakeBoss.size(); ++i)
		{
			std::string name1 = "fakeBossParticle1(" + std::to_string(i) + ")";
			auto fakeBossParticle1 = _boss->GetObjectScene()->CreateObject(name1);
			fakeBossParticle1->SetParent(_fakeBoss[i]);
			fakeBossParticle1->_autoAwake = true;
			fakeBossParticle1->AddComponent<Particle>();
			fakeBossParticle1->GetComponent<Particle>()->SetParticleEffect("Lightning2", "Resources/Textures/Particles/fx_Lightning2.dds", 1000);
			fakeBossParticle1->GetComponent<Particle>()->SetParticleDuration(0.4f, 2.0f);
			fakeBossParticle1->GetComponent<Particle>()->SetParticleVelocity(10.0f, true);
			fakeBossParticle1->GetComponent<Particle>()->SetParticleSize(15.f, 15.0f);
			fakeBossParticle1->GetComponent<Particle>()->AddParticleColor(1.0f, 6.f, 0.0f);
			fakeBossParticle1->GetComponent<Particle>()->SetParticleDirection(0.0f, 40.0f, 0.0f);
			fakeBossParticle1->GetComponent<Transform>()->SetPosition(-0.272f, 18.527f, -3.0f);
			fakeBossParticle1->GetComponent<Particle>()->SetActive(false);

			//std::string name2 = "fakeBossParticle2(" + std::to_string(i) + ")";
			//auto fakeBossParticle2 = _boss->GetObjectScene()->CreateObject(name2);
			//fakeBossParticle2->SetParent(_fakeBoss[i]);
			//fakeBossParticle2->AddComponent<Particle>();
			//fakeBossParticle2->GetComponent<Particle>()->SetParticleEffect("fx_BlastWave5", "Resources/Textures/Particles/fx_BlastWave5.dds", 1000);
			//fakeBossParticle2->GetComponent<Particle>()->SetParticleDuration(2.0f, 4.0f);
			//fakeBossParticle2->GetComponent<Particle>()->SetParticleVelocity(3.0f, true);
			//fakeBossParticle2->GetComponent<Particle>()->SetParticleSize(15.0f, 35.0f);
			//fakeBossParticle2->GetComponent<Particle>()->AddParticleColor(0.0f, 0.2f, 1.0f);
			//fakeBossParticle2->GetComponent<Particle>()->SetParticleDirection(0.0f, 0.0f, 0.0f);
			//fakeBossParticle2->GetComponent<Transform>()->SetPosition(0, 13.0f, 0.0f);
			//fakeBossParticle2->SetActive(false);

			//std::string name3 = "fakeBossParticle3(" + std::to_string(i) + ")";
			//auto fakeBossParticle3 = _boss->GetObjectScene()->CreateObject(name3);
			//fakeBossParticle3->SetParent(_fakeBoss[i]);
			//fakeBossParticle3->AddComponent<Particle>();
			//fakeBossParticle3->GetComponent<Particle>()->SetParticleEffect("fx_BlastWave3", "Resources/Textures/Particles/fx_BlastWave3.dds", 1000);
			//fakeBossParticle3->GetComponent<Particle>()->SetParticleDuration(2.0f, 4.0f);
			//fakeBossParticle3->GetComponent<Particle>()->SetParticleVelocity(3.0f, true);
			//fakeBossParticle3->GetComponent<Particle>()->SetParticleSize(15.0f, 35.0f);
			//fakeBossParticle3->GetComponent<Particle>()->AddParticleColor(0.2f, 1.0f, 0.0f);
			//fakeBossParticle3->GetComponent<Particle>()->SetParticleDirection(0.0f, 0.0f, 0.0f);
			//fakeBossParticle3->GetComponent<Transform>()->SetPosition(0, 13.0f, 0.0f);
			//fakeBossParticle3->SetActive(false);

			std::string name4 = "fakeBossParticle4(" + std::to_string(i) + ")";
			auto fakeBossParticle4 = _boss->GetObjectScene()->CreateObject(name4);
			fakeBossParticle4->SetParent(_fakeBoss[i]);
			fakeBossParticle4->_autoAwake = true;
			fakeBossParticle4->AddComponent<Particle>();
			fakeBossParticle4->GetComponent<Particle>()->SetParticleEffect("Lightning2", "Resources/Textures/Particles/fx_Lightning2.dds", 1000);
			fakeBossParticle4->GetComponent<Particle>()->SetParticleDuration(0.5f, 1.0f);
			fakeBossParticle4->GetComponent<Particle>()->SetParticleVelocity(10.0f, true);
			fakeBossParticle4->GetComponent<Particle>()->SetParticleSize(15.f, 40.0f);
			fakeBossParticle4->GetComponent<Particle>()->AddParticleColor(0.2f, 1.0f, 0.0f);
			fakeBossParticle4->GetComponent<Particle>()->SetParticleDirection(0.0f, 70.0f, 0.0f);
			fakeBossParticle4->GetComponent<Transform>()->SetPosition(0, 13.4, 0);
			fakeBossParticle4->GetComponent<Particle>()->SetActive(false);
		}
	}
	{
		std::string name = "verticalParticleSide1";
		auto verticalParticleside = _boss->GetObjectScene()->CreateObject(name);

		verticalParticleside->_autoAwake = true;
		verticalParticleside->GetComponent<Transform>()->SetPosition(-48, -4.4, -65.f);
		verticalParticleside->AddComponent<Particle>();
		verticalParticleside->GetComponent<Particle>()->SetParticleEffect("BlastWave5", "Resources/Textures/Particles/fx_BlastWave5.dds", 1000);
		verticalParticleside->GetComponent<Particle>()->SetParticleDuration(2.0f, 0.2f);
		verticalParticleside->GetComponent<Particle>()->SetParticleVelocity(0.0f, true);
		verticalParticleside->GetComponent<Particle>()->SetParticleSize(35.f, 35.0f);
		verticalParticleside->GetComponent<Particle>()->AddParticleColor(1.0f, 1.0f, 0.3f);
		verticalParticleside->GetComponent<Particle>()->SetParticleDirection(0.0f, 600.0f, 0.0f);
		verticalParticleside->GetComponent<Particle>()->SetParticleCameraApply(true);
		verticalParticleside->GetComponent<Particle>()->SetActive(false);

		_verticalParticleSide.emplace_back(verticalParticleside);
	}
	{
		std::string name = "verticalParticleSide2";
		auto verticalParticleside = _boss->GetObjectScene()->CreateObject(name);

		verticalParticleside->_autoAwake = true;
		verticalParticleside->GetComponent<Transform>()->SetPosition(-48, -4.4, -65.f);
		verticalParticleside->AddComponent<Particle>();
		verticalParticleside->GetComponent<Particle>()->SetParticleEffect("fx_Halo1", "Resources/Textures/Particles/fx_Halo1.dds", 1000);
		verticalParticleside->GetComponent<Particle>()->SetParticleDuration(2.0f, 0.2f);
		verticalParticleside->GetComponent<Particle>()->SetParticleVelocity(0.0f, true);
		verticalParticleside->GetComponent<Particle>()->SetParticleSize(35.f, 35.0f);
		verticalParticleside->GetComponent<Particle>()->AddParticleColor(0.0f, 1.0f, 0.3f);
		verticalParticleside->GetComponent<Particle>()->SetParticleDirection(0.0f, 600.0f, 0.0f);
		verticalParticleside->GetComponent<Particle>()->SetParticleCameraApply(true);
		verticalParticleside->GetComponent<Particle>()->SetActive(false);

		_verticalParticleSide.emplace_back(verticalParticleside);
	}
	{
		std::string name = "verticalParticle1";
		auto verticalParticle = _boss->GetObjectScene()->CreateObject(name);

		verticalParticle->_autoAwake = true;
		verticalParticle->AddComponent<Particle>();
		verticalParticle->GetComponent<Particle>()->SetParticleEffect("Flare6", "Resources/Textures/Particles/fx_Twister3.dds", 1000);
		verticalParticle->GetComponent<Particle>()->SetParticleDuration(2.4f, 2.0f);
		verticalParticle->GetComponent<Particle>()->SetParticleVelocity(1.0f, true);
		verticalParticle->GetComponent<Particle>()->SetParticleSize(1.f, 1.f);
		verticalParticle->GetComponent<Particle>()->AddParticleColor(1.f, 1.f, 1.f);
		verticalParticle->GetComponent<Particle>()->SetParticleDirection(0.0f, 400.0f, 0.0f);
		verticalParticle->GetComponent<Particle>()->SetActive(false);

		_verticalParticle.emplace_back(verticalParticle);
	}
	{
		std::string name = "verticalParticle2";
		auto verticalParticle = _boss->GetObjectScene()->CreateObject(name);

		verticalParticle->_autoAwake = true;
		verticalParticle->AddComponent<Particle>();
		verticalParticle->GetComponent<Particle>()->SetParticleEffect("Flare6", "Resources/Textures/Particles/fx_Twister3.dds", 1000);
		verticalParticle->GetComponent<Particle>()->SetParticleDuration(2.4f, 2.0f);
		verticalParticle->GetComponent<Particle>()->SetParticleVelocity(1.0f, true);
		verticalParticle->GetComponent<Particle>()->SetParticleSize(1.f, 1.0f);
		verticalParticle->GetComponent<Particle>()->AddParticleColor(0.3f, 2.0f, 0.1f);
		verticalParticle->GetComponent<Particle>()->SetParticleDirection(0.0f, 600.0f, 0.0f);
		verticalParticle->GetComponent<Particle>()->SetParticleCameraApply(true);
		verticalParticle->GetComponent<Particle>()->SetActive(false);

		_verticalParticle.emplace_back(verticalParticle);
	}

	{
		std::string name = "verticalParticle2test";
		auto verticalParticle = _boss->GetObjectScene()->CreateObject(name);

		verticalParticle->_autoAwake = true;
		verticalParticle->AddComponent<Particle>();
		verticalParticle->GetComponent<Particle>()->SetParticleEffect("fx_Beam3", "Resources/Textures/Particles/fx_Beam3.dds", 1000);
		verticalParticle->GetComponent<Particle>()->SetParticleDuration(0.4f, 0.3f);
		verticalParticle->GetComponent<Particle>()->SetParticleVelocity(10.0f, true);
		verticalParticle->GetComponent<Particle>()->SetParticleSize(12.f, 12.0f);
		verticalParticle->GetComponent<Particle>()->AddParticleColor(0.5f, 5.0f, 0.0f);
		verticalParticle->GetComponent<Particle>()->SetParticleDirection(0.0f, 0.0f, 0.0f);
		verticalParticle->GetComponent<Particle>()->SetParticleAngle(300.0f, 0.0f, 0.0f);
		verticalParticle->GetComponent<Particle>()->SetParticleCameraApply(true);
		verticalParticle->GetComponent<Particle>()->SetActive(false);
		verticalParticle->SetParent(_kamenSword);


		verticalParticle->GetComponent<Particle>()->SetTransform(_boss, "MiddleFinger1_R");

		_kamenSwordParticle.emplace_back(verticalParticle);
	}
	{
		for (int i = 0; i < 9; i++)
		{
			std::string name = "verticalParticle2test";
			auto verticalParticle = _boss->GetObjectScene()->CreateObject(name);

			verticalParticle->_autoAwake = true;
			verticalParticle->AddComponent<Particle>();
			verticalParticle->GetComponent<Particle>()->SetParticleEffect("fx_Beam4", "Resources/Textures/Particles/fx_Beam4.dds", 1000);
			verticalParticle->GetComponent<Particle>()->SetParticleDuration(2.0f, 0.1f);
			verticalParticle->GetComponent<Particle>()->SetParticleVelocity(1.0f, false);
			verticalParticle->GetComponent<Particle>()->SetParticleSize(8.f, 8.0f);
			verticalParticle->GetComponent<Particle>()->AddParticleColor(0.0f, 2.0f, 0.0f);
			verticalParticle->GetComponent<Particle>()->SetParticleDirection(0.0f, 0.0f, 0.0f);
			verticalParticle->GetComponent<Particle>()->SetParticleAngle(300.0f, 0.0f, 0.0f);
			verticalParticle->GetComponent<Particle>()->SetParticleCameraApply(true);
			verticalParticle->GetComponent<Particle>()->SetActive(false);
			verticalParticle->SetParent(_kamenSword);

			verticalParticle->GetComponent<Particle>()->SetTransform(_boss, "MiddleFinger1_R");

			switch (i)
			{
				case 0:
					verticalParticle->GetComponent<Particle>()->SetParticleDuration(0.2f, 10.f);
					break;
				case 1:
					verticalParticle->GetComponent<Particle>()->SetParticleDuration(0.3f, 10.f);
					break;
				case 2:
					verticalParticle->GetComponent<Particle>()->SetParticleDuration(0.4f, 10.f);
					break;
				case 3:
					verticalParticle->GetComponent<Particle>()->SetParticleDuration(0.5f, 10.f);
					break;
				case 4:
					verticalParticle->GetComponent<Particle>()->SetParticleDuration(0.6f, 10.f);
					break;
				case 5:
					verticalParticle->GetComponent<Particle>()->SetParticleDuration(0.7f, 10.f);
					break;
				case 6:
					verticalParticle->GetComponent<Particle>()->SetParticleDuration(0.8f, 10.f);
					break;
				case 7:
					verticalParticle->GetComponent<Particle>()->SetParticleDuration(0.9f, 10.f);
					break;
				case 8:
					verticalParticle->GetComponent<Particle>()->SetParticleDuration(1.0f, 10.f);
					break;
				default:
					break;
			}

			_kamenSwordAfterImageParticle.emplace_back(verticalParticle);
		}
	}
	{
		for (int i = 0; i < _blade.size(); i++)
		{

			for (int j = 0; j < 13; j++)
			{

				std::string name = "bladePrticle";
				auto bladePrticle = _boss->GetObjectScene()->CreateObject(name);

				bladePrticle->_autoAwake = true;
				bladePrticle->AddComponent<Particle>();
				bladePrticle->GetComponent<Particle>()->SetParticleEffect("fx_Beam4", "Resources/Textures/Particles/fx_Beam4.dds", 1000);
				bladePrticle->GetComponent<Particle>()->SetParticleDuration(0.1f, 1.0f);
				bladePrticle->GetComponent<Particle>()->SetParticleVelocity(30.0f, true);
				bladePrticle->GetComponent<Particle>()->SetParticleSize(30.f, 20.0f);
				bladePrticle->GetComponent<Particle>()->AddParticleColor(0.3f, 6.0f, 0.0f);
				bladePrticle->GetComponent<Particle>()->SetParticleDirection(0.0f, 0.0f, 0.0f);
				bladePrticle->GetComponent<Particle>()->SetParticleCameraApply(true);
				bladePrticle->GetComponent<Particle>()->SetParticleAngle(0.f, 0.0f, 0.0f);
				bladePrticle->GetComponent<Particle>()->SetActive(false);
				bladePrticle->AddComponent<Light>();
				bladePrticle->GetComponent<Light>()->CreatePointLight(Ambient, Diffuse, Specular, 300);
				bladePrticle->GetComponent<Light>()->SetDiffuse(0.15f, 0.5f, 0.05f, 1.0f);
				bladePrticle->GetComponent<Light>()->SetActive(false);

				switch (j)
				{
					case 0:
						bladePrticle->GetComponent<Transform>()->SetPosition(-60.0f, 0.0f, 18.0f);
						break;
					case 1:
						bladePrticle->GetComponent<Transform>()->SetPosition(-50.0f, 0.0f, 15.0f);
						break;
					case 2:
						bladePrticle->GetComponent<Transform>()->SetPosition(-40.0f, 0.0f, 12.0f);
						break;
					case 3:
						bladePrticle->GetComponent<Transform>()->SetPosition(-30.0f, 0.0f, 9.0f);
						break;
					case 4:
						bladePrticle->GetComponent<Transform>()->SetPosition(-20.0f, 0.0f, 6.0f);
						break;
					case 5:
						bladePrticle->GetComponent<Transform>()->SetPosition(-10.0f, 0.0f, 3.0f);
						break;
					case 6:
						bladePrticle->GetComponent<Transform>()->SetPosition(0.0f, 0.0f, 0.0f);
						break;
					case 7:
						bladePrticle->GetComponent<Transform>()->SetPosition(10.0f, 0.0f, 3.0f);
						break;
					case 8:
						bladePrticle->GetComponent<Transform>()->SetPosition(20.0f, 0.0f, 6.0f);
						break;
					case 9:
						bladePrticle->GetComponent<Transform>()->SetPosition(30.0f, 0.0f, 9.0f);
						break;
					case 10:
						bladePrticle->GetComponent<Transform>()->SetPosition(40.0f, 0.0f, 12.0f);
						break;
					case 11:
						bladePrticle->GetComponent<Transform>()->SetPosition(50.0f, 0.0f, 15.0f);
						break;
					case 12:
						bladePrticle->GetComponent<Transform>()->SetPosition(60.0f, 0.0f, 18.0f);
						break;
					default:
						break;
				}

				bladePrticle->SetParent(_blade[i]);
				if (i == 0)
				{
					_bladeRParticle.emplace_back(bladePrticle);
				}
				else if (i == 1)
				{
					_bladeLParticle.emplace_back(bladePrticle);
				}
			}
			for (int w = 0; w < 13; w++)
			{
				std::string name = "bladePrticleWave";
				auto bladePrticleWave = _boss->GetObjectScene()->CreateObject(name);

				bladePrticleWave->_autoAwake = true;
				bladePrticleWave->AddComponent<Particle>();
				bladePrticleWave->GetComponent<Particle>()->SetParticleEffect("fx_Lightning2", "Resources/Textures/Particles/fx_Lightning2.dds", 1000);
				bladePrticleWave->GetComponent<Particle>()->SetParticleDuration(0.3f, 1.0f);
				bladePrticleWave->GetComponent<Particle>()->SetParticleVelocity(50.0f, true);
				bladePrticleWave->GetComponent<Particle>()->SetParticleSize(30.f, 30.0f);
				bladePrticleWave->GetComponent<Particle>()->AddParticleColor(0.3f, 1.0f, 0.0f);
				bladePrticleWave->GetComponent<Particle>()->SetParticleDirection(0.0f, 0.0f, 0.0f);
				bladePrticleWave->GetComponent<Particle>()->SetParticleCameraApply(true);
				bladePrticleWave->GetComponent<Particle>()->SetParticleAngle(0.f, 0.0f, 0.0f);
				bladePrticleWave->GetComponent<Particle>()->SetActive(false);

				switch (w)
				{
					case 0:
						bladePrticleWave->GetComponent<Transform>()->SetPosition(-60.0f, 0.0f, 18.0f);
						break;
					case 1:
						bladePrticleWave->GetComponent<Transform>()->SetPosition(-50.0f, 0.0f, 15.0f);
						bladePrticleWave->GetComponent<Particle>()->SetParticleDuration(0.6f, 1.0f);
						break;
					case 2:
						bladePrticleWave->GetComponent<Transform>()->SetPosition(-40.0f, 0.0f, 12.0f);
						break;
					case 3:
						bladePrticleWave->GetComponent<Transform>()->SetPosition(-30.0f, 0.0f, 9.0f);
						break;
					case 4:
						bladePrticleWave->GetComponent<Transform>()->SetPosition(-20.0f, 0.0f, 6.0f);
						bladePrticleWave->GetComponent<Particle>()->SetParticleDuration(0.6f, 1.0f);
						break;
					case 5:
						bladePrticleWave->GetComponent<Transform>()->SetPosition(-10.0f, 0.0f, 3.0f);
						break;
					case 6:
						bladePrticleWave->GetComponent<Transform>()->SetPosition(0.0f, 0.0f, 0.0f);
						break;
					case 7:
						bladePrticleWave->GetComponent<Transform>()->SetPosition(10.0f, 0.0f, 3.0f);
						break;
					case 8:
						bladePrticleWave->GetComponent<Transform>()->SetPosition(20.0f, 0.0f, 6.0f);
						bladePrticleWave->GetComponent<Particle>()->SetParticleDuration(0.6f, 1.0f);
						break;
					case 9:
						bladePrticleWave->GetComponent<Transform>()->SetPosition(30.0f, 0.0f, 9.0f);
						break;
					case 10:
						bladePrticleWave->GetComponent<Transform>()->SetPosition(40.0f, 0.0f, 12.0f);
						break;
					case 11:
						bladePrticleWave->GetComponent<Transform>()->SetPosition(50.0f, 0.0f, 15.0f);
						bladePrticleWave->GetComponent<Particle>()->SetParticleDuration(0.6f, 1.0f);
						break;
					case 12:
						bladePrticleWave->GetComponent<Transform>()->SetPosition(60.0f, 0.0f, 18.0f);
						break;
					default:
						break;
				}

				bladePrticleWave->SetParent(_blade[i]);

				if (i == 0)
				{
					_bladeRParticleWave.emplace_back(bladePrticleWave);
				}
				else if (i == 1)
				{
					_bladeLParticleWave.emplace_back(bladePrticleWave);
				}

			}

		}
	}
	{
		for (int j = 0; j < 15; j++)
		{

			std::string name = "bladePrticle";
			auto bladePrticle = _boss->GetObjectScene()->CreateObject(name);

			bladePrticle->_autoAwake = true;
			bladePrticle->AddComponent<Particle>();
			bladePrticle->GetComponent<Particle>()->SetParticleEffect("fx_Beam4", "Resources/Textures/Particles/fx_Beam4.dds", 1000);
			bladePrticle->GetComponent<Particle>()->SetParticleDuration(0.1f, 1.0f);
			bladePrticle->GetComponent<Particle>()->SetParticleVelocity(30.0f, true);
			bladePrticle->GetComponent<Particle>()->SetParticleSize(30.f, 20.0f);
			bladePrticle->GetComponent<Particle>()->AddParticleColor(0.3f, 6.0f, 0.0f);
			bladePrticle->GetComponent<Particle>()->SetParticleDirection(0.0f, 0.0f, 0.0f);
			bladePrticle->GetComponent<Particle>()->SetParticleCameraApply(true);
			bladePrticle->GetComponent<Particle>()->SetParticleAngle(0.f, 0.0f, 0.0f);
			bladePrticle->GetComponent<Particle>()->SetActive(false);
			bladePrticle->AddComponent<Light>();
			bladePrticle->GetComponent<Light>()->CreatePointLight(Ambient, Diffuse, Specular, 300);
			bladePrticle->GetComponent<Light>()->SetDiffuse(0.15f, 0.5f, 0.05f, 1.0f);
			bladePrticle->GetComponent<Light>()->SetActive(false);

			switch (j)
			{
				case 0:
					bladePrticle->GetComponent<Transform>()->SetPosition(-70.0f, 0.0f, 21.0f);
					break;
				case 1:
					bladePrticle->GetComponent<Transform>()->SetPosition(-60.0f, 0.0f, 18.0f);
					break;
				case 2:
					bladePrticle->GetComponent<Transform>()->SetPosition(-50.0f, 0.0f, 15.0f);
					break;
				case 3:
					bladePrticle->GetComponent<Transform>()->SetPosition(-40.0f, 0.0f, 12.0f);
					break;
				case 4:
					bladePrticle->GetComponent<Transform>()->SetPosition(-30.0f, 0.0f, 9.0f);
					break;
				case 5:
					bladePrticle->GetComponent<Transform>()->SetPosition(-20.0f, 0.0f, 6.0f);
					break;
				case 6:
					bladePrticle->GetComponent<Transform>()->SetPosition(-10.0f, 0.0f, 3.0f);
					break;
				case 7:
					bladePrticle->GetComponent<Transform>()->SetPosition(0.0f, 0.0f, 0.0f);
					break;
				case 8:
					bladePrticle->GetComponent<Transform>()->SetPosition(10.0f, 0.0f, 3.0f);
					break;
				case 9:
					bladePrticle->GetComponent<Transform>()->SetPosition(20.0f, 0.0f, 6.0f);
					break;
				case 10:
					bladePrticle->GetComponent<Transform>()->SetPosition(30.0f, 0.0f, 9.0f);
					break;
				case 11:
					bladePrticle->GetComponent<Transform>()->SetPosition(40.0f, 0.0f, 12.0f);
					break;
				case 12:
					bladePrticle->GetComponent<Transform>()->SetPosition(50.0f, 0.0f, 15.0f);
					break;
				case 13:
					bladePrticle->GetComponent<Transform>()->SetPosition(60.0f, 0.0f, 18.0f);
					break;
				case 14:
					bladePrticle->GetComponent<Transform>()->SetPosition(70.0f, 0.0f, 21.0f);
					break;
				default:
					break;
			}

			bladePrticle->SetParent(_largeBlade);

			_largeBladeParticle.emplace_back(bladePrticle);

		}
		for (int w = 0; w < 15; w++)
		{
			std::string name = "bladePrticleWave";
			auto bladePrticleWave = _boss->GetObjectScene()->CreateObject(name);

			bladePrticleWave->_autoAwake = true;
			bladePrticleWave->AddComponent<Particle>();
			bladePrticleWave->GetComponent<Particle>()->SetParticleEffect("fx_Lightning2", "Resources/Textures/Particles/fx_Lightning2.dds", 1000);
			bladePrticleWave->GetComponent<Particle>()->SetParticleDuration(0.3f, 1.0f);
			bladePrticleWave->GetComponent<Particle>()->SetParticleVelocity(50.0f, true);
			bladePrticleWave->GetComponent<Particle>()->SetParticleSize(30.f, 30.0f);
			bladePrticleWave->GetComponent<Particle>()->AddParticleColor(0.3f, 1.0f, 0.0f);
			bladePrticleWave->GetComponent<Particle>()->SetParticleDirection(0.0f, 0.0f, 0.0f);
			bladePrticleWave->GetComponent<Particle>()->SetParticleCameraApply(true);
			bladePrticleWave->GetComponent<Particle>()->SetParticleAngle(0.f, 0.0f, 0.0f);
			bladePrticleWave->GetComponent<Particle>()->SetActive(false);


			switch (w)
			{
				case 0:
					bladePrticleWave->GetComponent<Transform>()->SetPosition(-70.0f, 0.0f, 21.0f);
					break;
				case 1:
					bladePrticleWave->GetComponent<Transform>()->SetPosition(-60.0f, 0.0f, 18.0f);
					bladePrticleWave->GetComponent<Particle>()->SetParticleDuration(0.7f, 1.0f);
					break;
				case 2:
					bladePrticleWave->GetComponent<Transform>()->SetPosition(-50.0f, 0.0f, 15.0f);
					break;
				case 3:
					bladePrticleWave->GetComponent<Transform>()->SetPosition(-40.0f, 0.0f, 12.0f);
					break;
				case 4:
					bladePrticleWave->GetComponent<Transform>()->SetPosition(-30.0f, 0.0f, 9.0f);
					break;
				case 5:
					bladePrticleWave->GetComponent<Transform>()->SetPosition(-20.0f, 0.0f, 6.0f);
					bladePrticleWave->GetComponent<Particle>()->SetParticleDuration(0.7f, 1.0f);
					break;
				case 6:
					bladePrticleWave->GetComponent<Transform>()->SetPosition(-10.0f, 0.0f, 3.0f);
					break;
				case 7:
					bladePrticleWave->GetComponent<Transform>()->SetPosition(0.0f, 0.0f, 0.0f);
					break;
				case 8:
					bladePrticleWave->GetComponent<Transform>()->SetPosition(10.0f, 0.0f, 3.0f);
					break;
				case 9:
					bladePrticleWave->GetComponent<Transform>()->SetPosition(20.0f, 0.0f, 6.0f);
					bladePrticleWave->GetComponent<Particle>()->SetParticleDuration(0.7f, 1.0f);
					break;
				case 10:
					bladePrticleWave->GetComponent<Transform>()->SetPosition(30.0f, 0.0f, 9.0f);
					break;
				case 11:
					bladePrticleWave->GetComponent<Transform>()->SetPosition(40.0f, 0.0f, 12.0f);
					break;
				case 12:
					bladePrticleWave->GetComponent<Transform>()->SetPosition(50.0f, 0.0f, 15.0f);
					break;
				case 13:
					bladePrticleWave->GetComponent<Transform>()->SetPosition(60.0f, 0.0f, 18.0f);
					bladePrticleWave->GetComponent<Particle>()->SetParticleDuration(0.7f, 1.0f);
					break;
				case 14:
					bladePrticleWave->GetComponent<Transform>()->SetPosition(70.0f, 0.0f, 21.0f);
					break;
				default:
					break;
			}

			bladePrticleWave->SetParent(_largeBlade);

			_largeBladeParticleWave.emplace_back(bladePrticleWave);


		}
	}
	{

		for (int i = 0; i < 5; i++)
		{
			// 레이저가 공통으로 사용하게 될 라이트용 오브젝트
			std::string name = "laserLight";
			auto laserLight = _boss->GetObjectScene()->CreateObject(name);

			laserLight->AddComponent<Light>();
			laserLight->GetComponent<Light>()->CreatePointLight(Ambient, Diffuse, Specular, 300);
			laserLight->GetComponent<Light>()->SetDiffuse(0.15f, 0.5f, 0.05f, 1.0f);
			laserLight->GetComponent<Light>()->SetActive(false);


			_laserLight.emplace_back(laserLight);
		}

	}
	{
		for (auto& multipleSwordVec : _multipleSwordVec)
		{
			GameObject* bossCoreBladeMove1 = SceneManager::GetInstance().GetCurrentScene()->CreateObject("bossCoreBladeMove1");
			bossCoreBladeMove1->AddComponent<Particle>();
			bossCoreBladeMove1->GetComponent<Particle>()->SetParticleEffect("fx_Lightning1", "Resources/Textures/Particles/fx_Lightning1.dds", 1000);
			bossCoreBladeMove1->GetComponent<Particle>()->SetParticleDuration(7.0f, 3.0f);
			bossCoreBladeMove1->GetComponent<Particle>()->SetParticleVelocity(10.0f, true);
			bossCoreBladeMove1->GetComponent<Particle>()->SetParticleSize(1.f, 1.0f);
			bossCoreBladeMove1->GetComponent<Particle>()->AddParticleColor(0.2f, 1.0f, 0.0f);
			bossCoreBladeMove1->GetComponent<Particle>()->SetParticleDirection(0.0f, 0.0f, 0.0f);
			bossCoreBladeMove1->GetComponent<Particle>()->SetParticleCameraApply(true);
			bossCoreBladeMove1->SetParent(multipleSwordVec);
			bossCoreBladeMove1->GetComponent<Transform>()->SetPosition(0, 18.0f, -1.2);

			GameObject* bossCoreBladeMove2 = SceneManager::GetInstance().GetCurrentScene()->CreateObject("bossCoreBladeMove2");
			bossCoreBladeMove2->AddComponent<Particle>();
			bossCoreBladeMove2->GetComponent<Particle>()->SetParticleEffect("fx_Lightning3", "Resources/Textures/Particles/fx_Lightning3.dds", 1000);
			bossCoreBladeMove2->GetComponent<Particle>()->SetParticleDuration(0.5f, 2.0f);
			bossCoreBladeMove2->GetComponent<Particle>()->SetParticleVelocity(3.0f, true);
			bossCoreBladeMove2->GetComponent<Particle>()->SetParticleSize(7.f, 7.0f);
			bossCoreBladeMove2->GetComponent<Particle>()->AddParticleColor(0.2f, 1.0f, 0.0f);
			bossCoreBladeMove2->GetComponent<Particle>()->SetParticleDirection(0.0f, 0.0f, 0.0f);
			bossCoreBladeMove2->GetComponent<Particle>()->SetParticleAngle(0.0f, 0.0f, 0.0f);
			bossCoreBladeMove2->GetComponent<Particle>()->SetParticleCameraApply(true);
			bossCoreBladeMove2->SetParent(multipleSwordVec);
			bossCoreBladeMove2->GetComponent<Transform>()->SetPosition(-0.1f, 19.0f, -3.0f);

			GameObject* bossCoreBladeMove3 = SceneManager::GetInstance().GetCurrentScene()->CreateObject("bossCoreBladeMove3");
			bossCoreBladeMove3->AddComponent<Particle>();
			bossCoreBladeMove3->GetComponent<Particle>()->SetParticleEffect("fx_Lightning3", "Resources/Textures/Particles/fx_Lightning3.dds", 1000);
			bossCoreBladeMove3->GetComponent<Particle>()->SetParticleDuration(0.5f, 2.0f);
			bossCoreBladeMove3->GetComponent<Particle>()->SetParticleVelocity(3.0f, true);
			bossCoreBladeMove3->GetComponent<Particle>()->SetParticleSize(7.f, 7.0f);
			bossCoreBladeMove3->GetComponent<Particle>()->AddParticleColor(0.2f, 1.0f, 0.0f);
			bossCoreBladeMove3->GetComponent<Particle>()->SetParticleDirection(0.0f, 0.0f, 0.0f);
			bossCoreBladeMove3->GetComponent<Particle>()->SetParticleAngle(0.0f, 0.0f, 0.0f);
			bossCoreBladeMove3->GetComponent<Particle>()->SetParticleCameraApply(true);
			bossCoreBladeMove3->SetParent(multipleSwordVec);
			bossCoreBladeMove3->GetComponent<Transform>()->SetPosition(-0.1f, 19.0f, -1.5f);

			GameObject* bossCoreBladeMove4 = SceneManager::GetInstance().GetCurrentScene()->CreateObject("bossCoreBladeMove4");
			bossCoreBladeMove4->AddComponent<Particle>();
			bossCoreBladeMove4->GetComponent<Particle>()->SetParticleEffect("fx_Lightning3", "Resources/Textures/Particles/fx_Lightning3.dds", 1000);
			bossCoreBladeMove4->GetComponent<Particle>()->SetParticleDuration(0.5f, 2.0f);
			bossCoreBladeMove4->GetComponent<Particle>()->SetParticleVelocity(3.0f, true);
			bossCoreBladeMove4->GetComponent<Particle>()->SetParticleSize(7.f, 7.0f);
			bossCoreBladeMove4->GetComponent<Particle>()->AddParticleColor(0.2f, 1.0f, 0.0f);
			bossCoreBladeMove4->GetComponent<Particle>()->SetParticleDirection(0.0f, 0.0f, 0.0f);
			bossCoreBladeMove4->GetComponent<Particle>()->SetParticleAngle(0.0f, 0.0f, 0.0f);
			bossCoreBladeMove4->GetComponent<Particle>()->SetParticleCameraApply(true);
			bossCoreBladeMove4->SetParent(multipleSwordVec);
			bossCoreBladeMove4->GetComponent<Transform>()->SetPosition(-0.1f, 19.0f, -0.6);
		}
	}
	{
		//GameObject* boss3Phase1 = SceneManager::GetInstance().GetCurrentScene()->CreateObject("Boss3Phase1");
		//boss3Phase1->AddComponent<Particle>();
		//boss3Phase1->GetComponent<Particle>()->SetParticleEffect("Lightning2", "Resources/Textures/Particles/fx_Lightning2.dds", 1000);
		//boss3Phase1->GetComponent<Particle>()->SetParticleDuration(0.4f, 2.0f);
		//boss3Phase1->GetComponent<Particle>()->SetParticleVelocity(10.0f, true);
		//boss3Phase1->GetComponent<Particle>()->SetParticleSize(7.f, 7.0f);
		//boss3Phase1->GetComponent<Particle>()->AddParticleColor(15.0f, 0.6f, 0.0f);
		//boss3Phase1->GetComponent<Particle>()->SetParticleDirection(0.0f, 40.0f, 0.0f);
		//boss3Phase1->SetParent(_boss);
		//boss3Phase1->GetComponent<Transform>()->SetPosition(-0.272f, 15.53f, -0.616f);
		//_kamenLastphaseParticle.emplace_back(boss3Phase1);

		GameObject* boss3Phase2 = SceneManager::GetInstance().GetCurrentScene()->CreateObject("Boss3Phase2");
		boss3Phase2->AddComponent<Particle>();
		boss3Phase2->GetComponent<Particle>()->SetParticleEffect("Halo1", "Resources/Textures/Particles/fx_Halo1.dds", 1000);
		boss3Phase2->GetComponent<Particle>()->SetParticleDuration(1.0f, 0.3f);
		boss3Phase2->GetComponent<Particle>()->SetParticleVelocity(5.0f, true);
		boss3Phase2->GetComponent<Particle>()->SetParticleSize(35.f, 35.0f);
		boss3Phase2->GetComponent<Particle>()->AddParticleColor(15.0f, 0.6f, 0.0f);
		boss3Phase2->GetComponent<Particle>()->SetParticleDirection(0.0f, 0.0f, 0.0f);
		boss3Phase2->GetComponent<Particle>()->SetParticleAngle(270.0f, 0.0f, 0.0f);
		boss3Phase2->GetComponent<Particle>()->SetParticleCameraApply(true);
		boss3Phase2->SetParent(_boss);
		boss3Phase2->GetComponent<Transform>()->SetPosition(0.0f, 18.f, 3.f);
		boss3Phase2->SetTotalComponentState(false);
		boss3Phase2->SetActive(false);
		_kamenLastphaseParticle.emplace_back(boss3Phase2);

		GameObject* boss3Phase2_2 = SceneManager::GetInstance().GetCurrentScene()->CreateObject("Boss3Phase2_2");
		boss3Phase2_2->AddComponent<Particle>();
		boss3Phase2_2->GetComponent<Particle>()->SetParticleEffect("Halo1", "Resources/Textures/Particles/fx_Halo1.dds", 1000);
		boss3Phase2_2->GetComponent<Particle>()->SetParticleDuration(1.0f, 0.3f);
		boss3Phase2_2->GetComponent<Particle>()->SetParticleVelocity(5.0f, true);
		boss3Phase2_2->GetComponent<Particle>()->SetParticleSize(35.f, 35.0f);
		boss3Phase2_2->GetComponent<Particle>()->AddParticleColor(15.0f, 0.6f, 0.0f);
		boss3Phase2_2->GetComponent<Particle>()->SetParticleDirection(0.0f, 0.0f, 0.0f);
		boss3Phase2_2->GetComponent<Particle>()->SetParticleAngle(270.0f, 180.0f, 0.0f);
		boss3Phase2_2->GetComponent<Particle>()->SetParticleCameraApply(true);
		boss3Phase2_2->SetParent(_boss);
		boss3Phase2_2->GetComponent<Transform>()->SetPosition(0.0f, 18.f, 3.f);
		boss3Phase2_2->SetTotalComponentState(false);
		boss3Phase2_2->SetActive(false);
		_kamenLastphaseParticle.emplace_back(boss3Phase2_2);

		GameObject* boss3Phase3 = SceneManager::GetInstance().GetCurrentScene()->CreateObject("Boss3Phase3");
		boss3Phase3->AddComponent<Particle>();
		boss3Phase3->GetComponent<Particle>()->SetParticleEffect("BlastWave3", "Resources/Textures/Particles/fx_BlastWave3.dds", 1000);
		boss3Phase3->GetComponent<Particle>()->SetParticleDuration(1.0f, 1.0f);
		boss3Phase3->GetComponent<Particle>()->SetParticleVelocity(3.0f, true);
		boss3Phase3->GetComponent<Particle>()->SetParticleSize(25.f, 25.0f);
		boss3Phase3->GetComponent<Particle>()->AddParticleColor(6.0f, 0.2f, 0.0f);
		boss3Phase3->GetComponent<Particle>()->SetParticleDirection(0.0f, 0.0f, 0.0f);
		boss3Phase3->GetComponent<Particle>()->SetParticleAngle(270.0f, 0, 0);
		boss3Phase3->GetComponent<Particle>()->SetParticleCameraApply(true);
		boss3Phase3->SetParent(_boss);
		boss3Phase3->GetComponent<Transform>()->SetPosition(0, 18.f, 3.f);
		boss3Phase3->SetTotalComponentState(false);
		boss3Phase3->SetActive(false);
		_kamenLastphaseParticle.emplace_back(boss3Phase3);

		GameObject* boss3Phase3_2 = SceneManager::GetInstance().GetCurrentScene()->CreateObject("Boss3Phase3_2");
		boss3Phase3_2->AddComponent<Particle>();
		boss3Phase3_2->GetComponent<Particle>()->SetParticleEffect("BlastWave3", "Resources/Textures/Particles/fx_BlastWave3.dds", 1000);
		boss3Phase3_2->GetComponent<Particle>()->SetParticleDuration(1.0f, 1.0f);
		boss3Phase3_2->GetComponent<Particle>()->SetParticleVelocity(3.0f, true);
		boss3Phase3_2->GetComponent<Particle>()->SetParticleSize(25.f, 25.0f);
		boss3Phase3_2->GetComponent<Particle>()->AddParticleColor(6.0f, 0.2f, 0.0f);
		boss3Phase3_2->GetComponent<Particle>()->SetParticleDirection(0.0f, 0.0f, 0.0f);
		boss3Phase3_2->GetComponent<Particle>()->SetParticleAngle(270.0f, 180, 0);
		boss3Phase3_2->GetComponent<Particle>()->SetParticleCameraApply(true);
		boss3Phase3_2->SetParent(_boss);
		boss3Phase3_2->GetComponent<Transform>()->SetPosition(0, 18.f, 3.f);
		boss3Phase3_2->SetTotalComponentState(false);
		boss3Phase3_2->SetActive(false);
		_kamenLastphaseParticle.emplace_back(boss3Phase3_2);
	}
}

void KunrealEngine::Kamen::CreateSubObject()
{
	DirectX::XMFLOAT4 Ambient = { 1.0f, 1.0f, 1.0f, 1.0f };
	DirectX::XMFLOAT4 Diffuse = { 0.0f, 1.0f, 0.0f, 1.0f };
	DirectX::XMFLOAT4 Specular = { 1.0f, 1.0f, 1.0f, 1.0f };

	//// 왼손
	//_leftHand = _boss->GetObjectScene()->CreateObject("LeftHand");
	//_leftHand->AddComponent<BoxCollider>();
	//_leftHand->GetComponent<BoxCollider>()->SetTransform(_boss, "MiddleFinger1_L");
	//_leftHand->GetComponent<BoxCollider>()->SetColliderScale(2.0f, 3.0f, 2.0f);
	//_leftHand->GetComponent<BoxCollider>()->SetActive(false);
	//_leftHand->SetTotalComponentState(false);
	//_leftHand->SetActive(false);
	//
	//// 오른손
	//_rightHand = _boss->GetObjectScene()->CreateObject("rightHand");
	//_rightHand->AddComponent<BoxCollider>();
	//_rightHand->GetComponent<BoxCollider>()->SetTransform(_boss, "MiddleFinger1_R");
	//_rightHand->GetComponent<BoxCollider>()->SetColliderScale(2.0f, 3.0f, 2.0f);
	//_rightHand->GetComponent<BoxCollider>()->SetActive(false);
	//_rightHand->SetTotalComponentState(false);
	//_rightHand->SetActive(false);

	//// call 투사체
	//_call = _boss->GetObjectScene()->CreateObject("call");
	//_call->AddComponent<BoxCollider>();
	//_call->GetComponent<BoxCollider>()->SetColliderScale(10.0f, 10.0f, 10.0f);
	//_call->GetComponent<BoxCollider>()->SetActive(false);
	//_call->AddComponent<Particle>();
	//_call->GetComponent<Particle>()->SetParticleEffect("Flame", "Resources/Textures/Particles/flare.dds", 1000);
	//_call->GetComponent<Particle>()->SetParticleDuration(2.0f, 2.0f);
	//_call->GetComponent<Particle>()->SetParticleVelocity(3.f, true);
	//_call->GetComponent<Particle>()->SetParticleSize(10.f, 30.0f);
	//_call->GetComponent<Particle>()->SetParticleDirection(0.0f, 7.0f, 0.0f);
	//_call->GetComponent<Particle>()->AddParticleColor(1.2f, 7.5f, 0.6f);
	//_call->GetComponent<Particle>()->SetActive(false);
	//DirectX::XMFLOAT4 diffuse = { 0.0f, 0.6f, 0.0f, 1.0f };
	//DirectX::XMFLOAT4 ambient = { 0.0f, 0.15f, 0.0f, 1.0f };
	//DirectX::XMFLOAT4 specular = { 0.0f, 0.15f, 0.0f, 1.0f };
	//_call->AddComponent<Light>();
	//_call->GetComponent<Light>()->CreatePointLight(ambient, diffuse, specular, 1.0f);
	//_call->GetComponent<Light>()->SetActive(false);
	//_call->SetTotalComponentState(false);
	//_call->SetActive(false);

	// 왼손 불 위치 체크용 메쉬
	_leftHandBone = _boss->GetObjectScene()->CreateObject("leftHandBone");
	_leftHandBone->AddComponent<MeshRenderer>();
	_leftHandBone->GetComponent<MeshRenderer>()->SetMeshObject("cube/cube");
	_leftHandBone->GetComponent<MeshRenderer>()->SetParentBone(_boss, "MiddleFinger1_L");
	_leftHandBone->GetComponent<Transform>()->SetScale(10.0f, 10.f, 10.f);
	_leftHandBone->GetComponent<MeshRenderer>()->SetActive(false);
	_leftHandBone->SetTotalComponentState(false);
	_leftHandBone->SetActive(false);

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
		handFire->AddComponent<Light>();
		handFire->GetComponent<Light>()->CreatePointLight(Ambient, Diffuse, Specular, 300);
		handFire->GetComponent<Light>()->SetDiffuse(0.15f, 0.5f, 0.05f, 1.0f);
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
	_bossInsideAttack->AddComponent<CylinderCollider>();
	_bossInsideAttack->GetComponent<CylinderCollider>()->SetColliderScale(70.0f, 70.0f, 70.0f);
	_bossInsideAttack->SetTotalComponentState(false);
	_bossInsideAttack->SetActive(false);

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
	_swordInsideAttack->AddComponent<CylinderCollider>();
	_swordInsideAttack->GetComponent<CylinderCollider>()->SetColliderScale(30.0f, 30.0f, 30.0f);
	_swordInsideAttack->GetComponent<CylinderCollider>()->SetActive(false);
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
	_swordOutsideWarning->GetComponent<TransparentMesh>()->SetTimer(4.0f);
	_swordOutsideWarning->GetComponent<TransparentMesh>()->SetRenderType(5);
	_swordOutsideWarning->GetComponent<TransparentMesh>()->SetExceptRange(DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f), 30.0f);
	_swordOutsideWarning->GetComponent<TransparentMesh>()->SetActive(false);
	_swordOutsideWarning->GetComponent<Transform>()->SetPosition(_centerPos.x + 1.0f, _centerPos.y, _centerPos.z - 6.0f);
	_swordOutsideWarning->GetComponent<Transform>()->SetScale(100.0f, 100.0f, 115.0f);
	_swordOutsideWarning->SetTotalComponentState(false);
	_swordOutsideWarning->SetActive(false);

	// 도넛 장판 1번
	_swordDonutWarning1 = _boss->GetObjectScene()->CreateObject("donutSafe1");
	_swordDonutWarning1->AddComponent<TransparentMesh>();
	_swordDonutWarning1->GetComponent<TransparentMesh>()->CreateTMesh("DonutSafe1", "Resources/Textures/Warning/Warning.dds", 0.6f, true);
	_swordDonutWarning1->GetComponent<Transform>()->SetScale(20.0f, 20.0f, 20.0f);
	_swordDonutWarning1->GetComponent<TransparentMesh>()->SetTimer(1.0f);
	_swordDonutWarning1->GetComponent<TransparentMesh>()->SetActive(false);
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
	_swordDonutWarning3->SetTotalComponentState(false);
	_swordDonutWarning3->SetActive(false);

	// 도넛 장판 공격
	for (int i = 0; i < 3; i++)
	{
		auto objectName = "DonutAttack" + std::to_string(i + 1);
		auto swordDonutAttack = _boss->GetObjectScene()->CreateObject(objectName);
		swordDonutAttack->AddComponent<CylinderCollider>();
		auto colliderSize = 20.0f + 40 * i;
		swordDonutAttack->GetComponent<CylinderCollider>()->SetColliderScale(colliderSize, colliderSize, colliderSize);
		swordDonutAttack->SetTotalComponentState(false);
		swordDonutAttack->SetActive(false);
		_swordDonutAttack.emplace_back(swordDonutAttack);
	}

	// 레이저 
	_lazer = _boss->GetObjectScene()->CreateObject("lazer");
	_lazer->AddComponent<Particle>();
	_lazer->GetComponent<Particle>()->SetParticleEffect("Laser", "Resources/Textures/Particles/fx_LightFlash1.dds", 1000);
	_lazer->GetComponent<Particle>()->SetParticleSize(15.f, 15.f);
	_lazer->GetComponent<Particle>()->SetParticleDuration(1.7f, 2.0f);
	_lazer->GetComponent<Particle>()->SetParticleVelocity(84.f, false);
	_lazer->GetComponent<Particle>()->SetParticleRotation(90.0f, _bossTransform->GetRotation().y, 0.0f);
	_lazer->GetComponent<Particle>()->AddParticleColor(0.0f, 1.0f, 0.5f);
	//_lazer->GetComponent<Particle>()->SetParticleCameraApply(true);
	_lazer->GetComponent<Particle>()->SetActive(false);
	_lazer->SetTotalComponentState(false);
	_lazer->SetActive(false);

	// 레이저 콜라이더
	_lazerCollider = _boss->GetObjectScene()->CreateObject("lazerCollider");
	_lazerCollider->AddComponent<BoxCollider>();
	_lazerCollider->GetComponent<BoxCollider>()->SetColliderScale(130.0f, 40.0f, 10.0f);
	_lazerCollider->GetComponent<BoxCollider>()->SetActive(false);
	_lazerCollider->AddComponent<Light>();
	_lazerCollider->GetComponent<Light>()->CreatePointLight(Ambient, Diffuse, Specular, 300);
	_lazerCollider->GetComponent<Light>()->SetDiffuse(0.1f, 1.0f, 0.3f, 1.0f);
	_lazerCollider->SetTotalComponentState(false);
	_lazerCollider->SetActive(false);

	// call2 투사체
	_call2 = _boss->GetObjectScene()->CreateObject("call2");
	_call2->AddComponent<BoxCollider>();
	_call2->GetComponent<BoxCollider>()->SetColliderScale(15.0f, 30.0f, 15.0f);
	_call2->GetComponent<BoxCollider>()->SetOffset(0.0f, 20.0f, 0.0f);
	_call2->GetComponent<BoxCollider>()->SetActive(false);
	_call2->SetTotalComponentState(false);
	_call2->SetActive(false);

	for (int i = 0; i < 5; i++)
	{
		std::string objectName = "FiveWarning" + std::to_string(i);
		auto fiveAttack = _boss->GetObjectScene()->CreateObject(objectName);
		fiveAttack->AddComponent<TransparentMesh>();
		fiveAttack->GetComponent<TransparentMesh>()->CreateTMesh(objectName, "Resources/Textures/Warning/Warning.dds", 0.6f);
		fiveAttack->GetComponent<TransparentMesh>()->SetTimer(1.5f);
		fiveAttack->GetComponent<TransparentMesh>()->SetRenderType(0);
		fiveAttack->GetComponent<Transform>()->SetScale(12.0f, 12.0f, 100.0f);
		fiveAttack->SetTotalComponentState(false);
		fiveAttack->SetActive(false);
		_fiveAttack.emplace_back(fiveAttack);
	}

	{
		// 따로 패턴하는 검
		_freeSword = _boss->GetObjectScene()->CreateObject("KamenFreeSword");
		_freeSword->AddComponent<MeshRenderer>();
		_freeSword->GetComponent<MeshRenderer>()->SetMeshObject("KamenSword/KamenSword");
		_freeSword->GetComponent<MeshRenderer>()->SetActive(false);
		auto texSize = _freeSword->GetComponent<MeshRenderer>()->GetTextures().size();
		for (int i = 0; i < texSize; i++)
		{
			_freeSword->GetComponent<MeshRenderer>()->SetDiffuseTexture(i, "KamenSword/KamenSword_BaseColor.png");
			_freeSword->GetComponent<MeshRenderer>()->SetNormalTexture(i, "KamenSword/KamenSword_Normal.png");
			_freeSword->GetComponent<MeshRenderer>()->SetEmissiveTexture(i, "KamenSword/KamenSword_Emissive.png");
		}
		_freeSword->SetTotalComponentState(false);
		_freeSword->SetActive(false);

		// 검 콜라이더
		_freeSwordCollider = _boss->GetObjectScene()->CreateObject("FreeSwordCollider");
		_freeSwordCollider->AddComponent<BoxCollider>();
		_freeSwordCollider->GetComponent<BoxCollider>()->SetColliderScale(5.0f, 5.0f, 30.0f);
		_freeSwordCollider->SetTotalComponentState(false);
		_freeSwordCollider->SetActive(false);

		// 검 경로 장판
		_swordPath = _boss->GetObjectScene()->CreateObject("swordPath");
		_swordPath->AddComponent<TransparentMesh>();
		_swordPath->GetComponent<TransparentMesh>()->CreateTMesh("SwordPath", "Resources/Textures/Warning/Warning.dds", 0.6f, false);
		_swordPath->GetComponent<Transform>()->SetScale(200.0f, 10.0f, 10.0f);
		_swordPath->GetComponent<TransparentMesh>()->SetTimer(4.5f);
		_swordPath->GetComponent<TransparentMesh>()->SetActive(false);
		_swordPath->SetTotalComponentState(false);
		_swordPath->SetActive(false);
	}
	// 카멘에 종속된 검
	{
		_kamenSword = _boss->GetObjectScene()->CreateObject("KamenSword");
		_kamenSword->AddComponent<MeshRenderer>();
		_kamenSword->GetComponent<MeshRenderer>()->SetMeshObject("KamenAttackSword/KamenAttackSword");
		_kamenSword->GetComponent<MeshRenderer>()->SetActive(false);
		auto texSize = _kamenSword->GetComponent<MeshRenderer>()->GetTextures().size();
		for (int i = 0; i < texSize; i++)
		{
			_kamenSword->GetComponent<MeshRenderer>()->SetDiffuseTexture(i, "KamenSword/KamenSword_BaseColor.png");
			_kamenSword->GetComponent<MeshRenderer>()->SetNormalTexture(i, "KamenSword/KamenSword_Normal.png");
			_kamenSword->GetComponent<MeshRenderer>()->SetEmissiveTexture(i, "KamenSword/KamenSword_Emissive.png");
		}

		_kamenSword->GetComponent<MeshRenderer>()->SetParentBone(_boss, "MiddleFinger1_R");
		_kamenSword->GetComponent<Transform>()->SetPosition(0.0f, 0.0f, 0.0f);
		_kamenSword->GetComponent<Transform>()->SetScale(1.0f, 1.0f, 1.0f);
		_kamenSword->SetTotalComponentState(false);
		_kamenSword->SetActive(false);

		_kamenSwordCollider = _boss->GetObjectScene()->CreateObject("KamenSwordCollider");
		_kamenSwordCollider->AddComponent<BoxCollider>();
		_kamenSwordCollider->GetComponent<BoxCollider>()->SetTransform(_boss, "MiddleFinger1_R");
		_kamenSwordCollider->GetComponent<BoxCollider>()->SetColliderScale(3.0f, 5.0f, 30.0f);
		_kamenSwordCollider->SetTotalComponentState(false);
		_kamenSwordCollider->SetActive(false);

		//_kamenSword->AddComponent<Light>();
		//_kamenSword->GetComponent<Light>()->CreatePointLight(DirectX::XMFLOAT4{ 1.0f, 1.0f, 1.0f, 1.0f }, DirectX::XMFLOAT4{ 0.0f, 1.0f, 0.0f, 1.0f });
		//_kamenSword->GetComponent<Light>()->SetPointRange(300.f);

	}

	// 칼 라이트
	{

		DirectX::XMFLOAT4 Ambient = { 1.0f, 1.0f, 1.0f, 1.0f };
		DirectX::XMFLOAT4 Diffuse = { 0.0f, 1.0f, 0.0f, 1.0f };
		DirectX::XMFLOAT4 Specular = { 1.0f, 1.0f, 1.0f, 1.0f };

		_kamenSwordLight = _boss->GetObjectScene()->CreateObject("KamenSwordLight");
		_kamenSwordLight->AddComponent<Light>();
		_kamenSwordLight->GetComponent<Light>()->CreatePointLight(Ambient, Diffuse, Specular, 300);
		//_kamenSwordLight->SetParent(_kamenSword);
	}

	// 칼 검기
	{
		for (int i = 0; i < 2; i++)
		{
			auto blade = _boss->GetObjectScene()->CreateObject("Blade");
			blade->AddComponent<MeshRenderer>();
			blade->GetComponent<MeshRenderer>()->SetMeshObject("Blade/Blade");

			auto texSize = blade->GetComponent<MeshRenderer>()->GetTextures().size();
			for (int i = 0; i < texSize; i++)
			{
				blade->GetComponent<MeshRenderer>()->SetDiffuseTexture(i, "Blade/Diffuse.png");
				blade->GetComponent<MeshRenderer>()->SetNormalTexture(i, "Blade/Normal.png");
			}
			blade->GetComponent<MeshRenderer>()->SetAlpha(0.6f);

			blade->GetComponent<Transform>()->SetScale(25.0f, 20.0f, 20.0f);

			blade->AddComponent<MeshCollider>();
			blade->GetComponent<MeshCollider>()->CreateMeshCollider("Blade");
			blade->GetComponent<MeshCollider>()->SetColliderScale(50.0f, 20.0f, 20.0f);

			blade->SetTotalComponentState(false);
			blade->SetActive(false);

			_blade.emplace_back(blade);
		}

		// 큰 검기
		{
			_largeBlade = _boss->GetObjectScene()->CreateObject("LargeBlade");
			_largeBlade->AddComponent<MeshRenderer>();
			_largeBlade->GetComponent<MeshRenderer>()->SetMeshObject("Blade/Blade");

			auto texSize = _largeBlade->GetComponent<MeshRenderer>()->GetTextures().size();
			for (int i = 0; i < texSize; i++)
			{
				_largeBlade->GetComponent<MeshRenderer>()->SetDiffuseTexture(i, "Blade/Diffuse.png");
				_largeBlade->GetComponent<MeshRenderer>()->SetNormalTexture(i, "Blade/Normal.png");
			}
			_largeBlade->GetComponent<MeshRenderer>()->SetAlpha(0.6f);

			_largeBlade->GetComponent<Transform>()->SetScale(28.0f, 20.0f, 21.0f);

			_largeBlade->AddComponent<MeshCollider>();
			_largeBlade->GetComponent<MeshCollider>()->CreateMeshCollider("Blade");
			_largeBlade->GetComponent<MeshCollider>()->SetColliderScale(56.0f, 20.0f, 21.0f);

			_largeBlade->SetTotalComponentState(false);
			_largeBlade->SetActive(false);
		}
	}

	/// 분신 서브 오브젝트들
	// 분신 소환
	_alterEgo = _boss->GetObjectScene()->CreateObject("Ego");

	_alterEgo->AddComponent<MeshRenderer>();
	_alterEgo->GetComponent<MeshRenderer>()->SetMeshObject("FakeLich/FakeLich");
	_alterEgo->GetComponent<Transform>()->SetPosition(0.0f, 2.0f, 0.0f);
	_alterEgo->GetComponent<Transform>()->SetScale(1.5f, 1.5f, 1.5f);

	auto egoTexSize = _alterEgo->GetComponent<MeshRenderer>()->GetTextures().size();
	for (int i = 0; i < egoTexSize; i++)
	{
		_alterEgo->GetComponent<MeshRenderer>()->SetDiffuseTexture(i, "Lich/T_Lich_1_D.tga");
		_alterEgo->GetComponent<MeshRenderer>()->SetNormalTexture(i, "Lich/T_Lich_N.tga");
		_alterEgo->GetComponent<MeshRenderer>()->SetEmissiveTexture(i, "Lich/T_Lich_01_E.tga");
	}
	_alterEgo->GetComponent<MeshRenderer>()->SetCartoonState(true);
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
		handFire->AddComponent<Light>();
		handFire->GetComponent<Light>()->CreatePointLight(Ambient, Diffuse, Specular, 300.0f);
		handFire->GetComponent<Light>()->SetDiffuse(0.15f, 0.5f, 0.05f, 1.0f);
		_egoHandFire.emplace_back(handFire);

		handFire->SetTotalComponentState(false);
		handFire->SetActive(false);
	}

	// call2 투사체
	_egoCall2 = _boss->GetObjectScene()->CreateObject("EgoCall2");
	_egoCall2->AddComponent<BoxCollider>();
	_egoCall2->GetComponent<BoxCollider>()->SetColliderScale(15.0f, 30.0f, 15.0f);
	_egoCall2->GetComponent<BoxCollider>()->SetOffset(0.0f, 20.0f, 0.0f);
	_egoCall2->GetComponent<BoxCollider>()->SetActive(false);
	_egoCall2->SetTotalComponentState(false);
	_egoCall2->SetActive(false);

	// 레이저 
	_egoLazer = _boss->GetObjectScene()->CreateObject("EgoLazer");
	_egoLazer->AddComponent<Particle>();
	_egoLazer->GetComponent<Particle>()->SetParticleEffect("Laser", "Resources/Textures/Particles/fx_LightFlash1.dds", 1000);
	_egoLazer->GetComponent<Particle>()->SetParticleDuration(1.7f, 2.0f);
	_egoLazer->GetComponent<Particle>()->SetParticleVelocity(84.f, false);
	_egoLazer->GetComponent<Particle>()->SetParticleRotation(90.0f, _bossTransform->GetRotation().y, 0.0f);
	_egoLazer->GetComponent<Particle>()->AddParticleColor(0.0f, 1.0f, 0.5f);
	_egoLazer->GetComponent<Particle>()->SetActive(false);
	_egoLazer->SetTotalComponentState(false);
	_egoLazer->SetActive(false);

	// 레이저 콜라이더
	_egoLazerCollider = _boss->GetObjectScene()->CreateObject("EgoLazerCollider");
	_egoLazerCollider->AddComponent<BoxCollider>();
	_egoLazerCollider->GetComponent<BoxCollider>()->SetColliderScale(130.0f, 40.0f, 10.0f);
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
	_egoInsideAttack->AddComponent<CylinderCollider>();
	_egoInsideAttack->GetComponent<CylinderCollider>()->SetActive(false);
	_egoInsideAttack->GetComponent<CylinderCollider>()->SetColliderScale(70.0f, 70.0f, 70.0f);
	_egoInsideAttack->SetTotalComponentState(false);
	_egoInsideAttack->SetActive(false);

	//// Fake Boss
	for (int i = 0; i < 5; i++)
	{
		std::string index = "fake" + std::to_string(i + 1);
		auto boss = _boss->GetObjectScene()->CreateObject(index);

		boss->AddComponent<MeshRenderer>();
		boss->GetComponent<MeshRenderer>()->SetMeshObject("FakeLich/FakeLich");
		boss->GetComponent<Transform>()->SetScale(1.5f, 1.5f, 1.5f);

		auto texSize = boss->GetComponent<MeshRenderer>()->GetTextures().size();
		for (int i = 0; i < texSize; i++)
		{
			boss->GetComponent<MeshRenderer>()->SetDiffuseTexture(i, "Lich/T_Lich_1_D.tga");
			boss->GetComponent<MeshRenderer>()->SetNormalTexture(i, "Lich/T_Lich_N.tga");
			boss->GetComponent<MeshRenderer>()->SetEmissiveTexture(i, "Lich/T_Lich_01_E.tga");
		}

		boss->AddComponent<BoxCollider>();
		boss->GetComponent<BoxCollider>()->SetTransform(boss, "Spine1_M");
		boss->GetComponent<BoxCollider>()->SetColliderScale(10.0f, 30.0f, 20.0f);
		boss->GetComponent<BoxCollider>()->SetOffset(0.0f, -6.0f, 0.0f);

		boss->GetComponent<MeshRenderer>()->SetAlpha(0.5f);
		boss->AddComponent<Light>();
		boss->GetComponent<Light>()->CreatePointLight(Ambient, Diffuse, Specular, 300);
		boss->GetComponent<Light>()->SetDiffuse(0.3f, 1.0f, 0.1f, 1.0f);
		boss->GetComponent<Light>()->SetOffSet(0.0f, 15.0f, 0.0f);
		boss->SetTotalComponentState(false);
		boss->SetActive(false);

		_fakeBoss.emplace_back(boss);

		_fakeDirVec.emplace_back();

		_fakeMoveDistance.emplace_back(0.0f);
	}

	// multiple sword
	for (int i = 0; i < 20; i++)
	{
		std::string name = "MultiAttackSword" + std::to_string(i + 1);
		auto sword = _boss->GetObjectScene()->CreateObject(name);
		sword->AddComponent<MeshRenderer>();
		sword->GetComponent<MeshRenderer>()->SetMeshObject("KamenSword/KamenSword");
		sword->GetComponent<MeshRenderer>()->SetActive(false);
		auto texSize = sword->GetComponent<MeshRenderer>()->GetTextures().size();
		for (int i = 0; i < texSize; i++)
		{
			sword->GetComponent<MeshRenderer>()->SetDiffuseTexture(i, "KamenSword/KamenSword_BaseColor.png");
			sword->GetComponent<MeshRenderer>()->SetNormalTexture(i, "KamenSword/KamenSword_Normal.png");
			sword->GetComponent<MeshRenderer>()->SetEmissiveTexture(i, "KamenSword/KamenSword_Emissive.png");
		}

		sword->GetComponent<Transform>()->SetPosition(0.0f, 30.0f, 0.0f);
		sword->GetComponent<Transform>()->SetRotation(180.0f, 90.0f, 0.0f);
		sword->GetComponent<Transform>()->SetScale(1.5f, 1.5f, 1.5f);

		sword->AddComponent<BoxCollider>();
		sword->GetComponent<BoxCollider>()->SetColliderScale(2.0f, 32.0f, 6.0f);
		sword->GetComponent<BoxCollider>()->SetOffset(1.0f, -12.0f, 0.0f);

		sword->SetTotalComponentState(false);
		sword->SetActive(false);

		_multipleSwordVec.emplace_back(sword);
	}

	for (int i = 0; i < 20; i++)
	{
		std::string objectName = "MultiAttackSwordWarning" + std::to_string(i + 1);
		auto swordAttack = _boss->GetObjectScene()->CreateObject(objectName);
		swordAttack->AddComponent<TransparentMesh>();
		swordAttack->GetComponent<TransparentMesh>()->CreateTMesh(objectName, "Resources/Textures/Warning/Warning.dds", 0.6f);
		swordAttack->GetComponent<TransparentMesh>()->SetTimer(2.0F);
		swordAttack->GetComponent<TransparentMesh>()->SetRenderType(3);
		swordAttack->GetComponent<Transform>()->SetScale(3.0f, 3.0f, 300.0f);
		swordAttack->SetTotalComponentState(false);
		swordAttack->SetActive(false);
		_multipleSwordWarningVec.emplace_back(swordAttack);
	}
}


void KunrealEngine::Kamen::LeftRightPattern()
{
	BossPattern* leftRightPattern = new BossPattern();

	leftRightPattern->SetNextPatternForcePlay(false);

	leftRightPattern->SetMaxColliderCount(0);

	leftRightPattern->SetRange(_info._attackRange);

	leftRightPattern->SetPattern(_leftAttack);

	leftRightPattern->SetPattern(_rightAttack);

	_basicPattern[0].emplace_back(leftRightPattern);
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

	_basicPattern[0].emplace_back(rightLeftPattern);
}


void KunrealEngine::Kamen::BasicPattern()
{
	_basicPattern[0].emplace_back(_spellAttack);
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
	pattern->SetAnimName("Left_Attack").SetDamage(10.0f).SetSpeed(20.0f).SetRange(_info._attackRange + 50.0f).SetAfterDelay(1.0f);
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
					//pattern->DeleteSubObject(_egoHandFire[i]);
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

			auto bossPos = DirectX::XMFLOAT3(_bossTransform->GetPosition().x, 30.0f, _bossTransform->GetPosition().z);
			auto fireSpeed = 5.0f * TimeManager::GetInstance().GetDeltaTime();

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

						_handFire[i]->GetComponent<Light>()->SetActive(true);

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
							auto egoPos = DirectX::XMFLOAT3(egoPosTransform.x, 30.0f, egoPosTransform.z);

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

							_egoHandFire[i]->GetComponent<Light>()->SetActive(true);

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
	pattern->SetAnimName("Right_Attack").SetDamage(10.0f).SetSpeed(20.0f).SetRange(_info._attackRange + 50.0f).SetAfterDelay(1.0);
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

			auto bossPos = DirectX::XMFLOAT3(_bossTransform->GetPosition().x, 30.0f, _bossTransform->GetPosition().z);
			auto fireSpeed = 5.0f * TimeManager::GetInstance().GetDeltaTime();;

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

						_handFire[i]->GetComponent<Light>()->SetActive(true);

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
							auto egoPos = DirectX::XMFLOAT3(egoPosTransform.x, 30.0f, egoPosTransform.z);

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

							_handFire[i]->GetComponent<Light>()->SetActive(true);

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

void KunrealEngine::Kamen::CreateTeleportRandomWithLook()
{
	BossPattern* pattern = new BossPattern();

	pattern->SetPatternName("TeleportRand");

	pattern->SetAnimName("Idle").SetRange(0.0f).SetMaxColliderCount(0).SetSpeed(20.0f);

	auto initLogic = [pattern, this]()
		{
			auto bossPos = _bossTransform->GetPosition();

			auto ranX = ToolBox::GetRandomFloat(bossPos.x - 40.0f, bossPos.x + 40.0f);
			auto ranZ = ToolBox::GetRandomFloat(bossPos.x - 40.0f, bossPos.x + 40.0f);

			_randomPos.x = ranX;
			_randomPos.y = _bossTransform->GetPosition().y;
			_randomPos.z = ranZ;
		};

	pattern->SetInitializeLogic(initLogic);

	auto teleport = [pattern, this]()
		{
			auto isTeleportFinish = Teleport(DirectX::XMFLOAT3(_randomPos.x, _randomPos.y, _randomPos.z), true, 1.0f);

			if (_isEgoAttack)
			{
				auto egoAnimator = _alterEgo->GetComponent<Animator>();
				egoAnimator->Play("Idle", pattern->_speed, true);
			}

			if (isTeleportFinish)
			{
				return false;
			}

			return true;
		};

	// 로직 함수 실행 가능하도록 넣어주기
	pattern->SetLogic(teleport);

	_teleportRandomWithLook = pattern;
}

void KunrealEngine::Kamen::CreateTeleportWithSwordSwing()
{
	BossPattern* pattern = new BossPattern();

	pattern->SetAnimName("SwordIdle").SetMaxColliderCount(0).SetSpeed(20.0f);

	auto initLogic = [pattern, this]()
		{
			auto index = ToolBox::GetRandomNum(3);

			auto bossRotation = _bossTransform->GetRotation().y;

			switch (index)
			{
				case 0:
				{
					_randomPos = { -65.0f, _centerPos.y, 0.0f };
					_bossTransform->SetRotation(0.0f, -90.0f, 0.0f);
					break;
				}
				case 1:
				{
					_randomPos = { 0.0f, _centerPos.y, 75.0f };
					_bossTransform->SetRotation(0.0f, 0.0f, 0.0f);
					break;
				}
				case 2:
				{
					_randomPos = { 67.0f, _centerPos.y, 0.0f };
					_bossTransform->SetRotation(0.0f, 90.0f, 0.0f);
					break;
				}
				case 3:
				{
					_randomPos = { 0.0f, _centerPos.y, -90.0f };
					_bossTransform->SetRotation(0.0f, 180.0f, 0.0f);
					break;
				}
				default:
					break;
			}
		};

	pattern->SetInitializeLogic(initLogic);

	auto teleport = [pattern, this]()
		{
			auto isTeleportFinish = Teleport(DirectX::XMFLOAT3(_randomPos.x, _randomPos.y, _randomPos.z), false, 1.0f);

			if (isTeleportFinish)
			{
				return false;
			}

			return true;
		};

	pattern->SetLogic(teleport);

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

			_boss->GetComponent<BoxCollider>()->FixedUpdate();
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

			_boss->GetComponent<BoxCollider>()->FixedUpdate();
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

			_boss->GetComponent<BoxCollider>()->FixedUpdate();
		};

	pattern->SetInitializeLogic(initializeLogic);

	auto reverseEmergence = [pattern, this]()
		{
			auto animator = _boss->GetComponent<Animator>();

			auto isPlaying = animator->Play(pattern->_animName, pattern->_speed);

			if (isPlaying)
			{
				if (_isEgoAttack)
				{
					auto egoAnimator = _alterEgo->GetComponent<Animator>();
					egoAnimator->Play("Idle", pattern->_speed, true);
				}

				return true;
			}
			else
			{
				_boss->GetComponent<BoxCollider>()->SetActive(false);
				_boss->GetComponent<MeshRenderer>()->SetActive(false);

				_boss->GetComponent<BoxCollider>()->FixedUpdate();

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

			_boss->GetComponent<BoxCollider>()->FixedUpdate();

			_boss->GetComponent<Transform>()->SetPosition(_emergencePos);

			auto angle = CalculateAngle(_bossTransform->GetPosition(), _playerTransform->GetPosition());
			_bossTransform->SetRotation(_bossTransform->GetRotation().x, angle, _bossTransform->GetRotation().z);

			_bossInsideAttack->GetComponent<Transform>()->SetPosition(_emergencePos.x, _bossTransform->GetPosition().y, _emergencePos.z);

			_timer = 0.0f;

			for (auto& index : _particleEmergenceAttack)
			{
				index->GetComponent<Particle>()->SetActive(true);
				index->GetComponent<Particle>()->SetParticleSize(80, 80);

				auto objectIndex = pattern->GetSubObjectIndex(_bossInsideAttack);
				pattern->_isColliderActive[objectIndex] = true;

				_bossInsideAttack->GetComponent<CylinderCollider>()->SetColliderScale(_bossCircleWarningSize * 1.5f, _bossCircleWarningSize * 1.5f, _bossCircleWarningSize * 1.5f);
			}

			if (_isEgoAttack)
			{
				auto egoPos = _alterEgo->GetComponent<Transform>()->GetPosition();
				_egoInsideAttack->GetComponent<Transform>()->SetPosition(egoPos.x, egoPos.y, egoPos.z);

				auto objectIndex = pattern->GetSubObjectIndex(_egoInsideAttack);
				pattern->_isColliderActive[objectIndex] = true;

				_egoInsideAttack->GetComponent<CylinderCollider>()->SetColliderScale(_bossCircleWarningSize * 1.5f, _bossCircleWarningSize * 1.5f, _bossCircleWarningSize * 1.5f);

				for (auto& index : _particleEgoEmergenceAttack)
				{
					index->GetComponent<Particle>()->SetActive(true);
					index->GetComponent<Particle>()->SetParticleSize(80, 80);
				}
			}
		};

	pattern->SetInitializeLogic(initializeLogic);

	auto emergence = [pattern, this]()
		{
			auto animator = _boss->GetComponent<Animator>();

			auto isPlaying = animator->Play(pattern->_animName, pattern->_speed);

			_timer += TimeManager::GetInstance().GetDeltaTime(); // 어디선가 더해지고있다

			if (_isEgoAttack)
			{
				auto egoAnimator = _alterEgo->GetComponent<Animator>();
				egoAnimator->Play("Idle", pattern->_speed, true);

				for (int i = 0; i < _particleEgoEmergenceAttack.size(); i++)
				{
					if (_timer >= 1.3f)
					{
						auto objectIndex = pattern->GetSubObjectIndex(_egoInsideAttack);
						pattern->_isColliderActive[objectIndex] = false;

						if (_timer < 2.6f)
						{
							_particleEgoEmergenceAttack[i]->GetComponent<Particle>()->SetParticleSize(80 - (60 * (_timer - 1.3f)), 80 - (60 * (_timer - 1.3f)));
						}
						else
						{
							_particleEgoEmergenceAttack[i]->GetComponent<Particle>()->SetActive(false);
						}
					}
				}
			}

			if (isPlaying)
			{
				for (int i = 0; i < _particleEmergenceAttack.size(); i++)
				{
					if (_timer >= 1.3f)
					{
						auto objectIndex = pattern->GetSubObjectIndex(_bossInsideAttack);
						pattern->_isColliderActive[objectIndex] = false;

						if (_timer < 2.6f)
						{
							_particleEmergenceAttack[i]->GetComponent<Particle>()->SetParticleSize(80 - (60 * (_timer - 1.3f)), 80 - (60 * (_timer - 1.3f)));
						}
						else
						{
							_particleEmergenceAttack[i]->GetComponent<Particle>()->SetActive(false);
						}
					}
				}
				return true;
			}
			else
			{
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

	//for (auto& pillarParticle : _swordInsideAttack->GetChilds())
	//{
	//	pattern->SetSubObject(pillarParticle);
	//}

	// 패턴 시작전에 초기화, 장판 켜줌
	auto initializeLogic = [pattern, this]()
		{
			pattern->SetSubObject(_swordInsideWarning);
			pattern->SetSubObject(_swordInsideAttack);

			_swordInsideWarning->GetComponent<TransparentMesh>()->Reset();
			_swordInsideWarning->GetComponent<TransparentMesh>()->SetActive(true);
			_swordInsideWarning->GetComponent<Transform>()->SetPosition(_swordOriginPos.x, _bossTransform->GetPosition().y, _swordOriginPos.z);
			_swordInsideAttack->GetComponent<Transform>()->SetPosition(_swordOriginPos.x, _bossTransform->GetPosition().y, _swordOriginPos.z);

			_swordTimer = 0.0f;

			_swordInsideWarning->GetComponent<Transform>()->SetScale(_swordCircleWarningSize, _swordCircleWarningSize, _swordCircleWarningSize);
			_swordInsideAttack->GetComponent<CylinderCollider>()->SetColliderScale(_swordCircleWarningSize * 1.5f, _swordCircleWarningSize * 1.5f, _swordCircleWarningSize * 1.5f);
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
				_swordTimer += TimeManager::GetInstance().GetDeltaTime();
				//_swordInsideAttack->GetComponent<Particle>()->SetActive(true);

				auto particleScaleUp = (_swordCircleWarningSize) / 100.0f;
				particleScaleUp += 1.0f;

				//_swordInsideAttack->GetComponent<Particle>()->SetParticleSize(100.f * particleScaleUp * ToolBox::GetRandomFloat(0.3f, 1.0f), 40.0f * particleScaleUp * ToolBox::GetRandomFloat(0.1f, 1.0f));

				for (auto& pillarParticle : _swordInsideAttack->GetChilds())
				{
					pillarParticle->GetComponent<Particle>()->SetActive(true);
				}

				if (_swordTimer >= 2.0f)
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

			_swordOutsideWarning->GetComponent<TransparentMesh>()->SetExceptRange(_swordOriginPos, _swordCircleWarningSize);
			_swordOutsideWarning->GetComponent<TransparentMesh>()->Reset();
			_swordOutsideWarning->GetComponent<TransparentMesh>()->SetActive(true);
			_swordInsideAttack->GetComponent<Transform>()->SetPosition(_swordOriginPos.x, _bossTransform->GetPosition().y, _swordOriginPos.z);
			_swordInsideAttack->GetComponent<CylinderCollider>()->SetColliderScale(_swordCircleWarningSize * 1.8f, _swordCircleWarningSize * 1.8f, _swordCircleWarningSize * 1.8f);

			_swordTimer = 0.0f;

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

				_swordTimer += TimeManager::GetInstance().GetDeltaTime();

				const float delays[] = { 0.1f, 0.15f, 0.2f, 0.25f, 0.3f, 0.35f, 0.4f, 0.45f, 0.5f, 0.55f, 0.6f, 0.65f, 0.7f, 0.75f, 0.8f, 0.85f, 0.9f, 0.95f, 1.0f, 1.05f };

				for (int i = 0; i < 20; ++i) {
					if (_swordTimer > delays[i])
					{
						auto particle = _particleSwordOutsideAttack[i + 1]->GetComponent<Particle>();
						particle->SetActive(true);
						float adjustedTime = _swordTimer - delays[i];
						float newSize = 60.0f + adjustedTime * 600.0f;
						particle->SetParticleSize(newSize, newSize);
					}
				}

				if (_swordTimer >= 1.1f)
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
			_swordDonutWarning2->GetComponent<TransparentMesh>()->SetExceptRange(swordPos, _swordCircleWarningSize);

			_swordDonutWarning3->GetComponent<TransparentMesh>()->Reset();
			_swordDonutWarning3->GetComponent<TransparentMesh>()->SetActive(true);
			_swordDonutWarning3->GetComponent<Transform>()->SetPosition(swordPos.x, _centerPos.y, swordPos.z);
			_swordDonutWarning3->GetComponent<TransparentMesh>()->SetExceptRange(swordPos, _swordCircleWarningSize * 2);

			_swordTimer = 0.0f;
		};

	pattern->SetInitializeLogic(initializeLogic);

	auto donutSafe = [pattern, this]()
		{
			_swordTimer += TimeManager::GetInstance().GetDeltaTime();

			auto warningInterval = 0.3f;
			auto playSecondTime = warningInterval;
			auto playThirdTime = playSecondTime + warningInterval;

			auto particleInterval = 0.5f;

			// 첫번째 장판 실행
			auto isPlayed1 = _swordDonutWarning1->GetComponent<TransparentMesh>()->PlayOnce();

			if (_swordTimer >= playSecondTime)
			{
				// 두번째 장판 실행
				auto isPlayed2 = _swordDonutWarning2->GetComponent<TransparentMesh>()->PlayOnce();
			}

			if (_swordTimer >= playThirdTime)
			{
				auto isPlayed3 = _swordDonutWarning3->GetComponent<TransparentMesh>()->PlayOnce();

				// 장판 실행이 완료되면
				if (isPlayed3)
				{
					{
						pattern->DeleteSubObject(_freeSword);
						return false;
					}
				}
			}
			return true;
		};

	// 로직 함수 실행 가능하도록 넣어주기
	pattern->SetLogic(donutSafe);

	_donutSafe = pattern;
}

void KunrealEngine::Kamen::CreateDonutAttack()
{
	BossPattern* pattern = new BossPattern();

	pattern->SetPatternName("DonutAttack");

	pattern->SetAnimName("Idle").SetRange(0.0f).SetMaxColliderCount(1).SetSpeed(20.0f).SetDamage(10.0f);
	pattern->SetAttackState(BossPattern::eAttackState::ePush);
	pattern->SetColliderType(BossPattern::eColliderType::eDonut);
	pattern->SetSubObject(_freeSword);


	auto initializeLogic = [pattern, this]()
		{
			_swordTimer = 0.0f;

			auto initPos = _swordStartPos;

			initPos.y = _centerPos.y;

			for (auto& index : _particleSwordDonut1)
			{
				index->GetComponent<Transform>()->SetPosition(initPos);

				pattern->SetSubObject(index);
			}

			for (auto& index : _particleSwordDonut2)
			{
				index->GetComponent<Transform>()->SetPosition(initPos);

				pattern->SetSubObject(index);
			}

			for (auto& index : _particleSwordDonut3)
			{
				index->GetComponent<Transform>()->SetPosition(initPos);

				pattern->SetSubObject(index);
			}

			for (auto& index : _swordDonutAttack)
			{
				index->GetComponent<Transform>()->SetPosition(initPos);

				pattern->SetSubObject(index);
			}

			auto donut1Size = _swordDonutWarning1->GetComponent<Transform>()->GetScale();
			_swordDonutAttack[0]->GetComponent<CylinderCollider>()->SetColliderScale(donut1Size.x * 2.0f, donut1Size.y * 2.0f, donut1Size.z * 2.0f);
			auto donut2Size = _swordDonutWarning2->GetComponent<Transform>()->GetScale();
			_swordDonutAttack[1]->GetComponent<CylinderCollider>()->SetColliderScale(donut2Size.x * 2.0f, donut2Size.y * 2.0f, donut2Size.z * 2.0f);
			auto donut3Size = _swordDonutWarning3->GetComponent<Transform>()->GetScale();
			_swordDonutAttack[2]->GetComponent<CylinderCollider>()->SetColliderScale(donut3Size.x * 2.0f, donut3Size.y * 2.0f, donut3Size.z * 2.0f);
		};

	pattern->SetInitializeLogic(initializeLogic);

	auto attackLogic = [pattern, this]()
		{
			auto objectIndex0 = pattern->GetSubObjectIndex(_swordDonutAttack[0]);
			auto objectIndex1 = pattern->GetSubObjectIndex(_swordDonutAttack[1]);
			auto objectIndex2 = pattern->GetSubObjectIndex(_swordDonutAttack[2]);

			auto donutInterval = 0.3f;

			_swordTimer += TimeManager::GetInstance().GetDeltaTime();

			if (_swordTimer <= donutInterval)
			{
				_particleSwordDonut1[0]->GetComponent<Particle>()->SetActive(true);
				_particleSwordDonut2[0]->GetComponent<Particle>()->SetActive(true);
				_particleSwordDonut3[0]->GetComponent<Particle>()->SetActive(true);
				//_swordDonutAttack[0]->GetComponent<BoxCollider>()->SetActive(true);
				pattern->_isColliderActive[objectIndex0] = true;

				_donutSize = 0.0f;
			}

			else if (_swordTimer > donutInterval && _swordTimer <= 2 * donutInterval)
			{
				_particleSwordDonut1[0]->GetComponent<Particle>()->SetActive(false);
				_particleSwordDonut2[0]->GetComponent<Particle>()->SetActive(false);
				_particleSwordDonut3[0]->GetComponent<Particle>()->SetActive(false);
				//_swordDonutAttack[0]->GetComponent<BoxCollider>()->SetActive(false);
				pattern->_isColliderActive[objectIndex0] = false;

				_particleSwordDonut1[1]->GetComponent<Particle>()->SetActive(true);
				_particleSwordDonut2[1]->GetComponent<Particle>()->SetActive(true);
				_particleSwordDonut3[1]->GetComponent<Particle>()->SetActive(true);
				//_swordDonutAttack[1]->GetComponent<BoxCollider>()->SetActive(true);
				pattern->_isColliderActive[objectIndex1] = true;

				_donutSize = 40.0f;
			}

			else if (_swordTimer > 2 * donutInterval)
			{
				_particleSwordDonut1[1]->GetComponent<Particle>()->SetActive(false);
				_particleSwordDonut2[1]->GetComponent<Particle>()->SetActive(false);
				_particleSwordDonut3[1]->GetComponent<Particle>()->SetActive(false);
				//_swordDonutAttack[1]->GetComponent<BoxCollider>()->SetActive(false);
				pattern->_isColliderActive[objectIndex1] = false;

				_particleSwordDonut1[2]->GetComponent<Particle>()->SetActive(true);
				_particleSwordDonut2[2]->GetComponent<Particle>()->SetActive(true);
				_particleSwordDonut2[2]->GetComponent<Particle>()->SetActive(true);
				//_swordDonutAttack[2]->GetComponent<BoxCollider>()->SetActive(true);
				pattern->_isColliderActive[objectIndex2] = true;

				_donutSize = 80.0f;
			}

			if (_swordTimer >= 3 * donutInterval)
			{
				_donutSize = 0.0f;
				pattern->DeleteSubObject(_freeSword);
				return false;
			}
		};

	pattern->SetLogic(attackLogic);

	_donutAttack = pattern;
}

void KunrealEngine::Kamen::CreateSpellAttack()
{
	BossPattern* pattern = new BossPattern();

	pattern->SetPatternName("Spell");

	pattern->SetAnimName("Spell").SetDamage(10.0f).SetSpeed(20.0f).SetRange(_info._attackRange + 50.0f).SetAfterDelay(1.0f);
	pattern->SetMaxColliderCount(1).SetAttackState(BossPattern::eAttackState::eParalysis).SetColliderType(BossPattern::eColliderType::eBox);
	pattern->SetSubObject(_lazer);
	pattern->SetSubObject(_lazerCollider);
	pattern->SetWithEgo(true);

	auto initLogic = [pattern, this]()
		{
			_boss->GetComponent<BoxCollider>()->SetActive(true);
			_boss->GetComponent<MeshRenderer>()->SetActive(true);

			_boss->GetComponent<BoxCollider>()->FixedUpdate();

			_timer = 0.0f;
			_timer2 = 0.0f;
			_timer3 = 0.0f;

			_spellDel = 0.0f;
			_spellWaveDel = 0.0f;
			auto lazerPosOffset = 20.0f;
			auto lazerScaleOffset = 90.0f;

			auto direction = GetDirection();

			auto lazerScaleDir = DirectX::XMVectorScale(direction, lazerPosOffset);
			_lazer->GetComponent<Transform>()->SetPosition(_bossTransform->GetPosition().x + lazerScaleDir.m128_f32[0], 25.0f, _bossTransform->GetPosition().z + lazerScaleDir.m128_f32[2]);
			_lazer->GetComponent<Particle>()->SetParticleRotation(90.0f, _bossTransform->GetRotation().y, 0.0f);
			_lazer->GetChilds()[0]->GetComponent<Particle>()->SetParticleRotation(90.0f, _bossTransform->GetRotation().y, 0.0f);

			pattern->SetSpeed(20.0f);

			auto lazerColliderScaleDir = DirectX::XMVectorScale(direction, lazerScaleOffset);

			_lazerCollider->GetComponent<Transform>()->SetPosition(_bossTransform->GetPosition().x + lazerColliderScaleDir.m128_f32[0], 25.0f, _bossTransform->GetPosition().z + lazerColliderScaleDir.m128_f32[2]);

			_lazerCollider->GetComponent<Transform>()->SetRotation(_bossTransform->GetRotation().x, _bossTransform->GetRotation().y + 90.0f, _bossTransform->GetRotation().z);

			if (_isEgoAttackReady)
			{
				auto egoTransform = _alterEgo->GetComponent<Transform>();
				auto egoPos = egoTransform->GetPosition();
				auto egoLazerDirection = GetEgoDirection();
				auto egoLazerScaleDir = DirectX::XMVectorScale(egoLazerDirection, lazerPosOffset);

				_egoLazer->GetComponent<Transform>()->SetPosition(egoPos.x + egoLazerScaleDir.m128_f32[0], 25.0f, egoPos.z + egoLazerScaleDir.m128_f32[2]);
				_egoLazer->GetComponent<Particle>()->SetParticleRotation(90.0f, egoTransform->GetRotation().y, 0.0f);
				_egoLazer->GetChilds()[0]->GetComponent<Particle>()->SetParticleRotation(90.0f, egoTransform->GetRotation().y, 0.0f);

				auto egolazerColliderScaleDir = DirectX::XMVectorScale(egoLazerDirection, lazerScaleOffset);

				_egoLazerCollider->GetComponent<Transform>()->SetPosition(egoTransform->GetPosition().x + egolazerColliderScaleDir.m128_f32[0], 25.0f, egoTransform->GetPosition().z + egolazerColliderScaleDir.m128_f32[2]);

				_egoLazerCollider->GetComponent<Transform>()->SetRotation(egoTransform->GetRotation().x, egoTransform->GetRotation().y + 90.0f, egoTransform->GetRotation().z);

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
					_lazerCollider->GetComponent<Light>()->SetActive(false);

					auto objectIndex = pattern->GetSubObjectIndex(_lazerCollider);
					pattern->_isColliderActive[objectIndex] = false;

					if (_isEgoAttack)
					{
						_egoLazer->GetComponent<Particle>()->SetActive(false);
						_egoLazer->GetChilds()[0]->GetComponent<Particle>()->SetActive(false);

						auto objectIndex = pattern->GetSubObjectIndex(_egoLazerCollider);
						pattern->_isColliderActive[objectIndex] = false;
					}

				}
				else
				{

					pattern->SetSpeed(10.0f);
					_lazer->GetComponent<Particle>()->SetActive(true);
					_lazer->GetComponent<Particle>()->SetParticleSize((60.f - _spellDel) * ToolBox::GetRandomFloat(0.3f, 1.0f), (60.0f - _spellDel) * ToolBox::GetRandomFloat(0.1f, 1.0f));

					_lazer->GetChilds()[0]->GetComponent<Particle>()->SetActive(true);
					_lazer->GetChilds()[0]->GetComponent<Particle>()->SetParticleSize((40.f - _spellWaveDel) * ToolBox::GetRandomFloat(0.3f, 1.0f), (40.0f - _spellWaveDel) * ToolBox::GetRandomFloat(0.1f, 1.0f));

					_lazerCollider->GetComponent<Light>()->SetActive(true);

					if (animator->GetCurrentFrame() >= 40)
					{
						_timer += TimeManager::GetInstance().GetDeltaTime();
						if (_timer < 1.0f)
						{
							_spellDel = _timer * 60;
							_spellWaveDel = _timer * 40;
						}
					}

					if (animator->GetCurrentFrame() >= 32.0f)
					{
						auto objectIndex = pattern->GetSubObjectIndex(_lazerCollider);
						// 라이트 고민중

						if (pattern->_isColliderHit[objectIndex] == false)
						{
							pattern->_isColliderActive[objectIndex] = true;
						}
						else
						{
							_timer2 += TimeManager::GetInstance().GetDeltaTime();

							if (_timer2 >= 0.5f)
							{
								pattern->_isColliderHit[objectIndex] = false;
								_timer2 = 0.0f;
							}
						}
					}


					if (_isEgoAttack)
					{
						_egoLazer->GetComponent<Particle>()->SetActive(true);
						_egoLazer->GetComponent<Particle>()->SetParticleSize((60.f - _spellDel) * ToolBox::GetRandomFloat(0.3f, 1.0f), (60.f - _spellDel) * ToolBox::GetRandomFloat(0.1f, 1.0f));

						_egoLazer->GetChilds()[0]->GetComponent<Particle>()->SetActive(true);
						_egoLazer->GetChilds()[0]->GetComponent<Particle>()->SetParticleSize((40.f - _spellWaveDel) * ToolBox::GetRandomFloat(0.3f, 1.0f), (40.f - _spellWaveDel) * ToolBox::GetRandomFloat(0.1f, 1.0f));

						if (animator->GetCurrentFrame() >= 32.0f)
						{
							auto objectIndex = pattern->GetSubObjectIndex(_egoLazerCollider);

							if (pattern->_isColliderHit[objectIndex] == false)
							{
								pattern->_isColliderActive[objectIndex] = true;
							}
							else
							{
								_timer3 += TimeManager::GetInstance().GetDeltaTime();

								if (_timer3 >= 0.5f)
								{
									pattern->_isColliderHit[objectIndex] = false;
									_timer3 = 0.0f;
								}
							}
						}
					}
				}
			}

			if (isAnimationPlaying == false)
			{
				_timer = 0.0F;
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
				DirectX::XMVECTOR newPosition = DirectX::XMVectorAdd(nowPosVec, DirectX::XMVectorScale(direction, 30.0f * step));

				auto call2Pos = _call2->GetComponent<Transform>()->GetPosition();
				_call2->GetComponent<Transform>()->SetPosition(newPosition.m128_f32[0], call2Pos.y, newPosition.m128_f32[2]);

				if (_isEgoAttack)
				{
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
					//}g

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
	pattern->SetSpeed(20.0f);

	// 패턴 시작전에 초기화, 장판 켜줌
	auto initializeLogic = [pattern, this]()
		{
			_bossCircleWarningSize = 40.0f;

			auto ranX = ToolBox::GetRandomFloat(-50.0f, 50.0f);
			auto ranY = ToolBox::GetRandomFloat(-90.0f, 10.0f);

			_emergencePos = { ranX , _bossTransform->GetPosition().y, ranY };

			_bossInsideWarning->GetComponent<TransparentMesh>()->Reset();
			_bossInsideWarning->GetComponent<TransparentMesh>()->SetActive(true);
			_bossInsideWarning->GetComponent<Transform>()->SetPosition(_emergencePos.x, _bossTransform->GetPosition().y, _emergencePos.z);
			_timer = 0.0f;

			_bossInsideWarning->GetComponent<Transform>()->SetScale(_bossCircleWarningSize, _bossCircleWarningSize, _bossCircleWarningSize);

			if (_isEgoAttackReady)
			{
				pattern->SetSubObject(_egoInsideWarning);

				_egoInsideWarning->GetComponent<TransparentMesh>()->Reset();
				_egoInsideWarning->GetComponent<TransparentMesh>()->SetActive(true);
				_egoInsideWarning->GetComponent<Transform>()->SetPosition(_alterEgo->GetComponent<Transform>()->GetPosition().x, _alterEgo->GetComponent<Transform>()->GetPosition().y, _alterEgo->GetComponent<Transform>()->GetPosition().z);

				_egoInsideWarning->GetComponent<Transform>()->SetScale(_bossCircleWarningSize, _bossCircleWarningSize, _bossCircleWarningSize);

				_isEgoAttackReady = false;

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
				auto egoAnimator = _alterEgo->GetComponent<Animator>();
				egoAnimator->Play("Idle", pattern->_speed, true);

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

void KunrealEngine::Kamen::CreateFiveWayAttack()
{
	BossPattern* pattern = new BossPattern();

	pattern->SetPatternName("FiveWayAttack");

	pattern->SetAnimName("Idle").SetDamage(10.0f).SetSpeed(20.0f).SetRange(_info._attackRange + 50.0f).SetAfterDelay(1.0f);
	pattern->SetMaxColliderCount(1).SetAttackState(BossPattern::eAttackState::ePush).SetColliderType(BossPattern::eColliderType::eBox);

	for (auto& index : _fiveAttack)
	{
		pattern->SetSubObject(index);
	}

	for (auto& index : _fakeBoss)
	{
		pattern->SetSubObject(index);
	}

	auto initLogic = [pattern, this]()
		{
			_timer = 0.0f;

			for (int i = 0; i < _fiveAttack.size(); i++)
			{
				auto bossPos = _bossTransform->GetPosition();
				auto angle = _bossTransform->GetRotation().y + (72.0f * i);

				_fiveAttack[i]->GetComponent<Transform>()->SetRotation(0.0f, angle, 0.0f);

				auto direction = DirectX::XMFLOAT3{ 0.0f, 0.0f, -1.0f };
				direction = ToolBox::RotateVector(direction, angle);

				auto interval = 120.0f;

				DirectX::XMVECTOR newPosition = DirectX::XMVectorAdd(DirectX::XMLoadFloat3(&bossPos), DirectX::XMVectorScale(DirectX::XMLoadFloat3(&direction), interval));

				_fiveAttack[i]->GetComponent<Transform>()->SetPosition(newPosition.m128_f32[0], bossPos.y, newPosition.m128_f32[2]);

				_fiveAttack[i]->GetComponent<TransparentMesh>()->Reset();
				_fiveAttack[i]->GetComponent<TransparentMesh>()->SetActive(true);

				_fakeBoss[i]->GetComponent<Transform>()->SetRotation(0.0f, angle, 0.0f);

				DirectX::XMVECTOR fakePosition = DirectX::XMVectorAdd(DirectX::XMLoadFloat3(&bossPos), DirectX::XMVectorScale(DirectX::XMLoadFloat3(&direction), 20.0f));

				_fakeBoss[i]->GetComponent<Transform>()->SetPosition(fakePosition.m128_f32[0], bossPos.y, fakePosition.m128_f32[2]);
				_fakeBoss[i]->GetComponent<Transform>()->SetRotation(0.0f, angle, 0.0f);

				_fakeDirVec[i] = direction;

				DirectX::XMVECTOR targetPos = DirectX::XMVectorAdd(DirectX::XMLoadFloat3(&bossPos), DirectX::XMVectorScale(DirectX::XMLoadFloat3(&direction), 1000.0f));

				Navigation::GetInstance().SetSEpos(1, fakePosition.m128_f32[0], 0.0f, fakePosition.m128_f32[2],
					targetPos.m128_f32[0], 0.0f, targetPos.m128_f32[2]);

				auto targetPosition = Navigation::GetInstance().FindRaycastPath(1);

				auto fakePos = _fakeBoss[i]->GetComponent<Transform>()->GetPosition();
				_fakeMoveDistance[i] = ToolBox::GetDistance(fakePos, targetPosition) - 5.0f;
			}
		};

	pattern->SetInitializeLogic(initLogic);

	auto attackLogic = [pattern, this]()
		{
			auto isPlaying = _boss->GetComponent<Animator>()->Play(pattern->_animName, pattern->_speed, true);

			bool warningFinsh = false;

			for (auto& index : _fiveAttack)
			{
				auto isPlayed = index->GetComponent<TransparentMesh>()->PlayOnce();

				if (isPlayed)
				{
					warningFinsh = true;

					break;
				}
			}

			if (warningFinsh)
			{
				auto fakeSpeed = 5.0f;

				int sumGoal = 0;

				for (int i = 0; i < _fakeBoss.size(); i++)
				{
					_fakeBoss[i]->GetComponent<Animator>()->Play("Run", pattern->_speed, true);

					_fakeBoss[i]->GetComponent<MeshRenderer>()->SetActive(true);

					for (int j = 0; j < _fakeBoss[i]->GetChilds().size(); j++)
					{
						_fakeBoss[i]->GetChilds()[j]->GetComponent<Particle>()->SetActive(true);
					}

					_fakeBoss[i]->GetComponent<Light>()->SetActive(true);

					// 콜라이더 키기
					auto objectIndex = pattern->GetSubObjectIndex(_fakeBoss[i]);
					pattern->_isColliderActive[objectIndex] = true;

					auto fakeBossPos = _fakeBoss[i]->GetComponent<Transform>()->GetPosition();

					auto moveSpeed = pattern->_speed * fakeSpeed * TimeManager::GetInstance().GetDeltaTime();

					DirectX::XMVECTOR fakePosition = DirectX::XMVectorAdd(DirectX::XMLoadFloat3(&fakeBossPos), DirectX::XMVectorScale(DirectX::XMLoadFloat3(&_fakeDirVec[i]), moveSpeed));

					_fakeMoveDistance[i] -= moveSpeed;

					_fakeBoss[i]->GetComponent<Transform>()->SetPosition(fakePosition.m128_f32[0], fakeBossPos.y, fakePosition.m128_f32[2]);

					if (_fakeMoveDistance[i] < 0.0f)
					{
						_fakeBoss[i]->GetComponent<MeshRenderer>()->SetActive(false);
						for (int j = 0; j < _fakeBoss[i]->GetChilds().size(); j++)
						{
							_fakeBoss[i]->GetChilds()[j]->GetComponent<Particle>()->SetActive(false);
						}
						_fakeBoss[i]->GetComponent<Light>()->SetActive(false);

						pattern->_isColliderActive[objectIndex] = false;

						sumGoal++;
					}
				}

				if (sumGoal == 5)
				{
					return false;
				}
			}

			return true;
		};

	pattern->SetLogic(attackLogic);

	_fiveWayAttack = pattern;
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
				}
				else
				{
					pattern->_speed = 15 * 0.3f;
				}
			}

			if (_swordTimer < 0.5f)
			{
				_swordTimer += TimeManager::GetInstance().GetDeltaTime() * 0.2f;
				_freeSword->GetComponent<MeshRenderer>()->SetDissolve(_swordTimer);
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

			pattern->SetSubObject(_freeSword);

			// 디졸브 이펙트 키기
			_freeSword->SetActive(true);
			_freeSword->GetComponent<MeshRenderer>()->SetActive(true);
			_freeSword->GetComponent<MeshRenderer>()->SetIsDissolve(true);

			_swordTimer = 0.0f;
			_freeSword->GetComponent<MeshRenderer>()->SetDissolve(_swordTimer);

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

			_swordTimer = 0.0f;
			_freeSword->GetComponent<MeshRenderer>()->SetDissolve(_swordTimer);

			_freeSword->GetComponent<Transform>()->SetRotation(180.0f, _freeSword->GetComponent<Transform>()->GetRotation().y, 0.0f);
		};

	pattern->SetInitializeLogic(swordInitLogic);

	auto emmergenceLogic = [pattern, this]()
		{
			if (_swordTimer < 0.5f)
			{
				_swordTimer += TimeManager::GetInstance().GetDeltaTime() * 0.3f;
				_freeSword->GetComponent<MeshRenderer>()->SetDissolve(_swordTimer);
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

			_swordTimer = 0.5f;
			_freeSword->GetComponent<MeshRenderer>()->SetDissolve(_swordTimer);
		};

	pattern->SetInitializeLogic(swordInitLogic);

	auto hideLogic = [pattern, this]()
		{
			if (_swordTimer > 0)
			{
				_swordTimer -= TimeManager::GetInstance().GetDeltaTime() * 0.3f;
				_freeSword->GetComponent<MeshRenderer>()->SetDissolve(_swordTimer);
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

			_freeSword->GetComponent<Transform>()->SetPosition(_swordStartPos.x, _freeSword->GetComponent<Transform>()->GetPosition().y, _swordStartPos.z);

			_swordRotation = _freeSword->GetComponent<Transform>()->GetRotation();

			_swordRotateAngle = 0.0f;
		};

	pattern->SetInitializeLogic(swordInitLogic);

	auto swordTurnLogic = [pattern, this]()
		{
			_swordRotateAngle += TimeManager::GetInstance().GetDeltaTime() * pattern->_speed;

			float x = _swordOriginPos.x - _swordCircleWarningSize * cos(_swordRotateAngle);
			float z = _swordOriginPos.z + _swordCircleWarningSize * sin(_swordRotateAngle);

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

			_freeSword->GetComponent<Transform>()->SetPosition(_swordStartPos.x, _freeSword->GetComponent<Transform>()->GetPosition().y, _swordStartPos.z);

			_swordRotation = _freeSword->GetComponent<Transform>()->GetRotation();

			_swordRotateAngle = 0.0f;
		};

	pattern->SetInitializeLogic(swordInitLogic);

	auto swordTurnLogic = [pattern, this]()
		{
			_swordRotateAngle += TimeManager::GetInstance().GetDeltaTime() * pattern->_speed;

			float x = _swordOriginPos.x - _swordCircleWarningSize * cos(_swordRotateAngle);
			float z = _swordOriginPos.z - _swordCircleWarningSize * sin(_swordRotateAngle);

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

	pattern->SetAnimName("Idle").SetSpeed(10.0f);
	pattern->SetMaxColliderCount(0);

	auto swordInitLogic = [pattern, this]()
		{
			pattern->SetSubObject(_freeSword);

			_freeSword->GetComponent<MeshRenderer>()->SetActive(true);

			_freeSword->GetComponent<MeshRenderer>()->SetIsDissolve(false);

			_freeSword->GetComponent<MeshRenderer>()->DeleteParentBone();

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

	pattern->SetAnimName("Idle").SetSpeed(70.0f).SetDamage(10.0f);
	pattern->SetAttackState(BossPattern::eAttackState::ePush);
	pattern->SetMaxColliderCount(1);
	pattern->SetColliderType(BossPattern::eColliderType::eBox);
	pattern->SetSubObject(_freeSwordCollider);

	auto lenearAttackInitLogic = [pattern, this]()
		{
			pattern->SetSubObject(_freeSword);

			_freeSword->GetComponent<MeshRenderer>()->DeleteParentBone();

			_freeSword->GetComponent<MeshRenderer>()->SetActive(true);

			_freeSword->GetComponent<MeshRenderer>()->SetIsDissolve(false);

			_swordDirection = DirectX::XMFLOAT3(1.0f, 0.0f, 0.0f);
			_swordDirection = ToolBox::RotateVector(_swordDirection, _freeSword->GetComponent<Transform>()->GetRotation().y);

			auto direction = DirectX::XMVector3Normalize(DirectX::XMLoadFloat3(&_swordDirection));

			DirectX::XMStoreFloat3(&_swordDirection, direction);

			auto swordRotation = _freeSword->GetComponent<Transform>()->GetRotation();

			_freeSwordCollider->GetComponent<Transform>()->SetRotation(0.0f, swordRotation.y + 90.0f, 0.0f);

			_freeSwordCollider->GetComponent<BoxCollider>()->SetActive(true);

			auto swordPos = _freeSword->GetComponent<Transform>()->GetPosition();
			DirectX::XMVECTOR targetPos = DirectX::XMVectorAdd(DirectX::XMLoadFloat3(&swordPos), DirectX::XMVectorScale(DirectX::XMLoadFloat3(&_swordDirection), 1000.0F));

			Navigation::GetInstance().SetSEpos(1, swordPos.x, 0.0f, swordPos.z,
				targetPos.m128_f32[0], 0.0f, targetPos.m128_f32[2]);

			auto targetPosition = Navigation::GetInstance().FindRaycastPath(1);

			_swordMoveDistance = ToolBox::GetDistance(swordPos, targetPosition) - 10.0f;
		};

	pattern->SetInitializeLogic(lenearAttackInitLogic);

	auto swordLinearAttackLogic = [pattern, this]()
		{
			auto objectIndex = pattern->GetSubObjectIndex(_freeSwordCollider);
			pattern->_isColliderActive[objectIndex] = true;

			float moveSpeed = pattern->_speed * TimeManager::GetInstance().GetDeltaTime();

			auto swordTransform = _freeSword->GetComponent<Transform>()->GetPosition();

			DirectX::XMVECTOR newPosition = DirectX::XMVectorAdd(DirectX::XMLoadFloat3(&swordTransform), DirectX::XMVectorScale(DirectX::XMLoadFloat3(&_swordDirection), moveSpeed));

			_freeSword->GetComponent<Transform>()->SetPosition(newPosition.m128_f32[0], newPosition.m128_f32[1], newPosition.m128_f32[2]);

			DirectX::XMVECTOR colliderNewPosition = DirectX::XMVectorAdd(DirectX::XMLoadFloat3(&swordTransform), DirectX::XMVectorScale(DirectX::XMLoadFloat3(&_swordDirection), 15.0F));

			_freeSwordCollider->GetComponent<Transform>()->SetPosition(colliderNewPosition.m128_f32[0], colliderNewPosition.m128_f32[1], colliderNewPosition.m128_f32[2]);

			_swordMoveDistance -= moveSpeed;

			if (_swordMoveDistance <= 0.0f)
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

void KunrealEngine::Kamen::CreateBattleCry()
{
	BossPattern* pattern = new BossPattern();

	pattern->SetPatternName("BattleCry");

	pattern->SetAnimName("BattleCry").SetSpeed(20.0f).SetSkipChase(true);

	auto initLogic = [pattern, this]()
		{
			StopSpecialPattern();

			_info._armor = 100.0f;

			_boss->GetComponent<MeshRenderer>()->SetActive(false);
			_boss->GetComponent<Transform>()->SetPosition(_centerPos);
			_boss->GetComponent<Transform>()->SetRotation(0.0f, 0.0f, 0.0f);

			_timer = 0.0f;
		};

	pattern->SetInitializeLogic(initLogic);

	auto attakLogic = [pattern, this]()
		{
			_timer += TimeManager::GetInstance().GetDeltaTime();

			if (_boss->GetComponent<MeshRenderer>()->GetActivated() == false)
			{
				if (_timer > 1.0f)
				{
					_boss->GetComponent<MeshRenderer>()->SetActive(true);
					_boss->GetComponent<Transform>()->SetRotation(0.0f, -30.0f, 0.0f);
				}
				else
				{
					return true;
				}
			}

			auto animator = _boss->GetComponent<Animator>();
			auto isAnimationPlaying = animator->Play(pattern->_animName, pattern->_speed, false);

			if (!isAnimationPlaying)
			{
				_boss->GetComponent<BoxCollider>()->SetActive(false);
				_boss->GetComponent<MeshRenderer>()->SetActive(false);
				_boss->GetComponent<Transform>()->SetRotation(0.0f, 0.0f, 0.0f);

				_boss->GetComponent<BoxCollider>()->FixedUpdate();

				_info._armor = 1.0f;
				return false;
			}

			return true;
		};

	pattern->SetLogic(attakLogic);

	_battleCry = pattern;
}

void KunrealEngine::Kamen::CreateDecalTest()
{
	BossPattern* pattern = new BossPattern();

	pattern->SetPatternName("decalTest");

	pattern->SetAnimName("BattleCry").SetSpeed(20.0f).SetSkipChase(true);

	std::string objectName = "testDecal";
	static GameObject* testDecal = _boss->GetObjectScene()->CreateObject(objectName);
	testDecal->AddComponent<TransparentMesh>();
	testDecal->GetComponent<TransparentMesh>()->CreateTMesh(objectName, "Resources/Textures/MeteorDecal/Decal.png", 0.6f);
	//testDecal->GetComponent<TransparentMesh>()->SetTimer(500.0f);
	testDecal->GetComponent<TransparentMesh>()->SetRenderType(7);
	testDecal->GetComponent<TransparentMesh>()->SetDecal(true);
	testDecal->GetComponent<Transform>()->SetScale(100.0f, 100.0f, 100.0f);
	testDecal->GetComponent<Transform>()->SetPosition(_centerPos);
	testDecal->SetTotalComponentState(false);
	testDecal->SetActive(false);
	testDecal->GetComponent<TransparentMesh>()->SetInfinite(true);

	pattern->SetSubObject(testDecal);

	auto initLogic = [pattern, this]()
		{
			testDecal->GetComponent<TransparentMesh>()->Reset();
			testDecal->GetComponent<TransparentMesh>()->SetActive(true);

			_largeBlade->GetComponent<Transform>()->SetPosition(_centerPos);

			_timer = 0.0f;
		};
	pattern->SetInitializeLogic(initLogic);

	auto attakLogic = [pattern, this]()
		{
			_largeBlade->SetActive(true);
			_largeBlade->GetComponent<MeshCollider>()->SetActive(true);

			testDecal->GetComponent<TransparentMesh>()->PlayOnce();

			_timer += TimeManager::GetInstance().GetDeltaTime();

			static bool a = false;

			if (10.0f >= _timer && _timer >= 5.0f)
			{
				testDecal->GetComponent<TransparentMesh>()->StopPlayingInfinite();

			}
			else if (_timer > 10.0f)
			{
				if (a == false)
				{
					testDecal->GetComponent<TransparentMesh>()->SetInfinite(false);
					testDecal->GetComponent<TransparentMesh>()->SetTimer(5.0f);
					testDecal->GetComponent<TransparentMesh>()->Reset();

					a = true;
				}

				testDecal->GetComponent<TransparentMesh>()->SetTexture("Resources/Textures/MeteorDecal/MeteorDecal.png");
			}


			//if (isPlayed)
			//{
			//	return false;
			//}

			return true;
		};

	pattern->SetLogic(attakLogic);

	_basicPattern[0].emplace_back(pattern);
}

void KunrealEngine::Kamen::CreateSwordMultipleAttack()
{
	BossPattern* pattern = new BossPattern();

	pattern->SetPatternName("SwordMultipleAttack");

	pattern->SetAnimName("Idle").SetMaxColliderCount(1).SetSpeed(15.0f).SetDamage(10.0f).SetRange(_info._attackRange + 40.0f).SetAttackState(BossPattern::eAttackState::ePush);
	pattern->SetColliderType(BossPattern::eColliderType::eSwordMultiple).SetSkipChase(true);

	for (auto& index : _multipleSwordVec)
	{
		pattern->SetSubObject(index);
	}

	for (auto& index : _multipleSwordWarningVec)
	{
		pattern->SetSubObject(index);
	}

	auto initLogic = [pattern, this]()
		{
			for (auto& index : _multipleSwordVec)
			{
				auto swordTransform = index->GetComponent<Transform>();

				auto ranX = ToolBox::GetRandomFloat(-1.0f, 1.0f);
				auto ranZ = ToolBox::GetRandomFloat(-1.0f, 1.0f);

				auto forPosVec = DirectX::XMFLOAT3(ranX, 0.0f, ranZ);

				DirectX::XMVECTOR startPos = DirectX::XMVectorAdd(DirectX::XMLoadFloat3(&_centerPos), DirectX::XMVectorScale(DirectX::XMLoadFloat3(&forPosVec), 1000.0f));

				Navigation::GetInstance().SetSEpos(1, _centerPos.x, 0.0f, _centerPos.y,
					startPos.m128_f32[0], 0.0f, startPos.m128_f32[2]);

				auto startPosition = Navigation::GetInstance().FindRaycastPath(1);

				swordTransform->SetPosition(startPosition.x, 30.0f, startPosition.z);

				_multipleSwordDirectionVec.emplace_back();
				_multipleSwordMoveDistance.emplace_back();
				_multipleSwordMoveStart.emplace_back(false);

				DirectX::XMFLOAT3 goalPosVec = DirectX::XMFLOAT3(0.0, 0.0f, 0.0f);

				if (_multipleSwordDirectionVec.size() % 4 == 1)
				{
					_multipleSwordLookPlayer.emplace_back(false);
				}

				else
				{

					if (forPosVec.x >= 0)
					{
						auto ran = ToolBox::GetRandomFloat(-1.0f, 0.0f);
						goalPosVec.x = ran;
					}
					else
					{
						auto ran = ToolBox::GetRandomFloat(0.0f, 1.0f);
						goalPosVec.x = ran;
					}

					if (forPosVec.z >= 0)
					{
						auto ran = ToolBox::GetRandomFloat(-1.0f, 0.0f);
						goalPosVec.z = ran;
					}
					else
					{
						auto ran = ToolBox::GetRandomFloat(0.0f, 1.0f);
						goalPosVec.z = ran;
					}

					auto normalizeVec = DirectX::XMVector3Normalize(DirectX::XMLoadFloat3(&goalPosVec));

					DirectX::XMStoreFloat3(&goalPosVec, normalizeVec);

					auto angle = ToolBox::CalculateAngleBetweenVectors(DirectX::XMFLOAT3(0, 0, -1), goalPosVec);

					_multipleSwordDirectionVec.back() = goalPosVec;

					swordTransform->SetRotation(swordTransform->GetRotation().x, angle, swordTransform->GetRotation().z);

					DirectX::XMVECTOR targetPos = DirectX::XMVectorAdd(DirectX::XMLoadFloat3(&startPosition), DirectX::XMVectorScale(DirectX::XMLoadFloat3(&goalPosVec), 1000.0f));

					Navigation::GetInstance().SetSEpos(1, startPosition.x, 0.0f, startPosition.z,
						targetPos.m128_f32[0], 0.0f, targetPos.m128_f32[2]);

					auto targetPosition = Navigation::GetInstance().FindRaycastPath(1);

					auto distance = ToolBox::GetDistance(startPosition, targetPosition) - 5.0f;

					_multipleSwordMoveDistance.back() = distance;

					int index = _multipleSwordMoveDistance.size() - 1;

					if (index % 4 > 0)
					{
						auto interval = 200.0f;

						_multipleSwordWarningVec[index]->GetComponent<Transform>()->SetRotation(0.0f, swordTransform->GetRotation().y, 0.0f);


						DirectX::XMVECTOR newPosition = DirectX::XMVectorAdd(DirectX::XMLoadFloat3(&startPosition), DirectX::XMVectorScale(DirectX::XMLoadFloat3(&_multipleSwordDirectionVec[index]), interval));

						_multipleSwordWarningVec[index]->GetComponent<Transform>()->SetPosition(newPosition.m128_f32[0], _centerPos.y, newPosition.m128_f32[2]);

						_multipleSwordWarningVec[index]->GetComponent<TransparentMesh>()->Reset();
						_multipleSwordWarningVec[index]->GetComponent<TransparentMesh>()->SetActive(true);
					}
				}
			}

			_timer = 0.0f;
		};

	pattern->SetInitializeLogic(initLogic);

	auto attackLogic = [pattern, this]()
		{
			_timer += TimeManager::GetInstance().GetDeltaTime();

			auto goalCount = 0;

			auto swordSpeed = 10.0f;
			auto moveSpeed = pattern->_speed * swordSpeed * TimeManager::GetInstance().GetDeltaTime();

			for (int i = 0; i < _multipleSwordVec.size(); i++)
			{

				if (_multipleSwordMoveDistance[i] < 0)
				{
					_multipleSwordVec[i]->GetComponent<MeshRenderer>()->SetActive(false);
					for (auto& multipleSwordParticleVec : _multipleSwordVec[i]->GetChilds())
					{
						multipleSwordParticleVec->GetComponent<Particle>()->SetActive(false);
					}

					auto objectIndex = pattern->GetSubObjectIndex(_multipleSwordVec[i]);
					pattern->_isColliderActive[objectIndex] = false;

					goalCount++;
				}

				else
				{
					// 플레이어 따라다니는 검 타겟 포지션 및 거리 지정해주기
					if (_timer >= 0.2 * i)
					{
						auto index = i % 4;

						if (index == 0)
						{
							if (_multipleSwordLookPlayer[(int)i * 0.25f] == false)
							{
								auto swordPos = _multipleSwordVec[i]->GetComponent<Transform>()->GetPosition();

								auto playerPos = _playerTransform->GetPosition();
								auto tempGoalPos = ToolBox::GetDirectionVec(swordPos, playerPos);
								auto goalPosVec = DirectX::XMFLOAT3(tempGoalPos.m128_f32[0], tempGoalPos.m128_f32[1], tempGoalPos.m128_f32[2]);

								auto angle = ToolBox::CalculateAngleBetweenVectors(DirectX::XMFLOAT3(0, 0, -1), goalPosVec);

								_multipleSwordDirectionVec[i] = goalPosVec;

								auto swordTransform = _multipleSwordVec[i]->GetComponent<Transform>();
								swordTransform->SetRotation(swordTransform->GetRotation().x, angle, swordTransform->GetRotation().z);

								DirectX::XMVECTOR targetPos = DirectX::XMVectorAdd(DirectX::XMLoadFloat3(&swordPos), DirectX::XMVectorScale(DirectX::XMLoadFloat3(&goalPosVec), 1000.0f));

								Navigation::GetInstance().SetSEpos(1, swordPos.x, 0.0f, swordPos.z,
									targetPos.m128_f32[0], 0.0f, targetPos.m128_f32[2]);

								auto targetPosition = Navigation::GetInstance().FindRaycastPath(1);

								auto distance = ToolBox::GetDistance(swordPos, targetPosition) - 5.0f;

								_multipleSwordMoveDistance[i] = distance;

								_multipleSwordLookPlayer[i * 0.25f] = true;



								auto interval = 200.0f;

								_multipleSwordWarningVec[i]->GetComponent<Transform>()->SetRotation(0.0f, swordTransform->GetRotation().y, 0.0f);


								DirectX::XMVECTOR newPosition = DirectX::XMVectorAdd(DirectX::XMLoadFloat3(&swordPos), DirectX::XMVectorScale(DirectX::XMLoadFloat3(&_multipleSwordDirectionVec[i]), interval));

								_multipleSwordWarningVec[i]->GetComponent<Transform>()->SetPosition(newPosition.m128_f32[0], _centerPos.y, newPosition.m128_f32[2]);

								_multipleSwordWarningVec[i]->GetComponent<TransparentMesh>()->Reset();
								_multipleSwordWarningVec[i]->GetComponent<TransparentMesh>()->SetActive(true);
							}
						}

						// 모든 공격중인 칼에 대한 이동 로직
						if (_multipleSwordMoveStart[i] == false)
						{
							_multipleSwordVec[i]->GetComponent<MeshRenderer>()->SetActive(true);
							for (auto& multipleSwordParticleVec : _multipleSwordVec[i]->GetChilds())
							{
								multipleSwordParticleVec->GetComponent<Particle>()->SetActive(true);
							}

							auto objectIndex = pattern->GetSubObjectIndex(_multipleSwordVec[i]);
							pattern->_isColliderActive[objectIndex] = true;

							_multipleSwordVec[i]->GetComponent<BoxCollider>()->FixedUpdate();

							_multipleSwordMoveStart[i] == true;
						}


						auto isPlayed = _multipleSwordWarningVec[i]->GetComponent<TransparentMesh>()->PlayOnce();

						if (isPlayed)
						{
							auto nowPos = _multipleSwordVec[i]->GetComponent<Transform>()->GetPosition();
							DirectX::XMVECTOR swordPosition = DirectX::XMVectorAdd(DirectX::XMLoadFloat3(&nowPos), DirectX::XMVectorScale(DirectX::XMLoadFloat3(&_multipleSwordDirectionVec[i]), moveSpeed));

							_multipleSwordMoveDistance[i] -= moveSpeed;

							_multipleSwordVec[i]->GetComponent<Transform>()->SetPosition(swordPosition.m128_f32[0], 30.0f, swordPosition.m128_f32[2]);
						}
					}
				}
			}

			// 끝날때 실행할 로직 -> 코어패턴이어서 한번만 들어오기에 변수들다 지워줌
			if (goalCount == _multipleSwordVec.size())
			{
				_multipleSwordMoveStart.clear();
				_multipleSwordLookPlayer.clear();
				_multipleSwordDirectionVec.clear();
				_multipleSwordMoveDistance.clear();

				_boss->GetComponent<MeshRenderer>()->SetActive(true);

				return false;
			}

			return true;
		};

	pattern->SetLogic(attackLogic);

	_swordMultipleAttack = pattern;
}

void KunrealEngine::Kamen::CreateKamenHoldSword()
{
	BossPattern* pattern = new BossPattern();

	pattern->SetPatternName("HoldSword");

	pattern->SetAnimName("SwordIdle").SetSpeed(20.0f).SetSkipChase(true);

	auto initializeLogic = [pattern, this]()
		{
			_kamenSwordLight->SetActive(true);

			_kamenSword->SetActive(true);
			_kamenSword->GetComponent<MeshRenderer>()->SetActive(true);

			auto swordChild = _kamenSword->GetChilds();
			for (auto& object : swordChild)
			{
				object->SetActive(true);
				object->SetTotalComponentState(true);
			}

			_kamenSword->GetComponent<MeshRenderer>()->SetIsDissolve(true);

			_timer = 0.0f;
		};

	pattern->SetInitializeLogic(initializeLogic);

	auto mainLogic = [pattern, this]()
		{
			auto animator = _boss->GetComponent<Animator>();
			auto isAnimationPlaying = animator->Play(pattern->_animName, pattern->_speed, true);

			if (_timer < 0.5f)
			{
				_timer += TimeManager::GetInstance().GetDeltaTime() * 0.15f;
				_kamenSword->GetComponent<MeshRenderer>()->SetDissolve(_timer);
			}

			else
			{
				_kamenSword->GetComponent<MeshRenderer>()->SetIsDissolve(false);
				ChangePhase(3);
				return false;
			}

			return true;
		};

	pattern->SetLogic(mainLogic);

	_holdSword = pattern;
}

void KunrealEngine::Kamen::CreateSwordSwingVertical()
{
	BossPattern* pattern = new BossPattern();

	pattern->SetPatternName("SwordSwingVertical");

	pattern->SetAnimName("SwordSwingVertical").SetMaxColliderCount(1).SetSpeed(10.0f).SetDamage(10.0f).SetRange(_info._attackRange + 40.0f).SetAttackState(BossPattern::eAttackState::ePush);
	pattern->SetColliderType(BossPattern::eColliderType::eSwordColliderWithCylinderCollider);

	pattern->SetSubObject(_swordDonutWarning1);
	pattern->SetSubObject(_swordDonutWarning2);
	pattern->SetSubObject(_swordDonutAttack[0]);
	pattern->SetSubObject(_kamenSwordCollider);

	auto initLogic = [pattern, this]()
		{
			auto bossDir = GetDirection();
			auto bossPos = _bossTransform->GetPosition();

			auto swordAttackPoint = DirectX::XMVectorAdd(DirectX::XMLoadFloat3(&bossPos), DirectX::XMVectorScale(bossDir, 62.0f));

			DirectX::XMFLOAT3 swordAttackFloat3;

			DirectX::XMStoreFloat3(&swordAttackFloat3, swordAttackPoint);

			_swordCircleWarningSize = 50.0f;
			auto outsideCircleSize = _swordCircleWarningSize * 2.0f;

			_swordDonutWarning1->GetComponent<TransparentMesh>()->Reset();
			_swordDonutWarning1->GetComponent<TransparentMesh>()->SetActive(true);
			_swordDonutWarning2->GetComponent<Transform>()->SetScale(_swordCircleWarningSize, _swordCircleWarningSize, _swordCircleWarningSize);
			_swordDonutWarning1->GetComponent<Transform>()->SetPosition(swordAttackFloat3.x, _centerPos.y, swordAttackFloat3.z);

			_swordDonutWarning2->GetComponent<TransparentMesh>()->Reset();
			_swordDonutWarning2->GetComponent<TransparentMesh>()->SetActive(true);
			_swordDonutWarning2->GetComponent<TransparentMesh>()->SetExceptRange(swordAttackFloat3, _swordCircleWarningSize);
			_swordDonutWarning2->GetComponent<Transform>()->SetPosition(swordAttackFloat3.x, _centerPos.y, swordAttackFloat3.z);
			_swordDonutWarning2->GetComponent<Transform>()->SetScale(outsideCircleSize, outsideCircleSize, outsideCircleSize);

			_verticalParticleSide[0]->GetComponent<Transform>()->SetPosition(swordAttackFloat3.x, _centerPos.y, swordAttackFloat3.z);
			_verticalParticleSide[1]->GetComponent<Transform>()->SetPosition(swordAttackFloat3.x, _centerPos.y, swordAttackFloat3.z);
			_verticalParticle[0]->GetComponent<Transform>()->SetPosition(swordAttackFloat3.x, _centerPos.y, swordAttackFloat3.z);
			_verticalParticle[1]->GetComponent<Transform>()->SetPosition(swordAttackFloat3.x, _centerPos.y, swordAttackFloat3.z);

			_swordDonutAttack[0]->GetComponent<Transform>()->SetPosition(swordAttackFloat3.x, _centerPos.y, swordAttackFloat3.z);

			_swordDonutAttack[0]->GetComponent<CylinderCollider>()->SetColliderScale(outsideCircleSize * 2, outsideCircleSize * 2, outsideCircleSize * 2);

			_timer = 0.0;
		};

	pattern->SetInitializeLogic(initLogic);

	auto attackLogic = [pattern, this]()
		{
			auto animator = _boss->GetComponent<Animator>();

			auto isAnimationPlaying = animator->Play(pattern->_animName, pattern->_speed, false);

			_swordDonutWarning1->GetComponent<TransparentMesh>()->PlayOnce();
			_swordDonutWarning2->GetComponent<TransparentMesh>()->PlayOnce();

			auto nowFrame = animator->GetCurrentFrame();

			SetKamenSwordCollider(21, 27, nowFrame);

			auto index = pattern->GetSubObjectIndex(_swordDonutAttack[0]);

			if (28 <= nowFrame && nowFrame <= 40)
			{
				auto outsideCircleSize = _swordCircleWarningSize * 2.0f;
				_timer += TimeManager::GetInstance().GetDeltaTime();

				pattern->_isColliderActive[index] = true;


				for (auto& verticalParticle : _verticalParticleSide)
				{
					verticalParticle->SetActive(true);
					verticalParticle->GetComponent<Particle>()->SetActive(true);
					verticalParticle->GetComponent<Particle>()->SetParticleSize(outsideCircleSize + (_timer * 500 * ToolBox::GetRandomFloat(0.8f, 1.0f)), outsideCircleSize + (_timer * 500 * ToolBox::GetRandomFloat(0.8f, 1.0f)));
				}

				for (auto& verticalParticle : _verticalParticle)
				{
					verticalParticle->SetActive(true);
					verticalParticle->GetComponent<Particle>()->SetActive(true);
				}

				_verticalParticle[0]->GetComponent<Particle>()->SetParticleSize((_timer * 30) * ToolBox::GetRandomFloat(0.8f, 1.0f), (_timer * 30) * ToolBox::GetRandomFloat(0.8f, 1.0f));
				_verticalParticle[1]->GetComponent<Particle>()->SetParticleSize((_timer * 50) * ToolBox::GetRandomFloat(0.8f, 1.0f), (_timer * 50) * ToolBox::GetRandomFloat(0.8f, 1.0f));
			}
			else
			{
				pattern->_isColliderActive[index] = false;

				for (auto& verticalParticle : _verticalParticleSide)
				{
					verticalParticle->SetActive(false);
				}
				for (auto& verticalParticle : _verticalParticle)
				{
					verticalParticle->SetActive(false);
				}
			}

			if (isAnimationPlaying == false)
			{
				_timer = 0.0f;
				return false;
			}

			return true;
		};

	pattern->SetLogic(attackLogic);

	_swordSwingVertical = pattern;
}

void KunrealEngine::Kamen::CreateSwordSwingTwice()
{
	BossPattern* pattern = new BossPattern();

	pattern->SetPatternName("SwordSwingTwice");

	pattern->SetAnimName("SwordSwingTwice").SetMaxColliderCount(1).SetSpeed(10.0f).SetDamage(10.0f).SetRange(_info._attackRange + 40.0f).SetAttackState(BossPattern::eAttackState::eParalysis);
	pattern->SetColliderType(BossPattern::eColliderType::eSwordDirection).SetSkipChase(true);

	pattern->SetSubObject(_kamenSwordCollider);

	auto initLogic = [pattern, this]()
		{
			_boss->GetComponent<MeshRenderer>()->SetActive(false);
			_boss->GetComponent<BoxCollider>()->SetActive(false);
			TeleportToPlayer();

			pattern->SetAttackState(BossPattern::eAttackState::eParalysis);
			pattern->SetDamage(10.0f);

			_isSwordSecondAttackStart = false;

			auto nowPos = _bossTransform->GetPosition();
			_bossTransform->SetPosition(nowPos.x, 30.0f, nowPos.z);
			auto nowRot = _bossTransform->GetRotation();
			_bossTransform->SetRotation(-33.0f, nowRot.y, nowRot.z);

			_boss->GetComponent<MeshRenderer>()->SetActive(true);
			_boss->GetComponent<BoxCollider>()->SetActive(true);
		};

	pattern->SetInitializeLogic(initLogic);

	auto attackLogic = [pattern, this]()
		{
			auto animator = _boss->GetComponent<Animator>();

			auto isAnimationPlaying = animator->Play(pattern->_animName, pattern->_speed, false);

			auto nowFrame = animator->GetCurrentFrame();

			if (19 <= nowFrame && nowFrame <= 36)
			{
				SetKamenSwordCollider();
			}
			else if (36 < nowFrame && nowFrame < 47)
			{
				auto nowRot = _bossTransform->GetRotation();
				if (nowRot.x > -65.0f)
				{
					auto rotSpeed = 30.0f;
					auto nextAngle = nowRot.x -= TimeManager::GetInstance().GetDeltaTime() * rotSpeed;
					_bossTransform->SetRotation(nextAngle, nowRot.y, nowRot.z);
				}

				auto index = pattern->GetSubObjectIndex(_kamenSwordCollider);

				pattern->_isColliderActive[index] = false;
			}
			else if (47 <= nowFrame && nowFrame <= 59)
			{
				if (!_isSwordSecondAttackStart)
				{
					pattern->SetAttackState(BossPattern::eAttackState::ePush);
					pattern->SetDamage(pattern->_damage * 1.5f);

					auto index = pattern->GetSubObjectIndex(_kamenSwordCollider);

					pattern->_isColliderHit[index] = false;

					_isSwordSecondAttackStart = true;
				}

				SetKamenSwordCollider();
			}
			else if (59 < nowFrame)
			{
				auto nowPos = _bossTransform->GetPosition();

				auto trsSpeed = 20.0f;
				auto rotSpeed = 30.0f;

				if (nowPos.y > _centerPos.y)
				{
					auto newPosY = nowPos.y - TimeManager::GetInstance().GetDeltaTime() * trsSpeed;
					_bossTransform->SetPosition(nowPos.x, newPosY, nowPos.z);
				}
				else
				{
					_bossTransform->SetPosition(nowPos.x, _centerPos.y, nowPos.z);
				}

				auto nowRot = _bossTransform->GetRotation();
				if (nowRot.x < 0.0f)
				{
					auto newRotX = nowRot.x + TimeManager::GetInstance().GetDeltaTime() * rotSpeed;
					_bossTransform->SetRotation(newRotX, nowRot.y, nowRot.z);
				}
				else
				{
					_bossTransform->SetRotation(0.0f, nowRot.y, nowRot.z);
				}

				auto index = pattern->GetSubObjectIndex(_kamenSwordCollider);

				pattern->_isColliderActive[index] = false;
			}

			else
			{
				auto index = pattern->GetSubObjectIndex(_kamenSwordCollider);

				pattern->_isColliderActive[index] = false;
			}

			if (_bossTransform->GetRotation().x == 0.0f && _bossTransform->GetPosition().y == _centerPos.y)
			{
				return false;
			}

			return true;
		};

	pattern->SetLogic(attackLogic);

	_swordSwingTwice = pattern;
}

void KunrealEngine::Kamen::CreateSwordSwingTwiceHard()
{
	BossPattern* pattern = new BossPattern();

	pattern->SetPatternName("SwordHardSwingTwice");

	pattern->SetAnimName("SwordHardSwingTwice").SetMaxColliderCount(1).SetSpeed(15.0f).SetDamage(10.0f).SetRange(_info._attackRange + 40.0f).SetAttackState(BossPattern::eAttackState::ePush);
	pattern->SetColliderType(BossPattern::eColliderType::eSwordColliderWithMeshCollider);

	pattern->SetSubObject(_kamenSwordCollider);

	for (auto& index : _blade)
	{
		pattern->SetSubObject(index);
	}


	auto initLogic = [pattern, this]()
		{
			pattern->SetAttackState(BossPattern::eAttackState::eParalysis);
			pattern->SetDamage(10.0f);

			_isSwordSecondAttackStart = false;

			for (int i = 0; i < _blade.size(); i++)
			{
				auto bossDirection = GetDirection();

				_swordDirection = { bossDirection.m128_f32[0], bossDirection.m128_f32[1], bossDirection.m128_f32[2] };

				_swordDirection2 = ToolBox::RotateVector(DirectX::XMFLOAT3(0, 0, -1.0f), _bossTransform->GetRotation().y + 180.0f);

				auto bossPos = _bossTransform->GetPosition();

				auto bladeStartPos = DirectX::XMVectorAdd(DirectX::XMLoadFloat3(&bossPos), DirectX::XMVectorScale(bossDirection, 40.0f));

				auto bladeStartPos2 = DirectX::XMVectorAdd(DirectX::XMLoadFloat3(&bossPos), DirectX::XMVectorScale(DirectX::XMLoadFloat3(&_swordDirection2), 40.0f));

				if (i == 0)
				{
					_blade[i]->GetComponent<Transform>()->SetPosition(bladeStartPos.m128_f32[0], 15.0f, bladeStartPos.m128_f32[2]);
					_blade[i]->GetComponent<Transform>()->SetRotation(0.0f, _bossTransform->GetRotation().y, 0.0f);
				}
				else
				{
					_blade[i]->GetComponent<Transform>()->SetPosition(bladeStartPos2.m128_f32[0], 15.0f, bladeStartPos2.m128_f32[2]);
					_blade[i]->GetComponent<Transform>()->SetRotation(0.0f, _bossTransform->GetRotation().y + 180, 0.0f);
				}

				auto bladePos = _blade[0]->GetComponent<Transform>()->GetPosition();
				auto bladePos2 = _blade[1]->GetComponent<Transform>()->GetPosition();

				DirectX::XMVECTOR targetPos1 = DirectX::XMVectorAdd(DirectX::XMLoadFloat3(&bossPos), DirectX::XMVectorScale(DirectX::XMLoadFloat3(&_swordDirection), 1000.0f));

				Navigation::GetInstance().SetSEpos(1, bladePos.x, 0.0f, bladePos.z,
					targetPos1.m128_f32[0], 0.0f, targetPos1.m128_f32[2]);

				auto targetPosition = Navigation::GetInstance().FindRaycastPath(1);


				DirectX::XMVECTOR targetPos2 = DirectX::XMVectorAdd(DirectX::XMLoadFloat3(&bossPos), DirectX::XMVectorScale(DirectX::XMLoadFloat3(&_swordDirection2), 1000.0f));

				Navigation::GetInstance().SetSEpos(1, bladePos2.x, 0.0f, bladePos.z,
					targetPos2.m128_f32[0], 0.0f, targetPos2.m128_f32[2]);

				auto targetPosition2 = Navigation::GetInstance().FindRaycastPath(1);

				_swordMoveDistance = ToolBox::GetDistance(bladePos, targetPosition) - 5.0f;

				_swordMoveDistance2 = ToolBox::GetDistance(bladePos2, targetPosition2) - 5.0f;

				_isRotateFinish = false;
			}
		};

	pattern->SetInitializeLogic(initLogic);

	auto attackLogic = [pattern, this]()
		{
			auto animator = _boss->GetComponent<Animator>();

			auto isAnimationPlaying = animator->Play(pattern->_animName, pattern->_speed, false);

			auto nowFrame = animator->GetCurrentFrame();

			auto bladeMesh1 = _blade[0]->GetComponent<MeshRenderer>();
			auto bladeMesh2 = _blade[1]->GetComponent<MeshRenderer>();

			if (52 < nowFrame && nowFrame < 69)
			{
				SetKamenSwordCollider();

				if (nowFrame > 64)
				{
					bladeMesh1->SetActive(true);
					for (auto& bladeParticle : _bladeRParticle)
					{
						bladeParticle->GetComponent<Particle>()->SetActive(true);
						bladeParticle->GetComponent<Light>()->SetActive(true);
					}
					for (auto& bladeParticle : _bladeRParticleWave)
					{
						bladeParticle->GetComponent<Particle>()->SetActive(true);
					}

					auto index = pattern->GetSubObjectIndex(_blade[0]);

					pattern->_isColliderActive[index] = true;
				}
			}
			else if (82 < nowFrame && nowFrame < 94)
			{
				SetKamenSwordCollider();

				if (nowFrame >= 91)
				{
					bladeMesh2->SetActive(true);
					for (auto& bladeParticle : _bladeLParticle)
					{
						bladeParticle->GetComponent<Particle>()->SetActive(true);
						bladeParticle->GetComponent<Light>()->SetActive(true);
					}
					for (auto& bladeParticle : _bladeLParticleWave)
					{
						bladeParticle->GetComponent<Particle>()->SetActive(true);
					}

					auto index = pattern->GetSubObjectIndex(_blade[1]);

					pattern->_isColliderActive[index] = true;
				}
			}
			else
			{
				auto index = pattern->GetSubObjectIndex(_kamenSwordCollider);

				pattern->_isColliderActive[index] = false;
			}

			if (68 < nowFrame && nowFrame < 83)
			{
				if (!_isRotateFinish)
				{
					auto rotateFinish = RotateClockWise(180.0f, pattern->_speed * 13.0f, true);

					_isRotateFinish = rotateFinish;
				}
			}

			auto bladeSpeed = 10.0f * TimeManager::GetInstance().GetDeltaTime();
			auto moveSpeed = pattern->_speed * bladeSpeed;

			if (bladeMesh1->GetActivated())
			{
				if (_swordMoveDistance < 0)
				{
					bladeMesh1->SetActive(false);
					for (auto& bladeParticle : _bladeRParticle)
					{
						bladeParticle->GetComponent<Particle>()->SetActive(false);
						bladeParticle->GetComponent<Light>()->SetActive(false);
					}
					for (auto& bladeParticle : _bladeRParticleWave)
					{
						bladeParticle->GetComponent<Particle>()->SetActive(false);
					}

					auto index = pattern->GetSubObjectIndex(_blade[0]);

					pattern->_isColliderActive[index] = false;
				}
				else
				{
					_swordMoveDistance -= moveSpeed;

					auto bladePos1 = _blade[0]->GetComponent<Transform>()->GetPosition();

					auto bladeMove = DirectX::XMVectorAdd(DirectX::XMLoadFloat3(&bladePos1), DirectX::XMVectorScale(DirectX::XMLoadFloat3(&_swordDirection), moveSpeed));

					_blade[0]->GetComponent<Transform>()->SetPosition(bladeMove.m128_f32[0], bladePos1.y, bladeMove.m128_f32[2]);
					swordDirPos = _blade[0]->GetComponent<Light>()->ChangeDirectionForPattern(_blade[0]->GetComponent<Transform>()->GetPosition());
					mainLight->SetDirection(swordDirPos.x, -1.0f, swordDirPos.z);
				}
			}
			if (69 < nowFrame && nowFrame < 82)
			{
				swordDirPos = mainLight->ResetDirection(swordDirPos, TimeManager::GetInstance().GetDeltaTime(), 2.0f);
				mainLight->SetDirection(swordDirPos.x, -1.0f, swordDirPos.z);
			}

			if (bladeMesh2->GetActivated())
			{
				if (_swordMoveDistance2 < 0)
				{
					bladeMesh2->SetActive(false);
					for (auto& bladeParticle : _bladeLParticle)
					{
						bladeParticle->GetComponent<Particle>()->SetActive(false);
						bladeParticle->GetComponent<Light>()->SetActive(false);
					}
					for (auto& bladeParticle : _bladeLParticleWave)
					{
						bladeParticle->GetComponent<Particle>()->SetActive(false);
					}

					auto index = pattern->GetSubObjectIndex(_blade[1]);

					pattern->_isColliderActive[index] = false;
				}
				else
				{
					_swordMoveDistance2 -= moveSpeed;

					auto bladePos2 = _blade[1]->GetComponent<Transform>()->GetPosition();

					auto bladeMove = DirectX::XMVectorAdd(DirectX::XMLoadFloat3(&bladePos2), DirectX::XMVectorScale(DirectX::XMLoadFloat3(&_swordDirection2), moveSpeed));

					_blade[1]->GetComponent<Transform>()->SetPosition(bladeMove.m128_f32[0], bladePos2.y, bladeMove.m128_f32[2]);
					swordDirPos = _blade[1]->GetComponent<Light>()->ChangeDirectionForPattern(_blade[1]->GetComponent<Transform>()->GetPosition());
					mainLight->SetDirection(swordDirPos.x, -1.0f, swordDirPos.z);
				}
			}
			if (nowFrame > 94)
			{
				swordDirPos = mainLight->ResetDirection(swordDirPos, TimeManager::GetInstance().GetDeltaTime(), 2.0f);
				mainLight->SetDirection(swordDirPos.x, -1.0f, swordDirPos.z);
			}

			if (isAnimationPlaying == false)
			{
				return false;
			}

			return true;
		};

	pattern->SetLogic(attackLogic);

	_swordSwingTwiceHard = pattern;
}

void KunrealEngine::Kamen::CreateSwordSwingHorizontal()
{
	BossPattern* pattern = new BossPattern();

	pattern->SetPatternName("SwordSwingHorizontal");

	pattern->SetAnimName("SwordSwingHorizontal").SetMaxColliderCount(1).SetSpeed(15.0f).SetDamage(10.0f).SetRange(_info._attackRange + 40.0f).SetAttackState(BossPattern::eAttackState::ePush);
	pattern->SetColliderType(BossPattern::eColliderType::eSwordColliderWithMeshCollider).SetSkipChase(true);

	pattern->SetSubObject(_kamenSwordCollider);
	pattern->SetSubObject(_largeBlade);

	auto initLogic = [pattern, this]()
		{
			int random = ToolBox::GetRandomNum(3);

			switch (random)
			{
				case 0:
				{
					// 왼쪽
					_bossTransform->SetPosition(-70.0f, _centerPos.y, 0.0f);
					_bossTransform->SetRotation(0.0f, -90.0f, 0.0f);
					_largeBlade->GetComponent<Transform>()->SetRotation(0.0f, -90.0f, 0.0f);
					break;
				}
				case 1:
				{
					// 오른쪽
					_bossTransform->SetPosition(72.0f, _centerPos.y, 0.0f);
					_bossTransform->SetRotation(0.0f, 90.0f, 0.0f);
					_largeBlade->GetComponent<Transform>()->SetRotation(0.0f, 90.0f, 0.0f);
					break;
				}
				case 2:
				{
					// 위
					_bossTransform->SetPosition(0.0f, _centerPos.y, 85.0f);
					_bossTransform->SetRotation(0.0f, 0.0f, 0.0f);
					_largeBlade->GetComponent<Transform>()->SetRotation(0.0f, 0.0f, 0.0f);
					break;
				}
				case 3:
				{
					// 아래
					_bossTransform->SetPosition(0.0f, _centerPos.y, -98.0f);
					_bossTransform->SetRotation(0.0f, 180.0f, 0.0f);
					_largeBlade->GetComponent<Transform>()->SetRotation(0.0f, 180.0f, 0.0f);
					break;
				}
				default:
					break;
			}

			auto bossDirection = GetDirection();

			_swordDirection = { bossDirection.m128_f32[0], bossDirection.m128_f32[1], bossDirection.m128_f32[2] };

			auto bossPos = _bossTransform->GetPosition();

			auto bladeStartPos = DirectX::XMVectorAdd(DirectX::XMLoadFloat3(&bossPos), DirectX::XMVectorScale(bossDirection, 40.0f));

			_largeBlade->GetComponent<Transform>()->SetPosition(bladeStartPos.m128_f32[0], 15.0f, bladeStartPos.m128_f32[2]);

			DirectX::XMVECTOR targetPos = DirectX::XMVectorAdd(DirectX::XMLoadFloat3(&bossPos), DirectX::XMVectorScale(DirectX::XMLoadFloat3(&_swordDirection), 1000.0f));

			auto bladePos = _largeBlade->GetComponent<Transform>()->GetPosition();

			Navigation::GetInstance().SetSEpos(1, bladePos.x, 0.0f, bladePos.z,
				targetPos.m128_f32[0], 0.0f, targetPos.m128_f32[2]);

			auto targetPosition = Navigation::GetInstance().FindRaycastPath(1);

			_swordMoveDistance = ToolBox::GetDistance(bladePos, targetPosition) - 5.0f;
		};

	pattern->SetInitializeLogic(initLogic);

	auto attackLogic = [pattern, this]()
		{
			auto animator = _boss->GetComponent<Animator>();

			auto isAnimationPlaying = animator->Play(pattern->_animName, pattern->_speed, false);

			auto nowFrame = animator->GetCurrentFrame();

			if (10 < nowFrame && nowFrame < 17)
			{
				// 여기에 베지어 기 모으는 파티클 넣기
				pattern->_speed = 3.0f;
			}
			else
			{
				pattern->_speed = 15.0f;
			}

			if (16 < nowFrame && nowFrame < 38)
			{
				SetKamenSwordCollider();
			}

			if (35 < nowFrame)
			{
				auto largeBladeMesh = _largeBlade->GetComponent<MeshRenderer>();

				largeBladeMesh->SetActive(true);

				for (auto& largeBladeParticle : _largeBladeParticle)
				{
					largeBladeParticle->GetComponent<Particle>()->SetActive(true);
					largeBladeParticle->GetComponent<Light>()->SetActive(true);
				}

				for (auto& largeBladeParticleWave : _largeBladeParticleWave)
				{
					largeBladeParticleWave->GetComponent<Particle>()->SetActive(true);
				}

				auto bladeSpeed = 10.0f * TimeManager::GetInstance().GetDeltaTime();

				auto moveSpeed = pattern->_speed * bladeSpeed;

				if (_swordMoveDistance < 0)
				{
					largeBladeMesh->SetActive(false);
					for (auto& largeBladeParticle : _largeBladeParticle)
					{
						largeBladeParticle->GetComponent<Particle>()->SetActive(false);
						largeBladeParticle->GetComponent<Light>()->SetActive(false);
					}

					for (auto& largeBladeParticleWave : _largeBladeParticleWave)
					{
						largeBladeParticleWave->GetComponent<Particle>()->SetActive(false);
					}

					auto index = pattern->GetSubObjectIndex(_largeBlade);

					pattern->_isColliderActive[index] = false;
				}
				else
				{
					_swordMoveDistance -= moveSpeed;

					auto bladePos = _largeBlade->GetComponent<Transform>()->GetPosition();

					auto bladeMove = DirectX::XMVectorAdd(DirectX::XMLoadFloat3(&bladePos), DirectX::XMVectorScale(DirectX::XMLoadFloat3(&_swordDirection), moveSpeed));

					_largeBlade->GetComponent<Transform>()->SetPosition(bladeMove.m128_f32[0], bladePos.y, bladeMove.m128_f32[2]);
					auto index = pattern->GetSubObjectIndex(_largeBlade);

					swordDirPos = _largeBlade->GetComponent<Light>()->ChangeDirectionForPattern(_largeBlade->GetComponent<Transform>()->GetPosition());
					mainLight->SetDirection(swordDirPos.x, -1.0f, swordDirPos.z);

					pattern->_isColliderActive[index] = true;
				}
			}
			if (nowFrame > 35)
			{
				swordDirPos = mainLight->ResetDirection(swordDirPos, TimeManager::GetInstance().GetDeltaTime(), 2.0f);
				mainLight->SetDirection(swordDirPos.x, -1.0f, swordDirPos.z);
			}

			if (!isAnimationPlaying)
			{
				return false;
			}

			return true;
		};

	pattern->SetLogic(attackLogic);

	_swordSwingHorizontal = pattern;
}

void KunrealEngine::Kamen::CreateSwordRotationAttack()
{
	BossPattern* pattern = new BossPattern();

	pattern->SetPatternName("SwordRotationAttack");

	pattern->SetAnimName("Idle").SetMaxColliderCount(1).SetSpeed(15.0f).SetDamage(10.0f).SetRange(_info._attackRange + 40.0f).SetAttackState(BossPattern::eAttackState::ePush);
	pattern->SetColliderType(BossPattern::eColliderType::eBox).SetSkipChase(true);

	auto initLogic = [pattern, this]()
		{

		};

	pattern->SetInitializeLogic(initLogic);

	auto attackLogic = [pattern, this]()
		{
			return false;
		};

	pattern->SetLogic(attackLogic);

	_swordRotationAttack = pattern;
}

void KunrealEngine::Kamen::CreateSwordLookPlayer()
{
	BossPattern* pattern = new BossPattern();

	pattern->SetPatternName("SwordLookPlayer");

	pattern->SetAnimName("Idle").SetSpeed(40.0f);
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

			_swordTimer = 0.0f;

			auto swordTransform = _freeSword->GetComponent<Transform>()->GetPosition();
			_swordPath->GetComponent<Transform>()->SetPosition(swordTransform.x, _bossTransform->GetPosition().y, swordTransform.z);

			auto swordPathTransform = _swordPath->GetComponent<Transform>();
			swordPathTransform->SetRotation(swordPathTransform->GetRotation().x, _freeSword->GetComponent<Transform>()->GetRotation().y, swordPathTransform->GetRotation().z);

			_swordDirection = DirectX::XMFLOAT3(1.0f, 0.0f, 0.0f);
			_swordDirection = ToolBox::RotateVector(_swordDirection, swordPathTransform->GetRotation().y);

			DirectX::XMVECTOR swordNewPosition = DirectX::XMVectorAdd(DirectX::XMLoadFloat3(&swordTransform), DirectX::XMVectorScale(DirectX::XMLoadFloat3(&_swordDirection), 200.0f));

			_swordPath->GetComponent<Transform>()->SetPosition(swordNewPosition.m128_f32[0], _bossTransform->GetPosition().y, swordNewPosition.m128_f32[2]);
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

			_swordTimer += TimeManager::GetInstance().GetDeltaTime();

			// 현재 각도가 목표로 하는 각도보다 작을 경우
			if (_swordTimer < _warningMaxTimer)
			{
				auto errorRange = std::abs(goalAngle) - std::abs(rotation.y);
				errorRange = std::abs(errorRange);
				if (errorRange > 1.0f)
				{
					if (goalAngle < rotation.y)
					{
						// 회전 속도만큼 회전
						swordTransform->SetRotation(swordTransform->GetRotation().x, swordTransform->GetRotation().y - speed, swordTransform->GetRotation().z);

						auto swordTransform = _freeSword->GetComponent<Transform>()->GetPosition();

						auto swordPathTransform = _swordPath->GetComponent<Transform>();
						swordPathTransform->SetRotation(swordPathTransform->GetRotation().x, _freeSword->GetComponent<Transform>()->GetRotation().y, swordPathTransform->GetRotation().z);

						_swordDirection = DirectX::XMFLOAT3(1.0f, 0.0f, 0.0f);
						_swordDirection = ToolBox::RotateVector(_swordDirection, swordPathTransform->GetRotation().y);

						DirectX::XMVECTOR swordNewPosition = DirectX::XMVectorAdd(DirectX::XMLoadFloat3(&swordTransform), DirectX::XMVectorScale(DirectX::XMLoadFloat3(&_swordDirection), 200.0f));

						_swordPath->GetComponent<Transform>()->SetPosition(swordNewPosition.m128_f32[0], _bossTransform->GetPosition().y, swordNewPosition.m128_f32[2]);

					}
					else
					{
						// 회전 속도만큼 회전
						swordTransform->SetRotation(swordTransform->GetRotation().x, swordTransform->GetRotation().y + speed, swordTransform->GetRotation().z);

						auto swordTransform = _freeSword->GetComponent<Transform>()->GetPosition();

						auto swordPathTransform = _swordPath->GetComponent<Transform>();
						swordPathTransform->SetRotation(swordPathTransform->GetRotation().x, _freeSword->GetComponent<Transform>()->GetRotation().y, swordPathTransform->GetRotation().z);

						_swordDirection = DirectX::XMFLOAT3(1.0f, 0.0f, 0.0f);
						_swordDirection = ToolBox::RotateVector(_swordDirection, swordPathTransform->GetRotation().y);

						DirectX::XMVECTOR swordNewPosition = DirectX::XMVectorAdd(DirectX::XMLoadFloat3(&swordTransform), DirectX::XMVectorScale(DirectX::XMLoadFloat3(&_swordDirection), 200.0f));

						_swordPath->GetComponent<Transform>()->SetPosition(swordNewPosition.m128_f32[0], _bossTransform->GetPosition().y, swordNewPosition.m128_f32[2]);

					}
				}
			}

			if (_swordTimer > _warningMaxTimer + 1.0f)
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


void KunrealEngine::Kamen::SetKamenSwordCollider(int startFrame, int EndFrame, int nowFrame)
{
	if (startFrame <= nowFrame && nowFrame <= EndFrame)
	{
		auto originBoneTransform = _boss->GetComponent<MeshRenderer>()->GetBoneTransform("MiddleFinger1_R");

		DirectX::XMVECTOR srt;
		DirectX::XMVECTOR rot;
		DirectX::XMVECTOR scl;

		DirectX::XMMatrixDecompose(&srt, &rot, &scl, DirectX::XMLoadFloat4x4(&originBoneTransform));

		DirectX::XMFLOAT3 originDir = { 0.0f, 0.0f, 1.0f };

		originDir = ToolBox::RotateVector(originDir, rot);

		auto vec = DirectX::XMVectorScale(DirectX::XMLoadFloat3(&originDir), 30.0f);

		_kamenSwordCollider->GetComponent<BoxCollider>()->SetOffset(vec.m128_f32[0], vec.m128_f32[1], vec.m128_f32[2]);

		auto index = _nowPlayingPattern->GetSubObjectIndex(_kamenSwordCollider);

		_nowPlayingPattern->_isColliderActive[index] = true;
	}
	else
	{
		auto index = _nowPlayingPattern->GetSubObjectIndex(_kamenSwordCollider);

		_nowPlayingPattern->_isColliderActive[index] = false;
	}
}

void KunrealEngine::Kamen::HoldKamenSword()
{
	if (_kamenSword->GetActivated())
	{
		/// 일단 여기서 카멘 소드 파티클에 대한 업데이트 처리
		auto originBoneTransform = _boss->GetComponent<MeshRenderer>()->GetBoneTransform("MiddleFinger1_R");

		DirectX::XMVECTOR scl;
		DirectX::XMVECTOR rot;
		DirectX::XMVECTOR trs;

		DirectX::XMMatrixDecompose(&scl, &rot, &trs, DirectX::XMLoadFloat4x4(&originBoneTransform));

		DirectX::XMFLOAT3 originDir = { 0.0f, 0.0f, 1.0f };

		originDir = ToolBox::RotateVector(originDir, rot);

		auto vec = DirectX::XMVectorScale(DirectX::XMLoadFloat3(&originDir), 10.0f);
		auto vec2 = DirectX::XMVectorScale(DirectX::XMLoadFloat3(&originDir), 10.0f);
		auto vec3 = DirectX::XMVectorScale(DirectX::XMLoadFloat3(&originDir), 14.0f);
		auto vec4 = DirectX::XMVectorScale(DirectX::XMLoadFloat3(&originDir), 18.0f);
		auto vec5 = DirectX::XMVectorScale(DirectX::XMLoadFloat3(&originDir), 22.0f);
		auto vec6 = DirectX::XMVectorScale(DirectX::XMLoadFloat3(&originDir), 26.0f);
		auto vec7 = DirectX::XMVectorScale(DirectX::XMLoadFloat3(&originDir), 30.0f);
		auto vec8 = DirectX::XMVectorScale(DirectX::XMLoadFloat3(&originDir), 34.0f);
		auto vec9 = DirectX::XMVectorScale(DirectX::XMLoadFloat3(&originDir), 38.0f);
		auto vec10 = DirectX::XMVectorScale(DirectX::XMLoadFloat3(&originDir), 42.0f);

		DirectX::XMFLOAT3 particleOriginDir = { originDir.x * 720.0f, originDir.y * 720.0f, originDir.z * 720.0f };
		_kamenSwordParticle[0]->GetComponent<Particle>()->SetParticleDirection(particleOriginDir.x, particleOriginDir.y, particleOriginDir.z);
		_kamenSwordParticle[0]->GetComponent<Particle>()->SetOffSet(vec.m128_f32[0], vec.m128_f32[1], vec.m128_f32[2]);


		_kamenSwordAfterImageParticle[0]->GetComponent<Particle>()->SetOffSet(vec2.m128_f32[0], vec2.m128_f32[1], vec2.m128_f32[2]);
		_kamenSwordAfterImageParticle[1]->GetComponent<Particle>()->SetOffSet(vec3.m128_f32[0], vec3.m128_f32[1], vec3.m128_f32[2]);
		_kamenSwordAfterImageParticle[2]->GetComponent<Particle>()->SetOffSet(vec4.m128_f32[0], vec4.m128_f32[1], vec4.m128_f32[2]);
		_kamenSwordAfterImageParticle[3]->GetComponent<Particle>()->SetOffSet(vec5.m128_f32[0], vec5.m128_f32[1], vec5.m128_f32[2]);
		_kamenSwordAfterImageParticle[4]->GetComponent<Particle>()->SetOffSet(vec6.m128_f32[0], vec6.m128_f32[1], vec6.m128_f32[2]);
		_kamenSwordAfterImageParticle[5]->GetComponent<Particle>()->SetOffSet(vec7.m128_f32[0], vec7.m128_f32[1], vec7.m128_f32[2]);
		_kamenSwordAfterImageParticle[6]->GetComponent<Particle>()->SetOffSet(vec8.m128_f32[0], vec8.m128_f32[1], vec8.m128_f32[2]);
		_kamenSwordAfterImageParticle[7]->GetComponent<Particle>()->SetOffSet(vec9.m128_f32[0], vec9.m128_f32[1], vec9.m128_f32[2]);
		_kamenSwordAfterImageParticle[8]->GetComponent<Particle>()->SetOffSet(vec10.m128_f32[0], vec10.m128_f32[1], vec10.m128_f32[2]);

		// 카멘 순간 이동 하면 라이트 위치 망가짐
		auto SwordLightPos = DirectX::XMVectorAdd(trs, vec5);
		_kamenSwordLight->GetComponent<Transform>()->SetPosition(SwordLightPos.m128_f32[0], SwordLightPos.m128_f32[1], SwordLightPos.m128_f32[2]);
	}
}

void KunrealEngine::Kamen::SetKamenSwordCollider()
{
	auto originBoneTransform = _boss->GetComponent<MeshRenderer>()->GetBoneTransform("MiddleFinger1_R");

	DirectX::XMVECTOR srt;
	DirectX::XMVECTOR rot;
	DirectX::XMVECTOR scl;

	DirectX::XMMatrixDecompose(&srt, &rot, &scl, DirectX::XMLoadFloat4x4(&originBoneTransform));

	DirectX::XMFLOAT3 originDir = { 0.0f, 0.0f, 1.0f };

	originDir = ToolBox::RotateVector(originDir, rot);

	auto vec = DirectX::XMVectorScale(DirectX::XMLoadFloat3(&originDir), 30.0f);

	_kamenSwordCollider->GetComponent<BoxCollider>()->SetOffset(vec.m128_f32[0], vec.m128_f32[1], vec.m128_f32[2]);

	auto index = _nowPlayingPattern->GetSubObjectIndex(_kamenSwordCollider);

	_nowPlayingPattern->_isColliderActive[index] = true;
}

const DirectX::XMFLOAT3& KunrealEngine::Kamen::GetSwordDirection()
{
	return _swordDirection;
}

const DirectX::XMFLOAT3& KunrealEngine::Kamen::GetSwordPos()
{
	return _freeSword->GetComponent<Transform>()->GetPosition();
}

const DirectX::XMFLOAT3& KunrealEngine::Kamen::GetEgoPos()
{
	return _alterEgo->GetComponent<Transform>()->GetPosition();
}

const float KunrealEngine::Kamen::GetDonutSize()
{
	return _donutSize;
}

const DirectX::XMFLOAT3& KunrealEngine::Kamen::GetMultipleSwordDirVec(int index)
{
	return _multipleSwordDirectionVec[index];
}

void KunrealEngine::Kamen::BackStepCallPattern()
{
	BossPattern* backStepCallPattern = new BossPattern();

	backStepCallPattern->SetSkipMove(true);

	backStepCallPattern->SetNextPatternForcePlay(true);

	backStepCallPattern->SetMaxColliderCount(0).SetWithEgo(true);

	backStepCallPattern->SetPattern(_backStep);

	backStepCallPattern->SetPattern(_call2Attack);

	backStepCallPattern->SetAfterDelay(1.0f);

	backStepCallPattern->SetRange(backStepCallPattern->_patternList[2]->_range - backStepCallPattern->_patternList[1]->_range);

	_basicPattern[0].emplace_back(backStepCallPattern);
}

void KunrealEngine::Kamen::TeleportSpellPattern()
{
	BossPattern* teleportSpellPattern = new BossPattern();

	teleportSpellPattern->SetNextPatternForcePlay(true);
	teleportSpellPattern->SetSkipChase(true);
	teleportSpellPattern->SetRange(100.0f);
	teleportSpellPattern->SetWithEgo(true);

	teleportSpellPattern->SetAfterDelay(1.0f);

	teleportSpellPattern->SetPattern(_teleportRandomWithLook);

	teleportSpellPattern->SetPattern(_spellAttack);


	_basicPattern[0].emplace_back(teleportSpellPattern);
}


void KunrealEngine::Kamen::EmergenceAttackPattern()
{
	BossPattern* emergenceAttackPattern = new BossPattern();

	emergenceAttackPattern->SetNextPatternForcePlay(true);
	emergenceAttackPattern->SetSkipChase(true);
	emergenceAttackPattern->SetRange(30.0f);
	emergenceAttackPattern->SetWithEgo(true);

	emergenceAttackPattern->SetAfterDelay(1.0f);

	emergenceAttackPattern->SetPattern(_reverseEmergence);
	emergenceAttackPattern->SetPattern(_bossRandomInsideWarning);
	emergenceAttackPattern->SetPattern(_emergence);

	_basicPattern[0].emplace_back(emergenceAttackPattern);
}

void KunrealEngine::Kamen::SwordTurnClockPattern()
{
	BossPattern* swordTurnClockPattern = new BossPattern();

	swordTurnClockPattern->SetSkipChase(true);

	swordTurnClockPattern->SetMaxColliderCount(0);

	swordTurnClockPattern->SetAfterDelay(1.0f);

	swordTurnClockPattern->SetPattern(_swordEmmergence);

	swordTurnClockPattern->SetPattern(_swordChopAttack);

	swordTurnClockPattern->SetPattern(_swordTurnClockWise);

	swordTurnClockPattern->SetPattern(_swordHide);

	swordTurnClockPattern->SetPattern(_insideSafe);

	swordTurnClockPattern->SetRange(swordTurnClockPattern->_patternList[1]->_range);

	auto swordInitLogic = [swordTurnClockPattern, this]()
		{
			_swordCircleWarningSize = 30.0f;

			auto ranX = ToolBox::GetRandomFloat(-90.0f, 100.0f);
			auto ranZ = ToolBox::GetRandomFloat(-110.0f, 90.0f);

			_swordOriginPos = DirectX::XMFLOAT3{ ranX, _freeSword->GetComponent<Transform>()->GetPosition().y + 30.0f, ranZ };

			float x = _swordOriginPos.x - _swordCircleWarningSize * cos(0.0f);
			float z = _swordOriginPos.z - _swordCircleWarningSize * sin(0.0f);

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

	swordTurnAntiClockPattern->SetAfterDelay(1.0f);

	swordTurnAntiClockPattern->SetPattern(_swordEmmergence);

	swordTurnAntiClockPattern->SetPattern(_swordChopAttack);

	swordTurnAntiClockPattern->SetPattern(_swordTurnAntiClockWise);

	swordTurnAntiClockPattern->SetPattern(_swordHide);

	swordTurnAntiClockPattern->SetPattern(_outsideSafe);

	swordTurnAntiClockPattern->SetRange(swordTurnAntiClockPattern->_patternList[1]->_range);

	auto swordInitLogic = [swordTurnAntiClockPattern, this]()
		{
			_swordCircleWarningSize = 40.0f;

			auto ranX = ToolBox::GetRandomFloat(-90.0f, 100.0f);
			auto ranZ = ToolBox::GetRandomFloat(-110.0f, 90.0f);

			_swordOriginPos = DirectX::XMFLOAT3{ ranX, _freeSword->GetComponent<Transform>()->GetPosition().y + 30.0f, ranZ };

			float x = _swordOriginPos.x - _swordCircleWarningSize * cos(0.0f);
			float z = _swordOriginPos.z - _swordCircleWarningSize * sin(0.0f);

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

	swordLinearAttack->SetAfterDelay(1.0f);

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

	swordChopAttack->SetPattern(_donutAttack);

	swordChopAttack->SetPattern(_swordHide);

	auto swordChopAttackInitLogic = [swordChopAttack, this]()
		{
			_swordCircleWarningSize = 40.0f;

			auto ranX = ToolBox::GetRandomFloat(-90.0f, 100.0f);
			auto ranZ = ToolBox::GetRandomFloat(-110.0f, 90.0f);

			_swordOriginPos = DirectX::XMFLOAT3{ ranX, 0.0f, ranZ };

			float x = _swordOriginPos.x - _swordCircleWarningSize * cos(0.0f);
			float z = _swordOriginPos.z - _swordCircleWarningSize * sin(0.0f);

			_swordStartPos = DirectX::XMFLOAT3{ ranX , 30.0f + 70.0f, ranZ };

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

	_basicPattern[2].emplace_back(basicSwordAttack);
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

	_corePattern.emplace_back(coreEmmergence);
}

void KunrealEngine::Kamen::CoreSwordMutipleAttackPattern()
{
	BossPattern* coreSwordMultipleAttack = new BossPattern();

	coreSwordMultipleAttack->SetSkipChase(true);

	coreSwordMultipleAttack->SetNextPatternForcePlay(true);

	float triggerHp = _info.GetMaxHP() * 0.3f;

	coreSwordMultipleAttack->SetTriggerHp(triggerHp);

	coreSwordMultipleAttack->SetMaxColliderCount(0);

	coreSwordMultipleAttack->SetPattern(_battleCry);

	coreSwordMultipleAttack->SetPattern(_swordMultipleAttack);

	coreSwordMultipleAttack->SetPattern(_holdSword);

	_corePattern.emplace_back(coreSwordMultipleAttack);
}
