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

		RawSkeleton() : bones(), rootBoneIndex(-1), clips() {}
		RawSkeleton(const RawSkeleton& other) : bones(other.bones), rootBoneIndex(other.rootBoneIndex), clips(other.clips)
		{
			bones.reserve(other.bones.size());

			for (int i = 0; i < (int)other.bones.size(); ++i)
			{
				auto& children = other.bones[i].transform.GetChildren();
				for (int j = 0; j < children.size(); ++j)
				{
					int childIndex = (int)(children[j] - &other.bones[0].transform);
					bones[i].transform.AddChild(&bones[childIndex].transform);
				}
			}
		}

		RawSkeleton(RawSkeleton&& other) noexcept : bones(std::move(other.bones)), rootBoneIndex(other.rootBoneIndex), clips(std::move(other.clips)) {}

		std::vector<Bone> bones;
		int rootBoneIndex;

		std::vector<RawAnimationClip> clips;
	};
}
