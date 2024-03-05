#pragma once
#include "TransformOwner.h"
#include "RawMesh.h"
#include "RawSkin.h"
#include "RawSkeleton.h"
#include "RawAnimation.h"
#include "Camera.h"

#include <string>
#include <vector>
//#include <map>


namespace flt
{
	struct RawNode : public TransformOwner
	{
		RawNode() : RawNode(L"") {}
		RawNode(const std::wstring& name);
		~RawNode();

		std::wstring name;
		std::vector<Resource<RawMesh>> meshes;
		RawSkeleton* skeleton;
		Camera* camera;

		int boneIndex;

		RawNode* parent;
		std::vector<RawNode*> children;

		bool isDraw;
	};
}
