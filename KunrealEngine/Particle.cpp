#include "Particle.h"
#include "GraphicsSystem.h"

KunrealEngine::Particle::Particle()
{

}

KunrealEngine::Particle::~Particle()
{

}

void KunrealEngine::Particle::Initialize()
{
	
}

void KunrealEngine::Particle::Release()
{
	
}

void KunrealEngine::Particle::FixedUpdate()
{
	
}

void KunrealEngine::Particle::Update()
{
	
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
	
}

void KunrealEngine::Particle::SetParticleEffect(std::string name, std::string fileName)
{

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

void KunrealEngine::Particle::SetParticleSize(float x, float y)
{
	DirectX::XMFLOAT2 size = { x, y };

	_particle->SetParticleSize(size);
}

void KunrealEngine::Particle::SetParticleVelocity(float velocity, bool isRandom)
{
	_particle->SetEmitVelocity(velocity, isRandom);
}

void KunrealEngine::Particle::SetParticleDuration(float fade, float life)
{
	_particle->SetParticleTime(fade, life);
}

void KunrealEngine::Particle::AddParticleColor(float x, float y, float z)
{
	DirectX::XMFLOAT3 color = { x, y, z };

	_particle->SetParticleColor(color);
}

void KunrealEngine::Particle::SetParticleDirection(float x, float y, float z)
{
	DirectX::XMFLOAT3 direction = { x, y, z };
	
	_particle->SetParticleDirection(direction);
}

