#include "Projectile.h"
#include "BoxCollider.h"
#include "MeshRenderer.h"
#include "Player.h"

KunrealEngine::Projectile::Projectile()
	:_collider(nullptr), _direction(), _mesh(nullptr)
{
	_direction = DirectX::XMVectorZero();
}

KunrealEngine::Projectile::~Projectile()
{

}

void KunrealEngine::Projectile::Initialize()
{
	// 투사체 오브젝트가 생성될때 충돌감지와 그려줄 3D가 필요
	this->GetOwner()->AddComponent<BoxCollider>();
	this->GetOwner()->AddComponent<MeshRenderer>();

}

void KunrealEngine::Projectile::Release()
{
	
}

void KunrealEngine::Projectile::FixedUpdate()
{
	
}

void KunrealEngine::Projectile::Update()
{
	
}

void KunrealEngine::Projectile::LateUpdate()
{
	if (_condition())
	{
		this->GetOwner()->SetActive(false);
	}
	else
	{
		this->GetOwner()->SetActive(true);
	}
}

void KunrealEngine::Projectile::OnTriggerEnter()
{

}
void KunrealEngine::Projectile::OnTriggerStay()
{
	
}
void KunrealEngine::Projectile::OnTriggerExit()
{
	
}

void KunrealEngine::Projectile::SetActive(bool active)
{
	
}

void KunrealEngine::Projectile::CreateInfo(GameObject* playerObj)
{
	// 생성되는 시점에 발사될 방향이 중요
	// 방향벡터는 투사체 생성 시점 플레이어가 바라보는 방향벡터를 가져옴

	_collider = this->GetOwner()->GetComponent<BoxCollider>();
	_mesh = this->GetOwner()->GetComponent<MeshRenderer>();
	_direction = playerObj->GetComponent<Player>()->GetDirectionVector();
}

void KunrealEngine::Projectile::SetMeshObject(const char* meshName, const char* textureName /*= ""*/, const char* normalName /*= ""*/)
{
	// mesh는 반드시 필요
	_mesh->SetMeshObject(meshName);

	// 텍스처를 정해주지 않았으면 넘김
	if (textureName != "")
	{
		_mesh->SetDiffuseTexture(0, textureName);
	}

	// 노말을 정해주지 않았으면 넘김
	if (normalName != "")
	{
		_mesh->SetNormalTexture(0, normalName);
	}
}

KunrealEngine::BoxCollider* KunrealEngine::Projectile::GetCollider()
{
	return this->_collider;
}

DirectX::XMVECTOR KunrealEngine::Projectile::GetDirection()
{
	return this->_direction;
}

void KunrealEngine::Projectile::SetDestoryCondition(std::function<bool()> cond)
{
	_condition = cond;
}
