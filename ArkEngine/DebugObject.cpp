#include <DirectXMath.h>
#include "GeometryGenerator.h"
#include "ArkBuffer.h"
#include "CommonStruct.h"
#include "ResourceManager.h"
#include "Transform.h"
#include "ArkEffect.h"
#include "ArkDevice.h"
#include "ICamera.h"
#include "Camera.h"
#include "d3dx11effect.h"
#include "DebugObject.h"

ArkEngine::ArkDX11::DebugObject::DebugObject(const std::string& objectName, eDebugType type)
	: _objectName(objectName), _effectName("Resources/FX/color.fx"), _effect(nullptr), _tech(nullptr), _fxWorldViewProj(nullptr),
	_world(), _view(), _proj(), _vertexBuffer(nullptr), _indexBuffer(nullptr), _arkDevice(nullptr), _arkEffect(nullptr), 
	_totalIndexCount(0), _isRendering(true), _meshTransform(nullptr), _width(0.0f), _height(0.0f), _depth(0.0f), _minPos(0.0f), _type(type)
{
	auto buffer = ResourceManager::GetInstance()->GetResource<ArkBuffer>(_objectName);
	auto maxPos = buffer->GetMaxPos();
	auto minPos = buffer->GetMinPos();

	_width = maxPos.x - minPos.x;
	_height = maxPos.y - minPos.y;
	_depth = maxPos.z - minPos.z;
	_minPos = minPos.y;

	_objectName += "/Debug";

	Initialize();
}

ArkEngine::ArkDX11::DebugObject::DebugObject(const std::string& objectName, eDebugType type, float centerPosY, float range)
	: _objectName(objectName), _effectName("Resources/FX/color.fx"), _effect(nullptr), _tech(nullptr), _fxWorldViewProj(nullptr),
	_world(), _view(), _proj(), _vertexBuffer(nullptr), _indexBuffer(nullptr), _arkDevice(nullptr), _arkEffect(nullptr),
	_totalIndexCount(0), _isRendering(true), _meshTransform(nullptr), _width(0.0f), _height(0.0f), _depth(0.0f), _minPos(0.0f), _type(type)
{
	_width = range;
	_height = range;
	_depth = range;
	_minPos = centerPosY - (range/2);

	_objectName += "/Debug";

	Initialize();
}

ArkEngine::ArkDX11::DebugObject::~DebugObject()
{
	Finalize();
}

void ArkEngine::ArkDX11::DebugObject::Initialize()
{
	_arkEffect = ResourceManager::GetInstance()->GetResource<ArkEngine::ArkDX11::ArkEffect>(_effectName);
	_effect = _arkEffect->GetEffect();

	_arkDevice = ResourceManager::GetInstance()->GetResource<ArkEngine::ArkDX11::ArkDevice>("Device");

	BuildGeomtryBuffers();
	SetEffect();

	ResourceManager::GetInstance()->AddDebugObject(this);

	_meshTransform = new Transform();
}

void ArkEngine::ArkDX11::DebugObject::Update(ArkEngine::ICamera* p_Camera)
{
	auto camera = static_cast<ArkEngine::ArkDX11::Camera*>(p_Camera);

	DirectX::XMStoreFloat4x4(&_world, DirectX::XMMatrixIdentity());
	DirectX::XMStoreFloat4x4(&_view, camera->GetViewMatrix());
	DirectX::XMStoreFloat4x4(&_proj, camera->GetProjMatrix());
}

void ArkEngine::ArkDX11::DebugObject::Render()
{
	auto deviceContext = _arkDevice->GetDeviceContext();

	deviceContext->IASetInputLayout(_arkEffect->GetInputLayOut());
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	deviceContext->RSSetState(_arkDevice->GetWireRS());

	UINT stride = sizeof(ArkEngine::ArkDX11::PosColor);
	UINT offset = 0;

	deviceContext->IASetVertexBuffers(0, 1, &_vertexBuffer, &stride, &offset);
	deviceContext->IASetIndexBuffer(_indexBuffer, DXGI_FORMAT_R32_UINT, 0);

	D3DX11_TECHNIQUE_DESC techDesc;
	_tech->GetDesc(&techDesc);

	for (UINT p = 0; p < techDesc.Passes; ++p)
	{
		DirectX::XMMATRIX world = XMLoadFloat4x4(&_world);
		world = XMMatrixMultiply(world, _meshTransform->GetTransformMatrix());

		DirectX::XMMATRIX view = XMLoadFloat4x4(&_view);
		DirectX::XMMATRIX proj = XMLoadFloat4x4(&_proj);
		DirectX::XMMATRIX WorldViewProj = world * view * proj;
		_fxWorldViewProj->SetMatrix(reinterpret_cast<float*>(&WorldViewProj));

		_tech->GetPassByIndex(p)->Apply(0, deviceContext);
		deviceContext->DrawIndexed(_totalIndexCount, 0, 0);
	}
}

void ArkEngine::ArkDX11::DebugObject::Finalize()
{
	ResourceManager::GetInstance()->DeleteDebugObject(this);

	delete _meshTransform;

	_arkEffect = nullptr;
	_arkDevice = nullptr;

	_indexBuffer = nullptr;
	_vertexBuffer = nullptr;

	_fxWorldViewProj->Release();

	_tech = nullptr;
	_effect = nullptr;
}

bool ArkEngine::ArkDX11::DebugObject::GetRenderingState()
{
	return _isRendering;
}

void ArkEngine::ArkDX11::DebugObject::SetRenderingState(bool tf)
{
	_isRendering = tf;
}

void ArkEngine::ArkDX11::DebugObject::SetTransformMatrix(DirectX::XMFLOAT4X4 matrix)
{
	_meshTransform->SetTransformMatrix(matrix);
}

void ArkEngine::ArkDX11::DebugObject::SetPosition(float x, float y, float z)
{
	_meshTransform->SetTranslationMatrix(x, y, z);
}

void ArkEngine::ArkDX11::DebugObject::SetRotation(float x, float y, float z)
{
	_meshTransform->SetRotationMatrix(x, y, z);
}

void ArkEngine::ArkDX11::DebugObject::SetScale(float x, float y, float z)
{
	_meshTransform->SetScaleMatrix(x, y, z);
}

void ArkEngine::ArkDX11::DebugObject::BuildGeomtryBuffers()
{
	auto buffer = ResourceManager::GetInstance()->GetResource<ArkBuffer>(_objectName);

	if (buffer == nullptr)
	{
		GeometryGenerator geomGen;

		if (_type == eDebugType::Box)
		{
			geomGen.CreateDebugBox(_objectName.c_str(), _minPos, _width, _height, _depth, DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f));
		}
		else if (_type == eDebugType::Shpere)
		{
			geomGen.CreateDebugSphere(_objectName.c_str(), _minPos, _width, DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f));
		}

		buffer = ResourceManager::GetInstance()->GetResource<ArkBuffer>(_objectName);
	}

	_vertexBuffer = buffer->GetVertexBuffer();
	_indexBuffer = buffer->GetIndexBuffer();

	_totalIndexCount = buffer->GetTotalIndexCount();
}

void ArkEngine::ArkDX11::DebugObject::SetEffect()
{
	_tech = _effect->GetTechniqueByIndex(0);

	_fxWorldViewProj = _effect->GetVariableByName("gWorldViewProj")->AsMatrix();
}