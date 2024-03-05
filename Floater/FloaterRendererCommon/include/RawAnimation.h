#pragma once
#include "RawAnimationClip.h"

namespace flt
{
	struct RawAnimation
	{
		std::wstring name;
		std::vector<RawAnimationClip> clips;
	};
}
