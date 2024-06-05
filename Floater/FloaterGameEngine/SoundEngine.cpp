#include "SoundEngine.h"
#include "../FloaterUtil/include/FloaterMacro.h"
#include "../External/include/fmod/fmod.hpp"
#include "../External/include/fmod/fmod_errors.h"
#include "Sound.h"

#pragma comment(lib, "../External/lib/x64/release/fmod_vc.lib")
#pragma comment(lib, "../External/lib/x64/release/fmodL_vc.lib")

flt::SoundEngine::SoundEngine()
{

}

void flt::SoundEngine::Initialize()
{
	FMOD_RESULT result;
	result = FMOD::System_Create(&_system);
	ASSERT(result == FMOD_OK, "");

	result = _system->init(512, FMOD_INIT_NORMAL, 0);
	ASSERT(result == FMOD_OK, "");

	result = _system->set3DSettings(1.0, DISTANCEFACTOR, 1.0f);
	ASSERT(result == FMOD_OK, "");

	int channelGroupsCount = Sound::Category::COUNT;
	_channelGroups.resize(channelGroupsCount);

	_system->createChannelGroup(Sound::s_categoryName[Sound::Category::MASTER].c_str(), &_channelGroups[Sound::Category::MASTER]);
	for (int i = 0; i < Sound::Category::MASTER; ++i)
	{
		_system->createChannelGroup(Sound::s_categoryName[i].c_str(), &_channelGroups[i]);
		_channelGroups[Sound::Category::MASTER]->addGroup(_channelGroups[i]);
	}


}

void flt::SoundEngine::Finalize()
{
	_system->release();
}

bool flt::SoundEngine::CreateSound(void* buff, FMOD_CREATESOUNDEXINFO* exinfo, FMOD::Sound** sound)
{
	FMOD_RESULT result = _system->createSound((const char*)buff, FMOD_OPENMEMORY, exinfo, sound);
	ASSERT(result == FMOD_OK, "");
	return result == FMOD_OK;
}

bool flt::SoundEngine::CreateSound(const char* name, FMOD::Sound** sound)
{
	FMOD_RESULT result = _system->createSound(name, FMOD_DEFAULT, 0, sound);
	ASSERT(result == FMOD_OK, "");
	return result == FMOD_OK;
}

void flt::SoundEngine::Play(flt::Sound* sound)
{
	_system->playSound(sound->_fSound, _channelGroups[sound->_category], false, &sound->_channel);
}
