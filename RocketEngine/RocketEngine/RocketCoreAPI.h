#pragma once
#include "DLLExporter.h"

#include <string>
#include <unordered_map>
#include "RocketAudio.h"

/// 에디터에게 엔진의 기능을 제공하기 위해 한번 랩핑하여 export 하는 코드.
/// 에디터에게 Open하고 싶은 부분만 제공할 수 있도록 했다.
///
/// 23.08.02 강석원&오수안 인재원.
namespace Rocket::Core
{
	ROCKET_API std::unordered_map<std::string, RocketAudio>& GetSoundMapFromSoundSystem();
}