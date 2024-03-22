#include <windows.h>

#include "FBXLoader.h"
#include "MathHeader.h"
#include "AssimpMathConverter.h"
#include "VertexStruct.h"
#include "Mesh.h"
#include "StaticMesh.h"
#include "SkinnedMesh.h"
#include "ResourceManager.h"
#include "GraphicsMacro.h"
#include "Texture.h"

const std::string MODEL_PATH = "Resources/Models/";

namespace Rocket::Core
{
	FBXLoader::FBXLoader()
		: _device(),
		_nowModel(nullptr),
		_sungchanBoneCount(0)
	{

	}

	void FBXLoader::Initialize(ID3D11Device* device)
	{
		// TODO : 이거 왜 NULL 에러나지?
		_device = device;
	}


	// TODO : 지금은 읽으면서 처리하고 있는데, 미리 다 읽어놓고 데이터 별로 넣어주는 식으로 하는게 더 좋을 거 같다.
	//		  ㄴ> 읽으면서 조건따라 처리하는게 너무 지저분해 보인다.
	//		  ㄴ> 그리고 매개변수랑 리턴타입을 이용해서 처리하는게 더 직관적으로 보일 거 같긴 한데.. 흠..
	void FBXLoader::LoadFBXFile(std::string fileName)
	{
		std::string fileNameWithExtension;

		/// 경로 제외하기 위한 로직
		size_t slashIndex = fileName.find_last_of("/\\");
		if (slashIndex != std::string::npos)
		{
			fileNameWithExtension = fileName.substr(slashIndex + 1, fileName.length() - slashIndex);
		}
		else
		{
			fileNameWithExtension = fileName;
		}

		std::string path = MODEL_PATH + fileNameWithExtension;

		Assimp::Importer importer;

		const aiScene* _scene = importer.ReadFile(path,
			aiProcess_Triangulate |
			aiProcess_ConvertToLeftHanded |
			aiProcess_PopulateArmatureData |
			aiProcess_CalcTangentSpace |
			aiProcess_LimitBoneWeights
		);

		if (_scene == nullptr || _scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || _scene->mRootNode == nullptr)
		{
			MessageBox(NULL, L"Model file couldn't be loaded", L"Error!", MB_ICONERROR | MB_OK);
			assert(false);
		}

		// Node를 Process 하면서 이 ModelData에 저장
		if (_scene->HasAnimations())
		{
			DynamicModel* dynamicModel = new DynamicModel();
			dynamicModel->name = fileNameWithExtension;
			_nowModel = dynamicModel;
		}
		else
		{
			StaticModel* staticModel = new StaticModel();
			staticModel->name = fileNameWithExtension;
			_nowModel = staticModel;
		}

		ProcessModel(_scene->mRootNode, _scene);	// 모델 데이터 로드 (모델,메쉬,노드,본)
		//ForSungchan(_nowModel->rootNode);


		if (_scene->HasAnimations())
		{
			LoadAnimation(_scene);						// 애니메이션 데이터 로드
		}

		// 모든 작업이 끝나면 리소스매니저에 해당 모델 데이터 등록
		ResourceManager::Instance()._models.insert({ fileNameWithExtension, _nowModel });
	}


	void FBXLoader::ProcessModel(aiNode* rootaiNode, const aiScene* scene)
	{
		_nowModel->rootNode = ReadNodeHierarchy(rootaiNode, scene);

		ProcessNode(rootaiNode, scene);

		for (auto& mesh : _nowModel->GetMeshes())
		{
			mesh->CreateBuffers();	// vertexBuffer와 indexBuffer 생성.
		}
	}

	void FBXLoader::ProcessNode(aiNode* ainode, const aiScene* scene)
	{
		for (UINT i = 0; i < ainode->mNumMeshes; ++i)
		{
			Mesh* mesh = ProcessMesh(scene->mMeshes[ainode->mMeshes[i]], scene);
			
			// TODO : 여기서 if문으로 분기타지않게 할 것.
			// TODO : reinterpret_cast 및 dynamic_cast 사용하지 않도록 수정하기.
			if (!scene->HasAnimations())
			{
				// TODO : SetNode할때 각각의 vertex한테도 nodeIndex 넣어주면 좋을거같은데?
				mesh->SetNode(_aiNodeToNodeMap.at(ainode));
				auto staticMesh = dynamic_cast<StaticMesh*>(mesh);
				reinterpret_cast<StaticModel*>(_nowModel)->meshes.emplace_back(staticMesh);
			}
			else
			{
				// 얘는 메쉬랑 본 읽으면서 각각의 버텍스한테 노드를 셋 해줬을것이다.
				auto skinnedMesh = dynamic_cast<SkinnedMesh*>(mesh);
				reinterpret_cast<DynamicModel*>(_nowModel)->meshes.emplace_back(skinnedMesh);
			}
			//_nowModel->meshes.emplace_back(mesh);
		}

		for (UINT i = 0; i < ainode->mNumChildren; ++i)
		{
			ProcessNode(ainode->mChildren[i], scene);
		}
	}

	Mesh* FBXLoader::ProcessMesh(aiMesh* mesh, const aiScene* scene)
	{
		if (scene->HasAnimations())
		{
			
			return ProcessSkinnedMesh(mesh, scene);
		}
		else
		{
			return ProcessStaticMesh(mesh, scene);
		}
	}

	Mesh* FBXLoader::ProcessStaticMesh(aiMesh* mesh, const aiScene* scene)
	{		
		std::vector<Vertex> vertices;
		std::vector<UINT> indices;

		// Load position, normal, uv
		for (UINT i = 0; i < mesh->mNumVertices; ++i)
		{
			Vertex vertex;

			// process position
			vertex.position.x = mesh->mVertices[i].x;
			vertex.position.y = mesh->mVertices[i].y;
			vertex.position.z = mesh->mVertices[i].z;

			// process normal
			vertex.normal.x = mesh->mNormals[i].x;
			vertex.normal.y = mesh->mNormals[i].y;
			vertex.normal.z = mesh->mNormals[i].z;

			// process uv
			vertex.UV.x = mesh->mTextureCoords[0][i].x;
			vertex.UV.y = mesh->mTextureCoords[0][i].y;

			vertex.tangent.x = mesh->mTangents[i].x;
			vertex.tangent.y = mesh->mTangents[i].y;
			vertex.tangent.z = mesh->mTangents[i].z;

			vertices.push_back(vertex);
		}

		// Load indices
		for (UINT i = 0; i < mesh->mNumFaces; ++i)
		{
			aiFace& face = mesh->mFaces[i];

			for (UINT j = 0; j < face.mNumIndices; ++j)
			{
				indices.push_back(face.mIndices[j]);
			}
		}

		/// 메타 데이터 활용하는 부분인 것 같은데 우선 주석
		/*

		int upAxis = 0;
		scene->mMetaData->Get<int>("UpAxis", upAxis);
		int upAxisSign = 1;
		scene->mMetaData->Get<int>("UpAxisSign", upAxisSign);
		int frontAxis = 0;
		scene->mMetaData->Get<int>("FrontAxis", frontAxis);
		int frontAxisSign = 1;
		scene->mMetaData->Get<int>("FrontAxisSign", frontAxisSign);
		int coordAxis = 0;
		scene->mMetaData->Get<int>("CoordAxis", coordAxis);
		int coordAxisSign = 1;
		scene->mMetaData->Get<int>("CoordAxisSign", coordAxisSign);

		int originalUpAxis = 0;
		scene->mMetaData->Get<int>("OriginalUpAxis", originalUpAxis);
		int originalUpAxisSign = 1;
		scene->mMetaData->Get<int>("OriginalUpAxisSign", originalUpAxisSign);

		float unitScaleFactor = 1.0f;
		scene->mMetaData->Get<float>("UnitScaleFactor", unitScaleFactor);
		float originalUnitScaleFactor = 1.0f;
		scene->mMetaData->Get<float>("OriginalUnitScaleFactor", originalUnitScaleFactor);

		aiVector3D upVec = upAxis == 0 ? aiVector3D(upAxisSign, 0, 0) : upAxis == 1 ? aiVector3D(0, upAxisSign, 0) : aiVector3D(0, 0, upAxisSign);
		aiVector3D forwardVec = frontAxis == 0 ? aiVector3D(frontAxisSign, 0, 0) : frontAxis == 1 ? aiVector3D(0, frontAxisSign, 0) : aiVector3D(0, 0, frontAxisSign);
		aiVector3D rightVec = coordAxis == 0 ? aiVector3D(coordAxisSign, 0, 0) : coordAxis == 1 ? aiVector3D(0, coordAxisSign, 0) : aiVector3D(0, 0, coordAxisSign);

		upVec *= unitScaleFactor;
		forwardVec *= unitScaleFactor;
		rightVec *= unitScaleFactor;

		// aiMatrix4x4 mat(
		//	rightVec.x, rightVec.y, rightVec.z, 0.0f,
		//	-upVec.x, -upVec.y, -upVec.z, 0.0f,
		//	forwardVec.x, forwardVec.y, forwardVec.z, 0.0f,
		//	0.0f, 0.0f, 0.0f, 1.0f);

		aiMatrix4x4 mat(
			rightVec.x, forwardVec.x, -upVec.x, 0.0f,
			rightVec.y, forwardVec.y, -upVec.y, 0.0f,
			rightVec.z, forwardVec.z, -upVec.z, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f);
*/

		if (mesh->mMaterialIndex >= 0)
		{
			aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
			for (UINT i = 0; i <= aiTextureType_UNKNOWN; ++i)
			{
				LoadMaterialTextures(material, (aiTextureType)i, scene);
			}
		}

		StaticMesh* newMesh = new StaticMesh(vertices, indices);

		return newMesh;
	}

	Mesh* FBXLoader::ProcessSkinnedMesh(aiMesh* mesh, const aiScene* scene)
	{
		std::vector<VertexSkinned> vertices;
		std::vector<UINT> indices;

		// Load position, normal, uv
		for (UINT i = 0; i < mesh->mNumVertices; ++i)
		{
			VertexSkinned vertex;

			// process position
			vertex.position.x = mesh->mVertices[i].x;
			vertex.position.y = mesh->mVertices[i].y;
			vertex.position.z = mesh->mVertices[i].z;

			// process normal
			vertex.normal.x = mesh->mNormals[i].x;
			vertex.normal.y = mesh->mNormals[i].y;
			vertex.normal.z = mesh->mNormals[i].z;

			// process uv
			vertex.UV.x = mesh->mTextureCoords[0][i].x;
			vertex.UV.y = mesh->mTextureCoords[0][i].y;

			vertex.boneIndices = DirectX::XMUINT4{ 0, 0, 0, 0 };
			vertex.weights = DirectX::XMFLOAT4{ 0.0f, 0.0f, 0.0f, 0.0f };

			vertices.push_back(vertex);
		}

		// 각각의 버텍스에 영향을 주는 모든 본에 대해서 저장한 다음
		// 버텍스 기준으로 본인의 position과 normal을 다시 계산한다.
		std::vector<uint32_t> boneIndecesPerVertex;
		boneIndecesPerVertex.resize(vertices.size());

		for (unsigned int i = 0; i < mesh->mNumBones; i++)
		{
			aiBone* aibone = mesh->mBones[i];

			Bone* bone = new Bone();
			bone->name = aibone->mName.C_Str();
			bone->index = _aiNodeToNodeMap.at(aibone->mNode)->index;
			bone->bindedNode = _aiNodeToNodeMap.at(aibone->mNode);
			bone->offsetMatrix = AIMatrix4x4ToXMMatrix(aibone->mOffsetMatrix.Transpose());

			_aiNodeToNodeMap.at(aibone->mNode)->bindedBone = bone;

			for (unsigned int j = 0; j < aibone->mNumWeights; j++)
			{
				int vertexIndex = aibone->mWeights[j].mVertexId;
				float weight = aibone->mWeights[j].mWeight;

				uint32_t boneCount = boneIndecesPerVertex[vertexIndex];

				switch (boneCount)
				{
					case 0:
						vertices[vertexIndex].weights.x = weight;
						vertices[vertexIndex].boneIndices.x = bone->index;
						break;
					case 1:
						vertices[vertexIndex].weights.y = weight;
						vertices[vertexIndex].boneIndices.y = bone->index;
						break;
					case 2:
						vertices[vertexIndex].weights.z = weight;
						vertices[vertexIndex].boneIndices.z = bone->index;
						break;
					case 3:
						vertices[vertexIndex].weights.w = weight;
						vertices[vertexIndex].boneIndices.w = bone->index;
						break;
					default:
						break;
				}
				boneIndecesPerVertex[vertexIndex]++;
				//vertices[vertexIndex].nodeIndex = bone->index;	// 버텍스가 여러 본(노드)에 영향을 받으므로 셰이더에서 그것을 이용해 연산한다.
			}
		}

		// Load indices
		for (UINT i = 0; i < mesh->mNumFaces; ++i)
		{
			aiFace& face = mesh->mFaces[i];
			for (UINT j = 0; j < face.mNumIndices; ++j)
			{
				indices.push_back(face.mIndices[j]);
			}
		}

		// UINT metaDataCount = scene->mMetaData->mNumProperties;
		// for (UINT i = 0; i < metaDataCount; ++i)
		// {
		//	const aiString* key;
		//	const aiMetadataEntry* entry;
		//	auto res = scene->mMetaData->Get(i, key, entry);
		//	auto type = entry->mType;
		// }

// 		int upAxis = 0;
// 		scene->mMetaData->Get<int>("UpAxis", upAxis);
// 		int upAxisSign = 1;
// 		scene->mMetaData->Get<int>("UpAxisSign", upAxisSign);
// 		int frontAxis = 0;
// 		scene->mMetaData->Get<int>("FrontAxis", frontAxis);
// 		int frontAxisSign = 1;
// 		scene->mMetaData->Get<int>("FrontAxisSign", frontAxisSign);
// 		int coordAxis = 0;
// 		scene->mMetaData->Get<int>("CoordAxis", coordAxis);
// 		int coordAxisSign = 1;
// 		scene->mMetaData->Get<int>("CoordAxisSign", coordAxisSign);
// 
// 		int originalUpAxis = 0;
// 		scene->mMetaData->Get<int>("OriginalUpAxis", originalUpAxis);
// 		int originalUpAxisSign = 1;
// 		scene->mMetaData->Get<int>("OriginalUpAxisSign", originalUpAxisSign);
// 
// 		float unitScaleFactor = 1.0f;
// 		scene->mMetaData->Get<float>("UnitScaleFactor", unitScaleFactor);
// 		float originalUnitScaleFactor = 1.0f;
// 		scene->mMetaData->Get<float>("OriginalUnitScaleFactor", originalUnitScaleFactor);
// 
// 		aiVector3D upVec = upAxis == 0 ? aiVector3D(upAxisSign, 0, 0) : upAxis == 1 ? aiVector3D(0, upAxisSign, 0) : aiVector3D(0, 0, upAxisSign);
// 		aiVector3D forwardVec = frontAxis == 0 ? aiVector3D(frontAxisSign, 0, 0) : frontAxis == 1 ? aiVector3D(0, frontAxisSign, 0) : aiVector3D(0, 0, frontAxisSign);
// 		aiVector3D rightVec = coordAxis == 0 ? aiVector3D(coordAxisSign, 0, 0) : coordAxis == 1 ? aiVector3D(0, coordAxisSign, 0) : aiVector3D(0, 0, coordAxisSign);
// 
// 		upVec *= unitScaleFactor;
// 		forwardVec *= unitScaleFactor;
// 		rightVec *= unitScaleFactor;
// 
// 		aiMatrix4x4 mat(
// 			rightVec.x, rightVec.y, rightVec.z, 0.0f,
// 			forwardVec.x, forwardVec.y, forwardVec.z, 0.0f,
// 			-upVec.x, -upVec.y, -upVec.z, 0.0f,
// 			0.0f, 0.0f, 0.0f, 1.0f);

		// aiMatrix4x4 mat(
		//	rightVec.x, forwardVec.x, -upVec.x, 0.0f,
		//	rightVec.y, forwardVec.y, -upVec.y, 0.0f,
		//	rightVec.z, forwardVec.z, -upVec.z, 0.0f,
		//	0.0f, 0.0f, 0.0f, 1.0f);

		if (mesh->mMaterialIndex >= 0)
		{
			aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
			for (UINT i = 0; i <= aiTextureType_UNKNOWN; ++i)
			{
				LoadMaterialTextures(material, (aiTextureType)i, scene);
			}
		}

		SkinnedMesh* newMesh = new SkinnedMesh(vertices, indices);

		return newMesh;
	}

	void FBXLoader::LoadMaterialTextures(aiMaterial* material, aiTextureType type, const aiScene* scene)
	{
		UINT textureCount = material->GetTextureCount(type);
		for (UINT i = 0; i < textureCount; ++i)
		{
			aiString str;
			material->GetTexture(type, i, &str);
			std::string s = std::string(str.C_Str());
			std::string fileName = s.substr(s.find_last_of("/\\") + 1, s.length() - s.find_last_of("/\\"));
			// Check if texture was loaded before and if so, continue to next iteration: skip loading a new texture
			auto iter = ResourceManager::Instance()._textures.find(fileName);
			if (iter == ResourceManager::Instance()._textures.end())
			{
				const aiTexture* embeddedTexture = scene->GetEmbeddedTexture(str.C_Str());
				if (embeddedTexture != nullptr)
				{
					// Map에서 내가 만든 클래스말고 그냥 SRV만 관리할까..?
					ID3D11ShaderResourceView* textureSRV = LoadEmbeddedTexture(embeddedTexture);
					Texture* texture = new Texture(nullptr, textureSRV);
					ResourceManager::Instance()._textures.insert(std::make_pair(fileName, texture));
				}
				else
				{
					ResourceManager::Instance().LoadTextureFile(fileName);
				}
			}
		}
	}

	ID3D11ShaderResourceView* FBXLoader::LoadEmbeddedTexture(const aiTexture* embeddedTexture)
	{
		HRESULT hr;
		ID3D11ShaderResourceView* texture = nullptr;

		if (embeddedTexture->mHeight != 0) {
			// Load an uncompressed ARGB8888 embedded texture
			D3D11_TEXTURE2D_DESC desc;
			desc.Width = embeddedTexture->mWidth;
			desc.Height = embeddedTexture->mHeight;
			desc.MipLevels = 1;
			desc.ArraySize = 1;
			desc.SampleDesc.Count = 1;
			desc.SampleDesc.Quality = 0;
			desc.Usage = D3D11_USAGE_DEFAULT;
			desc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
			desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
			desc.CPUAccessFlags = 0;
			desc.MiscFlags = 0;

			D3D11_SUBRESOURCE_DATA subresourceData;
			subresourceData.pSysMem = embeddedTexture->pcData;
			subresourceData.SysMemPitch = embeddedTexture->mWidth * 4;
			subresourceData.SysMemSlicePitch = embeddedTexture->mWidth * embeddedTexture->mHeight * 4;

			ID3D11Texture2D* texture2D = nullptr;
			hr = _device->CreateTexture2D(&desc, &subresourceData, &texture2D);
			if (FAILED(hr))
				MessageBox(nullptr, L"CreateTexture2D failed!", L"Error!", MB_ICONERROR | MB_OK);

			hr = _device->CreateShaderResourceView(texture2D, nullptr, &texture);
			if (FAILED(hr))
				MessageBox(nullptr, L"CreateShaderResourceView failed!", L"Error!", MB_ICONERROR | MB_OK);

			return texture;
		}

		return texture;
	}

	void FBXLoader::LoadAnimation(const aiScene* scene)
	{
		if (!scene->HasAnimations())
		{
			return;
		}

		// channel in animation contains aiNodeAnim (aiNodeAnim its transformation for bones)
		// numChannels == numBones
		UINT animCount = scene->mNumAnimations;
		for (UINT i = 0; i < animCount; ++i)
		{
			auto aiAnim = scene->mAnimations[i];
			Animation* myAnimStruct = new Animation();

			myAnimStruct->name = aiAnim->mName.C_Str();
			myAnimStruct->duration = aiAnim->mDuration;
		
			// mTicksPerSeciond 이거 그건가? 초당 프레임 수?
			if (aiAnim->mTicksPerSecond != 0.0)
			{
				myAnimStruct->ticksPerSecond = aiAnim->mTicksPerSecond;
			}
			else
			{
				myAnimStruct->ticksPerSecond = 30.0f;
			}

			for (UINT j = 0; j < aiAnim->mNumChannels; ++j)
			{
				const aiNodeAnim* aiNodeAnim = aiAnim->mChannels[j];
				NodeAnimationData* myNodeAnim = new NodeAnimationData();

				myNodeAnim->nodeName = aiNodeAnim->mNodeName.C_Str();

				for (unsigned int k = 0; k < aiNodeAnim->mNumPositionKeys; ++k)
				{
					myNodeAnim->positionTimestamps.push_back(aiNodeAnim->mPositionKeys[k].mTime);
					myNodeAnim->positions.push_back(AIVec3ToXMFloat3(aiNodeAnim->mPositionKeys[k].mValue));
				}
				for (unsigned int k = 0; k < aiNodeAnim->mNumRotationKeys; ++k)
				{
					myNodeAnim->rotationTimestamps.push_back(aiNodeAnim->mRotationKeys[k].mTime);
					myNodeAnim->rotations.push_back(AIQuaternionToXMFloat4(aiNodeAnim->mRotationKeys[k].mValue));
				}
				for (unsigned int k = 0; k < aiNodeAnim->mNumScalingKeys; ++k)
				{
					myNodeAnim->scaleTimestamps.push_back(aiNodeAnim->mScalingKeys[k].mTime);
					myNodeAnim->scales.push_back(AIVec3ToXMFloat3(aiNodeAnim->mScalingKeys[k].mValue));
				}

				myAnimStruct->nodeAnimations.push_back(myNodeAnim);
			}

			// TODO : reinterpret_cast 안쓰고싶은데 어떡하지
			reinterpret_cast<DynamicModel*>(_nowModel)->animations.insert({ myAnimStruct->name, myAnimStruct });
		}
	}

	Node* FBXLoader::ReadNodeHierarchy(aiNode* ainode, const aiScene* scene)
	{
	 	UINT index = 0;
	 	Node* rootNode = new Node();

		ReadNodeRecur(rootNode, ainode, scene, index);

		return rootNode;
	}

	void FBXLoader::ReadNodeRecur(Node* node, aiNode* ainode, const aiScene* scene, UINT& index)
	{
		node->name = ainode->mName.C_Str();
		// Assimp가 Column Major로 Matrix를 읽어오므로 Row Major 하게 Transpose 해준다.
		node->transformMatrix = AIMatrix4x4ToXMMatrix(ainode->mTransformation.Transpose());
		node->index = index;
		index++;

		_aiNodeToNodeMap.insert({ ainode,node });

 		for (UINT i = 0; i < ainode->mNumChildren; ++i)
		{
			Node* newNode = new Node();
			node->children.emplace_back(newNode);
			newNode->parent = node;

			ReadNodeRecur(newNode, ainode->mChildren[i], scene, index);
		}
	}

	void FBXLoader::ForSungchan(Node* node)
	{
		if (node->bindedBone)
		{
			_sungchanBoneCount++;
		}

		for (int i = 0; i < node->children.size(); i++)
		{
			ForSungchan(node->children[i]);
		}
	}
}
