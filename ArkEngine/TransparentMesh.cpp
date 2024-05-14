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
	_test(0.0f)
{
	Initialize();
}

ArkEngine::ArkDX11::TransparentMesh::~TransparentMesh()
{

}

void ArkEngine::ArkDX11::TransparentMesh::Initialize()
{
	_arkEffect = ResourceManager::GetInstance()->GetResource<ArkEngine::ArkDX11::ArkEffect>("Resources/FX/TransparentObject.fx");
	_effect = _arkEffect->GetEffect();

	_arkDevice = ResourceManager::GetInstance()->GetResource<ArkEngine::ArkDX11::ArkDevice>("Device");

	BuildGeomtryBuffers();
	SetEffect();
	SetTexture("Resources/Textures/bricks.dds");

	_meshTransform = new Transform();

	_meshTransform->SetTranslationMatrix(0.0f, 0.1f, 0.0f);

	_meshTransform->SetScaleMatrix(10.0f, 10.0f, 10.0f);

	if (! _isCircle)
	{
		_meshTransform->SetRotationMatrix(90.0f);
	}

	ResourceManager::GetInstance()->AddTransParentMesh(_objectName, this);
}

void ArkEngine::ArkDX11::TransparentMesh::Update(ArkEngine::ICamera* p_Camera)
{
	DirectX::XMStoreFloat4x4(&_world, _meshTransform->GetTransformMatrix());
	DirectX::XMStoreFloat4x4(&_view, p_Camera->GetViewMatrix());
	DirectX::XMStoreFloat4x4(&_proj, p_Camera->GetProjMatrix());

	if (GetAsyncKeyState('Y'))
	{
		_test = 0.0f;
	}
}

void ArkEngine::ArkDX11::TransparentMesh::Render()
{
	//if (_test < 1)
	{
		_test += 0.001f;
	}

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

	_fxTest->SetFloat(_test);

	_tech->GetPassByIndex(2)->Apply(0, deviceContext);
	deviceContext->DrawIndexed(_totalIndexCount, 0, 0);
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

	_tech = nullptr;
	_effect = nullptr;
}

const std::string& ArkEngine::ArkDX11::TransparentMesh::GetName()
{
	return _objectName;
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

	_fxTest = _effect->GetVariableByName("gTest")->AsScalar();
}

void ArkEngine::ArkDX11::TransparentMesh::SetTexture(const std::string& name)
{
	auto texture = ResourceManager::GetInstance()->GetResource<ArkTexture>(name);

	if (texture != nullptr)
	{
		_diffuseMapSRV = texture->GetDiffuseMapSRV();
	}
	else
	{
		ArkTexture* newTexture = new ArkTexture(name.c_str());
		_diffuseMapSRV = newTexture->GetDiffuseMapSRV();
	}
}
