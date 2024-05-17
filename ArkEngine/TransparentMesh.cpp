#include "ResourceManager.h"
#include "ArkEffect.h"
#include "ArkDevice.h"
#include "ArkBuffer.h"
#include "ArkTexture.h"
#include "d3dx11effect.h"
#include "ICamera.h"
#include "Transform.h"
#include "GeometryGenerator.h"
#include "CommonStruct.h"
#include "TransparentMesh.h"

ArkEngine::ArkDX11::TransparentMesh::TransparentMesh(const std::string& objectName, const std::string& textureName, float transParency, bool isCircle)
	: _objectName(objectName), _textureName(textureName), _transParency(transParency),
	_arkEffect(nullptr), _effect(nullptr), _arkDevice(nullptr),
	_meshTransform(nullptr), _vertexBuffer(nullptr), _indexBuffer(nullptr), _totalIndexCount(0),
	_tech(nullptr), _fxWorld(nullptr), _fxWorldViewProj(nullptr), _world(), _view(), _proj(),
	_fxTransParency(nullptr), _isCircle(isCircle),
	_timer(0.0f), _renderType(0),
	_renderTime(0.0f), _isRenderFinsh(false)
{
	Initialize();
}

ArkEngine::ArkDX11::TransparentMesh::~TransparentMesh()
{
	Finalize();
}

void ArkEngine::ArkDX11::TransparentMesh::Initialize()
{
	_arkEffect = ResourceManager::GetInstance()->GetResource<ArkEngine::ArkDX11::ArkEffect>("Resources/FX/TransparentObject.fx");
	_effect = _arkEffect->GetEffect();

	_arkDevice = ResourceManager::GetInstance()->GetResource<ArkEngine::ArkDX11::ArkDevice>("Device");

	BuildGeomtryBuffers();
	SetEffect();
	SetTexture(_textureName);

	_meshTransform = new Transform();

	ResourceManager::GetInstance()->AddTransParentMesh(this);
}

void ArkEngine::ArkDX11::TransparentMesh::Update(ArkEngine::ICamera* p_Camera)
{
	if (!_isRenderFinsh)
	{
		DirectX::XMStoreFloat4x4(&_world, _meshTransform->GetTransformMatrix());
		DirectX::XMStoreFloat4x4(&_view, p_Camera->GetViewMatrix());
		DirectX::XMStoreFloat4x4(&_proj, p_Camera->GetProjMatrix());
	}
}

void ArkEngine::ArkDX11::TransparentMesh::Render()
{
	if (!_isRenderFinsh)
	{
		auto deviceContext = _arkDevice->GetDeviceContext();

		deviceContext->IASetInputLayout(_arkEffect->GetInputLayOut());
		deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		deviceContext->RSSetState(_arkDevice->GetSolidRS());

		UINT stride = sizeof(ArkEngine::ArkDX11::Postex);
		UINT offset = 0;

		deviceContext->IASetVertexBuffers(0, 1, &_vertexBuffer, &stride, &offset);
		deviceContext->IASetIndexBuffer(_indexBuffer, DXGI_FORMAT_R32_UINT, 0);

		D3DX11_TECHNIQUE_DESC techDesc;
		_tech->GetDesc(&techDesc);


		DirectX::XMMATRIX world = XMLoadFloat4x4(&_world);

		DirectX::XMMATRIX view = XMLoadFloat4x4(&_view);
		DirectX::XMMATRIX proj = XMLoadFloat4x4(&_proj);
		DirectX::XMMATRIX WorldViewProj = world * view * proj;
		_fxWorldViewProj->SetMatrix(reinterpret_cast<float*>(&WorldViewProj));
		_texture->SetResource(_diffuseMapSRV);

		_fxTransParency->SetFloat(_transParency);

		_fxTime->SetFloat(_renderTime);

		_tech->GetPassByIndex(_renderType)->Apply(0, deviceContext);
		deviceContext->DrawIndexed(_totalIndexCount, 0, 0);
	}
}

void ArkEngine::ArkDX11::TransparentMesh::Reset()
{
	_timer = 0.0f;
	_renderTime = 0.0f;
	_isRenderFinsh = false;
}

void ArkEngine::ArkDX11::TransparentMesh::SetRenderType(unsigned int index)
{
	D3DX11_TECHNIQUE_DESC techDesc;
	_tech->GetDesc(&techDesc);

	if (index < techDesc.Passes)
	{
		_renderType = index;
	}
	else
	{
		_renderType = 0;
	}
}

void ArkEngine::ArkDX11::TransparentMesh::Finalize()
{
	delete _meshTransform;

	_arkEffect = nullptr;
	_arkDevice = nullptr;

	_indexBuffer = nullptr;
	_vertexBuffer = nullptr;

	_fxWorldViewProj->Release();
	_fxWorld->Release();

	_texture->Release();

	_fxTransParency->Release();

	_fxTime->Release();

	_tech = nullptr;
	_effect = nullptr;
}

void ArkEngine::ArkDX11::TransparentMesh::SetTransform(const DirectX::XMFLOAT4X4& matrix)
{
	_meshTransform->SetTransformMatrix(matrix);
}


void ArkEngine::ArkDX11::TransparentMesh::Delete()
{
	ResourceManager::GetInstance()->DeleteTransParentMesh(this);
}

void ArkEngine::ArkDX11::TransparentMesh::BuildGeomtryBuffers()
{
	ArkBuffer* buffer = nullptr;

	// 만들어진 버퍼가 있다면 가져옴
	if (_isCircle == true)
	{
		buffer = ResourceManager::GetInstance()->GetResource<ArkBuffer>("Circle");

		if (buffer == nullptr)
		{
			GeometryGenerator generator;

			generator.CreateCircle("Circle");

			buffer = ResourceManager::GetInstance()->GetResource<ArkBuffer>("Circle");
		}
	}
	else
	{
		buffer = ResourceManager::GetInstance()->GetResource<ArkBuffer>("Quad");

		if (buffer == nullptr)
		{
			GeometryGenerator generator;

			generator.CreateCircle("Quad");

			buffer = ResourceManager::GetInstance()->GetResource<ArkBuffer>("Quad");
		}
	}

	_vertexBuffer = buffer->GetVertexBuffer();
	_indexBuffer = buffer->GetIndexBuffer();

	_totalIndexCount = buffer->GetTotalIndexCount();
}

void ArkEngine::ArkDX11::TransparentMesh::SetEffect()
{
	_tech = _effect->GetTechniqueByIndex(0);
	_fxWorldViewProj = _effect->GetVariableByName("gWorldViewProj")->AsMatrix();

	_texture = _effect->GetVariableByName("gDiffuseMap")->AsShaderResource();

	_fxTransParency = _effect->GetVariableByName("gTransParency")->AsScalar();

	_fxTime = _effect->GetVariableByName("gTime")->AsScalar();
}

void ArkEngine::ArkDX11::TransparentMesh::SetTexture(const std::string& textureName)
{
	auto texture = ResourceManager::GetInstance()->GetResource<ArkTexture>(textureName);

	if (texture != nullptr)
	{
		_diffuseMapSRV = texture->GetDiffuseMapSRV();
	}
	else
	{
		ArkTexture* newTexture = new ArkTexture(textureName.c_str());
		_diffuseMapSRV = newTexture->GetDiffuseMapSRV();
	}
}

bool ArkEngine::ArkDX11::TransparentMesh::RenderWithTimer(float deltaTime, float timer)
{
	if (_isRenderFinsh)
	{
		return _isRenderFinsh;
	}

	if (_timer >= timer)
	{
		_isRenderFinsh = true;
	
		return _isRenderFinsh;
	}

	_timer += deltaTime;

	_renderTime = (_timer / timer) * 2;

	return _isRenderFinsh;
}
