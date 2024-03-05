#pragma once
#pragma warning(push)
#pragma warning(disable: 26813 26495)
#define FBXSDK_SHARED
#define FBXSDK_NAMESPACE_USING 0
#include "fbxsdk.h"
#pragma warning (pop)

#include "CoordSystem.h"
#include "../FloaterMath/include/Vector2f.h"
#include "../FloaterMath/include/Vector3f.h"
#include "../FloaterMath/include/Vector4f.h"
#include "../FloaterUtil/include/RBTree.h"
#include "./include/RawMesh.h"
#include <string>
#include <vector>


namespace flt
{
	struct RawScene;
	struct RawNode;
	struct RawMesh;
	struct RawAnimationClip;
	class Transform;

	class FBXLoader final
	{
	public:
		FBXLoader();
		~FBXLoader();

		void Load(const std::wstring& filePath, RawScene* outRawScene);

	private:
		CoordSystem CreateFBXCoodSystem(fbxsdk::FbxScene* pScene);

		bool LoadToRawNodeRecursive(fbxsdk::FbxNode* pNode, RawNode* outNode);

		bool LoadToTransform(fbxsdk::FbxAMatrix& fbxMatrix, Transform* outTransform);
		bool LoadToRawNode(fbxsdk::FbxNode& node, RawNode* outNode);
		bool CreateMesh(fbxsdk::FbxMesh& mesh, Resource<RawMesh>* outMesh);
		bool SetAnimationClip(fbxsdk::FbxNode& node, RawAnimationClip* outAnimClip);


		void GetVertexPosition(const fbxsdk::FbxMesh& mesh, std::vector<Vector3f>* outVector);
		void GetIndex(const fbxsdk::FbxMesh& mesh, std::vector<int>* outVector);
		void GetVertexNormal(const fbxsdk::FbxMesh& mesh, std::vector<std::vector<Vector3f>>* outVector);
		void GetVertexUV(fbxsdk::FbxMesh& mesh, std::vector<std::vector<Vector2f>>* outVector);
		void GetVertexColor(fbxsdk::FbxMesh& mesh, std::vector<std::vector<Vector4f>>* outVector);
		void GetVertexTangent(fbxsdk::FbxMesh& mesh, std::vector<std::vector<Vector3f>>* outVector);
		void GetVertexBinormal(fbxsdk::FbxMesh& mesh, std::vector<std::vector<Vector3f>>* outVector);

		fbxsdk::FbxVector2 ReadUV(fbxsdk::FbxMesh& mesh, int controlPointIndex, int vertexCounter, int uvLayer);

		void PrintNodeRecursive(fbxsdk::FbxNode* pNode, int depth);
		void PrintNodeVertex(fbxsdk::FbxNode* pNode);
		void PrintTransform(fbxsdk::FbxNode* pNode);

	private:
		fbxsdk::FbxManager* _pManager;
		fbxsdk::FbxIOSettings* _pIOSettings;
		fbxsdk::FbxImporter* _importer;
		fbxsdk::FbxScene* _scene;

		flt::RBTree<RawVertex, int> _splitVertexMap;
	};
}
