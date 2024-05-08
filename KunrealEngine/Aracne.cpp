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
	_boss->GetComponent<Transform>()->SetScale(5.0f, 5.0f, 5.0f);
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
	//leftAttack();
	//RightAttack();
	//ShootingWeb();
}

void KunrealEngine::Aracne::JumpAttack()
{
	BossPattern* pattern = new BossPattern();

	pattern->SetPatternName("Jump_Attack");

	pattern->SetAnimName("Anim_Jump").SetDamage(100.0f).SetSpeed(20.0f).SetRange(jumpAttackRange).SetAfterDelay(0.5);
	pattern->SetIsWarning(false).SetWarningName("");
	pattern->SetAttackState(BossPattern::eAttackState::ePush).SetMaxColliderCount(1);

	std::function logic = [this]()
		{
			Startcoroutine(JumpAttackCo);
			return true;
		};

	pattern->SetLogic(logic);
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

float KunrealEngine::Aracne::CalculateParabolaHeight(DirectX::XMFLOAT3 start, DirectX::XMFLOAT3 end, DirectX::XMFLOAT2 current)
{
	// 직선의 방정식 계산: y = mx + c
	float x1 = start.x;
	float z1 = start.z;
	float x2 = end.x;
	float z2 = end.z;
	float x3 = current.x;
	float z3 = current.y;

	// start와 end 지점을 지나는 직선의 기울기 계산
	float m = (z2 - z1) / (x2 - x1);

	// 직선의 y 절편 계산
	float c = z1 - m * x1;

	// current 지점이 직선 상에 있는지 확인
	float expected_z = m * x3 + c;
	if (expected_z != z3) {
		// current 지점이 직선 상에 있지 않으면 0을 반환
		return 0.0f;
	}

	// current 지점이 직선 상에 있을 경우, 포물선의 높이 계산
	// 포물선의 방정식: y = a*x^2 + b*x + c
	float a = (end.y - start.y) / ((end.x - start.x) * (end.x - start.x));
	float b = (start.y - a * start.x * start.x) / start.x;
	float c_parabola = a * current.x * current.x + b * current.x;

	return c_parabola;
}


