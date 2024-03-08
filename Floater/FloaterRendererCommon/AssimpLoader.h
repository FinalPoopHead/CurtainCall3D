#pragma once
#include <string>
#include <unordered_map>
//#include "../FloaterUtil/include/RBTree.h"

struct aiNode;

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
		void Load(const std::wstring& filePath, RawScene* outRawScene);

	private:
		void SetHierarchyRawNodeRecursive(aiNode* pNode, RawNode* pRawNode, RawScene* pRawScene);
		void SetRawMeshToRawNodeRecursive(aiNode* pNode, RawNode* outRawNode, RawScene* pRawScene);
		void PrintNodeNameRecursive(aiNode* pNode, int depth = 0);
		void SetSkeleton(aiNode* armature, RawSkeleton& skeleton);
		void SetSkeletonRecursive(aiNode* assimpBone, RawSkeleton& skeleton, int index);
		int CalcNodeCountRecursive(aiNode* pNode);

	private:
		std::unordered_map<std::wstring, RawNode*> _nodeMap;
		std::unordered_map<std::wstring, int> _boneNameIndexMap;
		std::unordered_map<aiNode*, RawSkeleton> _skeletonMap;
		std::vector<RawMaterial*> _materials;
		std::vector<RawMesh*> _meshes;
	};
}
