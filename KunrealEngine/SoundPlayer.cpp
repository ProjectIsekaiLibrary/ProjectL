#include "SoundPlayer.h"
#include "SoundSystem.h"

KunrealEngine::SoundPlayer::SoundPlayer()
	:_createdCnt(0)
{
	
}

KunrealEngine::SoundPlayer::~SoundPlayer()
{

}

void KunrealEngine::SoundPlayer::Initialize()
{

}

void KunrealEngine::SoundPlayer::Release()
{

}

void KunrealEngine::SoundPlayer::FixedUpdate()
{

}

void KunrealEngine::SoundPlayer::Update()
{

}

void KunrealEngine::SoundPlayer::LateUpdate()
{

}

void KunrealEngine::SoundPlayer::OnTriggerEnter()
{

}

void KunrealEngine::SoundPlayer::OnTriggerStay()
{

}

void KunrealEngine::SoundPlayer::OnTriggerExit()
{

}

void KunrealEngine::SoundPlayer::SetActive(bool active)
{
	this->_isActivated = active;
}

void KunrealEngine::SoundPlayer::CreateSoundInfo(std::string soundFile, bool is3D /*= false*/, bool isLoop /*= true*/, int volume /*= 100*/)
{
	//SoundInfo newSound(soundFile, is3D, isLoop, volume, _soundList.size());
	SoundInfo newSound;

	if (is3D)
	{
		newSound = { soundFile, is3D, isLoop, volume, SoundSystem::GetInstance().Add3DSound(soundFile, volume), (unsigned int)_soundList.size() };
	}
	else
	{
		newSound = { soundFile, is3D, isLoop, volume, SoundSystem::GetInstance().AddSound(soundFile, volume), (unsigned int)_soundList.size() };
	}

	_soundList.push_back(newSound);
}

void KunrealEngine::SoundPlayer::SetVolume(int vol, int index /*= 0*/)
{
	if (_soundList.size() > 1 && index == -1)
	{
		assert(0 && "SetVolume() 사운드를 여러 개 들고 있지만 index를 정해주지 않음");
	}
	else if (index == -1)
	{
		index = 0;
	}

	SoundSystem::GetInstance().Setvolume(_soundList[index]._soundIndex, vol);
}

void KunrealEngine::SoundPlayer::Play(int index)
{
	bool isLoop = _soundList[index]._isLoop;

	if (!isLoop)
	{
		SoundSystem::GetInstance().Play(_soundList[index]._soundIndex);
	}
	else
	{
		SoundSystem::GetInstance().Loop(_soundList[index]._soundIndex);
	}
}

void KunrealEngine::SoundPlayer::Pause(int index)
{
	SoundSystem::GetInstance().Pause(_soundList[index]._soundIndex);
}

void KunrealEngine::SoundPlayer::Stop(int index)
{
	SoundSystem::GetInstance().Stop(_soundList[index]._soundIndex);
}

void KunrealEngine::SoundPlayer::StopAll()
{
	for (auto sounds : _soundList)
	{
		SoundSystem::GetInstance().Stop(sounds._soundIndex);
	}
}
