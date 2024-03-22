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
	ASSERT(false, "Not implemented");
	return nullptr;
	//RawMesh* pRawMesh = new(std::nothrow) RawMesh();

	//ASSERT(pRawMesh, "메모리 할당 실패");

	//int vertexCount = mesh->mNumVertices;

	//pRawMesh->vertices.resize(vertexCount);

	//// 버텍스 별 데이터
	//if (mesh->HasPositions())
	//{
	//	for (int j = 0; j < vertexCount; ++j)
	//	{
	//		pRawMesh->vertices[j].pos.x = mesh->mVertices[j].x;
	//		pRawMesh->vertices[j].pos.y = mesh->mVertices[j].y;
	//		pRawMesh->vertices[j].pos.z = mesh->mVertices[j].z;
	//	}
	//}

	//if (mesh->HasNormals())
	//{
	//	for (int j = 0; j < vertexCount; ++j)
	//	{
	//		pRawMesh->vertices[j].normal.x = mesh->mNormals[j].x;
	//		pRawMesh->vertices[j].normal.y = mesh->mNormals[j].y;
	//		pRawMesh->vertices[j].normal.z = mesh->mNormals[j].z;
	//	}
	//}

	//if (mesh->HasTangentsAndBitangents())
	//{
	//	for (int j = 0; j < vertexCount; ++j)
	//	{
	//		pRawMesh->vertices[j].tangent.x = mesh->mTangents[j].x;
	//		pRawMesh->vertices[j].tangent.y = mesh->mTangents[j].y;
	//		pRawMesh->vertices[j].tangent.z = mesh->mTangents[j].z;

	//		pRawMesh->vertices[j].binormal.x = mesh->mBitangents[j].x;
	//		pRawMesh->vertices[j].binormal.y = mesh->mBitangents[j].y;
	//		pRawMesh->vertices[j].binormal.z = mesh->mBitangents[j].z;
	//	}
	//}

	//for (int j = 0; j < vertexCount; ++j)
	//{
	//	for (int k = 0; k < AI_MAX_NUMBER_OF_TEXTURECOORDS; ++k)
	//	{
	//		if (!mesh->HasTextureCoords(k))
	//		{
	//			continue;
	//		}

	//		int texCoordCount = mesh->mNumUVComponents[k];
	//		ASSERT(texCoordCount == 2, "텍스쳐 좌표가 2개가 아닙니다.");

	//		pRawMesh->vertices[j].uvs[k].x = mesh->mTextureCoords[k][j].x;
	//		pRawMesh->vertices[j].uvs[k].y = mesh->mTextureCoords[k][j].y;
	//	}
	//}

	//// 인덱스 데이터
	//int faceCount = mesh->mNumFaces;
	//for (int j = 0; j < faceCount; ++j)
	//{
	//	aiFace face = mesh->mFaces[j];

	//	int indexCount = face.mNumIndices;
	//	ASSERT(indexCount == 3, "인덱스 개수가 3개가 아닙니다.");
	//	for (int k = 0; k < indexCount; ++k)
	//	{
	//		pRawMesh->indices.push_back(face.mIndices[k]);
	//	}
	//}

	//pRawMesh->material = *pRawScene->materials[mesh->mMaterialIndex];
	//return pRawMesh;
}
