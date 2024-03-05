#include "VertexNormalData.h"
#include "../FloaterUtil/include/FloaterMacro.h"

flt::VertexNormalData::VertexNormalData(const fbxsdk::FbxMesh& mesh)
{
	int normalLayerCount = mesh.GetLayerCount(fbxsdk::FbxLayerElement::eNormal);
	vertexNormal.resize(normalLayerCount);

	for (int i = 0; i < normalLayerCount; ++i)
	{
		const fbxsdk::FbxLayerElementNormal* normalLayer = mesh.GetElementNormal(i);

		fbxsdk::FbxLayerElementArrayTemplate<fbxsdk::FbxVector4>& elementNormal = normalLayer->GetDirectArray();
		fbxsdk::FbxLayerElementArrayTemplate<int>* pIndices = nullptr;

		auto referenceMode = normalLayer->GetReferenceMode();
		if (referenceMode == fbxsdk::FbxLayerElement::eIndexToDirect || referenceMode == fbxsdk::FbxLayerElement::eIndex)
		{
			pIndices = &normalLayer->GetIndexArray();
		}

		auto mappingMode = normalLayer->GetMappingMode();
		// ByPolygonVertex : 폴리곤의 각 정점 별로 매핑
		// ByControlPoint : 컨트롤 포인트 별로 매핑
		// ByPolygon : 폴리곤 별로 매핑
		// TODO : 매핑 방법별로 별도 처리를 해줘야 함. 일단은 ByPolygonVertex만 처리.
		ASSERT(mappingMode == fbxsdk::FbxLayerElement::eByPolygonVertex, "mappingMode is not ByPolygonVertex");

		int elementCount = elementNormal.GetCount();

		vertexNormal[i].reserve(elementCount);

		for (int j = 0; j < elementCount; ++j)
		{
			int index = j;
			if (pIndices != nullptr)
			{
				index = pIndices->GetAt(j);
			}

			fbxsdk::FbxVector4 normal = elementNormal.GetAt(index);
			vertexNormal[i].emplace_back(normal.mData[0], normal.mData[1], normal.mData[2]);
		}
	}
}
