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

}

int flt::SoundComponent::AddSound(std::wstring path)
{
	Sound* sound = new Sound();

	void* buff;
	std::fstream file(path);
	file.seekg(0, file.end);
	int length = file.tellg();
	file.seekg(0, file.beg);
	buff = malloc(length);
	file.read((char*)buff, length);
	file.close();

	sound->_buffer = buff;

	FMOD_CREATESOUNDEXINFO exinfo{};
	exinfo.cbsize = sizeof(FMOD_CREATESOUNDEXINFO);
	exinfo.length = length;

	//bool isSuccess = _soundEngine->CreateSound(buff, &exinfo, &sound->_fSound);
	bool isSuccess = _soundEngine->CreateSound(ToString(path).c_str(), &sound->_fSound);
	ASSERT(isSuccess, "Failed to create sound");

	_sounds.push_back(sound);

	return _sounds.size() - 1;
}

void flt::SoundComponent::Play(int index)
{
	_soundEngine->Play(_sounds[index]);
}
