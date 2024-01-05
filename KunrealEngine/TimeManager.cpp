#include "TimeManager.h"

KunrealEngine::TimeManager::TimeManager()
	:m_startTime(std::chrono::high_resolution_clock::now()),
	m_prevTime(std::chrono::high_resolution_clock::now()),
	m_deltaTime(0.0),
	m_timeScale(1.0f)
{

}

KunrealEngine::TimeManager::~TimeManager()
{

}

void KunrealEngine::TimeManager::Initialize()
{

}

void KunrealEngine::TimeManager::Finalize()
{

}

//지속적으로 델타타임 등 필요한 부분들을 업데이트
void KunrealEngine::TimeManager::Update()
{
	///업데이트가 돌아가면서 currentTime을 계속 업데이트
	///prevTime과 새로 업데이트한 시간의 차이를 계산해서 델타타임 도출
	///그리고 prevTime을 currentTime로 업데이트
	m_currentTime = std::chrono::high_resolution_clock::now();
	m_deltaTime = std::chrono::duration<float>(m_currentTime - m_prevTime).count() * m_timeScale;
	m_prevTime = m_currentTime;
}

float KunrealEngine::TimeManager::GetDeltaTime()
{
	return m_deltaTime;
}

void KunrealEngine::TimeManager::SetTimeScale(float scale)
{
	if (scale <= 0)
	{
		return;
	}

	m_timeScale = scale;
}

float KunrealEngine::TimeManager::GetTimeScale()
{
	return m_timeScale;
}
