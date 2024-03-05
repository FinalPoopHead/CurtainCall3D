#pragma once
#include <d3d11_2.h>
#include <dxgi1_3.h>
#include <wrl.h>
#include <string>
#include <assimp/Importer.hpp>		// C++ importer interface
#include <assimp/scene.h>			// Output data structure
#include <assimp/postprocess.h>		// Post processing flags

using Microsoft::WRL::ComPtr;

namespace Rocket::Core
{
	struct ModelData;
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
		void ProcessNode(Node* node, aiNode* ainode, const aiScene* scene);
		Mesh* ProcessMesh(aiMesh* mesh, const aiScene* scene);
		Mesh* ProcessStaticMesh(aiMesh* mesh, const aiScene* scene);
		void ProcessSkinnedMesh(aiMesh* mesh, const aiScene* scene);
		void LoadMaterialTextures(aiMaterial* material, aiTextureType type, const aiScene* scene);
		ID3D11ShaderResourceView* LoadEmbeddedTexture(const aiTexture* embeddedTexture);
		void LoadAnimation(const aiScene* scene);

		void SetNodeIndex(UINT& index, Node* node);

	private:
		ComPtr<ID3D11Device> _device;
		ModelData* _nowModelData;
	};
}
