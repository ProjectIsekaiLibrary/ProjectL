#include "SoundSystem.h"
#include <assert.h>
#include <algorithm>
#include <typeinfo>
#include <filesystem>

#define ASsert(formula, message) assert(formula && message)
#define Assert(message) assert(0 && message)

namespace KunrealEngine
{ 
	SoundSystem::SoundSystem()
	{
	}

	SoundSystem::SoundSystem(const SoundSystem& soundClass)
	{
	}

	SoundSystem::~SoundSystem()
	{
	}

	/////////////////////////////////
	/////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////

	SoundSystem& SoundSystem::GetInstance()
	{
		static SoundSystem* instance;

		if (instance == nullptr)
		{
			instance = new SoundSystem();
		}
		return *instance;
	}

	bool SoundSystem::Initialize(HWND hWnd)
	{
		HRESULT hr;

		if (FAILED(hr = DirectSoundCreate8(NULL, &_directSound, NULL)))
		{
			return false;
		}

		if (FAILED(hr = _directSound->SetCooperativeLevel(hWnd, DSSCL_PRIORITY)))
		{
			return false;
		}

		DSBUFFERDESC bufferDesc;
		bufferDesc.dwSize = sizeof(DSBUFFERDESC);
		bufferDesc.dwFlags = DSBCAPS_PRIMARYBUFFER | DSBCAPS_CTRLVOLUME | DSBCAPS_CTRL3D;
		bufferDesc.dwBufferBytes = 0;
		bufferDesc.dwReserved = 0;
		bufferDesc.lpwfxFormat = NULL;
		bufferDesc.guid3DAlgorithm = GUID_NULL;

		if (FAILED(_directSound->CreateSoundBuffer(&bufferDesc, &_primaryBuffer, NULL)))
		{
			return false;
		}

		// Set the wave format of secondary buffer that this wave file will be loaded onto.
		WAVEFORMATEX waveFormat;
		waveFormat.wFormatTag = WAVE_FORMAT_PCM;
		waveFormat.nSamplesPerSec = 44100;
		waveFormat.wBitsPerSample = 16;
		waveFormat.nChannels = 2;
		waveFormat.nBlockAlign = (waveFormat.wBitsPerSample / 8) * waveFormat.nChannels;
		waveFormat.nAvgBytesPerSec = waveFormat.nSamplesPerSec * waveFormat.nBlockAlign;
		waveFormat.cbSize = 0;

		if (FAILED(_primaryBuffer->SetFormat(&waveFormat)))
		{
			return false;
		}

		if (FAILED(_primaryBuffer->QueryInterface(IID_IDirectSound3DListener8, (LPVOID*)&_listener)))
		{
			return false;
		}

		_listener->SetPosition(0.0f, 0.0f, 0.0f, DS3D_IMMEDIATE);

		return true;
	}

	void SoundSystem::Release()
	{
		ClearAllSound();

		if (_listener)
		{
			_listener->Release();
			_listener = 0;
		}

		if (_primaryBuffer)
		{
			_primaryBuffer->Release();
			_primaryBuffer = 0;
		}
	}

	/////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////

	int SoundSystem::AddSound(std::string filename, int volume, int index)
	{
		int soundvolume = 0;
		if (volume > 100 || volume < 0)
		{
			Assert("volume setting failed. volume is set over 100 or under 0");
		}
		else
		{
			soundvolume = (100 - volume) * -100;
		}

		Sound* soundstruct = new Sound;
		IDirectSoundBuffer8* tempsound;
		LoadWaveFile(filename, &tempsound);

		soundstruct->fileName = filename;
		soundstruct->soundBuffer = tempsound;
		soundstruct->volume = volume;

		if (FAILED(tempsound->SetCurrentPosition(0)))
		{
			Assert("PlayWaveFile function -> SetCurrentPosition 01 is failed");
		}

		if (FAILED(tempsound->SetVolume(soundvolume)))
		{
			Assert("PlayWaveFile function -> SetVolume 01 is failed");
		}

		if (index == -1)
		{
			_soundBuffer.push_back(soundstruct);
			int returnValue = FindIndex(_soundBuffer, soundstruct);
			return returnValue;
		}
		else
		{
			_soundBuffer.insert(_soundBuffer.begin() + index, soundstruct);
			return index;
		}
	}

	int SoundSystem::Add3DSound(std::string filename, int volume, int index, int xpos, int ypos, int zpos)
	{
		int soundvolume = 0;
		if (volume > 100 || volume < 0)
		{
			Assert("volume setting failed. volume is set over 100 or under 0");
		}
		else
		{
			soundvolume = (100 - volume) * -100;
		}

		Sound* soundstruct = new Sound;
		IDirectSound3DBuffer8* temp3DSound;
		IDirectSoundBuffer8* tempsound;
		Load3DWaveFile(filename, &tempsound, &temp3DSound);

		soundstruct->fileName = filename;
		soundstruct->soundBuffer = tempsound;
		soundstruct->sound3DBuffer = temp3DSound;
		soundstruct->volume = volume;

		if (FAILED(tempsound->SetCurrentPosition(0)))
		{
			Assert("PlayWaveFile function -> SetCurrentPosition 01 is failed");
		}

		if (FAILED(tempsound->SetVolume(soundvolume)))
		{
			Assert("PlayWaveFile function -> SetVolume 01 is failed");
		}

		soundstruct->sound3DBuffer->SetPosition(xpos, ypos, zpos, DS3D_IMMEDIATE);

		if (index == -1)
		{
			_soundBuffer.push_back(soundstruct);
			int returnValue = FindIndex(_soundBuffer, soundstruct);
			return returnValue;
		}
		else
		{
			_soundBuffer.insert(_soundBuffer.begin() + index, soundstruct);
			return index;
		}
	}

	void SoundSystem::RemoveSound(int index)
	{
		TerminateWaveFile(_soundBuffer[index]);
		_soundBuffer.erase(_soundBuffer.begin() + index);
	}

	void SoundSystem::ClearAllSound()
	{
		for (auto sound : _soundBuffer)
		{
			TerminateWaveFile(sound);
		}
		_soundBuffer.clear();
	}

	void SoundSystem::Setvolume(int index, int volume)
	{
		int soundvolume = 0;
		if (volume > 100 || volume < 0)
		{
			Assert("volume setting failed. volume is set over 100 or under 0");
		}
		else
		{
			soundvolume = (100 - volume) * -100;
		}

		_soundBuffer[index]->volume = soundvolume;

		if (FAILED(_soundBuffer[index]->soundBuffer->SetVolume(soundvolume)))
		{
			Assert("PlayWaveFile function -> SetVolume 01 is failed");
			return;
		}
	}

	int SoundSystem::Change3DorMono(int index)
	{
		if (_soundBuffer[index]->sound3DBuffer != nullptr)	// sound3DBuffer가 비어있지 않다 = 3D사운드다
		{ 
			int result = 0;
			std::string filename = _soundBuffer[index]->fileName;
			int volume = _soundBuffer[index]->volume;
			RemoveSound(index);
			result = AddSound(filename, volume, index);
			return result;
		}

		else if (_soundBuffer[index]->sound3DBuffer == nullptr) // sound3DBuffer가 비어있다면 = 2D사운드다
		{
			int result = 0;
			std::string filename = _soundBuffer[index]->fileName;
			int volume = _soundBuffer[index]->volume;
			RemoveSound(index);
			result = Add3DSound(filename, volume, index);
			return result;
		}
	}

	int SoundSystem::GetSoundListSize()
	{
		return _soundBuffer.size();
	}

	std::vector<std::string> KunrealEngine::SoundSystem::GetSoundPathList()
	{
		namespace fs = std::filesystem;
		std::string directory = "Resources/Sound";
		std::vector<std::string> fileNames;

		// Iterate over the files in the directory
		for (const auto& entry : fs::directory_iterator(directory)) {
			// Check if it's a regular file
			if (fs::is_regular_file(entry)) {
				// Get the filename from the path and add it to the vector
				fileNames.push_back(entry.path().filename().string());
			}
		}

		return fileNames;
	}

	/////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////

	void SoundSystem::updateSoundPosition(int index, float x, float y, float z)
	{
		_soundBuffer[index]->sound3DBuffer->SetPosition(x, y, z, DS3D_IMMEDIATE);
	}

	void SoundSystem::updateListenerPosition(float x, float y, float z)
	{
		_listener->SetPosition(x, y, z, DS3D_IMMEDIATE);
	}

	void SoundSystem::Play(int index)
	{
		_soundBuffer[index]->soundBuffer->Play(0, 0, 0);
	}

	void SoundSystem::PlayWithPos(int index, int xpos, int ypos, int zpos)
	{
		_soundBuffer[index]->sound3DBuffer->SetPosition(xpos, ypos, zpos, DS3D_IMMEDIATE);
		_soundBuffer[index]->soundBuffer->Play(0, 0, 0);
	}

	void SoundSystem::Loop(int index)
	{
		_soundBuffer[index]->soundBuffer->Play(0, 0, DSBPLAY_LOOPING);
	}

	void SoundSystem::Pause(int index)
	{
		// 사운드 정지
		_soundBuffer[index]->soundBuffer->Stop();
	}

	void SoundSystem::Stop(int index)
	{
		// 사운드 정지
		_soundBuffer[index]->soundBuffer->Stop();

		// 재생 위치 초기화
		_soundBuffer[index]->soundBuffer->SetCurrentPosition(0);
	}

	/////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////

	bool SoundSystem::LoadWaveFile(std::string filename, IDirectSoundBuffer8** secondaryBuffer, bool is3D)
	{
		// Open the wave file in binary.
		FILE* filePtr = nullptr;
		int error = fopen_s(&filePtr, filename.c_str(), "rb");
		if (error != 0)
		{
			Assert("LoadWaveFile function -> fopen_s is failed");
			return false;
		}

		// Read in the wave file header.
		WaveHeaderType waveFileHeader;
		unsigned int count = (unsigned int)fread(&waveFileHeader, sizeof(waveFileHeader), 1, filePtr);
		if (count != 1)
		{
			Assert("LoadWaveFile function -> fread is failed");
			return false;
		}

		// LIST 청크가 있다면 건너뛰기
		if (strncmp(waveFileHeader.dataChunkId, "LIST", 4) == 0)
		{
			fseek(filePtr, waveFileHeader.dataSize, SEEK_CUR);

			// "data" 청크 정보 읽기
			count = (unsigned int)fread(&waveFileHeader.dataChunkId, sizeof(waveFileHeader.dataChunkId), 1, filePtr);
			if (count != 1)
			{
				// fread 실패
				Assert("LoadWaveFile function -> fread is failed");
				fclose(filePtr);
				return false;
			}

			count = (unsigned int)fread(&waveFileHeader.dataSize, sizeof(waveFileHeader.dataSize), 1, filePtr);
			if (count != 1)
			{
				// fread 실패
				Assert("LoadWaveFile function -> fread is failed");
				fclose(filePtr);
				return false;
			}
		}

		// Check that the chunk ID is the RIFF format.
		if ((waveFileHeader.chunkId[0] != 'R') || (waveFileHeader.chunkId[1] != 'I') ||
			(waveFileHeader.chunkId[2] != 'F') || (waveFileHeader.chunkId[3] != 'F'))
		{
			return false;
		}

		// Check that the file format is the WAVE format.
		if ((waveFileHeader.format[0] != 'W') || (waveFileHeader.format[1] != 'A') ||
			(waveFileHeader.format[2] != 'V') || (waveFileHeader.format[3] != 'E'))
		{
			return false;
		}

		// Check that the sub chunk ID is the fmt format.
		if ((waveFileHeader.subChunkId[0] != 'f') || (waveFileHeader.subChunkId[1] != 'm') ||
			(waveFileHeader.subChunkId[2] != 't') || (waveFileHeader.subChunkId[3] != ' '))
		{
			return false;
		}

		// Check that the audio format is WAVE_FORMAT_PCM.
		if (waveFileHeader.audioFormat != WAVE_FORMAT_PCM)
		{
			return false;
		}

		if (waveFileHeader.sampleRate != 48000 && waveFileHeader.sampleRate != 44100)
		{
			Assert("해당 .wav 파일의 샘플레이트는 지원 하지 않습니다. \n48000 혹은 44100으로 맞춰주세요.");
			return false;
		}

		// Ensure that the wave file was recorded in 16 bit format.
		if (waveFileHeader.bitsPerSample != 16)
		{
			Assert("해당 .wav 파일의 샘플 당 비트를 지원하지 않습니다. \n16bit만 지원합니다.");
			return false;
		}

		// Check for the data chunk header.
		if ((waveFileHeader.dataChunkId[0] != 'd') || (waveFileHeader.dataChunkId[1] != 'a') ||
			(waveFileHeader.dataChunkId[2] != 't') || (waveFileHeader.dataChunkId[3] != 'a'))
		{
			return false;
		}

		// Set the wave format of secondary buffer that this wave file will be loaded onto.
		WAVEFORMATEX waveFormat;
		waveFormat.wFormatTag = WAVE_FORMAT_PCM;
		if (waveFileHeader.sampleRate == 48000)
			waveFormat.nSamplesPerSec = 48000;
		else if (waveFileHeader.sampleRate == 44100)
			waveFormat.nSamplesPerSec = 44100;
		waveFormat.wBitsPerSample = 16;
		waveFormat.cbSize = 0;

		// Set the buffer description of the secondary sound buffer that the wave file will be loaded onto.
		DSBUFFERDESC bufferDesc;
		bufferDesc.dwSize = sizeof(DSBUFFERDESC);
		bufferDesc.dwBufferBytes = waveFileHeader.dataSize;
		bufferDesc.dwReserved = 0;
		bufferDesc.lpwfxFormat = &waveFormat;
		bufferDesc.guid3DAlgorithm = GUID_NULL;

		//Check that the wave file was recorded in stereo format.
		if (waveFileHeader.numChannels == 2 && (is3D == false))
		{
			waveFormat.nChannels = 2;
			bufferDesc.dwFlags = DSBCAPS_CTRLVOLUME;
			waveFormat.nBlockAlign = (waveFormat.wBitsPerSample / 8) * waveFormat.nChannels;
			waveFormat.nAvgBytesPerSec = waveFormat.nSamplesPerSec * waveFormat.nBlockAlign;
		}

		else if (waveFileHeader.numChannels == 2 && (is3D == true))
		{
			Assert("This .wav file is not possible to play in 3D. \n check channel of .wav ");
		}

		else if (waveFileHeader.numChannels == 1 && (is3D == false))
		{
			waveFormat.nChannels = 1;
			bufferDesc.dwFlags = DSBCAPS_CTRLVOLUME;
			waveFormat.nBlockAlign = (waveFormat.wBitsPerSample / 8) * waveFormat.nChannels;
			waveFormat.nAvgBytesPerSec = waveFormat.nSamplesPerSec * waveFormat.nBlockAlign;
		}

		else if (waveFileHeader.numChannels == 1 && (is3D == true))
		{
			waveFormat.nChannels = 1;
			bufferDesc.dwFlags = DSBCAPS_CTRLVOLUME | DSBCAPS_CTRL3D;
			waveFormat.nBlockAlign = (waveFormat.wBitsPerSample / 8) * waveFormat.nChannels;
			waveFormat.nAvgBytesPerSec = waveFormat.nSamplesPerSec * waveFormat.nBlockAlign;
		}

		else
		{
			Assert("this sound is not 2 or 1 channels");
			return false;
		}

		HRESULT hr;

		// Create a temporary sound buffer with the specific buffer settings.
		IDirectSoundBuffer* tempBuffer = nullptr;
		if (FAILED(hr = _directSound->CreateSoundBuffer(&bufferDesc, &tempBuffer, NULL)))
		{
			Assert("LoadWaveFile function -> CreateSoundBuffer is failed");
			return false;
		}

		// Test the buffer format against the direct sound 8 interface and create the secondary buffer.
		if (FAILED(tempBuffer->QueryInterface(IID_IDirectSoundBuffer8, (void**)&*secondaryBuffer)))
		{
			Assert("LoadWaveFile function -> QueryInterface is failed");
			return false;
		}

		// Release the temporary buffer.
		tempBuffer->Release();
		tempBuffer = 0;

		// Move to the beginning of the wave data which starts at the end of the data chunk header.
		fseek(filePtr, sizeof(WaveHeaderType), SEEK_SET);

		// Create a temporary buffer to hold the wave file data.
		unsigned char* waveData = new unsigned char[waveFileHeader.dataSize];
		if (!waveData)
		{
			Assert("LoadWaveFile function -> Create a temporary buffer to hold the wave file data is failed");
			return false;
		}

		// Read in the wave file data into the newly created buffer.
		count = fread(waveData, 1, waveFileHeader.dataSize, filePtr);
		if (count != waveFileHeader.dataSize)
		{
			Assert("LoadWaveFile function -> fread is failed");
			return false;
		}

		// Close the file once done reading.
		error = fclose(filePtr);
		if (error != 0)
		{
			Assert("LoadWaveFile function -> fclose is failed");
			return false;
		}

		// Lock the secondary buffer to write wave data into it.
		unsigned char* bufferPtr = nullptr;
		unsigned long bufferSize = 0;
		if (FAILED((*secondaryBuffer)->Lock(0, waveFileHeader.dataSize, (void**)&bufferPtr, (DWORD*)&bufferSize, NULL, 0, 0)))
		{
			Assert("LoadWaveFile function -> Lock is failed");
			return false;
		}

		// Copy the wave data into the buffer.
		memcpy(bufferPtr, waveData, waveFileHeader.dataSize);

		// Unlock the secondary buffer after the data has been written to it.
		if (FAILED((*secondaryBuffer)->Unlock((void*)bufferPtr, bufferSize, NULL, 0)))
		{
			Assert("LoadWaveFile function -> Unlock is failed");
			return false;
		}

		// Release the wave data since it was copied into the secondary buffer.
		delete[] waveData;
		waveData = 0;

		return true;
	}

	bool SoundSystem::Load3DWaveFile(std::string filename, IDirectSoundBuffer8** secondaryBuffer, IDirectSound3DBuffer8** secondary3DBuffer)
	{
		HRESULT hr;
		LoadWaveFile(filename, secondaryBuffer, true);

		if (FAILED(hr = (*secondaryBuffer)->QueryInterface(IID_IDirectSound3DBuffer8, (void**)&*secondary3DBuffer)))
		{
			return false;
		}

		return true;
	}

	void SoundSystem::TerminateWaveFile(Sound* index)
	{
		if (index != 0)
		{
			index->soundBuffer->Release();
			index->soundBuffer = nullptr;

			if (index->sound3DBuffer != nullptr)
			{
				index->sound3DBuffer->Release();
				index->sound3DBuffer = nullptr;
			}
		}

		delete index;
	}

	int SoundSystem::FindIndex(const std::vector<Sound*>& vec, Sound* value)
	{
		auto it = std::find(vec.begin(), vec.end(), value);

		if (it != vec.end())
		{
			// 해당 값이 벡터 내에서 찾아진 경우
			return std::distance(vec.begin(), it);
		}
		else
		{
			// 해당 값이 벡터 내에서 찾아지지 않은 경우, 예외 처리 등을 수행할 수 있음
			return -1; // 또는 다른 값을 사용하여 실패를 나타낼 수 있음
		}
	}
}