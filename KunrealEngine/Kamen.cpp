#include <functional>
#include "ToolBox.h"
#include "TimeManager.h"
#include "SceneManager.h"
#include "Kamen.h"

KunrealEngine::Kamen::Kamen()
	: Boss(), _leftHand(nullptr), _rightHand(nullptr), _call(nullptr), _lazer(nullptr),
	_callMoveDistance(0.0f), _isRotateFinish(false), _isCoreStart(false), _isRandomStart(false),
	_leftAttack(nullptr), _rightAttack(nullptr), _spellAttack(nullptr), _callAttack(nullptr), 
	_turn180(nullptr), _backStep(nullptr), _turnClockWise(nullptr), _turnAntiClockWise(nullptr)
{
	BossBasicInfo info;

	info.SetHp(100).SetPhase(3).SetArmor(10).SetDamage(100).SetMoveSpeed(20.0f).SetsStaggeredGauge(100.0f);
	info.SetAttackRange(5.0f);

	SetInfo(info);
}

KunrealEngine::Kamen::~Kamen()
{
	
}

void KunrealEngine::Kamen::Initialize()
{
	// 반드시 해야함
	Boss::Initialize(this->GetOwner());
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
}

void KunrealEngine::Kamen::SetBossTransform()
{
	_boss->GetComponent<Transform>()->SetPosition(5.0f, 0.0f, -20.0f);
}


void KunrealEngine::Kamen::SetBossCollider()
{
	_boss->AddComponent<BoxCollider>();
	_boss->GetComponent<BoxCollider>()->SetTransform(_boss, "Spine1_M");
	_boss->GetComponent<BoxCollider>()->SetBoxSize(6.0f, 18.0f, 10.0f);
	_boss->GetComponent<BoxCollider>()->SetOffset(0.0f, -1.5f, 0.0f);
}

void KunrealEngine::Kamen::CreatePattern()
{
	CreateSubObject();

	CreateLeftAttack();
	CreateRightAttack();
	CreateTurn180();
	CreateSpellAttack();
	CreateCallAttack();
	CreateBackStep();
	CreateTeleport();
	//CreateTurnClockWise();
	//CreateTurnAntiClockWise();

	// 실제 사용중인 패턴들 모아놓음
	GamePattern();
}


void KunrealEngine::Kamen::GamePattern()
{
	//BasicPattern();
	
	//LeftRightPattern();
	//RightLeftPattern();
	//BackStepCallPattern();
	TeleportSpellPattern();
}

void KunrealEngine::Kamen::CreateSubObject()
{
	// 왼손
	_leftHand = _boss->GetObjectScene()->CreateObject("LeftHand");
	_leftHand->AddComponent<BoxCollider>();
	_leftHand->GetComponent<BoxCollider>()->SetTransform(_boss, "MiddleFinger1_L");
	_leftHand->GetComponent<BoxCollider>()->SetBoxSize(2.0f, 3.0f, 2.0f);
	_leftHand->GetComponent<BoxCollider>()->SetActive(false);

	// 오른손
	_rightHand = _boss->GetObjectScene()->CreateObject("rightHand");
	_rightHand->AddComponent<BoxCollider>();
	_rightHand->GetComponent<BoxCollider>()->SetTransform(_boss, "MiddleFinger1_R");
	_rightHand->GetComponent<BoxCollider>()->SetBoxSize(2.0f, 3.0f, 2.0f);
	_rightHand->GetComponent<BoxCollider>()->SetActive(false);

	// call 투사체
	_call = _boss->GetObjectScene()->CreateObject("call");
	_call->AddComponent<BoxCollider>();
	_call->GetComponent<BoxCollider>()->SetBoxSize(10.0f, 10.0f, 10.0f);
	_call->GetComponent<BoxCollider>()->SetActive(false);
	_call->AddComponent<Particle>();
	_call->GetComponent<Particle>()->SetParticleEffect("Flame", "Resources/Textures/Particles/flare.dds", 1000);
	_call->GetComponent<Particle>()->SetParticleDuration(2.0f, 2.0f);
	_call->GetComponent<Particle>()->SetParticleVelocity(3.f, true);
	_call->GetComponent<Particle>()->SetParticleSize(10.f, 30.0f);
	_call->GetComponent<Particle>()->SetParticleDirection(0.0f, 7.0f, 0.0f);
	_call->GetComponent<Particle>()->AddParticleColor(1.2f, 7.5f, 0.6f);

	_call->GetComponent<Particle>()->SetActive(false);

	_lazer = _boss->GetObjectScene()->CreateObject("lazer");
	_lazer->AddComponent<Particle>();
	
	_lazer->GetComponent<Particle>()->SetParticleEffect("Laser", "Resources/Textures/Particles/RailGun_64.dds", 1000);
	_lazer->GetComponent<Particle>()->SetParticleDuration(1.7f, 2.0f);
	_lazer->GetComponent<Particle>()->SetParticleVelocity(84.f, false);
	_lazer->GetComponent<Particle>()->SetParticleDirection(-8.5f, 0.0f, 82.6f);
	_lazer->GetComponent<Transform>()->SetPosition(0.0f, 16.0f, -4.0f);
	_lazer->GetComponent<Particle>()->SetActive(false);
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
	_basicPattern.emplace_back(_callAttack);
}

void KunrealEngine::Kamen::CreateLeftAttack()
{
	BossPattern* pattern = new BossPattern();

	pattern->SetPatternName("Left_Attack_Once");

	pattern->SetAnimName("Left_Attack").SetDamage(100.0f).SetSpeed(20.0f).SetRange(_info._attackRange).SetAfterDelay(0.5);
	pattern->SetIsWarning(false).SetWarningName("");
	pattern->SetAttackState(BossPattern::eAttackState::ePush).SetMaxColliderCount(1);

	auto leftHandLogic = CreateBasicAttackLogic(pattern, _leftHand, 10);

	pattern->SetLogic(leftHandLogic);

	_leftAttack = pattern;
}

void KunrealEngine::Kamen::CreateRightAttack()
{
	BossPattern* pattern = new BossPattern();

	pattern->SetPatternName("Right_Attack_Once");

	pattern->SetAnimName("Right_Attack").SetDamage(100.0f).SetSpeed(20.0f).SetRange(_info._attackRange).SetAfterDelay(0.5f);
	pattern->SetIsWarning(false).SetWarningName("");
	pattern->SetAttackState(BossPattern::eAttackState::ePush).SetMaxColliderCount(1);

	pattern->_subObject.emplace_back(_rightHand);

	auto rightHandLogic = CreateBasicAttackLogic(pattern, _rightHand, 10);

	pattern->SetLogic(rightHandLogic);

	_rightAttack = pattern;
}


void KunrealEngine::Kamen::CreateTurn180()
{
	BossPattern* pattern = new BossPattern();

	pattern->SetPatternName("Turn180");

	pattern->SetAnimName("Idle").SetMaxColliderCount(0).SetSpeed(200.0f);

	auto turn180 = [pattern, this]()
		{
			auto animator = _boss->GetComponent<Animator>();

			// 회전 진행 안됐다면
			//if (_isRotateFinish == false)
			{
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
					_isRotateFinish = false;
					return false;
				}
			}
		};

	pattern->SetLogic(turn180);

	_turn180 = pattern;
}


void KunrealEngine::Kamen::CreateBackStep()
{
	BossPattern* pattern = new BossPattern();

	pattern->SetPatternName("BackStep");

	pattern->SetAnimName("Idle").SetSpeed(50.0f).SetRange(30.0f).SetMaxColliderCount(0);

	// 로직 함수 실행 가능하도록 넣어주기
	pattern->SetLogic(CreateBackStepLogic(pattern, pattern->_speed, pattern->_range));

	_backStep = pattern;
}


void KunrealEngine::Kamen::CreateTeleport()
{
	BossPattern* pattern = new BossPattern();

	pattern->SetPatternName("Teleport");

	pattern->SetAnimName("Idle").SetRange(20.0f).SetMaxColliderCount(0);

	auto teleport = [pattern, this]()
		{
			auto isTeleportFinish = Teleport(DirectX::XMFLOAT3(10.0f, 10.0f, 10.0f), true, 1.0f);

			if (isTeleportFinish)
			{
				return false;
			}

			return true;
		};

	// 로직 함수 실행 가능하도록 넣어주기
	pattern->SetLogic(teleport);

	_teleport = pattern;
}


void KunrealEngine::Kamen::CreateTurnClockWise()
{
	BossPattern* pattern = new BossPattern();

	pattern->SetPatternName("TurnClockWise");

	pattern->SetAnimName("Idle").SetMaxColliderCount(0).SetSpeed(50.0f);

	auto turn = [pattern, this]()
		{
			auto animator = _boss->GetComponent<Animator>();

			// 회전 진행 안됐다면
			if (_isRotateFinish == false)
			{
				animator->Play("Idle", pattern->_speed, false);

				// 회전 시킴
				auto rotateFinish = RotateClockWise(60, false);

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

	_basicPattern.emplace_back(_turnClockWise);
}


void KunrealEngine::Kamen::CreateTurnAntiClockWise()
{
	BossPattern* pattern = new BossPattern();

	pattern->SetPatternName("TurnAntiClockWise");

	pattern->SetAnimName("Idle").SetMaxColliderCount(0).SetSpeed(50.0f);

	auto turn = [pattern, this]()
		{
			auto animator = _boss->GetComponent<Animator>();

			// 회전 진행 안됐다면
			if (_isRotateFinish == false)
			{
				animator->Play("Idle", pattern->_speed, false);

				// 회전 시킴
				auto rotateFinish = RotateClockWise(60, true);

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

void KunrealEngine::Kamen::CreateSpellAttack()
{
	BossPattern* pattern = new BossPattern();

	pattern->SetPatternName("Spell");

	pattern->SetAnimName("Spell").SetDamage(100.0f).SetSpeed(20.0f).SetRange(_info._attackRange + 20.0f).SetAfterDelay(2.0f);
	pattern->SetIsWarning(true).SetWarningName("Spell");

	pattern->_subObject.emplace_back(_lazer);

	auto spellLogic = [pattern, this]()
	{
		auto animator = _boss->GetComponent<Animator>();
		auto isAnimationPlaying = animator->Play(pattern->_animName, pattern->_speed, false);

		// 일정 프레임이 넘어가면 데미지 체크용 콜라이더를 키기
		if (pattern->_colliderOnCount > 0)
		{
			if (animator->GetCurrentFrame() >= 30.0f)
			{
				if (animator->GetCurrentFrame() >= 55.0f)
				{
					_lazer->GetComponent<Particle>()->SetActive(false);
				}
				else
				{
					pattern->SetSpeed(10.0f);
					_lazer->GetComponent<Particle>()->SetActive(true);
					_lazer->GetComponent<Particle>()->SetParticlePos(0.0f, 0.0f, 0.0f);
					_lazer->GetComponent<Particle>()->SetParticleSize(30.f * ToolBox::GetRandomFloat(0.3f, 1.0f), 20.0f * ToolBox::GetRandomFloat(0.1f, 1.0f));
				}
			}
		}
		if (isAnimationPlaying == false)
		{
			pattern->SetSpeed(20.0f);
			_lazer->SetActive(false);

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
	pattern->SetIsWarning(true).SetWarningName("Call").SetMaxColliderCount(1).SetAttackState(BossPattern::eAttackState::ePush);
	pattern->SetRangeOffset(-20.0f);
	pattern->SetAttackState(BossPattern::eAttackState::eParalysis);

	pattern->_subObject.emplace_back(_call);

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
				_callMoveDistance += (pattern->_speed*1.5) * TimeManager::GetInstance().GetDeltaTime();
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
			_call->GetComponent<BoxCollider>()->SetActive(false);
			_call->GetComponent<Particle>()->SetActive(false);
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


void KunrealEngine::Kamen::EmergenceAttack()
{
	BossPattern* pattern = new BossPattern();

	pattern->SetPatternName("core1");

	pattern->SetAnimName("Emergence").SetDamage(100.0f).SetSpeed(15.0f).SetRange(_info._attackRange).SetAfterDelay(3.0f);
	pattern->SetIsWarning(true).SetWarningName("Call").SetRangeOffset(-20.0f);
	pattern->SetMaxColliderCount(1);
	pattern->SetAttackState(BossPattern::eAttackState::eParalysis);
	pattern->SetTriggerHp(100.0f);

	for (int i = 0; i < 9; i++)
	{
		std::string index = "fake" + std::to_string(i+1);
		auto boss = _boss->GetObjectScene()->CreateObject(index);
		_fakeBoss.emplace_back(boss);

		boss->AddComponent<MeshRenderer>();
		boss->GetComponent<MeshRenderer>()->SetMeshObject("Lich/Lich");

		auto texSize = _boss->GetComponent<MeshRenderer>()->GetTextures().size();
		for (int i = 0; i < texSize; i++)
		{
			boss->GetComponent<MeshRenderer>()->SetDiffuseTexture(i, "Lich/T_Lich_1_D.tga");
			boss->GetComponent<MeshRenderer>()->SetNormalTexture(i, "Lich/T_Lich_N.tga");
			boss->GetComponent<MeshRenderer>()->SetEmissiveTexture(i, "Lich/T_Lich_01_E.tga");
		}

		boss->AddComponent<BoxCollider>();
		boss->GetComponent<BoxCollider>()->SetTransform(boss, "Spine1_M");
		boss->GetComponent<BoxCollider>()->SetBoxSize(6.0f, 18.0f, 10.0f);
		boss->GetComponent<BoxCollider>()->SetOffset(0.0f, -1.5f, 0.0f);

		boss->SetActive(false);

		pattern->_subObject.emplace_back(boss);
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
				int targetIndex = ToolBox::GetRandomNum(_fakeBoss.size() - 1);
				_fakeBoss[targetIndex]->SetActive(false);

				_isRandomStart = true;
			}

			// 패턴 시작 위해 중앙으로 텔레포트
			if (!_isRotateFinish)
			{
				auto isTeleportFinish = Teleport(DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f), true, 3.0f);

				// 텔포 끝나면 패턴 시작
				if (!isTeleportFinish)
				{
					return true;
				}
				else
				{

					// 텔포 끝나면 다시 돌아오지 않도록
					_isRotateFinish = true;
				}
			}

			/// emergence 역애니메이션 한번 재생시켜줌 아직 미구현

			// 패턴 시작 전 초기화해줄것들
			if (! _isCoreStart)
			{
				for (const auto& index : _fakeBoss)
				{
					if (index->GetActivated() == false)
					{
						ForceMove(index->GetComponent<Transform>()->GetPosition());
						_bossTransform->SetRotation(_bossTransform->GetRotation().x, index->GetComponent<Transform>()->GetRotation().y, _bossTransform->GetRotation().z);
					}
					else
					{
						index->GetComponent<MeshRenderer>()->SetActive(true);
						index->GetComponent<BoxCollider>()->SetActive(true);
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

				// 다음 패턴을 실행시킴
				pattern->_playNextPattern = true;
				return false;
			}

			return true;
		};

	pattern->SetLogic(core1Logic, false);


	auto attackLogic = [pattern, this]()
		{
			auto animator = _boss->GetComponent<Animator>();

			auto isAnimationPlaying = animator->Play("Left_Attack", pattern->_speed, false);

			if (animator->GetCurrentFrame() >= 0)
			{
				if (pattern->_colliderOnCount > 0)
				{
					// 콜라이더 키기
					_leftHand->SetActive(true);
					_leftHand->GetComponent<BoxCollider>()->SetActive(true);
				}
			}

			if (pattern->_colliderOnCount == 0)
			{
				_isRotateFinish = false;
				return false;
			}

			if (isAnimationPlaying == false)
			{
				if (pattern->_colliderOnCount > 0)
				{
					int a = 5;
				}
			}

			return true;
		};

	pattern->SetLogic(attackLogic, false);

	pattern->_subObject.emplace_back(_leftHand);

	_corePattern.emplace_back(pattern);
}

void KunrealEngine::Kamen::BackStepCallPattern()
{
	BossPattern* backStepCallPattern = new BossPattern();

	backStepCallPattern->SetNextPatternForcePlay(true);

	backStepCallPattern->SetMaxColliderCount(0);

	backStepCallPattern->SetPattern(_backStep);

	backStepCallPattern->SetPattern(_callAttack);

	backStepCallPattern->SetRange(backStepCallPattern->_patternList[2]->_range - backStepCallPattern->_patternList[1]->_range);

	_basicPattern.emplace_back(backStepCallPattern);
}

void KunrealEngine::Kamen::TeleportSpellPattern()
{
	BossPattern* teleportSpellPattern = new BossPattern();

	teleportSpellPattern->SetNextPatternForcePlay(true);

	teleportSpellPattern->SetMaxColliderCount(0);

	teleportSpellPattern->SetPattern(_teleport);

	teleportSpellPattern->SetPattern(_spellAttack);

	teleportSpellPattern->SetRange(teleportSpellPattern->_patternList[1]->_range);

	_basicPattern.emplace_back(teleportSpellPattern);
}
