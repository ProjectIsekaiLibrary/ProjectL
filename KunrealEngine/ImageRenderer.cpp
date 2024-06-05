#include "ImageRenderer.h"
#include "GameObject.h"
#include "Transform.h"
#include "ToolBox.h"

KunrealEngine::ImageRenderer::ImageRenderer()
	:_image(nullptr), _transform(nullptr), 
	_decomposedPos({ 0.0f, 0.0f, 0.0f }), _decomposedRot({ 0.0f, 0.0f, 0.0f }), _decomposedScale({ 0.0f, 0.0f, 0.0f })
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
		// 부모가 있으면
		if (this->GetOwner()->GetParent() != nullptr)
		{
			DirectX::XMFLOAT4X4 worldTM = this->GetOwner()->GetComponent<Transform>()->GetWorldTM();
			DirectX::XMMATRIX worldMat = DirectX::XMLoadFloat4x4(&worldTM);

			DirectX::XMVECTOR scale, quaternion, translation;

			DirectX::XMMatrixDecompose(&scale, &quaternion, &translation, worldMat);
			DirectX::XMStoreFloat3(&_decomposedPos, translation);
			_decomposedRot = ToolBox::QuaternionToEulerAngles(quaternion);
			DirectX::XMStoreFloat3(&_decomposedScale, scale);

			_image->SetPosition(_decomposedPos.x, _decomposedPos.y);
			_image->SetRotation(_decomposedRot.z);
			_image->SetScale(_decomposedScale.x, _decomposedScale.y);
		}
		// 부모가 없으면
		else
		{
			_image->SetPosition(_transform->GetPosition().x, _transform->GetPosition().y);
			_image->SetRotation(_transform->GetRotation().z);
			_image->SetScale(_transform->GetScale().x, _transform->GetScale().y);
		}
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
