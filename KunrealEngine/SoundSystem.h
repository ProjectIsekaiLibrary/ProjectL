#pragma once
#include <windows.h>
#include <mmsystem.h>
#include <dsound.h>
#include <stdio.h>
#include <assert.h>

#include <vector>
#include <string>

#pragma comment(lib, "dsound.lib")
#pragma comment(lib, "dxguid")
#pragma comment(lib, "winmm.lib")

namespace KunrealEngine
{
	struct WaveHeaderType
	{
		char chunkId[4];
		unsigned long chunkSize;
		char format[4];
		char subChunkId[4];
		unsigned long subChunkSize;
		unsigned short audioFormat;
		unsigned short numChannels;
		unsigned long sampleRate;
		unsigned long bytesPerSample;
		unsigned short blockAlign;
		unsigned short bitsPerSample;
		char dataChunkId[4];
		unsigned long dataSize;
	};

	struct Sound
	{
		virtual ~Sound() {}
		std::string fileName;
		int volume = 0; // 0~100
		IDirectSoundBuffer8* soundBuffer = nullptr;
		IDirectSound3DBuffer8* sound3DBuffer = nullptr;
	};

	class SoundSystem
	{
	public:
		// 싱글턴 형태의 반환
		static SoundSystem& GetInstance();

		//사운드 초기화
		bool Initialize(HWND hWnd);
		void Terminate();

		//사운드 관리
		int AddSound(std::string filename, int volume);
		int Add3DSound(std::string filename, int volume, int xpos = 0, int ypos = 0, int zpos = 0);
		void RemoveSound(int index);
		void ClearAllSound();
		void Setvolume(int index, int volume);

		//3D 사운드에서 위치 조정 관련
		void updateSoundPosition(int index, float x, float y, float z);	// 사운드의 위치를 변경
		void updateListenerPosition(float x, float y, float z);			// 리스너(청자)의 위치를 변경

		// 사운드 재생 관련
		void Play(int index);											// 재생
																		// 3D 사운드의 경우 마지막으로 정해져 있는 위치에서 재생
		void PlayWithPos(int index, int xpos, int ypos, int zpos);		// 같은 재생이지만 소리의 위치를 입력받은 포지션으로 이동
		void Loop(int index);											// 소리를 반복하여 재생
		void Pause(int index);											// 소리를 일시정지
		void Stop(int index);											// 소리를 정지

	private:
		SoundSystem();
		SoundSystem(const SoundSystem& soundClass);
		~SoundSystem();

		bool LoadWaveFile(std::string filename, IDirectSoundBuffer8** secondaryBuffer, bool is3D = false);
		bool Load3DWaveFile(std::string filename, IDirectSoundBuffer8** secondaryBuffer, IDirectSound3DBuffer8** secondary3DBuffer);
		void TerminateWaveFile(Sound* index);
		int FindIndex(const std::vector<Sound*>& vec, Sound* value);

	private:
		IDirectSound8* _directSound = nullptr;
		IDirectSoundBuffer* _primaryBuffer = nullptr;
		IDirectSound3DListener8* _listener = nullptr;
		std::vector<Sound*> _soundBuffer;
	};
}