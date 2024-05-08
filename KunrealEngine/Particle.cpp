#include "Particle.h"
#include "Transform.h"
#include "GraphicsSystem.h"

KunrealEngine::Particle::Particle()
	:_particle(nullptr), _transform(nullptr),
	_velocity(0.0f), _random(false), _fadeoutTime(0.0f), _lifeTime(0.0f),
	_size({ 0.0f, 0.0f }), _color({ 0.0f, 0.0f, 0.0f }), _direction({ 0.0f, 0.0f, 0.0f }),
	_parentObject(nullptr), _parentBoneName()
{
	
}

KunrealEngine::Particle::~Particle()
{

}

void KunrealEngine::Particle::Initialize()
{
	_transform = this->GetOwner()->GetComponent<Transform>();
}

void KunrealEngine::Particle::Release()
{
	
}

void KunrealEngine::Particle::FixedUpdate()
{
	
}

void KunrealEngine::Particle::Update()
{
	SetParticlePos(this->_transform->GetPosition());
	SetParticleRotation(this->_transform->GetRotation().x, this->_transform->GetRotation().y, this->_transform->GetRotation().z);
	//SetParticleSize(this->_transform->GetScale().x, this->_transform->GetScale().y);
}

void KunrealEngine::Particle::LateUpdate()
{
	
}

void KunrealEngine::Particle::OnTriggerEnter()
{
	
}

void KunrealEngine::Particle::OnTriggerStay()
{
	
}

void KunrealEngine::Particle::OnTriggerExit()
{
	
}

void KunrealEngine::Particle::SetActive(bool active)
{
	if (active)
	{
		Start();
	}
	else
	{
		Stop();
	}

	this->_isActivated = active;
}

void KunrealEngine::Particle::SetParticleEffect(std::string name, std::string fileName, unsigned int maxParticle)
{
	_particle = GRAPHICS->CreateParticle(name, fileName, maxParticle);
}

void KunrealEngine::Particle::Start()
{
	this->_particle->Start();
}

void KunrealEngine::Particle::Stop()
{
	this->_particle->Stop();
}

void KunrealEngine::Particle::Reset()
{
	this->_particle->Reset();
}

void KunrealEngine::Particle::SetParticlePos(float x, float y, float z)
{
	DirectX::XMFLOAT3 pos = { x, y, z };

	_particle->SetEmitPos(pos);
}


void KunrealEngine::Particle::SetParticlePos(DirectX::XMFLOAT3 pos)
{
	this->_particle->SetEmitPos(pos);
}

void KunrealEngine::Particle::SetParticleSize(float x, float y)
{
	DirectX::XMFLOAT2 size = { x, y };

	_particle->SetParticleSize(size);
	_size = size;
}

void KunrealEngine::Particle::SetParticleVelocity(float velocity, bool isRandom)
{
	_particle->SetEmitVelocity(velocity, isRandom);

	this->_velocity = velocity;
	this->_random = isRandom;
}

void KunrealEngine::Particle::SetParticleDuration(float fade, float life)
{
	this->_particle->SetParticleTime(fade, life);

	this->_fadeoutTime = fade;
	this->_lifeTime = life;
}

void KunrealEngine::Particle::AddParticleColor(float x, float y, float z)
{
	DirectX::XMFLOAT3 color = { x, y, z };
	_particle->SetParticleColor(color);

	this->_color = color;
}

void KunrealEngine::Particle::SetParticleDirection(float x, float y, float z)
{
	DirectX::XMFLOAT3 direction = { x, y, z };
	_particle->SetParticleDirection(direction);

	this->_direction = direction;
}


void KunrealEngine::Particle::SetParticleRotation(float x, float y, float z)
{
	DirectX::XMFLOAT3 rotation = { x, y + 180.0f, z };
	_particle->SetParticleRotation(rotation);

	this->_rotation = rotation;
}


void KunrealEngine::Particle::SetTransform(GameObject* renderable, std::string boneName)
{
	_parentObject = renderable;
	_parentBoneName = boneName;

	_transform->_haveParentBone = true;

	if (renderable != this->GetOwner())
	{
		this->GetOwner()->SetParent(renderable);
	}
}

DirectX::XMFLOAT2 KunrealEngine::Particle::GetSize()
{
	return this->_size;
}

float KunrealEngine::Particle::GetVelocity()
{
	return this->_velocity;
}

bool KunrealEngine::Particle::GetRandomState()
{
	return this->_random;
}

float KunrealEngine::Particle::GetFadeOutTime()
{
	return this->_fadeoutTime;
}

float KunrealEngine::Particle::GetLifeTime()
{
	return this->_lifeTime;
}

DirectX::XMFLOAT3 KunrealEngine::Particle::GetColor()
{
	return this->_color;
}

DirectX::XMFLOAT3 KunrealEngine::Particle::GetDirection()
{
	return this->_direction;
}

