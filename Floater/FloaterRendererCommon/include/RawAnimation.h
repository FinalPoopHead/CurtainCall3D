#pragma once
#include "RawAnimationClip.h"

namespace flt
{
	struct RawAnimation
	{
		std::wstring name;
		float duration;
		float ticksPerSecond;
		std::vector<RawAnimationClip> clips;
	};
}
