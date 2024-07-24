#include "FMODInstance.h"
#include "../FloaterUtil/include/FloaterMacro.h"
#include "../FloaterUtil/include/ConvString.h"
#include "../External/include/fmod/fmod.hpp"
#include "../External/include/fmod/fmod_errors.h"
#include "Sound.h"

#pragma comment(lib, "../External/lib/x64/release/fmod_vc.lib")
#pragma comment(lib, "../External/lib/x64/release/fmodL_vc.lib")

flt::FMODInstance::FMODInstance()
	: _system(nullptr)
	, _channelGroups()
{

}

void flt::FMODInstance::Initialize(int fmodDriverIndex)
{
	FMOD_RESULT result;
	result = FMOD::System_Create(&_system);
	ASSERT(result == FMOD_OK, "");

	// 딜레이를 줄이기 위해 세팅. init하기 전에 세팅을 해줘야 함.
	result = _system->setDSPBufferSize(512, 2);
	ASSERT(result == FMOD_OK, "");

	result = _system->init(512, FMOD_INIT_NORMAL, 0);
	ASSERT(result == FMOD_OK, "");

	int channelGroupsCount = Sound::Category::COUNT;
	_channelGroups.resize(channelGroupsCount);

	_system->createChannelGroup(Sound::s_categoryName[Sound::Category::MASTER].c_str(), &_channelGroups[Sound::Category::MASTER]);
	for (int i = 0; i < Sound::Category::MASTER; ++i)
	{
		result = _system->createChannelGroup(Sound::s_categoryName[i].c_str(), &_channelGroups[i]);
		ASSERT(result == FMOD_OK, "");

		result = _channelGroups[Sound::Category::MASTER]->addGroup(_channelGroups[i]);
		ASSERT(result == FMOD_OK, "");
	}


	result = _system->setDriver(fmodDriverIndex);
	ASSERT(result == FMOD_OK, "");
}

void flt::FMODInstance::Finalize()
{
	_system->release();
}

void flt::FMODInstance::Update()
{
	_system->update();
}

bool flt::FMODInstance::CreateSound(void* buff, FMOD_CREATESOUNDEXINFO* exinfo, FMOD::Sound** sound)
{
	FMOD_RESULT result = _system->createSound((const char*)buff, FMOD_OPENMEMORY, exinfo, sound);
	ASSERT(result == FMOD_OK, "");
	return result == FMOD_OK;
}

bool flt::FMODInstance::CreateSound(const char* name, FMOD::Sound** sound)
{
	FMOD_RESULT result = _system->createSound(name, FMOD_DEFAULT, 0, sound);
	ASSERT(result == FMOD_OK, "");
	return result == FMOD_OK;
}

void flt::FMODInstance::Play(flt::Sound* sound, bool isLoop/*= false*/)
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

	FMOD_RESULT result = _system->playSound(sound->_fSound, _channelGroups[sound->_category], false, &sound->_channel);
	ASSERT(result == FMOD_OK, "재생 실패");
}

bool flt::FMODInstance::isPlay(Sound* sound)
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
			isPlaying = !isPlaying;
		}
	}

	return isPlaying;
}

void flt::FMODInstance::Pause(Sound* sound)
{
	FMOD_RESULT result = sound->_channel->setPaused(true);
}

void flt::FMODInstance::Stop(flt::Sound* sound)
{
	FMOD_RESULT result = sound->_channel->stop();
}

int flt::FMODInstance::GetDriverIndex()
{
	int index = -1;
	_system->getDriver(&index);

	return index;
}
