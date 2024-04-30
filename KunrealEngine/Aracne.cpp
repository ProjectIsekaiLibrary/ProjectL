#include <functional>
#include "ToolBox.h"
#include "TimeManager.h"
#include "SceneManager.h"
#include "Aracne.h"

KunrealEngine::Aracne::Aracne()
{
	BossBasicInfo info;

	info.SetHp(10000).SetPhase(3).SetArmor(10).SetDamage(100).SetHp(10000).SetMoveSpeed(20.0f).SetsStaggeredGauge(100.0f);
	info.SetAttackRange(5.0f);

	SetInfo(info);
}

KunrealEngine::Aracne::~Aracne()
{

}

void KunrealEngine::Aracne::Initialize()
{
	Boss::Initialize(this->GetOwner());
}

void KunrealEngine::Aracne::Release()
{

}

void KunrealEngine::Aracne::FixedUpdate()
{

}

void KunrealEngine::Aracne::Update()
{
	Boss::Update();
}

void KunrealEngine::Aracne::LateUpdate()
{

}

void KunrealEngine::Aracne::OnTriggerEnter()
{

}

void KunrealEngine::Aracne::OnTriggerStay()
{

}

void KunrealEngine::Aracne::OnTriggerExit()
{

}

void KunrealEngine::Aracne::SetActive(bool active)
{

}

void KunrealEngine::Aracne::SetMesh()
{
	_boss->AddComponent<MeshRenderer>();
	_boss->GetComponent<MeshRenderer>()->SetMeshObject("SpiderQueen/SpiderQueen");
}

void KunrealEngine::Aracne::SetTexture()
{
	auto texSize = _boss->GetComponent<MeshRenderer>()->GetTextures().size();
	for (int i = 0; i < texSize; i++)
	{
		_boss->GetComponent<MeshRenderer>()->SetDiffuseTexture(i, "SpiderQueen/T_SpiderQueen_1_D.tga");
		_boss->GetComponent<MeshRenderer>()->SetNormalTexture(i, "SpiderQueen/T_SpiderQueen_N.tga");
		_boss->GetComponent<MeshRenderer>()->SetEmissiveTexture(i, "SpiderQueen/T_SpiderQueen_1_E.tga");
	}
}

void KunrealEngine::Aracne::SetBossTransform()
{
	_boss->GetComponent<Transform>()->SetPosition(5.0f, 0.0f, -20.0f);
	_boss->GetComponent<Transform>()->SetScale(5.0f , 5.0f , 5.0f);
}

void KunrealEngine::Aracne::SetBossCollider()
{
	_boss->AddComponent<BoxCollider>();
	//_boss->GetComponent<BoxCollider>()->SetTransform(_boss, "Spine1_M");
	_boss->GetComponent<BoxCollider>()->SetBoxSize(6.0f, 18.0f, 10.0f);
	_boss->GetComponent<BoxCollider>()->SetOffset(0.0f, -1.5f, 0.0f);
}

void KunrealEngine::Aracne::CreatePattern()
{
	JumpAttack();
	leftAttack();
	RightAttack();
	ShootingWeb();
}

void KunrealEngine::Aracne::JumpAttack()
{
	BossPattern* pattern = new BossPattern();

	pattern->SetPatternName("Left_Attack_Once");

	_basicPattern.emplace_back(pattern);
}

void KunrealEngine::Aracne::leftAttack()
{

}

void KunrealEngine::Aracne::RightAttack()
{

}

void KunrealEngine::Aracne::ShootingWeb()
{

}
