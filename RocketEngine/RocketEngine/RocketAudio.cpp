#include "RocketAudio.h"
#include "eSoundGroup.h"

#include "..\\SoundLib\\inc\\fmod.hpp"
#include "..\\SoundLib\\inc\\fmod_common.h"

namespace RocketCore
{
	struct RocketAudio
	{
	public:
		int soundNumber;
		const char* path;
		const char* soundName;
		FMOD_VECTOR transform;
		FMOD::Sound* sound;
		eSoundGroup group;
	};
}