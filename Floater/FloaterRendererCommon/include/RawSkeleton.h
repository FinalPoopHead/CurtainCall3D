#pragma once
#include "TransformOwner.h"
#include "RawAnimationClip.h"
#include "RawAnimation.h"
#include <vector>

namespace flt
{
	struct RawSkeleton
	{
		struct Bone : public TransformOwner
		{
			std::wstring name;
		};

		RawSkeleton() : bones(), boneOffsets(), rootBoneIndex(-1), animations() {}
		RawSkeleton(const RawSkeleton& other) : bones(other.bones), boneOffsets(other.boneOffsets), rootBoneIndex(other.rootBoneIndex), animations(other.animations)
		{
			bones.reserve(other.bones.size());

			for (int i = 0; i < (int)other.bones.size(); ++i)
			{
				auto& children = other.bones[i].transform.GetChildren();
				for (int j = 0; j < children.size(); ++j)
				{
					auto owner = children[j]->GetOwner();
					int childIndex = (int)((Bone*)owner - &other.bones[0]);
					bones[i].transform.AddChild(&bones[childIndex].transform);
				}
			}
		}

		RawSkeleton(RawSkeleton&& other) noexcept : bones(std::move(other.bones)), rootBoneIndex(other.rootBoneIndex), animations(std::move(other.animations)) {}

		std::vector<Bone> bones;
		std::vector<Matrix4f> boneOffsets;
		int rootBoneIndex;

		std::vector<RawAnimation> animations;
	};
}
