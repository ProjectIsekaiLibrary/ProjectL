#include <d3d11.h>
#include <DirectXMath.h>
#include "GeometryGenerator.h"
#include "ArkDevice.h"
#include "d3dx11effect.h"
#include "deferredBuffer.h"
#include "CommonStruct.h"
#include "Camera.h"
#include "ArkEffect.h"
#include "ArkBuffer.h"
#include "ResourceManager.h"
#include "LightManager.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "ShadowBuffer.h"
#include "deferredRenderer.h"
#include "DeferredBuffer.h"
#include "ShadowMap.h"


ArkEngine::ArkDX11::DeferredRenderer::DeferredRenderer(int clientWidth, int clientHeight)
	: _tech(nullptr), _fxDirLightCount(nullptr),_fxPointLightCount(nullptr),
	_fxDirLights(nullptr), _fxPointLights(nullptr), _fxEyePosW(nullptr), _fxLightView(nullptr), _fxLightProj(nullptr),
	_positionMap(nullptr), _normalMap(nullptr), _diffuseMap(nullptr), _emissionMap(nullptr),
	_materialMap(nullptr), _additionalMap(nullptr),
	_shadowDepthMap(nullptr),
	_deferredBuffer(nullptr), _shadowBuffer(nullptr),
	_eyePosW(), _arkDevice(nullptr), _arkEffect(nullptr), _arkBuffer(nullptr),
	_shadowWidth(clientWidth), _shadowHeight(clientHeight)
{
	for (int i = 0; i < 4; i++)
	{
		_backGroundColor[i] = 1.0f;
	}

	_shadowBuffer = new ShadowBuffer(_shadowWidth, _shadowHeight);
	_deferredBuffer = new deferredBuffer(clientWidth, clientHeight);

	Initailize();
}

ArkEngine::ArkDX11::DeferredRenderer::DeferredRenderer(int clientWidth, int clientHeight, int shadowWidth, int shadowHeight)
	: _tech(nullptr), _fxDirLightCount(nullptr), _fxPointLightCount(nullptr),
	_fxDirLights(nullptr), _fxPointLights(nullptr), _fxEyePosW(nullptr), _fxLightView(nullptr), _fxLightProj(nullptr),
	_positionMap(nullptr), _normalMap(nullptr), _diffuseMap(nullptr), _emissionMap(nullptr),
	_materialMap(nullptr), _additionalMap(nullptr),
	_shadowDepthMap(nullptr),
	_deferredBuffer(nullptr), _shadowBuffer(nullptr),
	_eyePosW(), _arkDevice(nullptr), _arkEffect(nullptr), _arkBuffer(nullptr),
	_shadowWidth(shadowWidth), _shadowHeight(shadowHeight)
{
	for (int i = 0; i < 4; i++)
	{
		_backGroundColor[i] = 1.0f;
	}

	_shadowBuffer = new ShadowBuffer(_shadowWidth, _shadowHeight);
	_deferredBuffer = new deferredBuffer(clientWidth, clientHeight);

	Initailize();
}

ArkEngine::ArkDX11::DeferredRenderer::~DeferredRenderer()
{
	
}

void ArkEngine::ArkDX11::DeferredRenderer::Initailize()
{
	_arkDevice = ResourceManager::GetInstance()->GetResource<ArkEngine::ArkDX11::ArkDevice>("Device");
	
	SetEffect();

	BuildQuadBuffers();
}

void ArkEngine::ArkDX11::DeferredRenderer::BeginRenderShadowDepth()
{
	_shadowBuffer->SetDepthStencilView();
}

void ArkEngine::ArkDX11::DeferredRenderer::BeginShadowRender()
{
	_shadowBuffer->SetDepthStencilView();
}

void ArkEngine::ArkDX11::DeferredRenderer::BeginRender()
{
	// Ãß°¡ 
	_deferredBuffer->SetRenderTargets();
	
	_deferredBuffer->ClearRenderTargets(_backGroundColor);
}

void ArkEngine::ArkDX11::DeferredRenderer::Update(ArkEngine::ICamera* pCamera)
{
	auto camera = static_cast<ArkEngine::ArkDX11::Camera*>(pCamera);

	_eyePosW = DirectX::XMFLOAT3(camera->GetCameraPos().x, camera->GetCameraPos().y, camera->GetCameraPos().z);

}

void ArkEngine::ArkDX11::DeferredRenderer::Render()
{
	auto deviceContext = _arkDevice->GetDeviceContext();

	deviceContext->IASetInputLayout(_arkEffect->GetInputLayOut());
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	deviceContext->RSSetState(_arkDevice->GetSolidRS());
	
	SetDirLight();

	SetPointLight();

	_fxEyePosW->SetRawValue(&_eyePosW, 0, sizeof(DirectX::XMFLOAT3));

	auto lightIndexList = LightManager::GetInstance()->GetActiveIndexList();

	if (lightIndexList.empty() == false)
	{
		auto lightView = ResourceManager::GetInstance()->GetShadowCamera()[0]->GetViewMatrix();
		auto lightProj = ResourceManager::GetInstance()->GetShadowCamera()[0]->GetProjMatrix();
		
		_fxLightView->SetMatrix(reinterpret_cast<float*>(&lightView));
		_fxLightProj->SetMatrix(reinterpret_cast<float*>(&lightProj));

		_shadowDepthMap->SetResource(_shadowBuffer->GetDepthSRV());
	}
	else
	{
		_shadowDepthMap->SetResource(nullptr);
	}

	_positionMap->SetResource(_deferredBuffer->GetSRV(static_cast<int>(eBUFFERTYPE::GBUFFER_POSITION)));
	_diffuseMap->SetResource(_deferredBuffer->GetSRV(static_cast<int>(eBUFFERTYPE::GBUFFER_DIFFUSE)));
	_normalMap->SetResource(_deferredBuffer->GetSRV(static_cast<int>(eBUFFERTYPE::GBUFFER_NORMALMAP)));
	_emissionMap->SetResource(_deferredBuffer->GetSRV(static_cast<int>(eBUFFERTYPE::GBUFFER_EMISSIONMAP)));
	_materialMap->SetResource(_deferredBuffer->GetSRV(static_cast<int>(eBUFFERTYPE::GBUFFER_MATERIAL)));
	_additionalMap->SetResource(_deferredBuffer->GetSRV(static_cast<int>(eBUFFERTYPE::GBUFFER_ADDITIONALINFO)));

	D3DX11_TECHNIQUE_DESC techDesc;
	_tech->GetDesc(&techDesc);

	UINT stride = sizeof(ArkEngine::ArkDX11::Postex);
	UINT offset = 0;

	auto vertexBuffer = _arkBuffer->GetVertexBuffer();
	deviceContext->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
	deviceContext->IASetIndexBuffer(_arkBuffer->GetIndexBuffer(), DXGI_FORMAT_R32_UINT, 0);

	_tech->GetPassByIndex(0)->Apply(0, deviceContext);
	deviceContext->DrawIndexed(6, 0, 0);
}

void ArkEngine::ArkDX11::DeferredRenderer::Finalize()
{
	_arkBuffer = nullptr;
	_arkEffect = nullptr;
	_arkDevice = nullptr;
	
	_shadowDepthMap = nullptr;
	_additionalMap = nullptr;
	_materialMap = nullptr;
	_emissionMap = nullptr;
	_diffuseMap = nullptr;
	_normalMap = nullptr;
	_positionMap = nullptr;
	_tech = nullptr;

	_fxPointLightCount->Release();
	_fxPointLights->Release();

	_fxDirLightCount->Release();
	_fxDirLights->Release();

	_fxEyePosW->Release();
	_fxLightView->Release();
	_fxLightProj->Release();

	delete _deferredBuffer;

	delete _shadowBuffer;
}

void ArkEngine::ArkDX11::DeferredRenderer::SetEffect()
{
	_arkEffect = ResourceManager::GetInstance()->GetResource<ArkEngine::ArkDX11::ArkEffect>("Resources/FX/finalDeferredToon.fx");
	auto effect = _arkEffect->GetEffect();

	_tech = effect->GetTechniqueByIndex(0);

	_fxDirLights = effect->GetVariableByName("gDirLights");
	_fxDirLightCount = effect->GetVariableByName("gDirLightCount")->AsScalar();

	_fxPointLights = effect->GetVariableByName("gPointLights");
	_fxPointLightCount = effect->GetVariableByName("gPointLightCount")->AsScalar();
	
	_fxEyePosW = effect->GetVariableByName("gEyePosW")->AsVector();

	_fxLightView = effect->GetVariableByName("gLightView")->AsMatrix();
	_fxLightProj = effect->GetVariableByName("gLightProj")->AsMatrix();

	_positionMap = effect->GetVariableByName("PositionTexture")->AsShaderResource();
	_normalMap = effect->GetVariableByName("BumpedNormalTexture")->AsShaderResource();
	_diffuseMap = effect->GetVariableByName("DiffuseAlbedoTexture")->AsShaderResource();
	_emissionMap = effect->GetVariableByName("EmissiveTexture")->AsShaderResource();
	_materialMap = effect->GetVariableByName("MaterialTexture")->AsShaderResource();
	_additionalMap = effect->GetVariableByName("AdditionalTexture")->AsShaderResource();
	_shadowDepthMap = effect->GetVariableByName("gShadowDepthMapTexture")->AsShaderResource();
}

void ArkEngine::ArkDX11::DeferredRenderer::BuildQuadBuffers()
{
	GeometryGenerator generator;

	generator.CreateQuad();

	_arkBuffer = ResourceManager::GetInstance()->GetResource<ArkBuffer>("Quad");
}

void ArkEngine::ArkDX11::DeferredRenderer::SetDirLight()
{
	auto dirLightList = LightManager::GetInstance()->GetActiveDirLightList();

	if (dirLightList.size() > 0)
	{
		size_t dataSize = dirLightList.size() * sizeof(ArkEngine::ArkDX11::DirectionalLight);

		if (dataSize <= UINT32_MAX)
		{
			uint32_t dataSize32 = static_cast<uint32_t>(dataSize);

			_fxDirLights->SetRawValue(dirLightList.data(), 0, dataSize32);
		}

		size_t size = dirLightList.size();

		if (size <= INT_MAX)
		{
			int intSize = static_cast<int>(size);
			_fxDirLightCount->SetInt(intSize);
		}
	}
	else
	{
		_fxDirLightCount->SetInt(0);
	}
}

void ArkEngine::ArkDX11::DeferredRenderer::SetPointLight()
{
	auto pointLightList = LightManager::GetInstance()->GetActivePointLightList();

	if (pointLightList.size() > 0)
	{
		size_t dataSize = pointLightList.size() * sizeof(ArkEngine::ArkDX11::PointLight);

		if (dataSize <= UINT32_MAX)
		{
			uint32_t dataSize32 = static_cast<uint32_t>(dataSize);

 			_fxPointLights->SetRawValue(pointLightList.data(), 0, dataSize32);
		}

		size_t size = pointLightList.size();

		if (size <= INT_MAX)
		{
			int intSize = static_cast<int>(size);
			_fxPointLightCount->SetInt(intSize);
		}
	}
	else
	{
		_fxPointLightCount->SetInt(0);
	}
}

ArkEngine::ArkDX11::deferredBuffer* ArkEngine::ArkDX11::DeferredRenderer::GetDeferredBuffer()
{
	return _deferredBuffer;
}

ArkEngine::ArkDX11::ShadowBuffer* ArkEngine::ArkDX11::DeferredRenderer::GetShadowBuffer()
{
	return _shadowBuffer;
}

int ArkEngine::ArkDX11::DeferredRenderer::GetShadowMapWidth()
{
	return _shadowWidth;
}

int ArkEngine::ArkDX11::DeferredRenderer::GetShadowMapHeight()
{
	return _shadowHeight;
}
