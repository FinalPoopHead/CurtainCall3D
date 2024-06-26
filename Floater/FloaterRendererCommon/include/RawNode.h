﻿#pragma once
#include "TransformOwner.h"
#include "RawMesh.h"
#include "RawSkin.h"

#include <string>
#include <vector>
//#include <map>


namespace flt
{
	struct RawSkeleton;
	struct RawAnimationClip;
	class Camera;
	struct Light;

	struct RawNode : public TransformOwner
	{
		RawNode() : RawNode(L"") {}
		RawNode(const std::wstring& name);
		~RawNode();

		std::wstring name;
		//std::vector<Resource<RawMesh>> meshes;
		std::vector<RawMesh> meshes;
		RawSkeleton* skeleton;
		RawAnimationClip* animationClip;					// 노드 자체의 애니메이션 데이터
		Camera* camera;
		Light* light;

		RawNode* parent;
		std::vector<RawNode*> children;
	};
}
