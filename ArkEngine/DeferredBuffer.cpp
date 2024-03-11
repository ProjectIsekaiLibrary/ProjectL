#include <d3d11.h>
#include "ResourceManager.h"
#include "ArkDevice.h"
#include "deferredBuffer.h"

ArkEngine::ArkDX11::deferredBuffer::deferredBuffer(int width, int height)
	:_textureWidth(width), _textureHeight(height)
{
	Initialize();
}

ArkEngine::ArkDX11::deferredBuffer::~deferredBuffer()
{
	Finalize();
}

void ArkEngine::ArkDX11::deferredBuffer::Initialize()
{
	for (int i = 0; i < static_cast<int>(eBUFFERTYPE::GBUFFER_COUNT); i++)
	{
		_renderTargetTextureArray[i] = nullptr;
		_renderTargetViewArray[i] = nullptr;
		_shaderResourceViewArray[i] = nullptr;
	}

	auto arkDevice = ResourceManager::GetInstance()->GetResource<ArkEngine::ArkDX11::ArkDevice>("Device");

	CreateRenderTargetTexture(arkDevice);
	CreateRenderTargetView(arkDevice);
	CreateShaderResourceView(arkDevice);
}

void ArkEngine::ArkDX11::deferredBuffer::Finalize()
{
	for (int i = 0; i < static_cast<int>(eBUFFERTYPE::GBUFFER_COUNT); i++)
	{
		_renderTargetTextureArray[i]->Release();
		_renderTargetViewArray[i]->Release();
		_shaderResourceViewArray[i]->Release();
	}
}

void ArkEngine::ArkDX11::deferredBuffer::SetRenderTargets()
{
	auto arkDevice = ResourceManager::GetInstance()->GetResource<ArkEngine::ArkDX11::ArkDevice>("Device");

	arkDevice->GetDeviceContext()->OMSetRenderTargets(static_cast<int>(eBUFFERTYPE::GBUFFER_COUNT), _renderTargetViewArray, arkDevice->GetDepthStecilView());
}

void ArkEngine::ArkDX11::deferredBuffer::ClearRenderTargets(float color[4])
{
	auto arkDevice = ResourceManager::GetInstance()->GetResource<ArkEngine::ArkDX11::ArkDevice>("Device");

	for (int i = 0; i < static_cast<int>(eBUFFERTYPE::GBUFFER_COUNT); i++)
	{
		arkDevice->GetDeviceContext()->ClearRenderTargetView(_renderTargetViewArray[i], color);
	}
}

ID3D11ShaderResourceView* ArkEngine::ArkDX11::deferredBuffer::GetSRV(int index)
{
	return _shaderResourceViewArray[index];
}

ID3D11Texture2D* ArkEngine::ArkDX11::deferredBuffer::GetTextrue(int index)
{
	return _renderTargetTextureArray[index];
}

void ArkEngine::ArkDX11::deferredBuffer::CreateRenderTargetTexture(ArkEngine::ArkDX11::ArkDevice* pDeivce)
{
	D3D11_TEXTURE2D_DESC textureDesc;

	textureDesc.Height = _textureHeight;
	textureDesc.Width = _textureWidth;
	textureDesc.MipLevels = 1;
	textureDesc.ArraySize = 1;
	textureDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.SampleDesc.Quality = 0;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	textureDesc.CPUAccessFlags = 0;
	textureDesc.MiscFlags = 0;

	for (int i = 0; i < static_cast<int>(eBUFFERTYPE::GBUFFER_COUNT); i++)
	{
		// Picking을 위한 해쉬 ID 생성 로직때문에 가장 뒤의 렌더타겟인
		// Color버퍼는 DXGI_FORMAT_R8G8B8A8_UNORM로 처리해야함
		if (i == static_cast<int>(eBUFFERTYPE::GBUFFER_COUNT) - 1)
		{
			textureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		}

		HRESULT result = pDeivce->GetDevice()->CreateTexture2D(&textureDesc, NULL, &_renderTargetTextureArray[i]);
	}
}

void ArkEngine::ArkDX11::deferredBuffer::CreateRenderTargetView(ArkEngine::ArkDX11::ArkDevice* pDeivce)
{
	D3D11_RENDER_TARGET_VIEW_DESC renderTagetViewDesc;

	renderTagetViewDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	renderTagetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	renderTagetViewDesc.Texture2D.MipSlice = 0;

	for (int i = 0; i < static_cast<int>(eBUFFERTYPE::GBUFFER_COUNT); i++)
	{
		// Picking을 위한 해쉬 ID 생성 로직때문에 가장 뒤의 렌더타겟인
		// Color버퍼는 DXGI_FORMAT_R8G8B8A8_UNORM로 처리해야함
		if (i == static_cast<int>(eBUFFERTYPE::GBUFFER_COUNT) - 1)
		{
			renderTagetViewDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		}

		HRESULT result = pDeivce->GetDevice()->CreateRenderTargetView(_renderTargetTextureArray[i], &renderTagetViewDesc, &_renderTargetViewArray[i]);
	}
}

void ArkEngine::ArkDX11::deferredBuffer::CreateShaderResourceView(ArkEngine::ArkDX11::ArkDevice* pDeivce)
{
	D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc;

	shaderResourceViewDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	shaderResourceViewDesc.Texture2D.MipLevels = 1;
	shaderResourceViewDesc.Texture2D.MostDetailedMip = 0;

	for (int i = 0; i < static_cast<int>(eBUFFERTYPE::GBUFFER_COUNT); i++)
	{
		// Picking을 위한 해쉬 ID 생성 로직때문에 가장 뒤의 렌더타겟인
		// Color버퍼는 DXGI_FORMAT_R8G8B8A8_UNORM로 처리해야함
		if (i == static_cast<int>(eBUFFERTYPE::GBUFFER_COUNT) - 1)
		{
			shaderResourceViewDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		}

		HRESULT result = pDeivce->GetDevice()->CreateShaderResourceView(_renderTargetTextureArray[i], &shaderResourceViewDesc, &_shaderResourceViewArray[i]);
	}
}