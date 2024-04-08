#include <d3d11.h>
#include "ResourceManager.h"
#include "ArkDevice.h"
#include "Camera.h"
#include "ArkEffect.h"
#include "d3dx11effect.h"
#include "ParticleEffect.h"

#include "ParticleSystem.h"

ArkEngine::ArkDX11::ParticleSystem::ParticleSystem()
	:_maxParticles(0), _firstRun(false), 
	_gameTime(0), _timeStep(0), _age(0),
	_eyePosW(0), _emitPosW(0), _emitDirW(0),
	_initVB(nullptr), _drawVB(nullptr), _streamOutVB(nullptr),
	_texArraySRV(nullptr), _randomTexSRV(nullptr)
{
}

ArkEngine::ArkDX11::ParticleSystem::~ParticleSystem()
{
}

float ArkEngine::ArkDX11::ParticleSystem::GetAge() const
{
	return 0.0f;
}

void ArkEngine::ArkDX11::ParticleSystem::SetEyePos(const DirectX::XMFLOAT3& eyePosW)
{
}

void ArkEngine::ArkDX11::ParticleSystem::SetEmitPos(const DirectX::XMFLOAT3& emitPosW)
{
}

void ArkEngine::ArkDX11::ParticleSystem::SetEmitDir(const DirectX::XMFLOAT3& emitDirW)
{
}

void ArkEngine::ArkDX11::ParticleSystem::Initialize(ID3D11Device* device, ParticleEffect* fx, ID3D11ShaderResourceView* texArraySRV, ID3D11ShaderResourceView* randomTexSRV, unsigned int maxParticle)
{
}

void ArkEngine::ArkDX11::ParticleSystem::Reset()
{
}

void ArkEngine::ArkDX11::ParticleSystem::Update(float deltaTime, float gameTime)
{
}

void ArkEngine::ArkDX11::ParticleSystem::Draw(ID3D11DeviceContext* dc, ArkEngine::ICamera* p_Camera)
{
	dc = _arkDevice->GetDeviceContext();
	auto camera = static_cast<ArkEngine::ArkDX11::Camera*>(p_Camera);

	auto cameraView = camera->GetViewMatrix();
	auto cameraProj = camera->GetProjMatrix();

	DirectX::XMMATRIX VP = cameraView * cameraProj;

	// 상수들을 설정한다(일단 보류)

	// 입력 조립기 단계를 설정
	dc->IASetInputLayout(_arkEffect->GetInputLayOut());
	dc->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);
	unsigned int stride = sizeof(Particle);
	unsigned int offset = 0;

	// 최초 실행이면 초기화용 정점 버퍼를 사용하고, 그러지 않다면
	// 현재의 입자 목록을 담은 정점 버퍼를 사용한다
	if (_firstRun)
	{
		dc->IASetVertexBuffers(0, 1, &_initVB, &stride, &offset);
	}
	else
	{
		dc->IASetVertexBuffers(0, 1, &_drawVB, &stride, &offset);
	}

	// 현재 입자 목록을 스트림 출력 전용 기법으로 그려서 입자들을 갱신한다
	// 갱신된 입자들은 스트림 출력을 통해서 대상 정점 버퍼에 기록된다
	dc->SOSetTargets(1, &_streamOutVB, &offset);

	D3DX11_TECHNIQUE_DESC techDesc;
	_fx->_streamOutTech->GetDesc(&techDesc);
	for (unsigned int p = 0; p < techDesc.Passes; ++p)
	{
		_fx->_streamOutTech->GetPassByIndex(p)->Apply(0, dc);

		if (_firstRun)
		{
			dc->Draw(1, 0);
			_firstRun = false;
		}
		else
		{
			dc->DrawAuto();
		}
	}

	ID3D11Buffer* bufferArray[1] = { 0 };
	dc->SOSetTargets(1, bufferArray, &offset);

	std::swap(_drawVB, _streamOutVB);

	dc->IASetVertexBuffers(0, 1, &_drawVB, &stride, &offset);

	_fx->_drawTech->GetDesc(&techDesc);
	for (unsigned int p = 0; p < techDesc.Passes; ++p)
	{
		_fx->_drawTech->GetPassByIndex(p)->Apply(0, dc);

		dc->DrawAuto();
	}
}

void ArkEngine::ArkDX11::ParticleSystem::BuildVB(ID3D11Device* device)
{
}

ArkEngine::ArkDX11::ParticleSystem::ParticleSystem(const ParticleSystem& rhs)
{
}


