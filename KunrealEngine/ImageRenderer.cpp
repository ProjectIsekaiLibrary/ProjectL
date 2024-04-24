#include "ImageRenderer.h"
#include "GameObject.h"
#include "Transform.h"

KunrealEngine::ImageRenderer::ImageRenderer()
	:_image(nullptr), _transform(nullptr)
{

}

KunrealEngine::ImageRenderer::~ImageRenderer()
{

}

void KunrealEngine::ImageRenderer::Initialize()
{
	_transform = GetOwner()->GetComponent<Transform>();
}

void KunrealEngine::ImageRenderer::Release()
{
	if (_image != nullptr)
	{
		_image->Delete();
	}
}

void KunrealEngine::ImageRenderer::FixedUpdate()
{
	
}

void KunrealEngine::ImageRenderer::Update()
{
	// image의 WorldTM은 0,0에서부터 다시 계산하는 공식이 그래픽스쪽에 있다
	// 그래서 오브젝트의 transform이 가진 worldTM을 넘겨주는게 아니라 그쪽에서 다시 계산하게 해야함
	if (_image != nullptr)
	{
		_image->SetPosition(_transform->GetPosition().x, _transform->GetPosition().y);
		_image->SetRotation(_transform->GetRotation().z);
		_image->SetScale(_transform->GetScale().x, _transform->GetScale().y);
	}
}

void KunrealEngine::ImageRenderer::LateUpdate()
{
	
}

void KunrealEngine::ImageRenderer::OnTriggerEnter()
{
	
}

void KunrealEngine::ImageRenderer::OnTriggerStay()
{
	
}

void KunrealEngine::ImageRenderer::OnTriggerExit()
{
	
}

void KunrealEngine::ImageRenderer::SetActive(bool active)
{
	if (_image != nullptr)
	{
		SetImageStatus(active);
	}

	this->_isActivated = active;
}

void KunrealEngine::ImageRenderer::SetImage(std::string imageName)
{
	if (_image != nullptr)
	{
		_image->Delete();
	}

	_image = GRAPHICS->CreateImage(imageName.c_str());
}

const std::string& KunrealEngine::ImageRenderer::GetImageName()
{
	if (_image != nullptr)
	{
		return _image->GetImageName();
	}
}

void KunrealEngine::ImageRenderer::SetPosition(float x, float y)
{
	_transform->SetPosition(x, y, 0.f);
}

void KunrealEngine::ImageRenderer::SetRotation(float x, float y)
{
	_transform->SetRotation(x, y, 0.f);
}

void KunrealEngine::ImageRenderer::SetScale(float x, float y)
{
	_transform->SetScale(x, y, 1.f);
}

void KunrealEngine::ImageRenderer::SetImageStatus(bool flag)
{
 	_image->SetRenderingState(flag);
}

bool KunrealEngine::ImageRenderer::GetImageStatus()
{
	if (_image != nullptr)
	{
		return _image->GetRenderingState();
	}
}

bool KunrealEngine::ImageRenderer::IsPicked(int mouseX, int mouseY)
{
	if ((mouseX == -1) || (mouseY == -1))
	{
		return false;
	}

	if (_image != nullptr && GRAPHICS->GetPickedImage(mouseX, mouseY) == _image)
	{
		return true;
	}
	else
	{
		return false;
	}
}
