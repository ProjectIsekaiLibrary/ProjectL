#include <functional>
#include "SceneManager.h"
#include "Kamen.h"

KunrealEngine::Kamen::Kamen()
	: Boss(), _leftHand(nullptr), _rightHand(nullptr)
{
	BossBasicInfo info;

	info.SetHp(10000).SetPhase(3).SetArmor(10).SetDamage(100).SetHp(10000).SetMoveSpeed(20.0f).SetsStaggeredGauge(100.0f);
	info.SetAttackRange(5.0f);

	SetInfo(info);

	LeftAttackOnce();
	RightAttackOnce();
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

}

void KunrealEngine::Kamen::CreateSubObject()
{
	auto leftHand = SceneManager::GetInstance().GetCurrentScene()->CreateObject("LeftHand");
	_subObjectList.emplace_back(leftHand);

	auto rightHand = SceneManager::GetInstance().GetCurrentScene()->CreateObject("RightHand");
	_subObjectList.emplace_back(rightHand);
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

void KunrealEngine::Kamen::SetCollider()
{
	// 보스의 콜라이더, 플레이어의 이동 체크를 위해 보스의 크기만큼 설정
	_boss->AddComponent<BoxCollider>();
	_boss->GetComponent<BoxCollider>()->SetTransform(_boss, "Spine1_M");
	_boss->GetComponent<BoxCollider>()->SetBoxSize(6.0f, 18.0f, 10.0f);
	_boss->GetComponent<BoxCollider>()->SetOffset(0.0f, -1.5f, 0.0f);

	_subObjectList[0]->AddComponent<BoxCollider>();
	_leftHand = _subObjectList[0]->GetComponent<BoxCollider>();
	_leftHand->SetTransform(_boss, "MiddleFinger1_L");
	_leftHand->SetBoxSize(2.0f, 3.0f, 2.0f);

	_subObjectList[1]->AddComponent<BoxCollider>();
	_rightHand = _subObjectList[1]->GetComponent<BoxCollider>();
	_rightHand->SetTransform(_boss, "MiddleFinger1_R");
	_rightHand->SetBoxSize(2.0f, 3.0f, 2.0f);
}

void KunrealEngine::Kamen::SetBossTransform()
{
	_boss->GetComponent<Transform>()->SetPosition(5.0f, 0.0f, -20.0f);
}

void KunrealEngine::Kamen::LeftAttackOnce()
{
	BossPattern* pattern = new BossPattern();

	pattern->SetPatternName("Left_Attack_Once");

	pattern->SetAnimName("Left_Attack").SetDamage(100.0f).SetSpeed(20.0f).SetRange(_info._attackRange).SetAfterDelay(5.0f);
	pattern->SetIsWarning(false).SetWarningName("");

	std::function<bool()> logic = [pattern, this]()
	{
		auto animator = _boss->GetComponent<Animator>();
		auto isAnimationPlaying = animator->Play(pattern->_animName, pattern->_speed, false);

		// 일정 프레임이 넘어가면 데미지 체크용 콜라이더를 키기
		if (_maxColliderOnCount > 0)
		{
			if (animator->GetCurrentFrame() >= 10)
			{
				_leftHand->SetActive(true);
			}
		}

		if (isAnimationPlaying == false)
		{
			return false;
		}

		return true;
	};

	pattern->SetLogic(logic);

	_basicPattern.emplace_back(pattern);
}

void KunrealEngine::Kamen::RightAttackOnce()
{
	BossPattern* pattern = new BossPattern();

	pattern->SetPatternName("Right_Attack_Once");

	pattern->SetAnimName("Right_Attack").SetDamage(100.0f).SetSpeed(20.0f).SetRange(_info._attackRange).SetAfterDelay(5.0f);
	pattern->SetIsWarning(false).SetWarningName("");

	std::function<bool()> logic = [pattern, this]()
	{
		auto animator = _boss->GetComponent<Animator>();
		auto isAnimationPlaying = animator->Play(pattern->_animName, pattern->_speed, false);

		// 일정 프레임이 넘어가면 데미지 체크용 콜라이더를 키기
		if (_maxColliderOnCount > 0)
		{
			if (animator->GetCurrentFrame() >= 10)
			{
				_rightHand->SetActive(true);
			}
		}

		if (isAnimationPlaying == false)
		{
			return false;
		}

		return true;
	};

	pattern->SetLogic(logic);

	_basicPattern.emplace_back(pattern);
}
