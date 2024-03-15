#pragma once
#include <vector>
#include <unordered_map>
#include "../FloaterRendererCommon/include/RawSkeleton.h"
#include "../FloaterRendererCommon/include/TransformOwner.h"
#include "DX11AnimationClip.h"

namespace flt
{
	struct DX11Skeleton
	{
		struct Bone : public TransformOwner
		{
			std::wstring name;
			Matrix4f boneOffset;
			std::vector<DX11AnimationClip> clip;
		};

		DX11Skeleton(RawSkeleton& rawSkeleton) :
			bones(rawSkeleton.bones.size())
		{
			int boneCount = rawSkeleton.bones.size();

			for (int i = 0; i < boneCount; i++)
			{
				bones[i].transform = rawSkeleton.bones[i].transform;
				bones[i].name = rawSkeleton.bones[i].name;
				bones[i].boneOffset = rawSkeleton.boneOffsets[i];

				bones[i].clip.resize(rawSkeleton.animations.size());
				for (int j = 0; j < rawSkeleton.animations.size(); ++j)
				{
					animationClipIndex[rawSkeleton.animations[j].name] = j;
					bones[i].clip[j] = rawSkeleton.animations[j].clips[i];
				}

				auto& children = rawSkeleton.bones[i].transform.GetChildren();
				for (int j = 0; j < children.size(); ++j)
				{
					auto owner = children[j]->GetOwner();
					int childIndex = (int)((RawSkeleton::Bone*)owner - &rawSkeleton.bones[0]);
					bones[i].transform.AddChild(&bones[childIndex].transform);
				}
			}
		}

		std::vector<Bone> bones;
		std::unordered_map<std::wstring, int> animationClipIndex;
	};
}
