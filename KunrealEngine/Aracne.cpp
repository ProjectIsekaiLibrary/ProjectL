#include <functional>
#include "ToolBox.h"
#include "TimeManager.h"
#include "SceneManager.h"
#include "Aracne.h"

KunrealEngine::Aracne::Aracne()
{
	BossBasicInfo info;

	info.SetHp(10000);
	info.SetPhase(3);
	info.SetArmor(10);
	info.SetDamage(100);
	info.SetHp(10000);
	info.SetMoveSpeed(20.0f);
	info.SetsStaggeredGauge(100.0f);
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
	_boss->GetComponent<Transform>()->SetScale(7.5f, 7.5f, 7.5f);
}

void KunrealEngine::Aracne::SetBossCollider()
{
	_boss->AddComponent<BoxCollider>();
	_boss->GetComponent<BoxCollider>()->SetTransform(_boss, "Spine1_M");
	_boss->GetComponent<BoxCollider>()->SetBoxSize(6.0f, 18.0f, 10.0f);
	_boss->GetComponent<BoxCollider>()->SetOffset(0.0f, -1.5f, 0.0f);
}

void KunrealEngine::Aracne::CreatesubObject()
{
	_colJumpAttack = _boss->GetObjectScene()->CreateObject("Jump");
	_colJumpAttack->AddComponent<BoxCollider>();
	//_colJumpAttack->GetComponent<BoxCollider>()->SetTransform(_boss, "Spine1_M");
	_colJumpAttack->GetComponent<BoxCollider>()->SetOffset(0.0f, 3.0f, 4.0f);
	_colJumpAttack->GetComponent<BoxCollider>()->SetBoxSize(27.0f, 6.0f, 30.0f);
	_colJumpAttack->SetActive(false);

	_colLeftHand = _boss->GetObjectScene()->CreateObject("left_hand");
	_colLeftHand->AddComponent<BoxCollider>();
  //_colLeftHand->GetComponent<BoxCollider>()->SetTransform(_boss, "Spine1_M");
	_colLeftHand->GetComponent<BoxCollider>()->SetOffset(0.0f, 3.0f, 4.0f);
	_colLeftHand->GetComponent<BoxCollider>()->SetBoxSize(27.0f, 6.0f, 30.0f);
	_colLeftHand->SetActive(false);

	_colRightHand = _boss->GetObjectScene()->CreateObject("Right_hand");
	_colRightHand->AddComponent<BoxCollider>();
  //_colRightHand->GetComponent<BoxCollider>()->SetTransform(_boss, "Spine1_M");
	_colRightHand->GetComponent<BoxCollider>()->SetOffset(0.0f, 3.0f, 4.0f);
	_colRightHand->GetComponent<BoxCollider>()->SetBoxSize(27.0f, 6.0f, 30.0f);
	_colRightHand->SetActive(false);
}

void KunrealEngine::Aracne::CreatePattern()
{
	CreatesubObject();

	//JumpAttack();
	LeftAttack();
	RightAttack();
	//ShootingWeb();
}

void KunrealEngine::Aracne::JumpAttack()
{
	_jumpAttack = new BossPattern();

	_jumpAttack->SetPatternName("Jump_Attack");

	_jumpAttack->SetAnimName("Anim_Jump").SetDamage(100.0f).SetSpeed(20.0f).SetRange(jumpAttackRange).SetAfterDelay(0.5);
	_jumpAttack->SetIsWarning(false).SetWarningName("");
	_jumpAttack->SetAttackState(BossPattern::eAttackState::ePush).SetMaxColliderCount(1);

	std::function logic = [this]()
		{
			Startcoroutine(JumpAttackCo);
			return true;
		};

	_jumpAttack->SetLogic(logic);
	_basicPattern.emplace_back(_jumpAttack);
}

void KunrealEngine::Aracne::LeftAttack()
{
	_leftAttack = new BossPattern();

	_leftAttack->SetPatternName("Left_Attack_Once");

	_leftAttack->SetAnimName("left_Attack").SetDamage(100.0f).SetSpeed(20.0f).SetRange(_info._attackRange).SetAfterDelay(0.5);
	_leftAttack->SetIsWarning(false).SetWarningName("");
	_leftAttack->SetAttackState(BossPattern::eAttackState::ePush).SetMaxColliderCount(1);

	auto leftHandLogic = CreateBasicAttackLogic(_leftAttack, _colLeftHand, 10);

	_leftAttack->SetLogic(leftHandLogic);
	_basicPattern.emplace_back(_leftAttack);
}

void KunrealEngine::Aracne::RightAttack()
{
	_righttAttack = new BossPattern();

	_righttAttack->SetPatternName("Right_Attack_Once");

	_righttAttack->SetAnimName("Right_attack").SetDamage(100.0f).SetSpeed(20.0f).SetRange(_info._attackRange).SetAfterDelay(0.5);
	_righttAttack->SetIsWarning(false).SetWarningName("");
	_righttAttack->SetAttackState(BossPattern::eAttackState::ePush).SetMaxColliderCount(1);

	auto rightHandLogic = CreateBasicAttackLogic(_righttAttack, _colRightHand, 10);

	_righttAttack->SetLogic(rightHandLogic);
	_basicPattern.emplace_back(_righttAttack);
}

void KunrealEngine::Aracne::ShootingWeb()
{

}

bool KunrealEngine::Aracne::Move(DirectX::XMFLOAT3& startPos, DirectX::XMFLOAT3& targetPos, float speed)
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
		_bossTransform->SetPosition(newPosition.m128_f32[0], 0.0f, newPosition.m128_f32[2]);

		return true;
	}

	return false;
}

float KunrealEngine::Aracne::CalculateParabolaHeight(DirectX::XMFLOAT3 start, DirectX::XMFLOAT3 end, DirectX::XMFLOAT3 current)
{
	const float max_height = 2.0;

	// start와 end 지점을 지나는 직선의 기울기 계산
	float m = (end.z - start.z) / (end.x - start.x);

	// 직선의 y 절편 계산
	float c = start.z - m * start.x;

// 	// current 지점이 직선 상에 있는지 확인
// 	float expected_z = m * current.x + c;
// 	if (expected_z != current.y)
// 	{
// 		// current 지점이 직선 상에 있지 않으면 0을 반환
// 		return 0.0f;
// 	}

	// current 지점이 직선 상에 있을 경우, 포물선의 높이 계산
	// 포물선의 방정식: y = a*x^2 + b*x + c
	float a = (end.y - start.y) / ((end.x - start.x) * (end.x - start.x));
	float b = (start.y - a * start.x * start.x) / start.x;

	// 포물선의 정점의 높이를 max_height로 설정
	float c_parabola = a * current.x * current.x + b * current.x + max_height;

	return c_parabola;
}


