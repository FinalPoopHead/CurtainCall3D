#include "AssimpLoader.h"
#include "assimp/Importer.hpp"
#include "assimp/postprocess.h"
#include "assimp/scene.h"
#include "../FloaterUtil/include/ConvString.h"
#include "../FloaterUtil/include/FloaterMacro.h"
#include "../FloaterUtil/include/FloaterType.h"

#include "./include/RawScene.h"
#include "./include/RawMesh.h"
#include "./include/RawMaterial.h"
#include "AssimpRawMeshBuilder.h"

#include <iostream>

#ifdef _DEBUG
#pragma comment(lib, "../External/lib/x64/debug/assimp-vc143-mtd.lib")
#else
#pragma comment(lib, "../External/lib/x64/release/assimp-vc143-mt.lib")
#endif

void flt::AssimpLoader::Load(const std::wstring& filePath, RawScene* outRawScene)
{
	ASSERT(outRawScene, "outRawScene is nullptr");

	Assimp::Importer importer;

	const unsigned int flags = aiProcess_Triangulate |
		aiProcess_ConvertToLeftHanded | aiProcess_JoinIdenticalVertices | aiProcess_GenBoundingBoxes |
		aiProcess_CalcTangentSpace | aiProcess_PopulateArmatureData |
		aiProcess_FlipWindingOrder | aiProcess_GenSmoothNormals | aiProcess_SplitLargeMeshes |
		aiProcess_SortByPType | aiProcess_LimitBoneWeights;

	//importer.SetPropertyBool(AI_CONFIG_IMPORT_FBX_PRESERVE_PIVOTS, 0);

	//const unsigned int flags = aiProcess_Triangulate |
	//	aiProcess_ConvertToLeftHanded |	aiProcess_CalcTangentSpace | aiProcess_PopulateArmatureData |
	//	aiProcess_LimitBoneWeights;

	std::string path = ConvertToString(filePath);
	const aiScene* assimpScene = importer.ReadFile(path, flags);
	size_t pos = path.find_last_of('\\');
	std::wstring directory = ConvertToWstring(path.substr(0, pos + 1));

	ClearPrivateData();
	SetAINodeMapRecursive(assimpScene->mRootNode);

	// 데이터 로드 전 벡터 등 크기 조절
	// 노드와 메쉬 등의 데이터가 떨어져있어서 두번 다시 돌면서 로드하지 않기 위해
	// 미리 데이터들을 넣을 공간만 잡아놓기.
	// 머테리얼
	const unsigned int materialCount = assimpScene->mNumMaterials;
	const unsigned int rawMaterialCount = (unsigned int)_materials.size();
	_materials.resize(rawMaterialCount + materialCount);
	for (unsigned int i = 0; i < materialCount; ++i)
	{
		_materials[rawMaterialCount + i] = new RawMaterial();
	}
	// 메쉬
	unsigned int meshCount = assimpScene->mNumMeshes;
	//unsigned int rawMeshCount = (unsigned int)outRawScene->meshes.size();
	_meshes.resize(assimpScene->mNumMeshes);

	// 먼저 머티리얼 로드
	if (assimpScene->HasMaterials())
	{
		for (unsigned int i = 0; i < materialCount; ++i)
		{
			aiMaterial* material = assimpScene->mMaterials[i];

			aiString outStr;

			auto ret = material->Get(AI_MATKEY_NAME, outStr);
			if (ret == AI_SUCCESS)
			{
				_materials[i]->name = ConvertToWstring(outStr.C_Str());
			}

			aiColor3D diffuse;
			ret = material->Get(AI_MATKEY_COLOR_DIFFUSE, diffuse);
			if (ret == AI_SUCCESS)
			{
				_materials[i]->baseColor[0] = diffuse.r;
				_materials[i]->baseColor[1] = diffuse.g;
				_materials[i]->baseColor[2] = diffuse.b;
			}

			aiColor3D ambient;
			ret = material->Get(AI_MATKEY_COLOR_AMBIENT, ambient);
			if (ret == AI_SUCCESS)
			{
				_materials[i]->ambient[0] = ambient.r;
				_materials[i]->ambient[1] = ambient.g;
				_materials[i]->ambient[2] = ambient.b;
			}

			aiColor3D specular;
			ret = material->Get(AI_MATKEY_COLOR_SPECULAR, specular);
			if (ret == AI_SUCCESS)
			{
				_materials[i]->specular[0] = specular.r;
				_materials[i]->specular[1] = specular.g;
				_materials[i]->specular[2] = specular.b;
			}

			float shininess;
			ret = material->Get(AI_MATKEY_SHININESS, shininess);
			if (ret == AI_SUCCESS)
			{
				_materials[i]->roughness = shininess;
			}

			float opacity;
			ret = material->Get(AI_MATKEY_OPACITY, opacity);
			if (ret == AI_SUCCESS)
			{
				_materials[i]->opacity = opacity;
			}

			ret = material->GetTexture(aiTextureType_DIFFUSE, 0, &outStr);
			if (ret == AI_SUCCESS)
			{
				_materials[i]->textures[RawMaterial::ALBEDO_OPACITY]->path = directory + ConvertToWstring(outStr.C_Str());
			}

			ret = material->GetTexture(aiTextureType_NORMALS, 0, &outStr);
			if (ret == AI_SUCCESS)
			{
				_materials[i]->textures[RawMaterial::NORMAL]->path = directory + ConvertToWstring(outStr.C_Str());

			}

			//material->GetTexture(aiTextureType_SPECULAR, 0, &outStr);
			//rawMaterials[i].specularMap->path = ConvertToWstring(outStr.C_Str());

			ret = material->GetTexture(aiTextureType_EMISSIVE, 0, &outStr);
			if (ret == AI_SUCCESS)
			{
				_materials[i]->textures[RawMaterial::EMISSIVE]->path = directory + ConvertToWstring(outStr.C_Str());
			}

			ret = material->GetTexture(aiTextureType_AMBIENT, 0, &outStr);
			if (ret == AI_SUCCESS)
			{
				_materials[i]->textures[RawMaterial::AO]->path = directory + ConvertToWstring(outStr.C_Str());
			}

			ret = material->GetTexture(aiTextureType_HEIGHT, 0, &outStr);
			if (ret == AI_SUCCESS)
			{
				_materials[i]->textures[RawMaterial::HEIGHT]->path = directory + ConvertToWstring(outStr.C_Str());
			}

			ret = material->GetTexture(aiTextureType_OPACITY, 0, &outStr);
			if (ret == AI_SUCCESS)
			{
				_materials[i]->textures[RawMaterial::OPACITY]->path = directory + ConvertToWstring(outStr.C_Str());
			}

			ret = material->GetTexture(aiTextureType_SHININESS, 0, &outStr);
			if (ret == AI_SUCCESS)
			{
				_materials[i]->textures[RawMaterial::ROUGHNESS]->path = directory + ConvertToWstring(outStr.C_Str());
			}

			ret = material->GetTexture(aiTextureType_UNKNOWN, 0, &outStr);
			if (ret == AI_SUCCESS)
			{
				_materials[i]->textures[RawMaterial::UNKNOWN]->path = directory + ConvertToWstring(outStr.C_Str());
			}

			/*ASSERT(!(material->GetTextureCount(aiTextureType_DIFFUSE)), "has Texture");
			ASSERT(!(material->GetTextureCount(aiTextureType_SPECULAR)), "has Texture");
			ASSERT(!(material->GetTextureCount(aiTextureType_AMBIENT)), "has Texture");
			ASSERT(!(material->GetTextureCount(aiTextureType_EMISSIVE)), "has Texture");
			ASSERT(!(material->GetTextureCount(aiTextureType_HEIGHT)), "has Texture");
			ASSERT(!(material->GetTextureCount(aiTextureType_NORMALS)), "has Texture");
			ASSERT(!(material->GetTextureCount(aiTextureType_SHININESS)), "has Texture");
			ASSERT(!(material->GetTextureCount(aiTextureType_OPACITY)), "has Texture");
			ASSERT(!(material->GetTextureCount(aiTextureType_DISPLACEMENT)), "has Texture");
			ASSERT(!(material->GetTextureCount(aiTextureType_LIGHTMAP)), "has Texture");
			ASSERT(!(material->GetTextureCount(aiTextureType_REFLECTION)), "has Texture");
			ASSERT(!(material->GetTextureCount(aiTextureType_BASE_COLOR)), "has Texture");
			ASSERT(!(material->GetTextureCount(aiTextureType_NORMAL_CAMERA)), "has Texture");
			ASSERT(!(material->GetTextureCount(aiTextureType_EMISSION_COLOR)), "has Texture");
			ASSERT(!(material->GetTextureCount(aiTextureType_METALNESS)), "has Texture");
			ASSERT(!(material->GetTextureCount(aiTextureType_DIFFUSE_ROUGHNESS)), "has Texture");
			ASSERT(!(material->GetTextureCount(aiTextureType_AMBIENT_OCCLUSION)), "has Texture");
			ASSERT(!(material->GetTextureCount(aiTextureType_SHEEN)), "has Texture");
			ASSERT(!(material->GetTextureCount(aiTextureType_CLEARCOAT)), "has Texture");
			ASSERT(!(material->GetTextureCount(aiTextureType_TRANSMISSION)), "has Texture");
			ASSERT(!(material->GetTextureCount(aiTextureType_UNKNOWN)), "has Texture");*/

			ret = material->GetTexture(aiTextureType_BASE_COLOR, 0, &outStr);
			ret = material->GetTexture(aiTextureType_NORMAL_CAMERA, 0, &outStr);
			ret = material->GetTexture(aiTextureType_EMISSION_COLOR, 0, &outStr);
			ret = material->GetTexture(aiTextureType_METALNESS, 0, &outStr);
			ret = material->GetTexture(aiTextureType_DIFFUSE_ROUGHNESS, 0, &outStr);
			ret = material->GetTexture(aiTextureType_AMBIENT_OCCLUSION, 0, &outStr);
		}
	}

	if (!assimpScene || assimpScene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !assimpScene->mRootNode)
	{
		auto errorStr = importer.GetErrorString();
		ASSERT(false, errorStr);

		return;
	}


	for (unsigned int i = 0; i < assimpScene->mNumMeshes; ++i)
	{
		auto mesh = assimpScene->mMeshes[i];
		if (mesh->HasBones())
		{
			auto armature = mesh->mBones[0]->mArmature;
			auto iter = _skeletonMap.find(armature);
			if (iter == _skeletonMap.end())
			{
				_skeletonMap.insert({ armature, RawSkeleton{} });

				auto& skeleton = _skeletonMap[armature];
				SetSkeleton(armature, skeleton);
				PrintBoneNameRecursive(skeleton.bones[skeleton.rootBoneIndex]);

				int meshCount = assimpScene->mNumMeshes;
				for (int j = 0; j < meshCount; ++j)
				{
					int boneCount = assimpScene->mMeshes[j]->mNumBones;
					for (int k = 0; k < boneCount; ++k)
					{
						auto& bone = assimpScene->mMeshes[j]->mBones[k];
						std::wstring boneName = ConvertToWstring(bone->mName.C_Str());

						auto& m = bone->mOffsetMatrix;
						//Matrix4f offset{
						//	m.a1, m.a2, m.a3, m.a4, 
						//	m.b1, m.b2, m.b3, m.b4, 
						//	m.c1, m.c2, m.c3, m.c4, 
						//	m.d1, m.d2, m.d3, m.d4 
						//};

						Matrix4f offset{
							m.a1, m.b1, m.c1, m.d1,
							m.a2, m.b2, m.c2, m.d2,
							m.a3, m.b3, m.c3, m.d3,
							m.a4, m.b4, m.c4, m.d4
						};

						auto boneIndex = _boneIndexMap[boneName].second;
						ASSERT((skeleton.boneOffsets[boneIndex] == Matrix4f::Zero() || skeleton.boneOffsets[boneIndex] == offset) , "assert");
						skeleton.boneOffsets[boneIndex] = offset;
					}
				}

			}
			//for (unsigned int j = 0; j < mesh->mNumBones; ++j)
			//{
			//	auto bone = mesh->mBones[j];
			//	std::wstring boneNodeName = ConvertToWstring(bone->mNode->mName.C_Str());
			//	// 본 이름 세팅.
			//}
		}
	}

	// 메쉬 로드
	if (assimpScene->HasMeshes())
	{
		for (unsigned int i = 0; i < meshCount; ++i)
		{
			aiMesh* mesh = assimpScene->mMeshes[i];

			auto& rawMesh = _meshes[i].first;
			rawMesh = new(std::nothrow) RawMesh();
			ASSERT(rawMesh, "메모리 할당 실패");

			_meshes[i].second = mesh->mBones[0]->mArmature;

			int vertexCount = mesh->mNumVertices;
			rawMesh->vertices.resize(vertexCount);

			// 버텍스 별 데이터
			if (mesh->HasPositions())
			{
				for (int j = 0; j < vertexCount; ++j)
				{
					rawMesh->vertices[j].pos.x = mesh->mVertices[j].x;
					rawMesh->vertices[j].pos.y = mesh->mVertices[j].y;
					rawMesh->vertices[j].pos.z = mesh->mVertices[j].z;
				}
			}

			if (mesh->HasNormals())
			{
				for (int j = 0; j < vertexCount; ++j)
				{
					rawMesh->vertices[j].normal.x = mesh->mNormals[j].x;
					rawMesh->vertices[j].normal.y = mesh->mNormals[j].y;
					rawMesh->vertices[j].normal.z = mesh->mNormals[j].z;
				}
			}

			if (mesh->HasTangentsAndBitangents())
			{
				for (int j = 0; j < vertexCount; ++j)
				{
					rawMesh->vertices[j].tangent.x = mesh->mTangents[j].x;
					rawMesh->vertices[j].tangent.y = mesh->mTangents[j].y;
					rawMesh->vertices[j].tangent.z = mesh->mTangents[j].z;

					rawMesh->vertices[j].binormal.x = mesh->mBitangents[j].x;
					rawMesh->vertices[j].binormal.y = mesh->mBitangents[j].y;
					rawMesh->vertices[j].binormal.z = mesh->mBitangents[j].z;
				}
			}

			for (int j = 0; j < vertexCount; ++j)
			{
				for (int k = 0; k < AI_MAX_NUMBER_OF_TEXTURECOORDS; ++k)
				{
					if (!mesh->HasTextureCoords(k))
					{
						continue;
					}

					int texCoordCount = mesh->mNumUVComponents[k];
					ASSERT(texCoordCount == 2, "텍스쳐 좌표가 2개가 아닙니다.");

					rawMesh->vertices[j].uvs[k].x = mesh->mTextureCoords[k][j].x;
					rawMesh->vertices[j].uvs[k].y = mesh->mTextureCoords[k][j].y;
				}
			}

			// 인덱스 데이터
			int faceCount = mesh->mNumFaces;
			for (int j = 0; j < faceCount; ++j)
			{
				aiFace face = mesh->mFaces[j];

				int indexCount = face.mNumIndices;
				ASSERT(indexCount == 3, "인덱스 개수가 3개가 아닙니다.");
				for (int k = 0; k < indexCount; ++k)
				{
					rawMesh->indices.push_back(face.mIndices[k]);
				}
			}

			// 본 데이터 내가 들고있는 skeleton 구조체의 boneindex와 매핑을 해야함.
			if (mesh->HasBones())
			{
				for (unsigned int j = 0; j < mesh->mNumBones; ++j)
				{
					aiBone* bone = mesh->mBones[j];
					std::wstring boneName = ConvertToWstring(bone->mName.C_Str());
					int boneIndex = _boneIndexMap[boneName].second;
					for (unsigned int k = 0; k < bone->mNumWeights; ++k)
					{
						aiVertexWeight weight = bone->mWeights[k];
						rawMesh->vertices[weight.mVertexId].boneIndice.push_back(boneIndex);
						rawMesh->vertices[weight.mVertexId].boneWeights.push_back(weight.mWeight);
					}
				}
			}

			// 머티리얼
			uint32 materialIndex = mesh->mMaterialIndex;
			rawMesh->material = *_materials[materialIndex];
		}
	}

	// 애니메이션 로드
	if (assimpScene->HasAnimations())
	{
		const int animationCount = assimpScene->mNumAnimations;
		for (int i = 0; i < animationCount; ++i)
		{
			aiAnimation* animation = assimpScene->mAnimations[i];

			aiString& animName = animation->mName;
			double duration = animation->mDuration;
			double ticksPerSecond = animation->mTicksPerSecond;

			const int channelCount = animation->mNumChannels;
			for (int j = 0; j < channelCount; ++j)
			{
				aiNodeAnim* nodeAnim = animation->mChannels[j];
				std::wstring nodeName = ConvertToWstring(nodeAnim->mNodeName.C_Str());

				auto iter = _boneIndexMap.find(nodeName);

				RawAnimationClip* clips = nullptr;
				if (iter == _boneIndexMap.end())
				{
					// 본이 아닌 노드에 애니메이션을 넣어야함.
					int i = 0;
					ASSERT(false, "not implemented");
				}
				else
				{
					RawSkeleton& rawSkeleton = *_boneIndexMap[nodeName].first;
					rawSkeleton.clips.resize(rawSkeleton.bones.size());
					clips = &rawSkeleton.clips[_boneIndexMap[nodeName].second];
				}

				clips->name = ConvertToWstring(animName.C_Str());

				int keyCount = (int)nodeAnim->mNumPositionKeys;
				clips->keyPosition.reserve(keyCount);
				for (int k = 0; k < keyCount; ++k)
				{
					aiVectorKey key = nodeAnim->mPositionKeys[k];
					clips->keyPosition.push_back
					(
						RawAnimationClip::KeyPosition{ (float)key.mTime, { key.mValue.x, key.mValue.y, key.mValue.z, 0.0f } }
					);
				}

				keyCount = (int)nodeAnim->mNumRotationKeys;
				clips->keyRotation.reserve(keyCount);
				for (int k = 0; k < keyCount; ++k)
				{
					aiQuatKey key = nodeAnim->mRotationKeys[k];
					clips->keyRotation.push_back
					(
						RawAnimationClip::KeyRotation{ (float)key.mTime, { key.mValue.x, key.mValue.y, key.mValue.z, key.mValue.w } }
					);
				}

				keyCount = (int)nodeAnim->mNumScalingKeys;
				clips->keyScale.reserve(keyCount);
				for (int k = 0; k < keyCount; ++k)
				{
					aiVectorKey key = nodeAnim->mScalingKeys[k];
					clips->keyScale.push_back
					(
						RawAnimationClip::KeyScale{ (float)key.mTime, { key.mValue.x, key.mValue.y, key.mValue.z, 0.0f } }
					);
				}
			}
		}
	}

	// 노드 트리 구조 구축 및 노드 이름 맵핑
	const int nodeCount = assimpScene->mRootNode->mNumChildren;
	//outRawScene->nodes.reserve(nodeCount);
	for (int i = 0; i < nodeCount; ++i)
	{
		std::wstring nodeName = ConvertToWstring(assimpScene->mRootNode->mChildren[i]->mName.C_Str());
		if (_boneIndexMap.find(nodeName) != _boneIndexMap.end())
		{
			continue;
		}

		outRawScene->nodes.push_back(new RawNode());
		SetHierarchyRawNodeRecursive(assimpScene->mRootNode->mChildren[i], outRawScene->nodes.back(), outRawScene);
	}
}

void flt::AssimpLoader::SetAINodeMapRecursive(aiNode* pNode)
{
	_aiNodeMap.insert({ ConvertToWstring(pNode->mName.C_Str()), pNode });

	const int childCount = pNode->mNumChildren;
	for (int i = 0; i < childCount; ++i)
	{
		SetAINodeMapRecursive(pNode->mChildren[i]);
	}
}

void flt::AssimpLoader::SetHierarchyRawNodeRecursive(aiNode* pNode, RawNode* pRawNode, RawScene* pRawScene)
{
	ASSERT(pNode, "pNode is nullptr");
	ASSERT(pRawNode, "pRawNode is nullptr");

	pRawNode->name = ConvertToWstring(pNode->mName.C_Str());
	_RawNodeMap.insert({ pRawNode->name , pRawNode });
	std::wcout << pRawNode->name << std::endl;

	aiVector3D position;
	aiQuaternion rotation;
	aiVector3D scale;

	pNode->mTransformation.Decompose(scale, rotation, position);

	pRawNode->transform.SetPosition(position.x, position.y, position.z);
	pRawNode->transform.SetRotation(rotation.x, rotation.y, rotation.z, rotation.w);
	pRawNode->transform.SetScale(scale.x, scale.y, scale.z);

	{
		float epsilon = 0.0001f;

		auto test = pNode->mTransformation;
		auto test2 = pRawNode->transform.GetLocalMatrix4f();
		float sub = test.a1 - test2.e[0][0];
		ASSERT(sub < epsilon && sub > -epsilon, "diff");
		sub = test.a2 - test2.e[1][0];
		ASSERT(sub < epsilon && sub > -epsilon, "diff");
		sub = test.a3 - test2.e[2][0];
		ASSERT(sub < epsilon && sub > -epsilon, "diff");
		sub = test.a4 - test2.e[3][0];
		ASSERT(sub < epsilon && sub > -epsilon, "diff");
		sub = test.b1 - test2.e[0][1];
		ASSERT(sub < epsilon && sub > -epsilon, "diff");
		sub = test.b2 - test2.e[1][1];
		ASSERT(sub < epsilon && sub > -epsilon, "diff");
		sub = test.b3 - test2.e[2][1];
		ASSERT(sub < epsilon && sub > -epsilon, "diff");
		sub = test.b4 - test2.e[3][1];
		ASSERT(sub < epsilon && sub > -epsilon, "diff");
		sub = test.c1 - test2.e[0][2];
		ASSERT(sub < epsilon && sub > -epsilon, "diff");
		sub = test.c2 - test2.e[1][2];
		ASSERT(sub < epsilon && sub > -epsilon, "diff");
		sub = test.c3 - test2.e[2][2];
		ASSERT(sub < epsilon && sub > -epsilon, "diff");
		sub = test.c4 - test2.e[3][2];
		ASSERT(sub < epsilon && sub > -epsilon, "diff");
		sub = test.d1 - test2.e[0][3];
		ASSERT(sub < epsilon && sub > -epsilon, "diff");
		sub = test.d2 - test2.e[1][3];
		ASSERT(sub < epsilon && sub > -epsilon, "diff");
		sub = test.d3 - test2.e[2][3];
		ASSERT(sub < epsilon && sub > -epsilon, "diff");
		sub = test.d4 - test2.e[3][3];
		ASSERT(sub < epsilon && sub > -epsilon, "diff");
	}

	const int childCount = pNode->mNumChildren;
	//pRawNode->children.reserve(childCount);

	int meshCount = pNode->mNumMeshes;
	//ASSERT(meshCount == 1 || meshCount == 0, "meshCount more then 1");

	pRawNode->meshes.reserve(meshCount);
	for (int i = 0; i < meshCount; ++i)
	{
		auto& [pRawMesh, armature] = _meshes[pNode->mMeshes[i]];
		pRawNode->meshes.push_back(*pRawMesh);
		pRawNode->skeleton = new RawSkeleton{ _skeletonMap[armature] };
	}

	//for (int i = 0; i < childCount; ++i)
	//{
	//	SetRawMeshToRawNodeRecursive(pNode->mChildren[i], pRawNode->children[i], pRawScene);
	//}

	for (int i = 0; i < childCount; ++i)
	{
		std::wstring childName = ConvertToWstring(pNode->mChildren[i]->mName.C_Str());
		if (_boneIndexMap.find(childName) != _boneIndexMap.end())
		{
			continue;
		}

		RawNode* childNode = new RawNode();
		pRawNode->children.push_back(childNode);
		childNode->parent = pRawNode;

		childNode->transform.SetParent(&pRawNode->transform);

		SetHierarchyRawNodeRecursive(pNode->mChildren[i], childNode, pRawScene);
	}
}

void flt::AssimpLoader::PrintNodeNameRecursive(aiNode* pNode, int depth /*= 0*/)
{
	for (int i = 0; i < depth; ++i)
	{
		std::wcout << L"| ";
	}
	std::wstring testName = ConvertToWstring(pNode->mName.C_Str());
	std::wcout << testName << std::endl;

	if (pNode->mNumChildren == 0)
	{
		return;
	}

	for (unsigned int i = 0; i < pNode->mNumChildren; ++i)
	{
		PrintNodeNameRecursive(pNode->mChildren[i], depth + 1);
	}
}

void flt::AssimpLoader::PrintBoneNameRecursive(RawSkeleton::Bone& bone, int depth /*= 0*/)
{
	for (int i = 0; i < depth; ++i)
	{
		std::wcout << L"| ";
	}
	std::wcout << bone.name << std::endl;

	for (auto& child : bone.transform.GetChildren())
	{
		PrintBoneNameRecursive(*(RawSkeleton::Bone*)child->GetOwner(), depth + 1);
	}
}

void flt::AssimpLoader::SetSkeleton(aiNode* armature, RawSkeleton& skeleton)
{
	int boneCount = CalcAINodeCountRecursive(armature);

	skeleton.boneOffsets.resize(boneCount);

	skeleton.bones.reserve(boneCount);
	skeleton.bones.push_back(RawSkeleton::Bone{});
	skeleton.rootBoneIndex = 0;

	SetSkeletonRecursive(armature, skeleton, (int)skeleton.bones.size() - 1);
}

void flt::AssimpLoader::SetSkeletonRecursive(aiNode* assimpBone, RawSkeleton& skeleton, const int index)
{
	RawSkeleton::Bone* bone = &skeleton.bones[index];
	bone->name = ConvertToWstring(assimpBone->mName.C_Str());
	_boneIndexMap[bone->name] = { &skeleton, index };

	aiVector3D position;
	aiQuaternion rotation;
	aiVector3D scale;

	assimpBone->mTransformation.Decompose(scale, rotation, position);

	bone->transform.SetPosition(position.x, position.y, position.z);
	bone->transform.SetRotation(rotation.x, rotation.y, rotation.z, rotation.w);
	bone->transform.SetScale(scale.x, scale.y, scale.z);

	for (unsigned int i = 0; i < assimpBone->mNumChildren; ++i)
	{
		skeleton.bones.push_back(RawSkeleton::Bone{});
		RawSkeleton::Bone* parentBone = &skeleton.bones[index];
		auto& childBone = skeleton.bones.back();
		childBone.transform.SetParent(&parentBone->transform);
		SetSkeletonRecursive(assimpBone->mChildren[i], skeleton, (int)skeleton.bones.size() - 1);
	}
}

int flt::AssimpLoader::CalcAINodeCountRecursive(aiNode* pNode)
{
	int count = 1;
	for (unsigned int i = 0; i < pNode->mNumChildren; ++i)
	{
		count += CalcAINodeCountRecursive(pNode->mChildren[i]);
	}

	return count;
}

void flt::AssimpLoader::ClearPrivateData()
{
	_aiNodeMap.clear();
	_RawNodeMap.clear();
	_boneIndexMap.clear();
	_skeletonMap.clear();

	for (auto& material : _materials)
	{
		delete material;
	}
	_materials.clear();
	for (auto& mesh : _meshes)
	{
		delete mesh.first;
	}
	_meshes.clear();
}
