#include "AssimpRawMeshBuilder.h"
#include "assimp/mesh.h"
#include "../FloaterUtil/include/FloaterMacro.h"
#include "./include/RawNode.h"
#include "./include/RawScene.h"


flt::AssimpRawMeshBuilder::AssimpRawMeshBuilder(std::unordered_map<std::wstring, RawNode*>& nodeMap) :
	AssimpRawMeshBuilder(nullptr, L"", 0, nullptr, nodeMap)
{

}

flt::AssimpRawMeshBuilder::AssimpRawMeshBuilder(aiMesh* mesh, std::wstring filePath, int index, RawScene* rawScene, std::unordered_map<std::wstring, RawNode*>& nodeMap) :
	mesh(mesh), pRawScene(rawScene), nodeMap(nodeMap)
{
	key = filePath + L"-" + ConvertToWstring(mesh->mName.C_Str()) + L"-" + std::to_wstring(index);
}

flt::RawMesh* flt::AssimpRawMeshBuilder::build() const
{
	RawMesh* pRawMesh = new(std::nothrow) RawMesh();

	ASSERT(pRawMesh, "메모리 할당 실패");

	int vertexCount = mesh->mNumVertices;

	pRawMesh->vertices.resize(vertexCount);

	// 버텍스 별 데이터
	if (mesh->HasPositions())
	{
		for (int j = 0; j < vertexCount; ++j)
		{
			pRawMesh->vertices[j].pos.x = mesh->mVertices[j].x;
			pRawMesh->vertices[j].pos.y = mesh->mVertices[j].y;
			pRawMesh->vertices[j].pos.z = mesh->mVertices[j].z;
		}
	}

	if (mesh->HasNormals())
	{
		for (int j = 0; j < vertexCount; ++j)
		{
			pRawMesh->vertices[j].normal.x = mesh->mNormals[j].x;
			pRawMesh->vertices[j].normal.y = mesh->mNormals[j].y;
			pRawMesh->vertices[j].normal.z = mesh->mNormals[j].z;
		}
	}

	if (mesh->HasTangentsAndBitangents())
	{
		for (int j = 0; j < vertexCount; ++j)
		{
			pRawMesh->vertices[j].tangent.x = mesh->mTangents[j].x;
			pRawMesh->vertices[j].tangent.y = mesh->mTangents[j].y;
			pRawMesh->vertices[j].tangent.z = mesh->mTangents[j].z;

			pRawMesh->vertices[j].binormal.x = mesh->mBitangents[j].x;
			pRawMesh->vertices[j].binormal.y = mesh->mBitangents[j].y;
			pRawMesh->vertices[j].binormal.z = mesh->mBitangents[j].z;
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

			pRawMesh->vertices[j].uvs[k].x = mesh->mTextureCoords[k][j].x;
			pRawMesh->vertices[j].uvs[k].y = mesh->mTextureCoords[k][j].y;
		}
	}

	// 본 데이터
	//if (mesh->HasBones())
	//{
	//	// 루트 본 찾기
	//	std::wstring firstBoneName = ConvertToWstring(mesh->mBones[0]->mName.C_Str());
	//	RawNode* rootBoneNode = nodeMap.find(firstBoneName)->second;
	//	while (rootBoneNode->parent)
	//	{
	//		if (rootBoneNode->parent->boneIndex != -1)
	//		{
	//			rootBoneNode = rootBoneNode->parent;
	//		}
	//		else
	//		{
	//			break;
	//		}
	//	}
	//	pRawMesh->pRootBone = &rootBoneNode->transform;

	//	int boneCount = mesh->mNumBones;
	//	for (int boneIndex = 0; boneIndex < boneCount; ++boneIndex)
	//	{
	//		aiBone* bone = mesh->mBones[boneIndex];

	//		std::wstring boneName = ConvertToWstring(bone->mName.C_Str());
	//		auto boneOffsetMatrix = bone->mOffsetMatrix;
	//		aiVector3D bonePosition;
	//		aiQuaternion boneRotation;
	//		aiVector3D boneScale;

	//		boneOffsetMatrix.Decompose(boneScale, boneRotation, bonePosition);

	//		auto iter = nodeMap.find(boneName);
	//		ASSERT(iter != nodeMap.end(), "boneName is not found");
	//		iter->second->boneIndex = boneIndex;

	//		/*{
	//			// 해당 노드의 로컬 트랜스폰 값과 본의 오프셋 매트릭스가 같은지 확인
	//			// 현재 다른경우가 있는데 좌표 변환이 노드에만 적용되고 본에서는 적용이 되지 않아 그럴 수 있을거라 생각됨.
	//			auto pos = iter->second->transform.GetPosition();
	//			float epsilon = 0.0001f;
	//			float subX = pos.x - bonePosition.x;
	//			float subY = pos.y - bonePosition.y;
	//			float subZ = pos.z - bonePosition.z;
	//			ASSERT(subX < epsilon && subX > -epsilon && subY < epsilon && subY > -epsilon && subZ < epsilon && subZ > -epsilon, "diff");

	//			auto rot = iter->second->transform.GetRotation();
	//			subX = rot.x - boneRotation.x;
	//			subY = rot.y - boneRotation.y;
	//			subZ = rot.z - boneRotation.z;
	//			float subW = rot.w - boneRotation.w;
	//			ASSERT(subX < epsilon && subX > -epsilon && subY < epsilon && subY > -epsilon && subZ < epsilon && subZ > -epsilon && subW < epsilon && subW > -epsilon, "diff");

	//			auto scale = iter->second->transform.GetScale();
	//			subX = scale.x - boneScale.x;
	//			subY = scale.y - boneScale.y;
	//			subZ = scale.z - boneScale.z;
	//			ASSERT(subX < epsilon&& subX > -epsilon && subY < epsilon && subY > -epsilon && subZ < epsilon && subZ > -epsilon, "diff");
	//		}*/

	//		int weightCount = bone->mNumWeights;
	//		for (int k = 0; k < weightCount; ++k)
	//		{
	//			aiVertexWeight vertexWeight = bone->mWeights[k];

	//			int vertexId = vertexWeight.mVertexId;
	//			float weight = vertexWeight.mWeight;

	//			pRawMesh->vertices[vertexId].boneIndice.push_back(boneIndex);
	//			pRawMesh->vertices[vertexId].boneWeights.push_back(weight);
	//		}
	//	}
	//}

	// 인덱스 데이터
	int faceCount = mesh->mNumFaces;
	for (int j = 0; j < faceCount; ++j)
	{
		aiFace face = mesh->mFaces[j];

		int indexCount = face.mNumIndices;
		ASSERT(indexCount == 3, "인덱스 개수가 3개가 아닙니다.");
		for (int k = 0; k < indexCount; ++k)
		{
			pRawMesh->indices.push_back(face.mIndices[k]);
		}
	}

	pRawMesh->material = *pRawScene->materials[mesh->mMaterialIndex];
	return pRawMesh;
}
