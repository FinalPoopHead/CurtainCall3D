#include "SoundEngine.h"
#include "../FloaterUtil/include/FloaterMacro.h"
#include "../External/include/fmod/fmod.hpp"
#include "../External/include/fmod/fmod_errors.h"
#include "Sound.h"

#pragma comment(lib, "../External/lib/x64/release/fmod_vc.lib")
#pragma comment(lib, "../External/lib/x64/release/fmodL_vc.lib")

flt::SoundEngine::SoundEngine()
	: _system(nullptr)
	, _channelGroups()
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

	_system->setDSPBufferSize(512, 2);

}

void flt::SoundEngine::Finalize()
{
	_system->release();
}

void flt::SoundEngine::Update()
{
	_system->update();
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

void flt::SoundEngine::Play(flt::Sound* sound, bool isLoop/*= false*/)
{
	sound->_fSound->setMode(isLoop ? FMOD_LOOP_NORMAL : FMOD_LOOP_OFF);
	sound->_isLoop = isLoop;

	FMOD::Channel* channel = sound->_channel;
	if (channel)
	{
		bool isPlaying = false;
		channel->isPlaying(&isPlaying);

		if (isPlaying)
		{
			bool ispaused = false;
			channel->getPaused(&ispaused);

			if (ispaused)
			{
				channel->setPaused(false);
				return;
			}

			FMOD_RESULT result = sound->_channel->stop();
		}
	}

	FMOD_RESULT result = _system->playSound(sound->_fSound, _channelGroups[sound->_category], false, &channel);
	ASSERT(result == FMOD_OK, "재생 실패");
}

bool flt::SoundEngine::isPlay(Sound* sound)
{
	ASSERT(sound, "sound is nullptr");

	FMOD::Channel* channel = sound->_channel;
	bool isPlaying = false;
	if (channel)
	{
		channel->isPlaying(&isPlaying);
		if (isPlaying)
		{
			channel->getPaused(&isPlaying);
		}
	}

	return isPlaying;
}

void flt::SoundEngine::Pause(Sound* sound)
{
	FMOD_RESULT result = sound->_channel->setPaused(true);
}

void flt::SoundEngine::Stop(flt::Sound* sound)
{
	FMOD_RESULT result = sound->_channel->stop();
}
