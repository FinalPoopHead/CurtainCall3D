#pragma once
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <string>

#include "RawModelStruct.h"

/// 그래픽스에 있는 FBXLoader를 이쪽으로 빼올 예정.
/// FBXLoader랑 그래픽스 엔진이랑 엮여있을 이유가 없다.
/// 여기서 데이터를 만들고 로드한 다음 내가 정의한 .rfbx(json?) 으로 저장하고 그걸 읽어서 활용한다.
/// 파일로 데이터 빼내는건 나중에해야되려나? 계층구조를 어떻게 빼내야하지?
/// 계층구조를 빼내려면 각자 id가 있어야되려나? 지금 index를 id처럼 쓸 수 있나?

namespace Rocket::Core
{
	class FBXLoaderToRaw
	{
	public:
		FBXLoaderToRaw();
		~FBXLoaderToRaw();

	public:
		RawModel* LoadFBXFile(std::string fileName);

	private:
		void ProcessNode(aiNode* ainode, const aiScene* scene);
		RawMesh* ProcessMesh(aiMesh* mesh, const aiScene* scene);
		RawMaterial* LoadMaterialTextures(aiMaterial* material, aiTextureType type, const aiScene* scene);
		void LoadAnimation(const aiScene* scene);

		RawNode* ReadNodeHierarchy(aiNode* ainode);
		void ReadNodeRecur(RawNode* node, aiNode* ainode, UINT& index);
	
	private:
		RawModel* _resultModel;
		std::unordered_map<aiNode*, RawNode*> _aiNodeToNodeMap;	// Bone에서 aiNode를 통해 Node를 찾기위한 맵


	private:
		void ForSungchan(RawNode* node);
		int _sungchanBoneCount;
	};
}
