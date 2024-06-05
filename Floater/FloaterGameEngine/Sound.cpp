#include "Sound.h"
#include "../External/include/fmod/fmod.hpp"


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
