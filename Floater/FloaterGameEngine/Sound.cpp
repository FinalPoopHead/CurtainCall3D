#include "Sound.h"
#include "../External/include/fmod/fmod.hpp"


flt::Sound::Sound() :
	_path(L""),
	_buffer(nullptr),
	_fSound(nullptr),
	_channel(nullptr),
	_category(Category::NONE),
	_isLoop(false)
{
}

void flt::Sound::SetLoop(bool isLoop)
{
	_isLoop = isLoop;
	_fSound->setMode(isLoop ? FMOD_LOOP_NORMAL : FMOD_LOOP_OFF);
}

bool flt::Sound::IsLoop() const
{
	return _isLoop;
}

void flt::Sound::SetCategory(Category category)
{
	_category = category;
}

flt::Sound::Category flt::Sound::GetCategory() const
{
	return _category;
}
