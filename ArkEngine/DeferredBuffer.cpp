#include <d3d11.h>
#include "ResourceManager.h"
#include "ArkDevice.h"
#include "deferredBuffer.h"

ArkEngine::ArkDX11::deferredBuffer::deferredBuffer(int width, int height)
	:_arkDevice(nullptr), _textureWidth(width), _textureHeight(height)
{
	Initialize();
}

ArkEngine::ArkDX11::deferredBuffer::~deferredBuffer()
{
	Finalize();
}

void ArkEngine::ArkDX11::deferredBuffer::Initialize()
{
	_arkDevice = ResourceManager::GetInstance()->GetResource<ArkEngine::ArkDX11::ArkDevice>("Device");

	for (int i = 0; i < static_cast<int>(eBUFFERTYPE::GBUFFER_COUNT); i++)
	{
		_renderTargetTextureArray[i] = nullptr;
		_renderTargetViewArray[i] = nullptr;
		_shaderResourceViewArray[i] = nullptr;
	}

	CreateRenderTargetTexture();
	CreateRenderTargetView();
	CreateShaderResourceView();

	for (int i = 0; i < static_cast<int>(eBLOOM::BLOOM_COUNT); i++)
	{
		_renderTargetTextureArrayForFinal[i] = nullptr;
		_renderTargetViewArrayForFinal[i] = nullptr;
		_shaderResourceViewArrayForFinal[i] = nullptr;
	}

	CreateRenderTargetTextureForFinal();
	CreateRenderTargetViewForFinal();
	CreateShaderResourceViewForFinal();

	for (int i = 0; i < static_cast<int>(eBLOOM::BLOOM_COUNT); i++)
	{
		_renderTargetTextureArrayForBloom[i] = nullptr;
		_renderTargetViewArrayForBloom[i] = nullptr;
		_shaderResourceViewArrayForBloom[i] = nullptr;
	}

	CreateRenderTargetTextureForBloom();
	CreateRenderTargetViewForBloom();
	CreateShaderResourceViewForBloom();
}

void ArkEngine::ArkDX11::deferredBuffer::Finalize()
{
	for (int i = 0; i < static_cast<int>(eBUFFERTYPE::GBUFFER_COUNT); i++)
	{
		_renderTargetTextureArray[i]->Release();
		_renderTargetViewArray[i]->Release();
		_shaderResourceViewArray[i]->Release();
	}

	for (int i = 0; i < static_cast<int>(eBLOOM::BLOOM_COUNT); i++)
	{
		_renderTargetTextureArrayForBloom[i]->Release();
		_renderTargetViewArrayForBloom[i]->Release();
		_shaderResourceViewArrayForBloom[i]->Release();
	}
}

void ArkEngine::ArkDX11::deferredBuffer::SetRenderTargets()
{
	_arkDevice->GetDeviceContext()->OMSetRenderTargets(static_cast<int>(eBUFFERTYPE::GBUFFER_COUNT), _renderTargetViewArray, _arkDevice->GetDepthStencilView());
}

void ArkEngine::ArkDX11::deferredBuffer::SetRenderTargets(ID3D11DepthStencilView* dsv)
{
	_arkDevice->GetDeviceContext()->OMSetRenderTargets(static_cast<int>(eBUFFERTYPE::GBUFFER_COUNT), _renderTargetViewArray, dsv);
}

void ArkEngine::ArkDX11::deferredBuffer::ClearRenderTargets(float color[4])
{
	for (int i = 0; i < static_cast<int>(eBUFFERTYPE::GBUFFER_COUNT); i++)
	{
		_arkDevice->GetDeviceContext()->ClearRenderTargetView(_renderTargetViewArray[i], color);
	}
}


void ArkEngine::ArkDX11::deferredBuffer::SetRenderTargetsForFinal()
{
	_arkDevice->GetDeviceContext()->OMSetRenderTargets(static_cast<int>(eBLOOM::BLOOM_COUNT), _renderTargetViewArrayForFinal, _arkDevice->GetDepthStencilView());
}


void ArkEngine::ArkDX11::deferredBuffer::ClearRenderTargetsForFinal(float color[4])
{
	for (int i = 0; i < static_cast<int>(eBLOOM::BLOOM_COUNT); i++)
	{
		_arkDevice->GetDeviceContext()->ClearRenderTargetView(_renderTargetViewArrayForFinal[i], color);
	}
}

void ArkEngine::ArkDX11::deferredBuffer::ClearRenderTargets(int index, float color[4])
{
	_arkDevice->GetDeviceContext()->ClearRenderTargetView(_renderTargetViewArray[index], color);
}

void ArkEngine::ArkDX11::deferredBuffer::SetRenderTargetsForBloom()
{
	_arkDevice->GetDeviceContext()->OMSetRenderTargets(static_cast<int>(eBLOOM::BLOOM_COUNT), _renderTargetViewArrayForBloom, _arkDevice->GetDepthStencilView());
}

void ArkEngine::ArkDX11::deferredBuffer::ClearRenderTargetsForBloom(float color[4])
{
	for (int i = 0; i < static_cast<int>(eBLOOM::BLOOM_COUNT); i++)
	{
		_arkDevice->GetDeviceContext()->ClearRenderTargetView(_renderTargetViewArrayForBloom[i], color);
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

ID3D11RenderTargetView* ArkEngine::ArkDX11::deferredBuffer::GetRenderTargetView(int index)
{
	return _renderTargetViewArray[index];
}


ID3D11ShaderResourceView* ArkEngine::ArkDX11::deferredBuffer::GetSRVForFinal(int index)
{
	return _shaderResourceViewArrayForFinal[index];
}


ID3D11Texture2D* ArkEngine::ArkDX11::deferredBuffer::GetTextrueForFinal(int index)
{
	return _renderTargetTextureArrayForFinal[index];
}


ID3D11RenderTargetView* ArkEngine::ArkDX11::deferredBuffer::GetRenderTargetViewForFinal(int index)
{
	return _renderTargetViewArrayForFinal[index];
}

ID3D11ShaderResourceView* ArkEngine::ArkDX11::deferredBuffer::GetSRVForBloom(int index)
{
	return _shaderResourceViewArrayForBloom[index];
}


ID3D11Texture2D* ArkEngine::ArkDX11::deferredBuffer::GetTextrueForBloom(int index)
{
	return _renderTargetTextureArrayForBloom[index];
}


ID3D11RenderTargetView* ArkEngine::ArkDX11::deferredBuffer::GetRenderTargetViewForBloom(int index)
{
	return _renderTargetViewArrayForBloom[index];
}

void ArkEngine::ArkDX11::deferredBuffer::CreateRenderTargetTexture()
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
		if (i == static_cast<int>(eBUFFERTYPE::GBUFFER_COLOR))
		{
			textureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		}

		HRESULT result = _arkDevice->GetDevice()->CreateTexture2D(&textureDesc, NULL, &_renderTargetTextureArray[i]);
	}
}

void ArkEngine::ArkDX11::deferredBuffer::CreateRenderTargetView()
{
	D3D11_RENDER_TARGET_VIEW_DESC renderTagetViewDesc;

	renderTagetViewDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	renderTagetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	renderTagetViewDesc.Texture2D.MipSlice = 0;

	for (int i = 0; i < static_cast<int>(eBUFFERTYPE::GBUFFER_COUNT); i++)
	{
		// Picking을 위한 해쉬 ID 생성 로직때문에 가장 뒤의 렌더타겟인
		// Color버퍼는 DXGI_FORMAT_R8G8B8A8_UNORM로 처리해야함
		if (i == static_cast<int>(eBUFFERTYPE::GBUFFER_COLOR))
		{
			renderTagetViewDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		}

		HRESULT result = _arkDevice->GetDevice()->CreateRenderTargetView(_renderTargetTextureArray[i], &renderTagetViewDesc, &_renderTargetViewArray[i]);
	}
}

void ArkEngine::ArkDX11::deferredBuffer::CreateShaderResourceView()
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
		if (i == static_cast<int>(eBUFFERTYPE::GBUFFER_COLOR))
		{
			shaderResourceViewDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		}

		HRESULT result = _arkDevice->GetDevice()->CreateShaderResourceView(_renderTargetTextureArray[i], &shaderResourceViewDesc, &_shaderResourceViewArray[i]);
	}
}


void ArkEngine::ArkDX11::deferredBuffer::CreateRenderTargetTextureForFinal()
{
	D3D11_TEXTURE2D_DESC textureDesc;

	textureDesc.Height = _textureHeight;
	textureDesc.Width = _textureWidth;
	textureDesc.MipLevels = 1;
	textureDesc.ArraySize = 1;
	textureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.SampleDesc.Quality = 0;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	textureDesc.CPUAccessFlags = 0;
	textureDesc.MiscFlags = 0;

	HRESULT result = _arkDevice->GetDevice()->CreateTexture2D(&textureDesc, NULL, &_renderTargetTextureArrayForFinal[0]);
}


void ArkEngine::ArkDX11::deferredBuffer::CreateRenderTargetViewForFinal()
{
	D3D11_RENDER_TARGET_VIEW_DESC renderTagetViewDesc;

	renderTagetViewDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	renderTagetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	renderTagetViewDesc.Texture2D.MipSlice = 0;

	HRESULT result = _arkDevice->GetDevice()->CreateRenderTargetView(_renderTargetTextureArrayForFinal[0], &renderTagetViewDesc, &_renderTargetViewArrayForFinal[0]);
}


void ArkEngine::ArkDX11::deferredBuffer::CreateShaderResourceViewForFinal()
{
	D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc;

	shaderResourceViewDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	shaderResourceViewDesc.Texture2D.MipLevels = 1;
	shaderResourceViewDesc.Texture2D.MostDetailedMip = 0;

	HRESULT result = _arkDevice->GetDevice()->CreateShaderResourceView(_renderTargetTextureArrayForFinal[0], &shaderResourceViewDesc, &_shaderResourceViewArrayForFinal[0]);
}

void ArkEngine::ArkDX11::deferredBuffer::CreateRenderTargetTextureForBloom()
{
	D3D11_TEXTURE2D_DESC textureDesc;

	textureDesc.Height = _textureHeight * 0.25f;
	textureDesc.Width = _textureWidth * 0.25f;
	textureDesc.MipLevels = 1;
	textureDesc.ArraySize = 1;
	textureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.SampleDesc.Quality = 0;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	textureDesc.CPUAccessFlags = 0;
	textureDesc.MiscFlags = 0;

	HRESULT result = _arkDevice->GetDevice()->CreateTexture2D(&textureDesc, NULL, &_renderTargetTextureArrayForBloom[0]);
}

void ArkEngine::ArkDX11::deferredBuffer::CreateRenderTargetViewForBloom()
{
	D3D11_RENDER_TARGET_VIEW_DESC renderTagetViewDesc;

	renderTagetViewDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	renderTagetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	renderTagetViewDesc.Texture2D.MipSlice = 0;

	HRESULT result = _arkDevice->GetDevice()->CreateRenderTargetView(_renderTargetTextureArrayForBloom[0], &renderTagetViewDesc, &_renderTargetViewArrayForBloom[0]);
}

void ArkEngine::ArkDX11::deferredBuffer::CreateShaderResourceViewForBloom()
{
	D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc;

	shaderResourceViewDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	shaderResourceViewDesc.Texture2D.MipLevels = 1;
	shaderResourceViewDesc.Texture2D.MostDetailedMip = 0;

	HRESULT result = _arkDevice->GetDevice()->CreateShaderResourceView(_renderTargetTextureArrayForBloom[0], &shaderResourceViewDesc, &_shaderResourceViewArrayForBloom[0]);
}
