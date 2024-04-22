#include <d3d11.h>
#include <string>
#include "ResourceManager.h"
#include "ArkDevice.h"
#include "Camera.h"
#include "ArkEffect.h"
#include "d3dx11effect.h"
#include "ParticleEffect.h"
#include "DXTK/DirectXTex.h"
#include "ParticleResource.h"

#include "ParticleSystem.h"


ArkEngine::ArkDX11::ParticleSystem::ParticleSystem(const std::string& particleName, const std::string& fileName, unsigned int maxParticle)
	: _particleName(particleName), _maxParticles(maxParticle), _firstRun(true), _fileName(fileName),
	_gameTime(0), _timeStep(0), _age(0),
	_initVB(nullptr), _drawVB(nullptr), _streamOutVB(nullptr),
	_texArraySRV(nullptr),
	_particleSizeEffect(nullptr), _emitVelocityEffect(nullptr), _isRandomEffect(nullptr),
	_isRandom(false),
	_particleFadeTime(1.0f), _particleLifeTime(1.0f),
	_particleColorEffect(nullptr)
{
	_eyePosW = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
	_emitPosW = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
	_emitDirW = DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f);

	_emitVelocity = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
	_particleSize = DirectX::XMFLOAT2(0.0f, 0.0f);
	_particleColor = DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f);

	Initialize(std::wstring(fileName.begin(), fileName.end()), _maxParticles);
}

ArkEngine::ArkDX11::ParticleSystem::ParticleSystem(const std::string& particleName, const std::vector<std::string>& fileNameList, unsigned int maxParticle)
	:_particleName(particleName), _maxParticles(maxParticle), _firstRun(true),
	_gameTime(0), _timeStep(0), _age(0),
	_initVB(nullptr), _drawVB(nullptr), _streamOutVB(nullptr),
	_texArraySRV(nullptr),
	_particleSizeEffect(nullptr), _emitVelocityEffect(nullptr), _isRandomEffect(nullptr),
	_isRandom(false),
	_particleFadeTime(1.0f), _particleLifeTime(1.0f),
	_particleColorEffect(nullptr)
{
	_eyePosW = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
	_emitPosW = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
	_emitDirW = DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f);

	_emitVelocity = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
	_particleSize = DirectX::XMFLOAT2(0.0f, 0.0f);
	_particleColor = DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f);

	std::vector<std::wstring> newStringVec;

	for (auto index : fileNameList)
	{
		newStringVec.emplace_back(std::wstring(index.begin(), index.end()));
	}


	Initialize(newStringVec, _maxParticles);
}

ArkEngine::ArkDX11::ParticleSystem::~ParticleSystem()
{
	_isRandomEffect->Release();
	_emitVelocityEffect->Release();
	_particleSizeEffect->Release();
	_texArray->Release();
	_randomTex->Release();

	_viewProjEffect->Release();
	_gameTimeEffect->Release();
	_timeStepEffect->Release();
	_eyePosWEffect->Release();
	_emitPosWEffect->Release();
	_emitDirWEffect->Release();

	_streamOutTech->Release();
	_drawTech->Release();

	_streamOutVB->Release();
	_drawVB->Release();
}

float ArkEngine::ArkDX11::ParticleSystem::GetAge() const
{
	return _age;
}

void ArkEngine::ArkDX11::ParticleSystem::SetEyePos(const DirectX::XMFLOAT3& eyePosW)
{
	_eyePosW = eyePosW;
}

void ArkEngine::ArkDX11::ParticleSystem::SetEmitPos(const DirectX::XMFLOAT3& emitPosW)
{
	_emitPosW = emitPosW;
}

void ArkEngine::ArkDX11::ParticleSystem::SetEmitDir(const DirectX::XMFLOAT3& emitDirW)
{
	_emitDirW = emitDirW;
}

// ��ƼŬ�� �ؽ��� ũ�⸦ �����ϴ� �Լ�
void ArkEngine::ArkDX11::ParticleSystem::SetParticleSize(const DirectX::XMFLOAT2& particleSize)
{
	_particleSize = particleSize;
}

// ��ƼŬ�� ����ӵ��� �����ϴ� �Լ�
void ArkEngine::ArkDX11::ParticleSystem::SetEmitVelocity(float particleSpeed, bool isRandom)
{
	if (_particleName == "Laser")
	{
		_emitVelocity = DirectX::XMFLOAT3(1.0f, particleSpeed, 1.0f);
	}
	else
	{
		_emitVelocity = DirectX::XMFLOAT3(particleSpeed, particleSpeed, particleSpeed);
	}
	_isRandom = isRandom;
}

// ��ƼŬ�� ������� �ð��� �����ϴ� �Լ�(��ƼŬ ���ڰ� ���� ������� �ð�, ��ƼŬ�� ������ ������� �ð�)
// ex) paeticleFadeTime = 1.0f ��� 1�ʵ��� ���� ����������
// ��������� particleFadeTime�� particleLifeTime���� �۰� �����Ѵ�
void ArkEngine::ArkDX11::ParticleSystem::SetParticleTime(float particleFadeTime, float particleLifeTime)
{
	_particleFadeTime = particleFadeTime;
	_particleLifeTime = particleLifeTime;
}

// ��ƼŬ�� RGB���� �������ش�.
// 1.0f, 1.0f, 1.0f�̸� �����ؽ����� ������ ���´�
void ArkEngine::ArkDX11::ParticleSystem::SetParticleColor(const DirectX::XMFLOAT3& particleColor)
{
	_particleColor = particleColor;
}

void ArkEngine::ArkDX11::ParticleSystem::SetParticleRotation(const DirectX::XMFLOAT3& rotation)
{
}

void ArkEngine::ArkDX11::ParticleSystem::Initialize(const std::vector<std::wstring>& fileNameList, unsigned int maxParticle)
{
	auto arkDevice = ResourceManager::GetInstance()->GetResource<ArkEngine::ArkDX11::ArkDevice>("Device");

	if (arkDevice->GetRandomTex() == nullptr)
	{
		CreateRandomTextureSRV();
	}

	auto particleResource = ResourceManager::GetInstance()->GetParticleResource(_particleName);

	// �̹� ������� ��ƼŬ�� ���� ���ҽ��� �������� �ʴ´ٸ�
	if (particleResource == nullptr)
	{
		CreateTexture2DArraySRV(fileNameList);

		BuildVB();

		auto resoruce = new ParticleResource(_particleName, _texArraySRV, _initVB);
	}

	// �����Ѵٸ� �ִ� �� ���
	else
	{
		/// �ΰ��� ���� vb�� �����ϸ� ����� �������� �ϳ��� ��; 	std::swap(_drawVB, _streamOutVB); �̰Ŷ����ΰ�? ��..
		//_drawVB = particleResource->GetDrawVB();
		//_streamOutVB = particleResource->GetStreamVB();
		// �׷��� �̰ŷ� draw, stream vb�� ������ִ���

		BuildDrawStreamVB();

		_texArraySRV = particleResource->GetTexArray();

		_initVB = particleResource->GetInitVB();
	}

	SetEffect();
}


void ArkEngine::ArkDX11::ParticleSystem::Initialize(const std::wstring& fileName, unsigned int maxParticle)
{
	_arkDevice = ResourceManager::GetInstance()->GetResource<ArkEngine::ArkDX11::ArkDevice>("Device");

	if (_arkDevice->GetRandomTex() == nullptr)
	{
		CreateRandomTextureSRV();
	}

	auto particleResource = ResourceManager::GetInstance()->GetParticleResource(_particleName);

	// �̹� ������� ��ƼŬ�� ���� ���ҽ��� �������� �ʴ´ٸ�
	if (particleResource == nullptr)
	{
		CreateTexture2DArraySRV(fileName);

		BuildVB();

		auto resoruce = new ParticleResource(_particleName, _texArraySRV, _initVB);

		ResourceManager::GetInstance()->AddParticleResource(_particleName, resoruce);
	}

	// �����Ѵٸ� �ִ� �� ���
	else
	{
		/// �ΰ��� ���� vb�� �����ϸ� ����� �������� �ϳ��� ��; 	std::swap(_drawVB, _streamOutVB); �̰Ŷ����ΰ�? ��..
		//_drawVB = particleResource->GetDrawVB();
		//_streamOutVB = particleResource->GetStreamVB();
		// �׷��� �̰ŷ� draw, stream vb�� ������ִ���
		BuildDrawStreamVB();

		_texArraySRV = particleResource->GetTexArray();

		_initVB = particleResource->GetInitVB();
	}

	SetEffect();
}

void ArkEngine::ArkDX11::ParticleSystem::Reset()
{
	_firstRun = true;
	_age = 0.0f;
}

void ArkEngine::ArkDX11::ParticleSystem::Update(float deltaTime, float gameTime)
{
	_gameTime += gameTime;
	_timeStep = deltaTime;
}

void ArkEngine::ArkDX11::ParticleSystem::Draw(ArkEngine::ICamera* p_Camera)
{
	auto dc = _arkDevice->GetDeviceContext();

	auto cameraView = p_Camera->GetViewMatrix();
	auto cameraProj = p_Camera->GetProjMatrix();

	DirectX::XMMATRIX VP = cameraView * cameraProj;

	// �Է� ������ �ܰ踦 ����
	dc->IASetInputLayout(_arkEffect->GetInputLayOut());
	dc->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);
	dc->RSSetState(_arkDevice->GetSolidRS());

	unsigned int stride = sizeof(Particle);
	unsigned int offset = 0;

	// ������� �����Ѵ�
	SetViewProj(VP);
	SetGameTime(_gameTime);
	SetTimeStep(_timeStep);
	SetEyePosW(_eyePosW);
	SetEmitPosW(_emitPosW);
	SetEmitDirW(_emitDirW);
	SetTexArray(_texArraySRV);
	SetRandomTex(_arkDevice->GetRandomTex());

	SetParticleSizeW(_particleSize);
	SetEmitVelocityW(_emitVelocity);
	SetParticleTimeW(_particleFadeTime, _particleLifeTime);
	SetParticleColorW(_particleColor);

	// ���� �����̸� �ʱ�ȭ�� ���� ���۸� ����ϰ�, �׷��� �ʴٸ�
	// ������ ���� ����� ���� ���� ���۸� ����Ѵ�
	if (_firstRun)
	{
		// _initVB�� ���� CPU�� �ö󰡼� ���������� �ҷ��´�
		// �������� ���� ����
		dc->IASetVertexBuffers(0, 1, &_initVB, &stride, &offset);
	}
	else
	{
		dc->IASetVertexBuffers(0, 1, &_drawVB, &stride, &offset);
	}

	// ���� ���� ����� ��Ʈ�� ��� ���� ������� �׷��� ���ڵ��� �����Ѵ�
	// ���ŵ� ���ڵ��� ��Ʈ�� ����� ���ؼ� ��� ���� ���ۿ� ��ϵȴ�
	// SO�ܰ迡 ���ؽ� ���۸� _streamOutVB�� ���ε����ִ� �Լ�
	dc->SOSetTargets(1, &_streamOutVB, &offset);

	D3DX11_TECHNIQUE_DESC techDesc;
	_streamOutTech->GetDesc(&techDesc);
	for (unsigned int p = 0; p < techDesc.Passes; ++p)
	{
		_streamOutTech->GetPassByIndex(p)->Apply(0, dc);

		if (_firstRun)
		{
			// ���������� �ϳ��̱⶧���� ������ 1���� ������ �ִ�
			dc->Draw(1, 0);
			_firstRun = false;
		}
		else
		{
			// ���� ������ ��Ȯ�� �� �� ���� ������
			dc->DrawAuto();
		}
	}

	// ��Ʈ�� ���� �н��� ������. ���� ���۸� �����
	ID3D11Buffer* bufferArray[1] = { 0 };
	dc->SOSetTargets(1, bufferArray, &offset);

	// ���� ���۵��� �¹ٲ۴�
	std::swap(_drawVB, _streamOutVB);

	// ��� ��Ʈ�� ��µ�, ���ŵ� ���ڸ� ȭ�鿡 �帰��
	dc->IASetVertexBuffers(0, 1, &_drawVB, &stride, &offset);

	_drawTech->GetDesc(&techDesc);
	for (unsigned int p = 0; p < techDesc.Passes; ++p)
	{
		_drawTech->GetPassByIndex(p)->Apply(0, dc);

		dc->DrawAuto();
	}

	SetEyePos(p_Camera->GetCameraPos());

	float blendFactor[] = { 0.0f, 0.0f, 0.0f, 0.0f };
	_arkDevice->GetDeviceContext()->OMSetBlendState(0, blendFactor, 0xffffffff); // restore default
}

ID3D11ShaderResourceView* ArkEngine::ArkDX11::ParticleSystem::CreateRandomTextureSRV()
{
	auto device = _arkDevice->GetDevice();

	if (_arkDevice->GetRandomTex() == nullptr)
	{
		// ���� �����͸� �����Ѵ�
		std::vector<DirectX::XMFLOAT4> randomValues(1024);

		for (int i = 0; i < 1024; i++)
		{
			randomValues[i].x = GetRandomFloat(-1.0f, 1.0f);
			randomValues[i].y = GetRandomFloat(-1.0f, 1.0f);
			randomValues[i].z = GetRandomFloat(-1.0f, 1.0f);
			randomValues[i].w = GetRandomFloat(-1.0f, 1.0f);
		}

		D3D11_SUBRESOURCE_DATA initData;
		initData.pSysMem = randomValues.data();
		initData.SysMemPitch = 1024 * sizeof(DirectX::XMFLOAT4);
		initData.SysMemSlicePitch = 0;

		// ���ο� �ؽ��ĸ� �����Ѵ�
		D3D11_TEXTURE1D_DESC texDesc;
		texDesc.Width = 1024;
		texDesc.MipLevels = 1;
		texDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		texDesc.Usage = D3D11_USAGE_IMMUTABLE;
		texDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
		texDesc.CPUAccessFlags = 0;
		texDesc.MiscFlags = 0;
		texDesc.ArraySize = 1;

		ID3D11Texture1D* randomTex;
		device->CreateTexture1D(&texDesc, &initData, &randomTex);

		// ResourceView�� �����Ѵ�
		D3D11_SHADER_RESOURCE_VIEW_DESC viewDesc;
		viewDesc.Format = texDesc.Format;
		viewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE1D;
		viewDesc.Texture1D.MipLevels = texDesc.MipLevels;
		viewDesc.Texture1D.MostDetailedMip = 0;

		ID3D11ShaderResourceView* randomTexSRV = 0;
		device->CreateShaderResourceView(randomTex, &viewDesc, &randomTexSRV);

		_arkDevice->SetRandomTex(randomTexSRV);
	}

	return _arkDevice->GetRandomTex();
}

ID3D11ShaderResourceView* ArkEngine::ArkDX11::ParticleSystem::CreateTexture2DArraySRV(const std::vector<std::wstring>& fileNameList)
{
	std::vector<DirectX::TexMetadata> metadata(fileNameList.size());
	std::vector<DirectX::ScratchImage> scratchImage(fileNameList.size());
	HRESULT hr;

	// �̹������� �ε��Ѵ�
	unsigned int size = fileNameList.size();

	for (int i = 0; i < size; i++)
	{
		hr = LoadFromDDSFile(fileNameList[i].c_str(), DirectX::DDS_FLAGS_NONE, &metadata[i], scratchImage[i]);
		if (FAILED(hr))
		{
			return nullptr;
		}
	}

	// �ؽ�ó �迭�� �����Ѵ�
	D3D11_TEXTURE2D_DESC texArrayDesc;
	ZeroMemory(&texArrayDesc, sizeof(D3D11_TEXTURE2D_DESC));
	texArrayDesc.Width = static_cast<UINT>(metadata[0].width);
	texArrayDesc.Height = static_cast<UINT>(metadata[0].height);
	texArrayDesc.MipLevels = metadata[0].mipLevels;
	texArrayDesc.ArraySize = static_cast<UINT>(fileNameList.size());
	texArrayDesc.Format = metadata[0].format;
	texArrayDesc.SampleDesc.Count = 1;
	texArrayDesc.SampleDesc.Quality = 0;
	texArrayDesc.Usage = D3D11_USAGE_DEFAULT;
	texArrayDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	texArrayDesc.CPUAccessFlags = 0;
	texArrayDesc.MiscFlags = 0;


	ID3D11Texture2D* textureArray = nullptr;
	hr = _arkDevice->GetDevice()->CreateTexture2D(&texArrayDesc, nullptr, &textureArray);
	if (FAILED(hr))
	{
		return nullptr; // �ؽ�ó ���� ����
	}

	// ���� �̹������� �ؽ�ó �迭�� �����Ѵ�
	for (UINT i = 0; i < fileNameList.size(); ++i)
	{
		for (UINT mipLevel = 0; mipLevel < metadata[i].mipLevels; ++mipLevel)
		{
			const DirectX::Image* img = scratchImage[i].GetImage(mipLevel, 0, 0);
			_arkDevice->GetDeviceContext()->UpdateSubresource
			(
				textureArray,
				D3D11CalcSubresource(mipLevel, i, texArrayDesc.MipLevels),
				nullptr,
				img->pixels,
				img->rowPitch,
				img->slicePitch
			);
		}
	}

	// SRV�� �����Ѵ�
	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	ZeroMemory(&srvDesc, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));
	srvDesc.Format = texArrayDesc.Format;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DARRAY;
	srvDesc.Texture2DArray.MostDetailedMip = 0;
	srvDesc.Texture2DArray.MipLevels = texArrayDesc.MipLevels;
	srvDesc.Texture2DArray.FirstArraySlice = 0;
	srvDesc.Texture2DArray.ArraySize = static_cast<UINT>(fileNameList.size());

	ID3D11ShaderResourceView* srv;
	hr = _arkDevice->GetDevice()->CreateShaderResourceView(textureArray, &srvDesc, &srv);
	textureArray->Release(); // �ؽ�ó �迭 ����� �������Ƿ� �޸� ����

	if (FAILED(hr))
	{
		return nullptr; // SRV ���� ����
	}

	_texArraySRV = srv;

	return srv; // ������ Shader Resource View ��ȯ
}


ID3D11ShaderResourceView* ArkEngine::ArkDX11::ParticleSystem::CreateTexture2DArraySRV(const std::wstring& fileName)
{
	std::vector<DirectX::TexMetadata> metadata(1);
	std::vector<DirectX::ScratchImage> scratchImage(1);
	HRESULT hr;

	// �̹������� �ε��Ѵ�
	unsigned int size = 1;

	for (int i = 0; i < size; i++)
	{
		hr = LoadFromDDSFile(fileName.c_str(), DirectX::DDS_FLAGS_NONE, &metadata[i], scratchImage[i]);
		if (FAILED(hr))
		{
			return nullptr;
		}
	}

	// �ؽ�ó �迭�� �����Ѵ�
	D3D11_TEXTURE2D_DESC texArrayDesc;
	ZeroMemory(&texArrayDesc, sizeof(D3D11_TEXTURE2D_DESC));
	texArrayDesc.Width = static_cast<UINT>(metadata[0].width);
	texArrayDesc.Height = static_cast<UINT>(metadata[0].height);
	texArrayDesc.MipLevels = metadata[0].mipLevels;
	texArrayDesc.ArraySize = static_cast<UINT>(1);
	texArrayDesc.Format = metadata[0].format;
	texArrayDesc.SampleDesc.Count = 1;
	texArrayDesc.SampleDesc.Quality = 0;
	texArrayDesc.Usage = D3D11_USAGE_DEFAULT;
	texArrayDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	texArrayDesc.CPUAccessFlags = 0;
	texArrayDesc.MiscFlags = 0;


	ID3D11Texture2D* textureArray = nullptr;
	hr = _arkDevice->GetDevice()->CreateTexture2D(&texArrayDesc, nullptr, &textureArray);
	if (FAILED(hr))
	{
		return nullptr; // �ؽ�ó ���� ����
	}

	// ���� �̹������� �ؽ�ó �迭�� �����Ѵ�
	for (UINT i = 0; i < 1; ++i)
	{
		for (UINT mipLevel = 0; mipLevel < metadata[i].mipLevels; ++mipLevel)
		{
			const DirectX::Image* img = scratchImage[i].GetImage(mipLevel, 0, 0);
			_arkDevice->GetDeviceContext()->UpdateSubresource
			(
				textureArray,
				D3D11CalcSubresource(mipLevel, i, texArrayDesc.MipLevels),
				nullptr,
				img->pixels,
				img->rowPitch,
				img->slicePitch
			);
		}
	}

	// SRV�� �����Ѵ�
	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	ZeroMemory(&srvDesc, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));
	srvDesc.Format = texArrayDesc.Format;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DARRAY;
	srvDesc.Texture2DArray.MostDetailedMip = 0;
	srvDesc.Texture2DArray.MipLevels = texArrayDesc.MipLevels;
	srvDesc.Texture2DArray.FirstArraySlice = 0;
	srvDesc.Texture2DArray.ArraySize = static_cast<UINT>(1);

	ID3D11ShaderResourceView* srv;
	hr = _arkDevice->GetDevice()->CreateShaderResourceView(textureArray, &srvDesc, &srv);
	textureArray->Release(); // �ؽ�ó �迭 ����� �������Ƿ� �޸� ����

	if (FAILED(hr))
	{
		return nullptr; // SRV ���� ����
	}

	_texArraySRV = srv;

	return srv; // ������ Shader Resource View ��ȯ
}

void ArkEngine::ArkDX11::ParticleSystem::BuildVB()
{
	auto device = _arkDevice->GetDevice();
	//
	// Create the buffer to kick-off the particle system.
	//
	D3D11_BUFFER_DESC vbd;
	vbd.Usage = D3D11_USAGE_DEFAULT;
	vbd.ByteWidth = sizeof(Particle) * 1;
	vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbd.CPUAccessFlags = 0;
	vbd.MiscFlags = 0;
	vbd.StructureByteStride = 0;

	// The initial particle emitter has type 0 and age 0.  The rest
	// of the particle attributes do not apply to an emitter.
	Particle p;
	ZeroMemory(&p, sizeof(Particle));
	p.Age = 0.0f;
	p.Type = 0;
	//p.Rotation = DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f);

	D3D11_SUBRESOURCE_DATA vinitData;
	vinitData.pSysMem = &p;

	device->CreateBuffer(&vbd, &vinitData, &_initVB);

	//
	// Create the ping-pong buffers for stream-out and drawing.
	//
	vbd.ByteWidth = sizeof(Particle) * _maxParticles;
	vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER | D3D11_BIND_STREAM_OUTPUT;

	(device->CreateBuffer(&vbd, 0, &_drawVB));
	(device->CreateBuffer(&vbd, 0, &_streamOutVB));
}


void ArkEngine::ArkDX11::ParticleSystem::BuildDrawStreamVB()
{
	auto device = _arkDevice->GetDevice();
	//
	// Create the buffer to kick-off the particle system.
	//
	D3D11_BUFFER_DESC vbd;
	vbd.Usage = D3D11_USAGE_DEFAULT;
	vbd.ByteWidth = sizeof(Particle) * 1;
	vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbd.CPUAccessFlags = 0;
	vbd.MiscFlags = 0;
	vbd.StructureByteStride = 0;

	// The initial particle emitter has type 0 and age 0.  The rest
	// of the particle attributes do not apply to an emitter.
	Particle p;
	ZeroMemory(&p, sizeof(Particle));
	p.Age = 0.0f;
	p.Type = 0;
	//p.Rotation = DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f);

	// Create the ping-pong buffers for stream-out and drawing.
	//
	vbd.ByteWidth = sizeof(Particle) * _maxParticles;
	vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER | D3D11_BIND_STREAM_OUTPUT;

	(device->CreateBuffer(&vbd, 0, &_drawVB));
	(device->CreateBuffer(&vbd, 0, &_streamOutVB));
}

void ArkEngine::ArkDX11::ParticleSystem::SetEffect()
{
	if (_fileName == "Resources/Textures/Particles/raindrop.dds")
	{
		_arkEffect = ResourceManager::GetInstance()->GetResource<ArkEngine::ArkDX11::ArkEffect>("Resources/FX/ParticleRain.fx");
	}
	else
	{
		_arkEffect = ResourceManager::GetInstance()->GetResource<ArkEngine::ArkDX11::ArkEffect>("Resources/FX/ParticleFire.fx");
	}

	auto effect = _arkEffect->GetEffect();

	_streamOutTech = effect->GetTechniqueByName("StreamOutTech");
	_drawTech = effect->GetTechniqueByName("DrawTech");

	_viewProjEffect = effect->GetVariableByName("gViewProj")->AsMatrix();
	_gameTimeEffect = effect->GetVariableByName("gGameTime")->AsScalar();
	_timeStepEffect = effect->GetVariableByName("gTimeStep")->AsScalar();
	_eyePosWEffect = effect->GetVariableByName("gEyePosW")->AsVector();
	_emitPosWEffect = effect->GetVariableByName("gEmitPosW")->AsVector();
	_emitDirWEffect = effect->GetVariableByName("gEmitDirW")->AsVector();
	_texArray = effect->GetVariableByName("gTexArray")->AsShaderResource();
	_randomTex = effect->GetVariableByName("gRandomTex")->AsShaderResource();

	_particleSizeEffect = effect->GetVariableByName("gParticleSize")->AsVector();
	_emitVelocityEffect = effect->GetVariableByName("gEmitVelocity")->AsVector();
	_isRandomEffect = effect->GetVariableByName("gIsRandom")->AsScalar();
	_particleFadeTimeEffect = effect->GetVariableByName("gParticleFadeTime")->AsScalar();
	_particleLifeTimeEffect = effect->GetVariableByName("gParticleLifeTime")->AsScalar();
	_particleColorEffect = effect->GetVariableByName("gParticleColor")->AsVector();
}

float ArkEngine::ArkDX11::ParticleSystem::GetRandomFloat(float minNum, float maxNum)
{
	std::random_device rd;
	std::mt19937 generator(rd());
	std::uniform_real_distribution<> dist(minNum, maxNum);

	return dist(generator);
}

void ArkEngine::ArkDX11::ParticleSystem::SetViewProj(DirectX::CXMMATRIX m)
{
	_viewProjEffect->SetMatrix(reinterpret_cast<const float*>(&m));
}

void ArkEngine::ArkDX11::ParticleSystem::SetGameTime(float f)
{
	_gameTimeEffect->SetFloat(f);
}

void ArkEngine::ArkDX11::ParticleSystem::SetTimeStep(float f)
{
	_timeStepEffect->SetFloat(f);
}

void ArkEngine::ArkDX11::ParticleSystem::SetEyePosW(const DirectX::XMFLOAT3& v)
{
	_eyePosWEffect->SetRawValue(&v, 0, sizeof(DirectX::XMFLOAT3));
}

void ArkEngine::ArkDX11::ParticleSystem::SetEmitPosW(const DirectX::XMFLOAT3& v)
{
	_emitPosWEffect->SetRawValue(&v, 0, sizeof(DirectX::XMFLOAT3));
}

void ArkEngine::ArkDX11::ParticleSystem::SetEmitDirW(const DirectX::XMFLOAT3& v)
{
	_emitDirWEffect->SetRawValue(&v, 0, sizeof(DirectX::XMFLOAT3));
}

void ArkEngine::ArkDX11::ParticleSystem::SetTexArray(ID3D11ShaderResourceView* tex)
{
	_texArray->SetResource(tex);
}

void ArkEngine::ArkDX11::ParticleSystem::SetRandomTex(ID3D11ShaderResourceView* tex)
{
	_randomTex->SetResource(tex);
}

void ArkEngine::ArkDX11::ParticleSystem::SetParticleSizeW(const DirectX::XMFLOAT2& v)
{
	_particleSizeEffect->SetRawValue(&v, 0, sizeof(DirectX::XMFLOAT2));
}

void ArkEngine::ArkDX11::ParticleSystem::SetEmitVelocityW(const DirectX::XMFLOAT3& v)
{
	_emitVelocityEffect->SetRawValue(&v, 0, sizeof(DirectX::XMFLOAT3));
	_isRandomEffect->SetBool(_isRandom);
}

void ArkEngine::ArkDX11::ParticleSystem::SetParticleTimeW(float f1, float f2)
{
	_particleFadeTimeEffect->SetFloat(f1);
	_particleLifeTimeEffect->SetFloat(f2);
}

void ArkEngine::ArkDX11::ParticleSystem::SetParticleColorW(const DirectX::XMFLOAT3& v)
{
	_particleColorEffect->SetRawValue(&v, 0, sizeof(DirectX::XMFLOAT3));
}

void ArkEngine::ArkDX11::ParticleSystem::SetParticleRotationW(const DirectX::XMFLOAT3& v)
{
}
