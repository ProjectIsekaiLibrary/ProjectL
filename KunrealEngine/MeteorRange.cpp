#include "MeteorRange.h"
#include "Transform.h"
#include "MeshRenderer.h"
#include "GraphicsSystem.h"
#include "InputSystem.h"
#include "Navigation.h"

KunrealEngine::MeteorRange::MeteorRange()
	:_transform(nullptr), _mesh(nullptr), _onCast(false)
{

}

void KunrealEngine::MeteorRange::Initialize()
{
	this->_transform = this->GetOwner()->GetComponent<Transform>();
	this->_transform->SetTotalScale(0.3f);

	this->_mesh = this->GetOwner()->AddComponent<MeshRenderer>();
	this->_mesh->SetMeshObject("MagicCircle/MagicCircle");
}

void KunrealEngine::MeteorRange::Release()
{
	
}

void KunrealEngine::MeteorRange::FixedUpdate()
{
	
}

void KunrealEngine::MeteorRange::Update()
{
	// 마우스 방향에 계속 생성되도록
	if (!_onCast)
	{
		DirectX::XMFLOAT3 pos = GRAPHICS->ScreenToWorldPoint(InputSystem::GetInstance()->GetEditorMousePos().x, InputSystem::GetInstance()->GetEditorMousePos().y);
		pos.y = 2.0f;

		this->_transform->SetPosition(pos);
	}

	SetTransparencyByMouse(CheckisOnMap());
}

void KunrealEngine::MeteorRange::LateUpdate()
{
	
}

void KunrealEngine::MeteorRange::OnTriggerEnter()
{
	
}

void KunrealEngine::MeteorRange::OnTriggerStay()
{
	
}

void KunrealEngine::MeteorRange::OnTriggerExit()
{
	
}

void KunrealEngine::MeteorRange::SetActive(bool active)
{
	
}

KunrealEngine::MeteorRange::~MeteorRange()
{

}

void KunrealEngine::MeteorRange::SetTransparencyByMouse(bool onMap)
{
	if (onMap)
	{
		this->_mesh->SetAlpha(1.0f);
	}
	else
	{
		this->_mesh->SetAlpha(0.5f);
	}
}

bool KunrealEngine::MeteorRange::CheckisOnMap()
{
	DirectX::XMFLOAT3 mousePos = GRAPHICS->ScreenToWorldPoint(InputSystem::GetInstance()->GetEditorMousePos().x, InputSystem::GetInstance()->GetEditorMousePos().y);

	bool isOnmap = Navigation::GetInstance().GetPositionOnMap(0, mousePos.x, mousePos.y, mousePos.z);

	return isOnmap;
}
