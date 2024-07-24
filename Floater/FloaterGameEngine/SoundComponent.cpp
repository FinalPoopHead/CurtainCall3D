#include "./include/internal/SoundComponent.h"
#include "./include/internal/GameEngine.h"
#include "../FloaterUtil/include/FloaterMacro.h"
#include "../FloaterUtil/include/ConvString.h"
#include "SoundEngine.h"
#include "Sound.h"
#include "../External/include/fmod/fmod.hpp"
#include <fstream>

flt::SoundComponent::SoundComponent() : 
	_soundEngine(GameEngine::Instance()->GetSoundEngine())
{
	_soundsPerSpeaker.resize(_soundEngine->GetCurrOutputDriverNum());
}

flt::SoundComponent::~SoundComponent()
{
}

void flt::SoundComponent::OnDestroy()
{
	for (int i = 0; i < _soundsPerSpeaker.size(); ++i)
	{
		for (auto& sound : _soundsPerSpeaker[i])
		{
			_soundEngine->Stop(sound.get(), i);
		}
	}

}

int flt::SoundComponent::AddSound(std::wstring path)
{
	for(int i = 0; i < _soundsPerSpeaker.size(); ++i)
	{
		Sound* sound = new Sound();

		bool isSuccess = _soundEngine->CreateSound(ToString(path).c_str(), &sound->_fSound, i);
		ASSERT(isSuccess, "Failed to create sound");

		_soundsPerSpeaker[i].emplace_back(sound);
	}
	return (int)_soundsPerSpeaker[0].size() - 1;
}

void flt::SoundComponent::Play(int index, bool isLoop/*= false*/)
{
	for(int i = 0; i < _soundsPerSpeaker.size(); ++i)
	{
		_soundEngine->Play(_soundsPerSpeaker[i][index].get(), isLoop, i);
	}
}

bool flt::SoundComponent::isPlay(int index)
{
	return _soundEngine->isPlay(_soundsPerSpeaker[0][index].get(), 0);
}

void flt::SoundComponent::Pause(int index)
{
	for (int i = 0; i < _soundsPerSpeaker.size(); ++i)
	{
		_soundEngine->Pause(_soundsPerSpeaker[i][index].get(), i);
	}
}

void flt::SoundComponent::Stop(int index)
{
	for (int i = 0; i < _soundsPerSpeaker.size(); ++i)
	{
		_soundEngine->Stop(_soundsPerSpeaker[i][index].get(), i);
	}
}
