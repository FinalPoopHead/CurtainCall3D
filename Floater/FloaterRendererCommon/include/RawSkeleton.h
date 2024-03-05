#pragma once
#include "Transform.h"
#include "RawAnimationClip.h"
#include <vector>

namespace flt
{
	struct RawSkeleton
	{
		struct Bone
		{
			std::wstring name;
			Transform transform;
		};

		std::vector<Bone> bones;
		int rootBoneIndex;

		std::vector<RawAnimationClip> clips;
	};
}
