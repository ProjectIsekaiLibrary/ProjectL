#include <functional>
#include "ToolBox.h"
#include "TimeManager.h"
#include "SceneManager.h"
#include "Ent.h"

KunrealEngine::Ent::Ent()
	: Boss(), _leftHand(nullptr), _rightHand(nullptr), _call(nullptr), _lazer(nullptr),
	_callMoveDistance(0.0f), _isRotateFinish(false), _isCoreStart(false), _isRandomStart(false),
	_leftAttack(nullptr), _rightAttack(nullptr), _spellAttack(nullptr), _callAttack(nullptr), _backStep(nullptr)
{
	BossBasicInfo info;

	info.SetHp(100).SetPhase(3).SetArmor(10).SetDamage(100).SetMoveSpeed(20.0f).SetsStaggeredGauge(100.0f);
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
	this->_isActivated = active;
}

void KunrealEngine::Ent::SetMesh()
{
	_boss->AddComponent<MeshRenderer>();
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
	_boss->GetComponent<KunrealEngine::Transform>()->SetPosition(5.0f, 0.0f, -20.0f);
}

void KunrealEngine::Ent::SetBossCollider()
{
	_boss->AddComponent<BoxCollider>();
	_boss->GetComponent<BoxCollider>()->SetTransform(_boss, "Spine1_M");
	_boss->GetComponent<BoxCollider>()->SetBoxSize(6.0f, 18.0f, 10.0f);
	_boss->GetComponent<BoxCollider>()->SetOffset(0.0f, -1.5f, 0.0f);
}

void KunrealEngine::Ent::CreatePattern()
{

}
    
void KunrealEngine::Ent::CreateLeftAttack()
{
	BossPattern* pattern = new BossPattern();

	pattern->SetPatternName("Left_Attack_Once");
	// 애니메이션 추가해야함
	pattern->SetDamage(100.0f);
	pattern->SetSpeed(20.0f);         
	pattern->SetRange(_info._attackRange);
	pattern->SetAfterDelay(0.5f);

	pattern->SetIsWarning(false).SetWarningName("");
	pattern->SetAttackState(BossPattern::eAttackState::ePush).SetMaxColliderCount(1);

	auto leftHandLogic = CreateBasicAttackLogic(pattern, _leftHand, 10);

	pattern->SetLogic(leftHandLogic);

	_leftAttack = pattern;
}

void KunrealEngine::Ent::CreateRightAttack()
{
	BossPattern* pattern = new BossPattern();

	pattern->SetPatternName("Right_Attack_Once");
	// 애니메이션 추가해야함
	pattern->SetDamage(100.0f);
	pattern->SetSpeed(20.0f);
	pattern->SetRange(_info._attackRange);
	pattern->SetAfterDelay(0.5f);

	pattern->SetIsWarning(false).SetWarningName("");
	pattern->SetAttackState(BossPattern::eAttackState::ePush).SetMaxColliderCount(1);

	pattern->_subObject.emplace_back(_rightHand);

	auto rightHandLogic = CreateBasicAttackLogic(pattern, _rightHand, 10);

	pattern->SetLogic(rightHandLogic);

	_rightAttack = pattern;
}

void KunrealEngine::Ent::CreateLeftRootShot()
{
	BossPattern* pattern = new BossPattern();


	pattern->SetPatternName("Left_Root_Shot");
	pattern->SetDamage(100.0f);
	pattern->SetSpeed(20.0f);
	//pattern->SetRange(100.0f);
	pattern->SetAfterDelay(0.5f);
}

void KunrealEngine::Ent::CreateRightRootShot()
{

}

void KunrealEngine::Ent::CreateRandomRootAttack()
{

}

void KunrealEngine::Ent::CreateJumpAttack()
{

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
