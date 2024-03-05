#include "FBXLoader.h"
#include "../FloaterUtil/include/FloaterMacro.h"
#include "../FloaterUtil/include/ConvString.h"
#include "./include/RawNode.h"
#include "./include/RawAnimation.h"
#include <iostream>


#ifdef _DEBUG
#pragma comment(lib, "../External/lib/x64/debug/libfbxsdk.lib")
#else
#pragma comment(lib, "../External/lib/x64/release/libfbxsdk.lib")
#endif


flt::FBXLoader::FBXLoader() :
	_pManager(nullptr),
	_pIOSettings(nullptr),
	_importer(nullptr),
	_scene(nullptr)
{
	_pManager = fbxsdk::FbxManager::Create();
	_importer = fbxsdk::FbxImporter::Create(_pManager, "");
	_pIOSettings = fbxsdk::FbxIOSettings::Create(_pManager, IOSROOT);
	_pManager->SetIOSettings(_pIOSettings);
	_scene = fbxsdk::FbxScene::Create(_pManager, "");
}

flt::FBXLoader::~FBXLoader()
{
	_scene->Destroy();
	_importer->Destroy();
	_pIOSettings->Destroy();
	_pManager->Destroy();
}

void flt::FBXLoader::Load(const std::wstring& filePath, RawScene* outRawScene)
{
	ASSERT(outRawScene, "outRawScene is nullptr");

	std::string filePathStr = flt::ConvertToString(filePath);
	if (!_importer->Initialize(filePathStr.c_str(), -1, _pManager->GetIOSettings()))
	{
		auto var = _importer->GetStatus().GetErrorString();
		ASSERT(false, "importer Init Fail");
	}
	std::cout << "-------------------------------------" << std::endl;
	std::wcout << filePath << std::endl;
	int major;
	int minor;
	int revision;
	_importer->GetFileVersion(major, minor, revision);
	std::cout << major << "." << minor << "." << revision << std::endl;


	bool ret = _importer->Import(_scene);
	ASSERT(ret, "import Scene fail");

	// 일단 fbx 자체기능으로 좌표계를 바꾸자.
	fbxsdk::FbxAxisSystem dx11Axis(fbxsdk::FbxAxisSystem::eDirectX);
	dx11Axis.ConvertScene(_scene);

	// 삼각형 폴리곤으로 변경.
	fbxsdk::FbxGeometryConverter meshConverter(_pManager);
	meshConverter.Triangulate(_scene, true);


	auto fileCoord = CreateFBXCoodSystem(_scene);
	fileCoord.PrintfCoord();

	//auto animStack = _scene->GetSrcObject<FbxAnimStack>(0);
	//int animStackCount = _scene->GetSrcObjectCount<fbxsdk::FbxAnimStack>();
	//
	//FbxAxisSystem{ FbxAxisSystem::eMax }.ConvertScene(scene);
	//dxCoordSystem = CreateFBXCoodSystem(scene);
	//
	// animation
	//auto animStackCount = _scene->GetSrcObjectCount<fbxsdk::FbxAnimStack>();
	//for (int i = 0; i < animStackCount; ++i)
	//{
	//	auto animStack = _scene->GetSrcObject<fbxsdk::FbxAnimStack>(i);
	//	auto animLayerCount = animStack->GetMemberCount<fbxsdk::FbxAnimLayer>();
	//	for (int j = 0; j < animLayerCount; ++j)
	//	{
	//		auto animLayer = animStack->GetMember<fbxsdk::FbxAnimLayer>(j);
	//		auto animCurveNodeCount = node.GetSrcObjectCount<fbxsdk::FbxAnimCurveNode>();
	//		for (int k = 0; k < animCurveNodeCount; ++k)
	//		{
	//			auto animCurveNode = node.GetSrcObject<fbxsdk::FbxAnimCurveNode>(k);
	//			auto animCurveCount = animCurveNode->GetChannelsCount();
	//			for (int l = 0; l < animCurveCount; ++l)
	//			{
	//				auto animCurve = animCurveNode->GetCurve(l);
	//				auto animCurveKeyCount = animCurve->KeyGetCount();
	//				for (int m = 0; m < animCurveKeyCount; ++m)
	//				{
	//					auto animCurveKey = animCurve->KeyGet(m);
	//					auto animCurveKeyValue = animCurveKey.GetValue();
	//					auto animCurveKeyTime = animCurveKey.GetTime();
	//				}
	//			}
	//		}
	//	}
	//}

	fbxsdk::FbxNode* rootNode = _scene->GetRootNode();

	if (rootNode)
	{
		// 일단 루트 노드의 경우 identity가 아닌 경우는 고려하지 말자.
		Transform transform;
		LoadToTransform(rootNode->EvaluateLocalTransform(), &transform);
		auto matrix = transform.GetLocalMatrix4f();
		ASSERT((matrix.Determinant() - 1.0f) < FLT_EPSILON || (matrix.Determinant() - 1.0f) > -FLT_EPSILON, "root Transform is not identity");

		int childCount = rootNode->GetChildCount();
		for (int i = 0; i < childCount; ++i)
		{
			fbxsdk::FbxNode* childNode = rootNode->GetChild(i);
			//if (childNode->GetNodeAttribute() == nullptr)
			//	continue;

			RawNode* childRawNode = new RawNode();
			LoadToRawNodeRecursive(childNode, childRawNode);
		}
	}

	return;
}

flt::CoordSystem flt::FBXLoader::CreateFBXCoodSystem(fbxsdk::FbxScene* pScene)
{
	int upSign = 0;
	auto upVector = pScene->GetGlobalSettings().GetAxisSystem().GetUpVector(upSign);

	// up벡터가 x 라면 나머지 두 축은 y, z이기 때문에 ParityEven이면 y, ParityOdd이면 z
	// up벡터가 y 라면 나머지 두 축은 x, z이기 때문에 ParityEven이면 x, ParityOdd이면 z
	// up벡터가 z 라면 나머지 두 축은 x, y이기 때문에 ParityEven이면 x, ParityOdd이면 y
	int frontSign = 0;
	auto frontVector = pScene->GetGlobalSettings().GetAxisSystem().GetFrontVector(frontSign);

	auto coordSystem = pScene->GetGlobalSettings().GetAxisSystem().GetCoorSystem();
	bool rightHanded = false;

	if (coordSystem == fbxsdk::FbxAxisSystem::eRightHanded)
	{
		rightHanded = true;
	}
	else
	{
		rightHanded = false;
	}

	CoordSystem::Axis upAxis = CoordSystem::Axis::X;
	CoordSystem::Axis frontAxis = CoordSystem::Axis::Y;

	switch (upVector)
	{
		case fbxsdk::FbxAxisSystem::eXAxis:
		{
			upAxis = CoordSystem::Axis::X;
			if (frontVector == fbxsdk::FbxAxisSystem::eParityEven)
				frontAxis = CoordSystem::Axis::Y;
			else
				frontAxis = CoordSystem::Axis::Z;
		}

		break;
		case fbxsdk::FbxAxisSystem::eYAxis:
		{
			upAxis = CoordSystem::Axis::Y;
			if (frontVector == fbxsdk::FbxAxisSystem::eParityEven)
				frontAxis = CoordSystem::Axis::X;
			else
				frontAxis = CoordSystem::Axis::Z;
		}
		break;
		case fbxsdk::FbxAxisSystem::eZAxis:
		{
			upAxis = CoordSystem::Axis::Z;
			if (frontVector == fbxsdk::FbxAxisSystem::eParityEven)
				frontAxis = CoordSystem::Axis::X;
			else
				frontAxis = CoordSystem::Axis::Y;
		}
		break;
	}

	return CoordSystem{ upSign, frontSign, upAxis, frontAxis, rightHanded };
}

bool flt::FBXLoader::LoadToRawNodeRecursive(fbxsdk::FbxNode* pNode, RawNode* outNode)
{
	ASSERT(outNode, "outNode is nullptr");

	if (!LoadToRawNode(*pNode, outNode))
	{
		return false;
	}

	int childCount = pNode->GetChildCount();
	for (int i = 0; i < childCount; ++i)
	{
		auto child = pNode->GetChild(i);
		RawNode* childRawNode = new RawNode();
		outNode->children.push_back(childRawNode);
		childRawNode->parent = outNode;
		if (!LoadToRawNodeRecursive(child, childRawNode))
		{
			return false;
		}
	}

	return true;
}

bool flt::FBXLoader::LoadToTransform(fbxsdk::FbxAMatrix& fbxMatrix, Transform* outTransform)
{
	auto localTranslation = fbxMatrix.GetT();
	auto localRotation = fbxMatrix.GetR();
	auto localScale = fbxMatrix.GetS();

	outTransform->SetPosition(localTranslation.mData[0], localTranslation.mData[1], localTranslation.mData[2]);
	outTransform->SetRotation(localRotation.mData[0], localRotation.mData[1], localRotation.mData[2]);
	outTransform->SetScale(localScale.mData[0], localScale.mData[1], localScale.mData[2]);

	return true;
}

bool flt::FBXLoader::LoadToRawNode(fbxsdk::FbxNode& node, RawNode* outNode)
{
	ASSERT(outNode != nullptr, "outNode is nullptr");

	outNode->name = ConvertToWstring(node.GetName());

	// Transform 세팅
	LoadToTransform(node.EvaluateLocalTransform(), &outNode->transform);

	fbxsdk::FbxVector4 tl = node.GetGeometricTranslation(fbxsdk::FbxNode::eSourcePivot);
	fbxsdk::FbxVector4 ro = node.GetGeometricRotation(fbxsdk::FbxNode::eSourcePivot);
	fbxsdk::FbxVector4 sc = node.GetGeometricScaling(fbxsdk::FbxNode::eSourcePivot);



	// Mesh 세팅
	fbxsdk::FbxMesh* pMesh = node.GetMesh();
	if (pMesh != nullptr)
	{
		outNode->meshes.push_back(Resource<RawMesh>());
		CreateMesh(*pMesh, &outNode->meshes.back());
	}

	// animation 세팅
	outNode->skeleton->clips.push_back(RawAnimationClip{});
	SetAnimationClip(node, &outNode->skeleton->clips.back());

	return true;
}

bool flt::FBXLoader::CreateMesh(fbxsdk::FbxMesh& mesh, Resource<RawMesh>* outMesh)
{
	RawMeshBuilder builder(L"", ConvertToWstring(mesh.GetName()));

	// 아래 레이어들 특히 UV레이어는 여러개 있을 수 있음.
	std::vector<Vector3f> testPositionVector;
	GetVertexPosition(mesh, &testPositionVector);

	//std::vector<int> testIndexVector;
	//GetIndex(mesh, &testIndexVector);

	std::vector<std::vector<Vector3f>> testNormalVector;
	GetVertexNormal(mesh, &testNormalVector);

	std::vector<std::vector<Vector2f>> testUvVector;
	GetVertexUV(mesh, &testUvVector);

	std::vector<std::vector<Vector4f>> testColorVector;
	GetVertexColor(mesh, &testColorVector);

	std::vector<std::vector<Vector3f>> testTangentVector;
	GetVertexTangent(mesh, &testTangentVector);

	std::vector<std::vector<Vector3f>> testBinormalVector;
	GetVertexBinormal(mesh, &testBinormalVector);

	for (int i = 0; i < testPositionVector.size(); ++i)
	{
		RawVertex vertex;
		vertex.pos = testPositionVector[i];
		vertex.normal = testNormalVector[0][i];
		vertex.uvs[0] = testUvVector[0][i];
		//vertex.tangent = testTangentVector[0][i];
		//vertex.binormal = testBinormalVector[0][i];

		auto iter = _splitVertexMap.Find(vertex);
		int index = -1;
		if (iter == _splitVertexMap.end())
		{
			index = (int)(builder.vertices.size());
			_splitVertexMap.Insert(vertex, index);
			builder.vertices.push_back(vertex);
		}
		else
		{
			index = iter->value;
		}
		builder.indices.push_back(index);
	}

	_splitVertexMap.Clear();

	auto materialCount = mesh.GetElementMaterialCount();
	auto materialElement = mesh.GetElementMaterial();
	auto materialMappingMode = materialElement->GetMappingMode();
	auto materialReferenceMode = materialElement->GetReferenceMode();

	outMesh->Set(builder);

	return true;
}

bool flt::FBXLoader::SetAnimationClip(fbxsdk::FbxNode& node, RawAnimationClip* outAnimClip)
{
	int animCurveNodeCount = node.GetSrcObjectCount<fbxsdk::FbxAnimCurveNode>();

	if (animCurveNodeCount == 0)
		return true;

	for (int i = 0; i < animCurveNodeCount; ++i)
	{
		auto animCurveNode = node.GetSrcObject<fbxsdk::FbxAnimCurveNode>(i);


		int animCurveCount = animCurveNode->GetChannelsCount();
		for (int j = 0; j < animCurveCount; ++j)
		{
			auto animCurve = animCurveNode->GetCurve(j);
			int animCurveKeyCount = animCurve->KeyGetCount();
			outAnimClip->keyPosition.reserve(animCurveKeyCount);
			outAnimClip->keyRotation.reserve(animCurveKeyCount);
			outAnimClip->keyScale.reserve(animCurveKeyCount);
			for (int k = 0; k < animCurveKeyCount; ++k)
			{
				auto animCurveKey = animCurve->KeyGet(k);
				auto animCurveKeyValue = animCurveKey.GetValue();
				auto animCurveKeyTime = animCurveKey.GetTime();
				float animCurveKeyTimeSecond = static_cast<float>(animCurveKeyTime.GetSecondDouble());

				Transform transform;
				LoadToTransform(node.EvaluateLocalTransform(animCurveKeyTime), &transform);
				outAnimClip->keyPosition.emplace_back(animCurveKeyTimeSecond, transform.GetLocalPosition());
				outAnimClip->keyRotation.emplace_back(animCurveKeyTimeSecond, transform.GetLocalRotation());
				outAnimClip->keyScale.emplace_back(animCurveKeyTimeSecond, transform.GetLocalScale());
			}
		}
	}

	return true;
}

void flt::FBXLoader::GetVertexPosition(const fbxsdk::FbxMesh& mesh, std::vector<Vector3f>* outVector)
{
	ASSERT(outVector, "outVector is nullptr");

	// 먼저 인덱스를 모두 가져옴.
	const int polygonCount = mesh.GetPolygonCount();
	const int indexCount = polygonCount * 3;
	//int polygonVertexCount = mesh.GetPolygonVertexCount();

	std::vector<int> indexVector;
	indexVector.reserve(indexCount);
	for (int i = 0; i < polygonCount; ++i)
	{
		int polygonSize = mesh.GetPolygonSize(i);
		ASSERT(polygonSize == 3, "polygonSize is not 3");

		if (polygonSize != 3)
		{
			return;
		}

		for (int j = 0; j < polygonSize; ++j)
		{
			int index = mesh.GetPolygonVertex(i, j); // 인덱스 버퍼에 들어갈 것.
			indexVector.push_back(index);
		}
	}

	int vertexCount = mesh.GetControlPointsCount();
	outVector->reserve(indexCount);
	for (int i = 0; i < indexCount; ++i)
	{
		auto vertex = mesh.GetControlPointAt(indexVector[i]);
		ASSERT(vertex.mData[3] == 1.0f || vertex.mData[3] == 0.0f, "vertex.w is not 1.0f or 0.0f");
		outVector->emplace_back(vertex.mData[0], vertex.mData[1], vertex.mData[2]);
	}
}

void flt::FBXLoader::GetIndex(const fbxsdk::FbxMesh& mesh, std::vector<int>* outVector)
{
	ASSERT(outVector, "outVector is nullptr");

	int polygonCount = mesh.GetPolygonCount();
	//int polygonVertexCount = mesh.GetPolygonVertexCount();

	outVector->reserve(polygonCount * 3);
	for (int i = 0; i < polygonCount; ++i)
	{
		int polygonSize = mesh.GetPolygonSize(i);
		ASSERT(polygonSize == 3, "polygonSize is not 3");

		if (polygonSize != 3)
		{
			return;
		}

		for (int j = 0; j < polygonSize; ++j)
		{
			int index = mesh.GetPolygonVertex(i, j); // 인덱스 버퍼에 들어갈 것.
			outVector->push_back(index);
		}
	}
}

void flt::FBXLoader::GetVertexNormal(const fbxsdk::FbxMesh& mesh, std::vector<std::vector<Vector3f>>* outVector)
{
	/*ASSERT(outVector, "outVector is nullptr");

	int normalLayerCount = mesh.GetLayerCount(fbxsdk::FbxLayerElement::eNormal);
	outVector->resize(normalLayerCount);

	for (int i = 0; i < normalLayerCount; ++i)
	{
		const fbxsdk::FbxLayerElementNormal* normalLayer = mesh.GetElementNormal(i);

		fbxsdk::FbxLayerElementArrayTemplate<fbxsdk::FbxVector4>& elementNormal = normalLayer->GetDirectArray();
		fbxsdk::FbxLayerElementArrayTemplate<int>* pIndices = nullptr;
		int elementCount = elementNormal.GetCount();

		auto referenceMode = normalLayer->GetReferenceMode();
		if (referenceMode == fbxsdk::FbxLayerElement::eIndexToDirect || referenceMode == fbxsdk::FbxLayerElement::eIndex)
		{
			pIndices = &normalLayer->GetIndexArray();
			elementCount = pIndices->GetCount();
		}

		auto mappingMode = normalLayer->GetMappingMode();
		// ByPolygonVertex : 폴리곤의 각 정점 별로 매핑
		// ByControlPoint : 컨트롤 포인트 별로 매핑
		// ByPolygon : 폴리곤 별로 매핑
		// TODO : 매핑 방법별로 별도 처리를 해줘야 함. 일단은 ByPolygonVertex만 처리.
		ASSERT(mappingMode == fbxsdk::FbxLayerElement::eByPolygonVertex, "mappingMode is not ByPolygonVertex");

		(*outVector)[i].reserve(elementCount);

		std::cout << "normalIndex : " << std::endl;

		for (int j = 0; j < elementCount; ++j)
		{
			int index = j;
			if (pIndices != nullptr)
			{
				index = pIndices->GetAt(j);
			}
			std::cout << index << std::endl;
			fbxsdk::FbxVector4 normal = elementNormal.GetAt(index);
			(*outVector)[i].emplace_back(normal.mData[0], normal.mData[1], normal.mData[2]);
		}
	}*/
	ASSERT(outVector, "outVector is nullptr");

	int layerCount = mesh.GetLayerCount(fbxsdk::FbxLayerElement::eNormal);
	outVector->resize(layerCount);

	for (int i = 0; i < layerCount; ++i)
	{
		int triCount = mesh.GetPolygonCount();
		(*outVector)[i].reserve(triCount * 3);
		const fbxsdk::FbxLayerElementNormal* pNormalLayer = mesh.GetElementNormal(i);

		switch (pNormalLayer->GetMappingMode())
		{
			case fbxsdk::FbxGeometryElement::eByControlPoint:
			{
				int controlPointIndex = mesh.GetPolygonVertex(0, 0);

				switch (pNormalLayer->GetReferenceMode())
				{
					case fbxsdk::FbxGeometryElement::eDirect:
					{
						for (int triIndex = 0; triIndex < triCount; ++triIndex)
						{
							for (int vertexIndex = 0; vertexIndex < 3; ++vertexIndex)
							{
								int controlPointIndex = mesh.GetPolygonVertex(triIndex, vertexIndex);
								fbxsdk::FbxVector4 normal = pNormalLayer->GetDirectArray().GetAt(controlPointIndex);
								(*outVector)[i].emplace_back(normal.mData[0], normal.mData[1], normal.mData[2]);
							}
						}
					}
					break;
					case fbxsdk::FbxGeometryElement::eIndexToDirect:
					{
						for (int triIndex = 0; triIndex < triCount; ++triIndex)
						{
							for (int vertexIndex = 0; vertexIndex < 3; ++vertexIndex)
							{
								int controlPointIndex = mesh.GetPolygonVertex(triIndex, vertexIndex);
								int index = pNormalLayer->GetIndexArray().GetAt(controlPointIndex);
								fbxsdk::FbxVector4 normal = pNormalLayer->GetDirectArray().GetAt(index);
								(*outVector)[i].emplace_back(normal.mData[0], normal.mData[1], normal.mData[2]);
							}
						}
					}
					break;
					default:
					{
						ASSERT(false, "Unhandled enum");
					}
					break;
				}
			}
			break;

			case fbxsdk::FbxGeometryElement::eByPolygonVertex:
			{
				switch (pNormalLayer->GetReferenceMode())
				{
					case fbxsdk::FbxGeometryElement::eDirect:
					{
						for (int triIndex = 0; triIndex < triCount; ++triIndex)
						{
							for (int vertexIndex = 0; vertexIndex < 3; ++vertexIndex)
							{
								int vertexCounter = triIndex * 3 + vertexIndex;
								fbxsdk::FbxVector4 normal = pNormalLayer->GetDirectArray().GetAt(vertexCounter);
								(*outVector)[i].emplace_back(normal.mData[0], normal.mData[1], normal.mData[2]);
							}
						}
					}
					break;
					case fbxsdk::FbxGeometryElement::eIndexToDirect:
					{
						for (int triIndex = 0; triIndex < triCount; ++triIndex)
						{
							for (int vertexIndex = 0; vertexIndex < 3; ++vertexIndex)
							{
								int vertexCounter = triIndex * 3 + vertexIndex;
								int index = pNormalLayer->GetIndexArray().GetAt(vertexCounter);
								fbxsdk::FbxVector4 normal = pNormalLayer->GetDirectArray().GetAt(index);
								(*outVector)[i].emplace_back(normal.mData[0], normal.mData[1], normal.mData[2]);
							}
						}
					}
					break;
					default:
					{
						ASSERT(false, "Unhandled enum");
					}
					break;
				}
			}
			break;

			default:
			{
				ASSERT(false, "Unhandled enum");
			}
			break;
		}
	}
}

void flt::FBXLoader::GetVertexUV(fbxsdk::FbxMesh& mesh, std::vector<std::vector<Vector2f>>* outVector)
{
	/*
		ASSERT(outVector, "outVector is nullptr");

		int uvLayerCount = mesh.GetLayerCount(fbxsdk::FbxLayerElement::eUV);
		outVector->resize(uvLayerCount);

		for (int i = 0; i < uvLayerCount; ++i)
		{
			const fbxsdk::FbxLayerElementUV* pUVLayer = mesh.GetElementUV(i);

			fbxsdk::FbxLayerElementArrayTemplate<fbxsdk::FbxVector2>& elementUV = pUVLayer->GetDirectArray();
			fbxsdk::FbxLayerElementArrayTemplate<int>* pIndices = nullptr;
			int elementCount = elementUV.GetCount();

			auto referenceMode = pUVLayer->GetReferenceMode();
			if (referenceMode == fbxsdk::FbxLayerElement::eIndexToDirect || referenceMode == fbxsdk::FbxLayerElement::eIndex)
			{
				// 인덱스로 참조중일 경우 인덱스의 숫자가 실제 uv의 갯수보다 많을 수 있음.
				pIndices = &pUVLayer->GetIndexArray();
				elementCount = pIndices->GetCount();
			}

			auto mappingMode = pUVLayer->GetMappingMode();
			// TODO : 매핑 방법 별로 별도 처리를 해줘야 함. 일단은 ByPolygonVertex만 처리.
			ASSERT(mappingMode == fbxsdk::FbxLayerElement::eByPolygonVertex, "mappingMode is not ByPolygonVertex");

			(*outVector)[i].reserve(elementCount);

			for (int j = 0; j < elementCount; ++j)
			{
				int index = j;
				if (pIndices != nullptr)
				{
					index = pIndices->GetAt(j);
				}

				fbxsdk::FbxVector2 uv = elementUV.GetAt(index);
				if (referenceMode == fbxsdk::FbxLayerElement::eByPolygonVertex)
				{
					auto index = mesh.GetTextureUVIndex(i, j);
				}
				(*outVector)[i].emplace_back(uv.mData[0], uv.mData[1]);
			}
		}*/
	ASSERT(outVector, "outVector is nullptr");

	int layerCount = mesh.GetLayerCount(fbxsdk::FbxLayerElement::eUV);
	outVector->resize(layerCount);

	auto materialCount = mesh.GetElementMaterialCount();
	fbxsdk::FbxLayerElementMaterial* materialLayer = mesh.GetElementMaterial(0);
	auto mode = materialLayer->GetMappingMode();
	int materialIndex = materialLayer->GetIndexArray().GetAt(0);


	auto indexCount = materialLayer->GetIndexArray().GetCount();
	auto index = materialLayer->GetIndexArray().GetAt(0);
	fbxsdk::FbxSurfaceMaterial* material = mesh.GetNode()->GetMaterial(index);


	for (int i = 0; i < layerCount; ++i)
	{
		int triCount = mesh.GetPolygonCount();
		(*outVector)[i].reserve(triCount * 3);
		const fbxsdk::FbxLayerElementUV* pUVLayer = mesh.GetElementUV(i);
		//fbxsdk::FbxTexture* pTexture = mesh.GetScene()->GetTexture(i);

		switch (pUVLayer->GetMappingMode())
		{
			case fbxsdk::FbxGeometryElement::eByControlPoint:
			{
				int controlPointIndex = mesh.GetPolygonVertex(0, 0);

				switch (pUVLayer->GetReferenceMode())
				{
					case fbxsdk::FbxGeometryElement::eDirect:
					{
						for (int triIndex = 0; triIndex < triCount; ++triIndex)
						{
							for (int vertexIndex = 0; vertexIndex < 3; ++vertexIndex)
							{
								int controlPointIndex = mesh.GetPolygonVertex(triIndex, vertexIndex);
								fbxsdk::FbxVector2 uv = pUVLayer->GetDirectArray().GetAt(controlPointIndex);
								(*outVector)[i].emplace_back(uv.mData[0], uv.mData[1]);
							}
						}
					}
					break;
					case fbxsdk::FbxGeometryElement::eIndexToDirect:
					{
						for (int triIndex = 0; triIndex < triCount; ++triIndex)
						{
							for (int vertexIndex = 0; vertexIndex < 3; ++vertexIndex)
							{
								int controlPointIndex = mesh.GetPolygonVertex(triIndex, vertexIndex);
								int index = pUVLayer->GetIndexArray().GetAt(controlPointIndex);
								fbxsdk::FbxVector2 uv = pUVLayer->GetDirectArray().GetAt(index);
								(*outVector)[i].emplace_back(uv.mData[0], uv.mData[1]);
							}
						}
					}
					break;
					default:
					{
						ASSERT(false, "Unhandled enum");
					}
					break;
				}
			}
			break;

			case fbxsdk::FbxGeometryElement::eByPolygonVertex:
			{
				switch (pUVLayer->GetReferenceMode())
				{
					case fbxsdk::FbxGeometryElement::eDirect:
					{
						for (int triIndex = 0; triIndex < triCount; ++triIndex)
						{

							for (int vertexIndex = 0; vertexIndex < 3; ++vertexIndex)
							{
								int vertexCounter = mesh.GetTextureUVIndex(triIndex, vertexIndex);
								fbxsdk::FbxVector2 uv = pUVLayer->GetDirectArray().GetAt(vertexCounter);
								(*outVector)[i].emplace_back(uv.mData[0], uv.mData[1]);
							}
						}
					}
					break;
					case fbxsdk::FbxGeometryElement::eIndexToDirect:
					{
						for (int triIndex = 0; triIndex < triCount; ++triIndex)
						{

							for (int vertexIndex = 0; vertexIndex < 3; ++vertexIndex)
							{
								int vertexCounter = mesh.GetTextureUVIndex(triIndex, vertexIndex);
								int index = pUVLayer->GetIndexArray().GetAt(vertexCounter);
								fbxsdk::FbxVector2 uv = pUVLayer->GetDirectArray().GetAt(index);
								(*outVector)[i].emplace_back(uv.mData[0], uv.mData[1]);
							}
						}
					}
					break;
					default:
					{
						ASSERT(false, "Unhandled enum");
					}
					break;
				}
			}
			break;
			default:
			{
				ASSERT(false, "Unhandled enum");
			}
			break;
		}
	}
}

void flt::FBXLoader::GetVertexColor(fbxsdk::FbxMesh& mesh, std::vector<std::vector<Vector4f>>* outVector)
{
	ASSERT(outVector, "outVector is nullptr");

	int colorLayerCount = mesh.GetLayerCount(fbxsdk::FbxLayerElement::eVertexColor);
	outVector->resize(colorLayerCount);

	for (int i = 0; i < colorLayerCount; ++i)
	{
		fbxsdk::FbxLayerElementVertexColor* colorLayer = mesh.GetElementVertexColor(i);

		fbxsdk::FbxLayerElementArrayTemplate<fbxsdk::FbxColor>& elementColor = colorLayer->GetDirectArray();
		fbxsdk::FbxLayerElementArrayTemplate<int>* pIndices = nullptr;
		int elementCount = elementColor.GetCount();

		auto referenceMode = colorLayer->GetReferenceMode();
		if (referenceMode == fbxsdk::FbxLayerElement::eIndexToDirect || referenceMode == fbxsdk::FbxLayerElement::eIndex)
		{
			pIndices = &colorLayer->GetIndexArray();
			elementCount = pIndices->GetCount();
		}

		auto mappingMode = colorLayer->GetMappingMode();
		// ByPolygonVertex : 폴리곤의 각 정점 별로 매핑
		// ByControlPoint : 컨트롤 포인트 별로 매핑
		// ByPolygon : 폴리곤 별로 매핑
		// TODO : 매핑 방법별로 별도 처리를 해줘야 함. 일단은 ByPolygonVertex만 처리.
		ASSERT(mappingMode == fbxsdk::FbxLayerElement::eByPolygonVertex, "mappingMode is not ByPolygonVertex");


		(*outVector)[i].reserve(elementCount);

		for (int j = 0; j < elementCount; ++j)
		{
			int index = i;
			if (pIndices != nullptr)
			{
				index = pIndices->GetAt(i);
			}

			fbxsdk::FbxColor color = elementColor.GetAt(index);

			(*outVector)[i].emplace_back(color.mRed, color.mGreen, color.mBlue, color.mAlpha);
		}
	}


}

void flt::FBXLoader::GetVertexTangent(fbxsdk::FbxMesh& mesh, std::vector<std::vector<Vector3f>>* outVector)
{
	/*ASSERT(outVector, "outVector is nullptr");

	int tangentLayerCount = mesh.GetLayerCount(fbxsdk::FbxLayerElement::eTangent);
	outVector->resize(tangentLayerCount);

	for (int i = 0; i < tangentLayerCount; ++i)
	{
		fbxsdk::FbxLayerElementTangent* pTangentLayer = mesh.GetElementTangent(i);

		fbxsdk::FbxLayerElementArrayTemplate<fbxsdk::FbxVector4>& elementTangent = pTangentLayer->GetDirectArray();
		fbxsdk::FbxLayerElementArrayTemplate<int>* pIndices = nullptr;
		int elementCount = elementTangent.GetCount();

		auto referenceMode = pTangentLayer->GetReferenceMode();
		if (referenceMode == fbxsdk::FbxLayerElement::eIndexToDirect || referenceMode == fbxsdk::FbxLayerElement::eIndex)
		{
			pIndices = &pTangentLayer->GetIndexArray();
			elementCount = pIndices->GetCount();
		}

		auto mappingMode = pTangentLayer->GetMappingMode();
		// ByPolygonVertex : 폴리곤의 각 정점 별로 매핑
		// ByControlPoint : 컨트롤 포인트 별로 매핑
		// ByPolygon : 폴리곤 별로 매핑
		// TODO : 매핑 방법별로 별도 처리를 해줘야 함. 일단은 ByPolygonVertex만 처리.
		ASSERT(mappingMode == fbxsdk::FbxLayerElement::eByPolygonVertex, "mappingMode is not ByPolygonVertex");


		(*outVector)[i].reserve(elementCount);

		for (int i = 0; i < elementCount; ++i)
		{
			int index = i;
			if (pIndices != nullptr)
			{
				index = pIndices->GetAt(i);
			}

			fbxsdk::FbxVector4 tangent = elementTangent.GetAt(index);
			(*outVector)[i].emplace_back(tangent.mData[0], tangent.mData[1], tangent.mData[2]);
		}
	}*/
	ASSERT(outVector, "outVector is nullptr");

	int layerCount = mesh.GetLayerCount(fbxsdk::FbxLayerElement::eTangent);
	outVector->resize(layerCount);

	for (int i = 0; i < layerCount; ++i)
	{
		int triCount = mesh.GetPolygonCount();
		(*outVector)[i].reserve(triCount * 3);
		const fbxsdk::FbxLayerElementTangent* pTangentLayer = mesh.GetElementTangent(i);

		switch (pTangentLayer->GetMappingMode())
		{
			case fbxsdk::FbxGeometryElement::eByControlPoint:
			{
				int controlPointIndex = mesh.GetPolygonVertex(0, 0);

				switch (pTangentLayer->GetReferenceMode())
				{
					case fbxsdk::FbxGeometryElement::eDirect:
					{
						for (int triIndex = 0; triIndex < triCount; ++triIndex)
						{
							for (int vertexIndex = 0; vertexIndex < 3; ++vertexIndex)
							{
								int controlPointIndex = mesh.GetPolygonVertex(triIndex, vertexIndex);
								fbxsdk::FbxVector4 tangent = pTangentLayer->GetDirectArray().GetAt(controlPointIndex);
								(*outVector)[i].emplace_back(tangent.mData[0], tangent.mData[1], tangent.mData[2]);
							}
						}
					}
					break;
					case fbxsdk::FbxGeometryElement::eIndexToDirect:
					{
						for (int triIndex = 0; triIndex < triCount; ++triIndex)
						{
							for (int vertexIndex = 0; vertexIndex < 3; ++vertexIndex)
							{
								int controlPointIndex = mesh.GetPolygonVertex(triIndex, vertexIndex);
								int index = pTangentLayer->GetIndexArray().GetAt(controlPointIndex);
								fbxsdk::FbxVector4 tangent = pTangentLayer->GetDirectArray().GetAt(index);
								(*outVector)[i].emplace_back(tangent.mData[0], tangent.mData[1], tangent.mData[2]);
							}
						}
					}
					break;
					default:
					{
						ASSERT(false, "Unhandled enum");
					}
					break;
				}
			}
			break;

			case fbxsdk::FbxGeometryElement::eByPolygonVertex:
			{
				switch (pTangentLayer->GetReferenceMode())
				{
					case fbxsdk::FbxGeometryElement::eDirect:
					{
						for (int triIndex = 0; triIndex < triCount; ++triIndex)
						{
							for (int vertexIndex = 0; vertexIndex < 3; ++vertexIndex)
							{
								int vertexCounter = triIndex * 3 + vertexIndex;
								fbxsdk::FbxVector4 tangent = pTangentLayer->GetDirectArray().GetAt(vertexCounter);
								(*outVector)[i].emplace_back(tangent.mData[0], tangent.mData[1], tangent.mData[2]);
							}
						}
					}
					break;
					case fbxsdk::FbxGeometryElement::eIndexToDirect:
					{
						for (int triIndex = 0; triIndex < triCount; ++triIndex)
						{
							for (int vertexIndex = 0; vertexIndex < 3; ++vertexIndex)
							{
								int vertexCounter = triIndex * 3 + vertexIndex;
								int index = pTangentLayer->GetIndexArray().GetAt(vertexCounter);
								fbxsdk::FbxVector4 tangent = pTangentLayer->GetDirectArray().GetAt(index);
								(*outVector)[i].emplace_back(tangent.mData[0], tangent.mData[1], tangent.mData[2]);
							}
						}
					}
					break;
					default:
					{
						ASSERT(false, "Unhandled enum");
					}
					break;
				}
			}
			break;

			default:
			{
				ASSERT(false, "Unhandled enum");
			}
			break;
		}
	}
}

void flt::FBXLoader::GetVertexBinormal(fbxsdk::FbxMesh& mesh, std::vector<std::vector<Vector3f>>* outVector)
{
	/*ASSERT(outVector, "outVector is nullptr");

	int binormalLayerCount = mesh.GetLayerCount(fbxsdk::FbxLayerElement::eBiNormal);
	outVector->resize(binormalLayerCount);

	for (int i = 0; i < binormalLayerCount; ++i)
	{
		fbxsdk::FbxLayerElementBinormal* pBinormalLayer = mesh.GetElementBinormal(i);

		fbxsdk::FbxLayerElementArrayTemplate<fbxsdk::FbxVector4>& elementBinormal = pBinormalLayer->GetDirectArray();
		fbxsdk::FbxLayerElementArrayTemplate<int>* pIndices = nullptr;
		int elementCount = elementBinormal.GetCount();

		auto referenceMode = pBinormalLayer->GetReferenceMode();
		if (referenceMode == fbxsdk::FbxLayerElement::eIndexToDirect || referenceMode == fbxsdk::FbxLayerElement::eIndex)
		{
			pIndices = &pBinormalLayer->GetIndexArray();
			elementCount = pIndices->GetCount();
		}

		auto mappingMode = pBinormalLayer->GetMappingMode();
		// ByPolygonVertex : 폴리곤의 각 정점 별로 매핑
		// ByControlPoint : 컨트롤 포인트 별로 매핑
		// ByPolygon : 폴리곤 별로 매핑
		// TODO : 매핑 방법별로 별도 처리를 해줘야 함. 일단은 ByPolygonVertex만 처리.
		ASSERT(mappingMode == fbxsdk::FbxLayerElement::eByPolygonVertex, "mappingMode is not ByPolygonVertex");


		(*outVector)[i].reserve(elementCount);

		for (int i = 0; i < elementCount; ++i)
		{
			int index = i;
			if (pIndices != nullptr)
			{
				index = pIndices->GetAt(i);
			}

			fbxsdk::FbxVector4 binormal = elementBinormal.GetAt(index);
			(*outVector)[i].emplace_back(binormal.mData[0], binormal.mData[1], binormal.mData[2]);
		}
	}*/
	ASSERT(outVector, "outVector is nullptr");

	int layerCount = mesh.GetLayerCount(fbxsdk::FbxLayerElement::eBiNormal);
	outVector->resize(layerCount);

	for (int i = 0; i < layerCount; ++i)
	{
		int triCount = mesh.GetPolygonCount();
		(*outVector)[i].reserve(triCount * 3);
		const fbxsdk::FbxLayerElementBinormal* pBinormalLayer = mesh.GetElementBinormal(i);

		switch (pBinormalLayer->GetMappingMode())
		{
			case fbxsdk::FbxGeometryElement::eByControlPoint:
			{
				int controlPointIndex = mesh.GetPolygonVertex(0, 0);

				switch (pBinormalLayer->GetReferenceMode())
				{
					case fbxsdk::FbxGeometryElement::eDirect:
					{
						for (int triIndex = 0; triIndex < triCount; ++triIndex)
						{
							for (int vertexIndex = 0; vertexIndex < 3; ++vertexIndex)
							{
								int controlPointIndex = mesh.GetPolygonVertex(triIndex, vertexIndex);
								fbxsdk::FbxVector4 binormal = pBinormalLayer->GetDirectArray().GetAt(controlPointIndex);
								(*outVector)[i].emplace_back(binormal.mData[0], binormal.mData[1], binormal.mData[2]);
							}
						}
					}
					break;
					case fbxsdk::FbxGeometryElement::eIndexToDirect:
					{
						for (int triIndex = 0; triIndex < triCount; ++triIndex)
						{
							for (int vertexIndex = 0; vertexIndex < 3; ++vertexIndex)
							{
								int controlPointIndex = mesh.GetPolygonVertex(triIndex, vertexIndex);
								int index = pBinormalLayer->GetIndexArray().GetAt(controlPointIndex);
								fbxsdk::FbxVector4 binormal = pBinormalLayer->GetDirectArray().GetAt(index);
								(*outVector)[i].emplace_back(binormal.mData[0], binormal.mData[1], binormal.mData[2]);
							}
						}
					}
					break;
					default:
					{
						ASSERT(false, "Unhandled enum");
					}
					break;
				}
			}
			break;

			case fbxsdk::FbxGeometryElement::eByPolygonVertex:
			{
				switch (pBinormalLayer->GetReferenceMode())
				{
					case fbxsdk::FbxGeometryElement::eDirect:
					{
						for (int triIndex = 0; triIndex < triCount; ++triIndex)
						{
							for (int vertexIndex = 0; vertexIndex < 3; ++vertexIndex)
							{
								int vertexCounter = triIndex * 3 + vertexIndex;
								fbxsdk::FbxVector4 binormal = pBinormalLayer->GetDirectArray().GetAt(vertexCounter);
								(*outVector)[i].emplace_back(binormal.mData[0], binormal.mData[1], binormal.mData[2]);
							}
						}
					}
					break;
					case fbxsdk::FbxGeometryElement::eIndexToDirect:
					{
						for (int triIndex = 0; triIndex < triCount; ++triIndex)
						{
							for (int vertexIndex = 0; vertexIndex < 3; ++vertexIndex)
							{
								int vertexCounter = triIndex * 3 + vertexIndex;
								int index = pBinormalLayer->GetIndexArray().GetAt(vertexCounter);
								fbxsdk::FbxVector4 binormal = pBinormalLayer->GetDirectArray().GetAt(index);
								(*outVector)[i].emplace_back(binormal.mData[0], binormal.mData[1], binormal.mData[2]);
							}
						}
					}
					break;
					default:
					{
						ASSERT(false, "Unhandled enum");
					}
					break;
				}
			}
			break;

			default:
			{
				ASSERT(false, "Unhandled enum");
			}
			break;
		}
	}
}

fbxsdk::FbxVector2 flt::FBXLoader::ReadUV(fbxsdk::FbxMesh& mesh, int controlPointIndex, int vertexCounter, int uvLayer)
{
	fbxsdk::FbxVector2 ret;

	if (mesh.GetElementUVCount() < vertexCounter)
	{
		ASSERT(false, "mesh.GetElementUVCount() < vertexCounter");
		return ret;
	}

	fbxsdk::FbxGeometryElementUV* pVertexUV = mesh.GetElementUV(uvLayer);

	switch (pVertexUV->GetMappingMode())
	{
		case fbxsdk::FbxGeometryElement::eByControlPoint:
		{
			switch (pVertexUV->GetReferenceMode())
			{
				case fbxsdk::FbxGeometryElement::eDirect:
				{
					ret = pVertexUV->GetDirectArray().GetAt(controlPointIndex);
				}
				break;
				case fbxsdk::FbxGeometryElement::eIndexToDirect:
				{
					int index = pVertexUV->GetIndexArray().GetAt(controlPointIndex);
					ret = pVertexUV->GetDirectArray().GetAt(index);
				}
				break;
				default:
				{
					ASSERT(false, "Unhandled enum");
				}
				break;
			}
		}
		break;

		case fbxsdk::FbxGeometryElement::eByPolygonVertex:
		{
			switch (pVertexUV->GetReferenceMode())
			{
				case fbxsdk::FbxGeometryElement::eDirect:
				{
					ret = pVertexUV->GetDirectArray().GetAt(vertexCounter);
				}
				break;
				case fbxsdk::FbxGeometryElement::eIndexToDirect:
				{
					int index = pVertexUV->GetIndexArray().GetAt(vertexCounter);
					ret = pVertexUV->GetDirectArray().GetAt(index);
				}
				break;
				default:
				{
					ASSERT(false, "Unhandled enum");
				}
				break;
			}
		}
		break;

		default:
			break;
	}

	return ret;
}

void flt::FBXLoader::PrintNodeRecursive(fbxsdk::FbxNode* pNode, int depth)
{
	if (pNode == nullptr)
		return;
	for (int i = 0; i < depth; ++i)
	{
		std::cout << "-" << depth << "-";
	}
	std::cout << pNode->GetName() << std::endl;

	for (int i = 0; i < pNode->GetChildCount(); ++i)
		PrintNodeRecursive(pNode->GetChild(i), depth + 1);
}

void flt::FBXLoader::PrintNodeVertex(fbxsdk::FbxNode* pNode)
{
	fbxsdk::FbxMesh* mesh = pNode->GetMesh();
	if (mesh == nullptr)
		return;

	fbxsdk::FbxVector4* vertices = mesh->GetControlPoints();
	for (int j = 0; j < mesh->GetControlPointsCount(); ++j)
	{
		fbxsdk::FbxVector4 vertex = vertices[j];
		printf("v : %3.2f %3.2f %3.2f\n", vertex.mData[0], vertex.mData[1], vertex.mData[2]);
	}
}

void flt::FBXLoader::PrintTransform(fbxsdk::FbxNode* pNode)
{
	fbxsdk::FbxDouble3 translation = pNode->LclTranslation;
	fbxsdk::FbxDouble3 rotation = pNode->LclRotation;
	fbxsdk::FbxDouble3 scaling = pNode->LclScaling;
	fbxsdk::FbxEuler::EOrder rotationOrder = pNode->RotationOrder.Get();

	std::string rotationOrderString;
	switch (rotationOrder)
	{
		case fbxsdk::FbxEuler::eOrderXYZ:
			rotationOrderString = "XYZ";
			break;
		case fbxsdk::FbxEuler::eOrderXZY:
			rotationOrderString = "XZY";
			break;
		case fbxsdk::FbxEuler::eOrderYZX:
			rotationOrderString = "YZX";
			break;
		case fbxsdk::FbxEuler::eOrderYXZ:
			rotationOrderString = "YXZ";
			break;
		case fbxsdk::FbxEuler::eOrderZXY:
			rotationOrderString = "ZXY";
			break;
		case fbxsdk::FbxEuler::eOrderZYX:
			rotationOrderString = "ZYX";
			break;
		case fbxsdk::FbxEuler::eOrderSphericXYZ:
			rotationOrderString = "SphericXYZ";
			break;
		default:
			break;
	}

	printf("translation : %3.2f %3.2f %3.2f\n", translation.mData[0], translation.mData[1], translation.mData[2]);
	printf("  rotation  : %3.2f %3.2f %3.2f\n", rotation.mData[0], rotation.mData[1], rotation.mData[2]);
	printf("  scaling   : %3.2f %3.2f %3.2f\n", scaling.mData[0], scaling.mData[1], scaling.mData[2]);
	printf("  rotOrder  : %s\n", rotationOrderString.c_str());
}
