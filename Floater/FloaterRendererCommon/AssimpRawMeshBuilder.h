#pragma once
#include "./include/RawMesh.h"
#include "./include/IBuilder.h"
#include <unordered_map>
#include <string>

struct aiMesh;

namespace flt
{
	struct RawNode;
	struct RawScene;

	template struct flt::IBuilder<flt::RawMesh>;

	struct AssimpRawMeshBuilder : public IBuilder<RawMesh>
	{
	public:
		AssimpRawMeshBuilder(std::unordered_map<std::wstring, RawNode*>& nodeMap);
		AssimpRawMeshBuilder(aiMesh* mesh, std::wstring filePath, int index, RawScene* rawScene, std::unordered_map<std::wstring, RawNode*>& nodeMap);

		virtual RawMesh* build() const override;

		aiMesh* mesh;
		RawScene* pRawScene;
		std::unordered_map<std::wstring, RawNode*>& nodeMap;
	};
}

