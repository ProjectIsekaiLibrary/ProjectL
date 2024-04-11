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
	// ����ü ������Ʈ�� �����ɶ� �浹������ �׷��� 3D�� �ʿ�
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
	// �����Ǵ� ������ �߻�� ������ �߿�
	// ���⺤�ʹ� ����ü ���� ���� �÷��̾ �ٶ󺸴� ���⺤�͸� ������

	_collider = this->GetOwner()->GetComponent<BoxCollider>();
	_mesh = this->GetOwner()->GetComponent<MeshRenderer>();
	_direction = playerObj->GetComponent<Player>()->GetDirectionVector();
}

void KunrealEngine::Projectile::SetMeshObject(const char* meshName, const char* textureName /*= ""*/, const char* normalName /*= ""*/)
{
	// mesh�� �ݵ�� �ʿ�
	_mesh->SetMeshObject(meshName);

	// �ؽ�ó�� �������� �ʾ����� �ѱ�
	if (textureName != "")
	{
		_mesh->SetDiffuseTexture(0, textureName);
	}

	// �븻�� �������� �ʾ����� �ѱ�
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