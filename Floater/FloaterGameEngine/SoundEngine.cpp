#include "SoundEngine.h"
#include "../FloaterUtil/include/FloaterMacro.h"
#include "../External/include/fmod/fmod.hpp"
#include "../External/include/fmod/fmod_errors.h"

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

}

void flt::SoundEngine::Finalize()
{

}
