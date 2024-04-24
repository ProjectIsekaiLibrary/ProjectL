#include "ResourceManager.h"
#include "Camera.h"
#include "Transform.h"
#include "CommonStruct.h"
#include "ArkEffect.h"
#include "ArkTexture.h"
#include "d3dx11effect.h"
#include "ArkDevice.h"
#include "ArkBuffer.h"
#include "UIImage.h"

ArkEngine::ArkDX11::UIImage::UIImage(const std::string& imageName, unsigned int screenWidth, unsigned int screenHeight)
	: _imageName("Resources/Textures/" + imageName), _effect(nullptr), _tech(nullptr), _fxWorldViewProj(nullptr),
	_vertexBuffer(nullptr), _indexBuffer(nullptr),
	_diffuseMapSRV(nullptr), _fxColor(nullptr),
	_arkDevice(nullptr), _arkBuffer(nullptr), _arkEffect(nullptr),
	_meshTransform(nullptr),
	_hashValue(0), _objectIndex(0), _isRendering(true),
	_halfScreenWidth(screenWidth/2), _halfScreenHeight(screenHeight/2),
	_imageHalfWidth(screenWidth / 2.0f), _imageHalfHeight(screenHeight / 2.0f)
{
	Initialize();
}

ArkEngine::ArkDX11::UIImage::~UIImage()
{	
	delete _meshTransform;

	_arkEffect = nullptr;
	_arkBuffer = nullptr;
	_arkDevice = nullptr;

	_fxColor->Release();

	_diffuseMapSRV = nullptr;

	_diffuseMap->Release();

	_vertexBuffer = nullptr;
	_indexBuffer = nullptr;

	_tech = nullptr;
	_effect = nullptr;
}

void ArkEngine::ArkDX11::UIImage::Render(bool isFinal)
{
	auto deviceContext = _arkDevice->GetDeviceContext();

	deviceContext->IASetInputLayout(_arkEffect->GetInputLayOut());
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	deviceContext->RSSetState(_arkDevice->GetSolidRS());

	UINT stride = sizeof(ArkEngine::ArkDX11::Postex);
	UINT offset = 0;
	auto vertexBuffer = _arkBuffer->GetVertexBuffer();

	deviceContext->IASetVertexBuffers(0, 1, &_vertexBuffer, &stride, &offset);
	deviceContext->IASetIndexBuffer(_indexBuffer, DXGI_FORMAT_R32_UINT, 0);

	DirectX::XMMATRIX world = _meshTransform->GetTransformMatrix();

	_fxWorldViewProj->SetMatrix(reinterpret_cast<float*>(&world));

	_diffuseMap->SetResource(_diffuseMapSRV);

	_fxColor->SetFloatVector(_color);

	D3DX11_TECHNIQUE_DESC techDesc;
	_tech->GetDesc(&techDesc);

	deviceContext->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
	deviceContext->IASetIndexBuffer(_arkBuffer->GetIndexBuffer(), DXGI_FORMAT_R32_UINT, 0);

	if (isFinal)
	{
		_tech->GetPassByIndex(0)->Apply(0, deviceContext);
	}
	else
	{
		_tech->GetPassByIndex(1)->Apply(0, deviceContext);
	}

	deviceContext->DrawIndexed(6, 0, 0);
}

bool ArkEngine::ArkDX11::UIImage::GetRenderingState()
{
	return _isRendering;
}

void ArkEngine::ArkDX11::UIImage::SetRenderingState(bool tf)
{
	_isRendering = tf;
}

const std::string& ArkEngine::ArkDX11::UIImage::GetImageName()
{
	return _imageName;
}

void ArkEngine::ArkDX11::UIImage::SetTransform(DirectX::XMFLOAT4X4 matrix)
{
	_meshTransform->SetTransformMatrix(matrix);
}

void ArkEngine::ArkDX11::UIImage::SetPosition(float x, float y)
{
	_meshTransform->SetTranslationMatrix(NomalizeToScreenX(x), NomalizeToScreenY(y));
}

void ArkEngine::ArkDX11::UIImage::SetRotation(float angle)
{
	_meshTransform->SetRotationMatrix(0.0f, 0.0f, angle);
}

void ArkEngine::ArkDX11::UIImage::SetScale(float x, float y)
{
	_meshTransform->SetScaleMatrix(0.1*x, 0.1*y);
	_imageHalfWidth = _halfScreenWidth * (0.1 * x);
	_imageHalfHeight = _halfScreenHeight * (0.1 * y);
}

void ArkEngine::ArkDX11::UIImage::Delete()
{
	delete this;
}

unsigned int ArkEngine::ArkDX11::UIImage::GetHashID()
{
	return _hashValue;
}

void ArkEngine::ArkDX11::UIImage::Initialize()
{
	_objectIndex = ResourceManager::GetInstance()->GetObjectIndex();

	for (int i = 0; i < 4; i++)
	{
		_color[i] = 0.0f;
	}

	_arkEffect = ResourceManager::GetInstance()->GetResource<ArkEngine::ArkDX11::ArkEffect>("Resources/FX/UIImage.fx");
	_effect = _arkEffect->GetEffect();

	_arkDevice = ResourceManager::GetInstance()->GetResource<ArkEngine::ArkDX11::ArkDevice>("Device");

	BuildGeometryBuffers();

	SetEffect();

	SetHashValue(_objectIndex);

	_meshTransform = new Transform();

	SetScale();
	SetPosition();

	SetTexture();

	ResourceManager::GetInstance()->AddUIImage(this);
}

void ArkEngine::ArkDX11::UIImage::SetEffect()
{
	_tech = _effect->GetTechniqueByIndex(0);

	_fxWorldViewProj = _effect->GetVariableByName("gWorldViewProj")->AsMatrix();

	_diffuseMap = _effect->GetVariableByName("gDiffuseMap")->AsShaderResource();

	_fxColor = _effect->GetVariableByName("gColor")->AsVector();
}

void ArkEngine::ArkDX11::UIImage::BuildGeometryBuffers()
{
	_arkBuffer = ResourceManager::GetInstance()->GetResource<ArkBuffer>("Quad");

	_vertexBuffer = _arkBuffer->GetVertexBuffer();
	_indexBuffer = _arkBuffer->GetIndexBuffer();
}

void ArkEngine::ArkDX11::UIImage::ConvertHashToRGBA(int hashValue)
{
	float a = (hashValue >> 24) & 0xff;
	float b = (hashValue >> 16) & 0xff;
	float g = (hashValue >> 8) & 0xff;
	float r = hashValue & 0xff;

	_color[0] = r / 255.0f;
	_color[1] = g / 255.0f;
	_color[2] = b / 255.0f;
	_color[3] = a / 255.0f;
}

void ArkEngine::ArkDX11::UIImage::SetHashValue(unsigned int index)
{
	if (_hashValue == 0)
	{
		std::hash<int> intHash;

		_hashValue = intHash(index);
		ConvertHashToRGBA(_hashValue);
	}
}

void ArkEngine::ArkDX11::UIImage::SetTexture()
{
	auto texture = ResourceManager::GetInstance()->GetResource<ArkTexture>(_imageName);

	if (texture != nullptr)
	{
		_diffuseMapSRV = texture->GetDiffuseMapSRV();
	}
	else
	{
		ArkTexture* newTexture = new ArkTexture(_imageName.c_str());
		_diffuseMapSRV = newTexture->GetDiffuseMapSRV();
	}
}

float ArkEngine::ArkDX11::UIImage::NomalizeToScreenX(float num)
{
	auto pad = -_halfScreenWidth + _imageHalfWidth;

	auto distance = (float)(pad + num) / (float)_halfScreenWidth;

	return distance;
}

float ArkEngine::ArkDX11::UIImage::NomalizeToScreenY(float num)
{
	auto pad = -_halfScreenHeight + _imageHalfHeight;

	auto distance = (float)(pad + num) / (float)_halfScreenHeight;

	return -1 * distance;
}
