#pragma once
#include <d3d11_2.h>
#include <dxgi1_3.h>
#include <wrl.h>
#include <string>
#include <assimp/Importer.hpp>		// C++ importer interface
#include <assimp/scene.h>			// Output data structure
#include <assimp/postprocess.h>		// Post processing flags
#include <unordered_map>

#include "ModelStruct.h"

using Microsoft::WRL::ComPtr;

namespace Rocket::Core
{
	struct Node;
	class Mesh;

	class FBXLoader
	{
	public:
		FBXLoader();
		void Initialize(ID3D11Device* device);

	public:
		void LoadFBXFile(std::string fileName);

	private:
		void ProcessModel(aiNode* rootaiNode, const aiScene* scene);
		void ProcessNode(aiNode* ainode, const aiScene* scene);
		Mesh* ProcessMesh(aiMesh* mesh, const aiScene* scene);
		Mesh* ProcessStaticMesh(aiMesh* mesh, const aiScene* scene);
		Mesh* ProcessSkinnedMesh(aiMesh* mesh, const aiScene* scene);
		void LoadMaterialTextures(aiMaterial* material, aiTextureType type, const aiScene* scene);
		ID3D11ShaderResourceView* LoadEmbeddedTexture(const aiTexture* embeddedTexture);
		void LoadAnimation(const aiScene* scene);

		Node* ReadNodeHierarchy(aiNode* ainode, const aiScene* scene);
		void ReadNodeRecur(Node* node, aiNode* ainode, const aiScene* scene, UINT& index);

	private:
		ComPtr<ID3D11Device> _device;
		// TODO : 지금은 nowModel을 전역적으로 사용하는데 반환값을 이용해서 깔끔하게 정리하는게 낫지않을까?
		Model* _nowModel;
		std::unordered_map<aiNode*, Node*> _aiNodeToNodeMap;	// Bone에서 Node를 찾기위한 맵
		std::unordered_map<std::string, Bone*> _boneMap;
	};
}
