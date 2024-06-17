#include <functional>
#include "ToolBox.h"
#include "TimeManager.h"
#include "SceneManager.h"
#include "Ent.h"

#include <algorithm>
#include <random>

KunrealEngine::Ent::Ent()
	: Boss(), _leftHand(nullptr), _rightHand(nullptr), _leftRoot(nullptr), _rightRoot(nullptr), _bigRootShotStart(false),
	_callMoveDistance(0.0f), _isRotateFinish(false), _isCoreStart(false), _isRandomStart(false),
	_leftAttack(nullptr), _rightAttack(nullptr), _bigRootShot(nullptr), _isIdleHealing(false), _rootY(-50.0f), randomX(0.0f), randomZ(0.0f), isEyesLightStart(false), _successCountCoreFirst(0), _baseNumber{0,1,2,3}
	, _firstQuestion(true), _secendQuestion(false), _thirdQuestion(false), _isShuffle(false), _isPartternStart(true), _firstProblem(true), _answerCount(0)
{
	BossBasicInfo info;

	info.SetHp(100).SetPhase(3).SetArmor(10).SetDamage(100).SetMoveSpeed(30.0f).SetsStaggeredGauge(100.0f);
	info.SetAttackRange(5.0f);

	SetInfo(info);
}

KunrealEngine::Ent::~Ent()
{

}

void KunrealEngine::Ent::Initialize()
{
	// 반드시 해야한다고 함
	Boss::Initialize(this->GetOwner());	

	CorePatternObjectFirst();
}

void KunrealEngine::Ent::Release()
{

}

void KunrealEngine::Ent::FixedUpdate()
{

}

void KunrealEngine::Ent::Update()
{
	// 반드시 해야한다고 함
	Boss::Update();
}

void KunrealEngine::Ent::LateUpdate()
{

}

void KunrealEngine::Ent::OnTriggerEnter()
{

}

void KunrealEngine::Ent::OnTriggerStay()
{

}

void KunrealEngine::Ent::OnTriggerExit()
{

}

void KunrealEngine::Ent::SetActive(bool active)
{
	//this->_isActivated = active;
}

void KunrealEngine::Ent::SetMesh()
{
	_boss->AddComponent<MeshRenderer>();
	_boss->GetComponent<MeshRenderer>()->SetMeshObject("Ent_Generic/Ent_Generic");
}

void KunrealEngine::Ent::SetTexture()
{
	auto texSize = _boss->GetComponent<MeshRenderer>()->GetTextures().size();
	for (int i = 0; i < texSize; i++)
	{
		_boss->GetComponent<MeshRenderer>()->SetDiffuseTexture(i, "Ent_Generic/T_Ent_1_D.tga");
		_boss->GetComponent<MeshRenderer>()->SetNormalTexture(i, "Ent_Generic/T_Ent_N.tga");
		_boss->GetComponent<MeshRenderer>()->SetEmissiveTexture(i, "Ent_Generic/T_Ent_1_E.tga");
	}
}

void KunrealEngine::Ent::SetBossTransform()
{
	_boss->GetComponent<Transform>()->SetPosition(5.0f, 6.0f, -20.0f);
	_boss->GetComponent<Transform>()->SetScale(10.0f, 10.0f, 10.0f);
}

void KunrealEngine::Ent::SetBossCollider()
{
	_boss->AddComponent<BoxCollider>();
	//_boss->GetComponent<BoxCollider>()->SetTransform(_boss, "Spine1_M");
	_boss->GetComponent<BoxCollider>()->SetBoxSize(6.0f, 18.0f, 10.0f);
	_boss->GetComponent<BoxCollider>()->SetOffset(0.0f, -1.5f, 0.0f);
}

void KunrealEngine::Ent::CreatePattern()
{
	CreateSubObject();
	CreateRightAttack();
	CreateLeftRootShot();
	CreateRightRootShot();
	CreateRandomRootAttack();
	CreateEyeLight();
	CreateSwiping();
	CreateJumpAttack();
	//CreateCorePatternFirst();
}
    
void KunrealEngine::Ent::CreateSubObject()
{
	// 왼손
	_leftHand = _boss->GetObjectScene()->CreateObject("LeftHand");
	_leftHand->SetTag("BossSub");
	_leftHand->AddComponent<BoxCollider>();
	_leftHand->GetComponent<BoxCollider>()->SetTransform(_boss, "hand_l");
	_leftHand->GetComponent<BoxCollider>()->SetBoxSize(4.0f, 3.0f, 4.0f);
	_leftHand->GetComponent<BoxCollider>()->SetActive(false);

	_leftHand->AddComponent<Particle>();
	_leftHand->GetComponent<Particle>()->SetParticleEffect("Flame", "Resources/Textures/Particles/flare.dds", 1000);
	_leftHand->GetComponent<Particle>()->SetParticleDuration(1.0f, 0.2f);
	_leftHand->GetComponent<Particle>()->SetParticleVelocity(10.0f, true);
	_leftHand->GetComponent<Particle>()->SetParticleSize(15.f, 10.0f);
	_leftHand->GetComponent<Particle>()->AddParticleColor(15.0f, 0.6f, 0.0f);
	_leftHand->GetComponent<Particle>()->SetActive(false);

	
	// 오른손
	_rightHand = _boss->GetObjectScene()->CreateObject("rightHand");
	_rightHand->SetTag("BossSub");
	_rightHand->AddComponent<BoxCollider>();
	_rightHand->GetComponent<BoxCollider>()->SetTransform(_boss, "ik_hand_r");
	_rightHand->GetComponent<BoxCollider>()->SetBoxSize(4.0f, 3.0f, 4.0f);
	_rightHand->GetComponent<BoxCollider>()->SetActive(false);

	_leftRoot = _boss->GetObjectScene()->CreateObject("leftRoot");
	_leftRoot->SetTag("BossSub");
	_leftRoot->AddComponent<MeshRenderer>();
	_leftRoot->GetComponent<MeshRenderer>()->SetMeshObject("SM_root_large_02/SM_root_large_02"); // w줄기 이름
	_leftRoot->GetComponent<MeshRenderer>()->SetActive(true);
	_leftRoot->GetComponent<Transform>()->SetRotation(86.0f, -200.0f, 250.0f);
	_leftRoot->GetComponent<Transform>()->SetScale(0.1f, 0.1f, 0.1f);
	_leftRoot->AddComponent<BoxCollider>();
	_leftRoot->GetComponent<BoxCollider>()->SetBoxSize(5.0f, 5.0f, 5.0f);
	_leftRoot->GetComponent<BoxCollider>()->SetOffset(-5.0f, 10.0f, 0.0f);
	_leftRoot->SetActive(false);

	_rightRoot = _boss->GetObjectScene()->CreateObject("rightRoot");
	_rightRoot->SetTag("BossSub");
	_rightRoot->AddComponent<MeshRenderer>();
	_rightRoot->GetComponent<MeshRenderer>()->SetMeshObject("SM_root_large_02/SM_root_large_02"); // w줄기 이름
	_rightRoot->GetComponent<MeshRenderer>()->SetActive(true);
	_rightRoot->GetComponent<Transform>()->SetRotation(86.0f, -200.0f, 250.0f);
	_rightRoot->GetComponent<Transform>()->SetScale(0.1f, 0.1f, 0.1f);
	_rightRoot->AddComponent<BoxCollider>();
	_rightRoot->GetComponent<BoxCollider>()->SetBoxSize(5.0f, 5.0f, 5.0f);
	_rightRoot->GetComponent<BoxCollider>()->SetOffset(-5.0f, 10.0f, 0.0f);
	_rightRoot->SetActive(false);

	
	int smallRootCount = 10;

	for (int i = 0; i < smallRootCount;  ++i)
	{
		GameObject* smallRoot = nullptr;

		smallRoot = _boss->GetObjectScene()->CreateObject("smallRoot");
		smallRoot->SetTag("BossSub");
		smallRoot->AddComponent<MeshRenderer>();
		smallRoot->GetComponent<MeshRenderer>()->SetMeshObject("SM_root_large_02/SM_root_large_02");
		smallRoot->GetComponent<MeshRenderer>()->SetActive(true);
		smallRoot->GetComponent<Transform>()->SetRotation(90.0f, 0.0f,0.0f);
		smallRoot->GetComponent<Transform>()->SetScale(0.05f, 0.05f, 0.05f);
		smallRoot->AddComponent<BoxCollider>();
		smallRoot->GetComponent<BoxCollider>()->SetBoxSize(3.0f, 3.0f, 12.0f);
		smallRoot->GetComponent<BoxCollider>()->SetOffset(-3.0f, 8.0f, 0.0f);
		smallRoot->SetActive(false);
		_smallRootVector.push_back(smallRoot);
	}

	_colJumpAttack = _boss->GetObjectScene()->CreateObject("Jump");
	_colJumpAttack->SetTag("BossSub");
	_colJumpAttack->AddComponent<BoxCollider>();
	_colJumpAttack->GetComponent<BoxCollider>()->SetOffset(0.0f, 3.0f, 0.0f);
	_colJumpAttack->GetComponent<BoxCollider>()->SetBoxSize(10.0f, 5.0f, 10.0f);
	_colJumpAttack->SetActive(false);


	// 안광
	// 페이드타임 3
	// 라이프타임 12
	// 사이즈 2

	_eyeLight = _boss->GetObjectScene()->CreateObject("EyeLight");
	_eyeLight->SetTag("BossSub");
	_eyeLight->AddComponent<BoxCollider>();
	_eyeLight->GetComponent<BoxCollider>()->SetBoxSize(5.0f, 5.0f, 5.0f);
	_eyeLight->GetComponent<BoxCollider>()->SetOffset(_playerTransform->GetPosition().x, _playerTransform->GetPosition().y - 15.0f, _playerTransform->GetPosition().z + 8.0f);  //(0.0f, -17.0f, 8.0f);

	_eyeLight->GetComponent<BoxCollider>()->SetActive(false);

	_eyeLight->AddComponent<Particle>();
	_eyeLight->GetComponent<Particle>()->SetParticleEffect("Laser", "Resources/Textures/Particles/RailGun_64.dds", 1000);
	_eyeLight->GetComponent<Particle>()->SetParticleDuration(3.0f, 12.0f);
	_eyeLight->GetComponent<Particle>()->SetParticleVelocity(60.0f, false);
	_eyeLight->GetComponent<Particle>()->SetTransform(_boss, "jaw");
	_eyeLight->GetComponent<Particle>()->SetParticleRotation(210.0f, _bossTransform->GetRotation().y, 0.0f);  // 여기서 동적으로
	_eyeLight->GetComponent<Particle>()->SetParticleSize(1.5f, 1.5f);
	_eyeLight->GetComponent<Particle>()->AddParticleColor(7.0f, 0.0f, 0.0f);
	_eyeLight->GetComponent<Particle>()->SetOffSet(0.0f, 1.3f, 0.0f);
	_eyeLight->GetComponent<Particle>()->SetActive(false);
	//_eyeLight->SetActive(false);

	


}

void KunrealEngine::Ent::CreateLeftAttack()
{
	BossPattern* pattern = new BossPattern();

	pattern->SetPatternName("Left_Attack_Once");
	pattern->SetAnimName("Anim_Hand_Attack_L");
	// 애니메이션 추가해야함
	pattern->SetDamage(100.0f);
	pattern->SetSpeed(60.0f);         
	pattern->SetRange(_info._attackRange);
	pattern->SetAfterDelay(0.5f);

	pattern->SetAttackState(BossPattern::eAttackState::ePush).SetMaxColliderCount(1);

	pattern->_subObject.emplace_back(_leftHand);

	auto leftHandLogic = CreateBasicAttackLogic(pattern, _leftHand, 10);

	pattern->SetLogic(leftHandLogic);

	//_leftAttack = pattern;
	_basicPattern.emplace_back(pattern);
}

void KunrealEngine::Ent::CreateRightAttack()
{
	BossPattern* pattern = new BossPattern();

	pattern->SetPatternName("Right_Attack_Once");
	pattern->SetAnimName("Anim_Hand_Attack_R");
	pattern->SetDamage(100.0f);
	pattern->SetSpeed(60.0f);
	pattern->SetRange(_info._attackRange);
	pattern->SetAfterDelay(0.5f);

	pattern->SetAttackState(BossPattern::eAttackState::ePush).SetMaxColliderCount(1);

	pattern->_subObject.emplace_back(_rightHand);

	auto rightHandLogic = CreateBasicAttackLogic(pattern, _rightHand, 10);

	pattern->SetLogic(rightHandLogic);

	//_rightAttack = pattern;
	_basicPattern.emplace_back(pattern);
}

void KunrealEngine::Ent::CreateLeftRootShot()
{
	BossPattern* pattern = new BossPattern();

	pattern->SetPatternName("Left_Root_Shot");
	pattern->SetAnimName("Anim_Leg_Attack_L");
	pattern->SetDamage(100.0f);
	pattern->SetSpeed(60.0f);
	pattern->SetRange(40.0f);
	pattern->SetAfterDelay(0.5f);

	pattern->SetAttackState(BossPattern::eAttackState::ePush);
	pattern->SetMaxColliderCount(1);

	pattern->_subObject.emplace_back(_leftRoot);

	auto bigRootShotLogic = [pattern, this]()
		{
			
			auto animator = _boss->GetComponent<Animator>();
			auto isAnimationPlaying = animator->Play(pattern->_animName, pattern->_speed, false);

			// 현재 보스의 포지션
			auto nowPos = _boss->GetComponent<Transform>()->GetPosition();

			if (animator->GetCurrentFrame() >= 55)
			{				
				if (pattern->_colliderOnCount > 0)
				{
					// 콜라이더 키기
					_leftRoot->GetComponent<BoxCollider>()->SetActive(true);
					// 메쉬 키기
					
				}

				// 보스가 바라보는 방향 가져옴
				auto direction = GetDirection();
			
				auto nowPosVec = DirectX::XMLoadFloat3(&nowPos);

				auto callNowPos = _leftRoot->GetComponent<Transform>()->GetPosition();
				auto distance = ToolBox::GetDistance(nowPos, callNowPos);

				if (distance < pattern->_range)
				{
					_callMoveDistance += (pattern->_speed * 3) * TimeManager::GetInstance().GetDeltaTime();
					_leftRoot->GetComponent<MeshRenderer>()->SetActive(true);
				}
				else
				{
					animator->Stop();
					_leftRoot->GetComponent<Transform>()->SetPosition(nowPos);      
					isAnimationPlaying = false;
				}
				// 현재 보스의 포지션에서 바라보는 백터 방향으로 더해줌
				DirectX::XMVECTOR newPosition = DirectX::XMVectorAdd(nowPosVec, DirectX::XMVectorScale(direction, _callMoveDistance));

				_leftRoot->GetComponent<Transform>()->SetPosition(newPosition.m128_f32[0], newPosition.m128_f32[1], newPosition.m128_f32[2]);
			}

			if (isAnimationPlaying == false)
			{
			//	pattern->SetSpeed(20.0f);
				_leftRoot->GetComponent<Transform>()->SetPosition(nowPos);
				_callMoveDistance = 0;
				return false;
			}			
			return true;
		};

	pattern->SetLogic(bigRootShotLogic);

	_basicPattern.emplace_back(pattern);

}

void KunrealEngine::Ent::CreateRightRootShot()
{
	BossPattern* pattern = new BossPattern();

	pattern->SetPatternName("Right_Root_Shot");
	pattern->SetAnimName("Anim_Leg_Attack_R");
	pattern->SetDamage(100.0f);
	pattern->SetSpeed(60.0f);
	pattern->SetRange(40.0f);
	pattern->SetAfterDelay(0.5f);

	pattern->SetAttackState(BossPattern::eAttackState::ePush);
	pattern->SetMaxColliderCount(1);

	pattern->_subObject.emplace_back(_rightRoot);

	auto bigRootShotLogic = [pattern, this]()
		{

			auto animator = _boss->GetComponent<Animator>();
			auto isAnimationPlaying = animator->Play(pattern->_animName, pattern->_speed, false);

			// 현재 보스의 포지션
			auto nowPos = _boss->GetComponent<Transform>()->GetPosition();

			if (animator->GetCurrentFrame() >= 40)
			{
				if (pattern->_colliderOnCount > 0)
				{
					// 콜라이더 키기
					_rightRoot->GetComponent<BoxCollider>()->SetActive(true);
					// 메쉬 키기
					_rightRoot->GetComponent<MeshRenderer>()->SetActive(true);
				}

				// 보스가 바라보는 방향 가져옴
				auto direction = GetDirection();

				auto nowPosVec = DirectX::XMLoadFloat3(&nowPos);

				auto callNowPos = _rightRoot->GetComponent<Transform>()->GetPosition();
				auto distance = ToolBox::GetDistance(nowPos, callNowPos);

				if (distance < pattern->_range)
				{
					_callMoveDistance += (pattern->_speed * 3) * TimeManager::GetInstance().GetDeltaTime();
					
				}
				else
				{
					animator->Stop();
					_rightRoot->GetComponent<Transform>()->SetPosition(nowPos);
					isAnimationPlaying = false;
				}
				// 현재 보스의 포지션에서 바라보는 백터 방향으로 더해줌
				DirectX::XMVECTOR newPosition = DirectX::XMVectorAdd(nowPosVec, DirectX::XMVectorScale(direction, _callMoveDistance));

				_rightRoot->GetComponent<Transform>()->SetPosition(newPosition.m128_f32[0] + 5, newPosition.m128_f32[1], newPosition.m128_f32[2] + 5);
			}

			if (isAnimationPlaying == false)
			{
				//pattern->SetSpeed(20.0f);
				_rightRoot->GetComponent<Transform>()->SetPosition(nowPos);
				_callMoveDistance = 0;
				return false;
			}
			return true;
		};

	pattern->SetLogic(bigRootShotLogic);

	_basicPattern.emplace_back(pattern);
}

void KunrealEngine::Ent::CreateRandomRootAttack()
{
	BossPattern* pattern = new BossPattern();

	pattern->SetPatternName("Small_Root_Shot");
	pattern->SetAnimName("Anim_Root_Shot");
	pattern->SetDamage(100.0f);
	pattern->SetSpeed(40.0f);	
	pattern->SetRange(80.0f);
	//pattern->SetAfterDelay(0.5f);

	pattern->SetAttackState(BossPattern::eAttackState::ePush);
	pattern->SetMaxColliderCount(1);

	for (auto smallRoot : _smallRootVector)
	{
		pattern->_subObject.emplace_back(smallRoot);	
	}

	auto SmallRootShotLogic = [pattern, this]()
	{
			auto animator = _boss->GetComponent<Animator>();
			auto isAnimationPlaying = animator->Play(pattern->_animName, pattern->_speed, false);

			// 현재 보스의 포지션
			auto nowPos = _boss->GetComponent<Transform>()->GetPosition();
			auto nowPlayerPos = _player->GetComponent<Transform>()->GetPosition();

			float smallRootSummonRange = 20;


			float currentTime = TimeManager::GetInstance().GetDeltaTime();
			static float checkTime = 0.0f;



				
			for (auto smallRoot : _smallRootVector)
			{
				if (animator->GetCurrentFrame() >= 15 && animator->GetCurrentFrame() < 16)
				{
					randomX = GetRandomRange(nowPlayerPos.x, smallRootSummonRange);
					randomZ = GetRandomRange(nowPlayerPos.z, smallRootSummonRange);
					smallRoot->GetComponent<Transform>()->SetPosition(randomX, _rootY, randomZ);
					
				}

				if (animator->GetCurrentFrame() >= 40)
				{
					smallRoot->GetComponent<MeshRenderer>()->SetActive(true);

					if (pattern->_colliderOnCount > 0)
					{
						// 콜라이더 키기
						smallRoot->GetComponent<BoxCollider>()->SetActive(true);
					}

					smallRoot->GetComponent<MeshRenderer>()->SetActive(true);

					smallRoot->SetActive(true);
					
					// 배성근 리건호의 디테일 살리기
					if (_rootY < 0.0f)
					{
						_rootY += 0.5f;
						int a = 10;
					}
					smallRoot->GetComponent<Transform>()->SetPosition(smallRoot->GetComponent<Transform>()->GetPosition().x, _rootY, smallRoot->GetComponent<Transform>()->GetPosition().z);
				}
			}

			if (isAnimationPlaying == false)
			{
				//pattern->SetSpeed(20.0f);
				for (auto smallRoot : _smallRootVector)
				{
					smallRoot->GetComponent<Transform>()->SetPosition(nowPos);
				}
				_rootY = -20.0f;
				return false;
			}


	};

	pattern->SetLogic(SmallRootShotLogic);

	_basicPattern.emplace_back(pattern);
}

void KunrealEngine::Ent::CreateJumpAttack()
{
	BossPattern* pattern = new BossPattern();

	pattern->SetPatternName("Jump_Attack");
	pattern->SetAnimName("Anim_Jump_Start");
	pattern->SetDamage(100.0f);
	pattern->SetSpeed(60.0f);
	pattern->SetRange(100.0f);
	//pattern->SetAfterDelay(0.5f);
	pattern->SetAttackState(BossPattern::eAttackState::ePush);
	pattern->SetMaxColliderCount(1);

	std::function logic = [this]()
		{
			Startcoroutine(JumpAttackCo);
			return true;
			return _jumpAttack_end;
		};

	pattern->SetLogic(logic);
	_basicPattern.emplace_back(pattern);
}

void KunrealEngine::Ent::CreateEyeLight()
{
	BossPattern* pattern = new BossPattern();

	pattern->SetPatternName("Eye_Light");
	pattern->SetAnimName("Mutant_Roaring");
	pattern->SetDamage(100.0f);
	pattern->SetSpeed(40.0f);
	pattern->SetRange(30.0f);
	//pattern->SetAfterDelay(0.5f);
	pattern->SetAttackState(BossPattern::eAttackState::ePush);
	pattern->SetMaxColliderCount(1);

	pattern->_subObject.emplace_back(_eyeLight);

	auto eyeLightLogic = [pattern, this]()
		{
			auto animator = _boss->GetComponent<Animator>();
			auto isAnimationPlaying = animator->Play(pattern->_animName, pattern->_speed, false);

			if (pattern->_colliderOnCount > 0)
			{
				if (isEyesLightStart == false)
				{
					auto playerPos = _playerTransform->GetPosition();
					auto bossPos = _bossTransform->GetPosition();
					auto direction = DirectX::XMVectorSubtract(DirectX::XMLoadFloat3(&playerPos), DirectX::XMLoadFloat3(&bossPos));
					direction = DirectX::XMVector3Normalize(direction);
					auto scaleDir = DirectX::XMVectorScale(direction, 15.0f);
					_eyeLight->GetComponent<Transform>()->SetPosition(_bossTransform->GetPosition().x + scaleDir.m128_f32[0], 16.0f, _bossTransform->GetPosition().z + scaleDir.m128_f32[2]);
					
					_eyeLight->GetComponent<BoxCollider>()->SetOffset(scaleDir.m128_f32[0], -15.0f, scaleDir.m128_f32[2]);
					_eyeLight->GetComponent<Particle>()->SetParticleRotation(135.0f, _bossTransform->GetRotation().y, 0.0f);

					isEyesLightStart = true;				
				}

				if (animator->GetCurrentFrame() >= 35)
				{
					if (_eyeLight->GetComponent<Particle>() != nullptr)
					{
						_eyeLight->GetComponent<Particle>()->SetActive(true);
					}

					if (_eyeLight->GetComponent<BoxCollider>() != nullptr)
					{
						_eyeLight->GetComponent<BoxCollider>()->SetActive(true);
					}
				}

			}

			//auto nowRot = _boss->GetComponent<Transform>()->GetRotation();
			//_eyeLight->GetComponent<Particle>()->SetParticleRotation(nowRot.x, nowRot.y, nowRot.z);

			if (pattern->_colliderOnCount == 0 && animator->GetCurrentFrame() >= 50 && animator->GetCurrentFrame() <= 51)
			{
				++pattern->_colliderOnCount;
				//pattern->SetNextPatternForcePlay(true);
			}

			if (isAnimationPlaying == false)
			{
				isEyesLightStart = false;
				return false;
			}			
		};

	pattern->SetLogic(eyeLightLogic);
	_basicPattern.emplace_back(pattern);
}

void KunrealEngine::Ent::CreateSwiping()
{
	BossPattern* pattern = new BossPattern();

	pattern->SetPatternName("Swiping");
	pattern->SetAnimName("Mutant_Swiping");
	pattern->SetDamage(100.0f);
	pattern->SetSpeed(40.0f);
	pattern->SetRange(20.f);
	//pattern->SetAfterDelay(0.5f);
	pattern->SetAttackState(BossPattern::eAttackState::ePush);
	pattern->SetMaxColliderCount(1);

	pattern->_subObject.emplace_back(_leftHand);

	_leftHand->GetComponent<BoxCollider>()->SetBoxSize(7.0f, 8.0f, 7.0f);

	auto swipingLogic = [pattern, this]
		{
			auto animator = _boss->GetComponent<Animator>();
			auto isAnimationPlaying = animator->Play(pattern->_animName, pattern->_speed, false);



			if (animator->GetCurrentFrame() >= 25)
			{
				_leftHand->GetComponent<BoxCollider>()->SetActive(true);

				_leftHand->GetComponent<Particle>()->SetActive(true);
			}

			if (isAnimationPlaying == false)
			{
				//_leftHand->GetComponent<BoxCollider>()->SetActive(false);
				//_leftHand->GetComponent<Particle>()->SetActive(false);
				return false;
			}
		};

	pattern->SetLogic(swipingLogic);
	_basicPattern.emplace_back(pattern);

}

void KunrealEngine::Ent::CreateLeftFireShot()
{

}    

void KunrealEngine::Ent::CreateRightFireShot()
{

}

void KunrealEngine::Ent::CreateRandomFireAttack()
{

}

void KunrealEngine::Ent::CreateCorePatternFirst()
{
	BossPattern* pattern = new BossPattern();

	pattern->SetPatternName("Core_Pattern_First"); // 세부조정은 이후에
	pattern->SetAnimName("Anim_Healing");
	pattern->SetDamage(100.0f);
	pattern->SetSpeed(10.0f);
	pattern->SetRange(100.0f);
	//pattern->SetAfterDelay(0.5f);

	pattern->SetAttackState(BossPattern::eAttackState::ePush);
	pattern->SetMaxColliderCount(1);



	auto CorePatternFirstLogic = [pattern, this]()
	{
			auto isPatternEnd = false;
			int patternAnimCount = 5;
			auto animator = _boss->GetComponent<Animator>();
			// 현재 보스의 포지션
			auto nowPos = _boss->GetComponent<Transform>()->GetPosition();
			auto nowPlayerPos = _player->GetComponent<Transform>()->GetPosition();

			/*if (_isIdleHealing == false)
			{
				while (true)
				{
					if (!(animator->Play("Anim_Idle_To_Healing", pattern->_speed, false)))
					{
						_isIdleHealing = true;
						animator->Stop();
						break;
					}
				}
			}*/

			auto isAnimationPlaying = animator->Play(pattern->_animName, pattern->_speed, true); // 모션 시작
			if (_isPartternStart == true)
			{
				for (auto treeObject : _treeObject)
				{
					treeObject->SetActive(true);
					treeObject->GetComponent<MeshRenderer>()->SetActive(true);
					// 포지션은 종화형쪽에 비슷한게 있었다
				}
				int index = 0;
				for (int j = 0; j < 2; j++)
				{
					_treeObject[index]->GetComponent<Transform>()->SetPosition(40.0f - (45.0 * j * 2.0f), 0.0f, -30.0f);
					index++;
					_treeObject[index]->GetComponent<Transform>()->SetPosition(-5.0f, 0.0f, (45.0f - (45.0 * j * 2.0f)) - 30.0f);
					index++;

				}
				_isPartternStart = false;
			}


			if (_firstQuestion == true) // 첫번째 질문         초기화 해주어야 하는것 셔플bool, 
			{
				if (_isShuffle == false)
				{
					unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
					std::default_random_engine engine(seed);

					int numberOfRapetitions = 4;

					std::shuffle(_baseNumber.begin(), _baseNumber.end(), engine);

					_firstPattarn.insert(_firstPattarn.end(), _baseNumber.begin(), _baseNumber.end()); // 숫자 추가

					_isShuffle = true;

					_problemAnswer.emplace_back(_treeObject[_firstPattarn[0]]);
					_problemAnswer.emplace_back(_treeObject[_firstPattarn[1]]);
					_problemAnswer.emplace_back(_treeObject[_firstPattarn[2]]);
					_problemAnswer.emplace_back(_treeObject[_firstPattarn[3]]); // 0,1,2,3

				}

				if (_firstProblem)  // 첫번째 문제 제시, 오답시 다시 여기부터 시작하도록 추가 구현 필요
				{
					if (animator->GetCurrentFrame() >= patternAnimCount && animator->GetCurrentFrame() <= patternAnimCount + 3) // 첫번째
					{
						// 첫번째차례의 나무
						_treeObject[_firstPattarn[0]]->GetComponent<Particle>()->SetActive(true);
					}

					if (animator->GetCurrentFrame() >= 2 * patternAnimCount && animator->GetCurrentFrame() <= 2 * patternAnimCount + 3)
					{
						_treeObject[_firstPattarn[0]]->GetComponent<Particle>()->SetActive(false);
					}

					if (animator->GetCurrentFrame() >= 3 * patternAnimCount && animator->GetCurrentFrame() <= 3 * patternAnimCount + 3)
					{
						// 두번째차례의 나무
						_treeObject[_firstPattarn[1]]->GetComponent<Particle>()->SetActive(true);
					}

					if (animator->GetCurrentFrame() >= 4 * patternAnimCount && animator->GetCurrentFrame() <= 4 * patternAnimCount + 3)
					{
						_treeObject[_firstPattarn[1]]->GetComponent<Particle>()->SetActive(false);
					}

					if (animator->GetCurrentFrame() >= 5 * patternAnimCount && animator->GetCurrentFrame() <= 5 * patternAnimCount + 3)
					{
						// 세번째차례의 나무
						_treeObject[_firstPattarn[2]]->GetComponent<Particle>()->SetActive(true);
					}

					if (animator->GetCurrentFrame() >= 6 * patternAnimCount && animator->GetCurrentFrame() <= 6 * patternAnimCount + 3)
					{
						_treeObject[_firstPattarn[2]]->GetComponent<Particle>()->SetActive(false);
					}

					if (animator->GetCurrentFrame() >= 7 * patternAnimCount && animator->GetCurrentFrame() <= 7 * patternAnimCount + 3)
					{
						// 네번째차례의 나무
						_treeObject[_firstPattarn[3]]->GetComponent<Particle>()->SetActive(true);
					}

					if (animator->GetCurrentFrame() >= 8 * patternAnimCount && animator->GetCurrentFrame() <= 8 * patternAnimCount + 1)
					{
						_treeObject[_firstPattarn[3]]->GetComponent<Particle>()->SetActive(false);
						_firstProblem = false;
					}
				}

			}
			else if (_secendQuestion == true) // 두번째 질문
			{

			}
			else if (_thirdQuestion == true) // 세번째 질문
			{

			}
			
			// 답변

			//for (auto treeObject : _treeObject)
			//{
			//	if (treeObject->GetComponent<BoxCollider>()->IsCollided())
			//	{
			//		if (treeObject->GetComponent<BoxCollider>()->GetTargetObject() == _player && )  // 현재 보스가 피격 당하는 구현이 없음, 나중에 수정
			//		{
			//			_playerAnswer.emplace_back(treeObject);
			//			++_answerCount;
			//		}
			//	}
			//}









				

				







			if (_successCountCoreFirst == 3)
			{
				return false;
			}
	};

	pattern->SetLogic(CorePatternFirstLogic);
	_basicPattern.emplace_back(pattern);
}

void KunrealEngine::Ent::CorePatternObjectFirst()
{
	int treeCount = 4;

	for (int i = 0; i < treeCount; ++i)
	{
		GameObject* treeObject = nullptr;
		treeObject = _boss->GetObjectScene()->CreateObject("treeObject");
		treeObject->AddComponent<MeshRenderer>();
		treeObject->GetComponent<MeshRenderer>()->SetMeshObject("SM_tree_large_ancient_01/SM_tree_large_ancient_01");
		treeObject->GetComponent<MeshRenderer>()->SetActive(true);
		treeObject->GetComponent<Transform>()->SetRotation(0.0f, 0.0f, 0.0f);
		treeObject->GetComponent<Transform>()->SetScale(0.1f, 0.1f, 0.1f);
		treeObject->AddComponent<BoxCollider>();
		treeObject->GetComponent<BoxCollider>()->SetBoxSize(5.0f, 10.0f, 5.0f);
		treeObject->GetComponent<BoxCollider>()->SetOffset(0.0f, 8.0f, 0.0f);
		treeObject->AddComponent<Particle>();

		// 이쁜 파티클 추가
		treeObject->GetComponent<Particle>()->SetParticleEffect("Flame", "Resources/Textures/Particles/flare.dds", 1000);
		treeObject->GetComponent<Particle>()->SetParticleDuration(1.0f, 0.2f);
		treeObject->GetComponent<Particle>()->SetParticleVelocity(10.0f, true);
		treeObject->GetComponent<Particle>()->SetParticleSize(20.0f, 20.0f);
		treeObject->GetComponent<Particle>()->AddParticleColor(15.0f, 0.6f, 0.0f);
		treeObject->GetComponent<Particle>()->SetOffSet(0.0f, 40.0f, 0.0f);
		// 임시 파티클

		treeObject->GetComponent<Particle>()->SetActive(false);
		treeObject->SetTag("BossSub");
		treeObject->SetActive(false);
		_treeObject.push_back(treeObject);
	}

	//_treeObjectReal = _boss->GetObjectScene()->CreateObject("treeObjectReal");
	//_treeObjectReal->AddComponent<MeshRenderer>();
	//_treeObjectReal->GetComponent<MeshRenderer>()->SetMeshObject("SM_tree_large_ancient_01/SM_tree_large_ancient_01");
	//_treeObjectReal->GetComponent<MeshRenderer>()->SetActive(true);
	//_treeObjectReal->GetComponent<Transform>()->SetRotation(0.0f, 0.0f, 0.0f);
	//_treeObjectReal->GetComponent<Transform>()->SetScale(0.1f, 0.1f, 0.1f);
	//_treeObjectReal->AddComponent<BoxCollider>();
	//_treeObjectReal->GetComponent<BoxCollider>()->SetBoxSize(5.0f, 10.0f, 5.0f);
	//_treeObjectReal->GetComponent<BoxCollider>()->SetOffset(0.0f, 8.0f, 0.0f);
	//_treeObjectReal->SetActive(false);
	//_treeObject.push_back(_treeObjectReal);

}

void KunrealEngine::Ent::LightControl(int onNumber)
{
	//for (auto treeObject : _treeObject)
	//{
	//	treeObject->GetComponent<Particle>()->SetActive(false);
	//}

	_treeObject[onNumber]->GetComponent<Particle>()->SetActive(true);
}

float KunrealEngine::Ent::GetRandomRange(float center, float range)
{
	float randomNumber = ((float)rand() / RAND_MAX) * 2.0f - 1.0f; 
	return center + randomNumber * range;
}

bool KunrealEngine::Ent::Move(DirectX::XMFLOAT3& startPos, DirectX::XMFLOAT3& targetPos, float speed)
{
	float moveSpeed = speed * TimeManager::GetInstance().GetDeltaTime();

	// 목적지까지 거리 계산
	auto dist = ToolBox::GetDistance(startPos, targetPos);
	if (dist > 0.5f)
	{
		DirectX::XMVECTOR currentPosVec = DirectX::XMLoadFloat3(&startPos);

		DirectX::XMVECTOR direction = DirectX::XMVectorSubtract(DirectX::XMLoadFloat3(&targetPos), currentPosVec);

		direction = DirectX::XMVector3Normalize(direction);

		DirectX::XMVECTOR newPosition = DirectX::XMVectorAdd(currentPosVec, DirectX::XMVectorScale(direction, moveSpeed));
		_bossTransform->SetPosition(newPosition.m128_f32[0], newPosition.m128_f32[1], newPosition.m128_f32[2]);

		return true;
	}

	return false;
}
