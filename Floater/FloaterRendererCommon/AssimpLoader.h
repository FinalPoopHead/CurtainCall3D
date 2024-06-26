﻿#pragma once
#include <string>
#include <unordered_map>
//#include "../FloaterUtil/include/RBTree.h"
#include "./include/RawSkeleton.h"


struct aiNode;
struct aiMesh;

namespace flt
{
	struct RawScene;
	struct RawNode;
	struct RawSkeleton;
	struct RawMaterial;
	struct RawMesh;

	class AssimpLoader
	{
	public:
		~AssimpLoader();

		void Load(const std::wstring& filePath, RawScene* outRawScene);

	private:
		void SetAINodeMapRecursive(aiNode* pNode);
		void SetHierarchyRawNodeRecursive(aiNode* pNode, RawNode* pRawNode, RawScene* pRawScene);
		void SetSkeleton(aiNode* armature, RawSkeleton& skeleton);
		void SetSkeletonRecursive(aiNode* assimpBone, RawSkeleton& skeleton, int index);
		int CalcAINodeCountRecursive(aiNode* pNode);

		void PrintNodeNameRecursive(aiNode* pNode, int depth = 0);
		void PrintBoneNameRecursive(RawSkeleton::Bone& bone, int depth = 0);
		void ClearPrivateData();

		void CheckVertexBoneName(RawMesh* pRawMesh, aiMesh* pAiMesh);

	private:
		std::unordered_map<std::wstring, aiNode*> _aiNodeMap;
		std::unordered_map<std::wstring, RawNode*> _RawNodeMap;
		std::unordered_map<std::wstring, std::pair<RawSkeleton*, int>> _boneIndexMap;
		std::vector<RawMaterial*> _materials;
		std::vector<std::pair<RawMesh*, aiNode*>> _meshArmatures;
		std::unordered_map<aiNode*, RawSkeleton> _skeletonMap;
	};
}
