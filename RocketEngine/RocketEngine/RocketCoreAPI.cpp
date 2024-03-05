#include "RocketAPI.h"
#include "SoundSystem.h"

namespace Rocket::Core
{
	std::unordered_map<std::string, RocketAudio>& GetSoundMapFromSoundSystem()
	{
		return SoundSystem::Instance().GetSoundMap();
	}

}