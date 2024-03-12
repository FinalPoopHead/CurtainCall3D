#pragma once
#include <vector>
#include "../FloaterRendererCommon/include/RawSkeleton.h"

namespace flt
{
	struct DX11Skeleton
	{
		DX11Skeleton(RawSkeleton& rawSkeleton) :
			boneOffsetMatrix(rawSkeleton.boneOffsets),
			bones(rawSkeleton.bones.size()),
			clips(rawSkeleton.clips)
		{
			int boneCount = rawSkeleton.bones.size();
			for(int i = 0; i < boneCount; i++)
			{
				bones[i] = rawSkeleton.bones[i].transform;
				//boneOffsetMatrix[i] = rawSkeleton.bones[i].transform.GetWorldMatrix4f() * rawSkeleton.bones[rawSkeleton.rootBoneIndex].transform.GetWorldMatrix4f().Inverse();
				//boneOffsetMatrix[i] = boneOffsetMatrix[i].Inverse();
				auto& children = rawSkeleton.bones[i].transform.GetChildren();
				for (int j = 0; j < children.size(); ++j)
				{
					auto owner = children[j]->GetOwner();
					int childIndex = (int)((RawSkeleton::Bone*)owner - &rawSkeleton.bones[0]);
					bones[i].AddChild(&bones[childIndex]);
				}
			}
		}

		std::vector<Matrix4f> boneOffsetMatrix;
		std::vector<Transform> bones;
		std::vector<RawAnimationClip> clips;
	};
}


