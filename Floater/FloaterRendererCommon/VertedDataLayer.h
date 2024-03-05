#pragma once
#include "../FloaterMath/include/Vector2f.h"
#include "../FloaterMath/include/Vector3f.h"
#include "../FloaterMath/include/Vector4f.h"
#pragma warning(push)
#pragma warning(disable: 26813 26495)
#define FBXSDK_SHARED
#define FBXSDK_NAMESPACE_USING 0
#include "fbxsdk.h"
#pragma warning (pop)
#include <vector>

namespace flt
{
	template<typename FbxLayerElementType>
	struct FLTVector;

	template<>
	struct FLTVector<fbxsdk::FbxLayerElementNormal>
	{
		using type = flt::Vector3f;
		static constexpr int count = 3;
		static constexpr fbxsdk::FbxLayerElement::EType enumType = fbxsdk::FbxLayerElement::eNormal;
	};

	template<>
	struct FLTVector<fbxsdk::FbxLayerElementUV>
	{
		using type = flt::Vector2f;
		static constexpr int count = 2;
		static constexpr fbxsdk::FbxLayerElement::EType enumType = fbxsdk::FbxLayerElement::eUV;
	};

	template<>
	struct FLTVector<fbxsdk::FbxLayerElementVertexColor>
	{
		using type = flt::Vector4f;
		static constexpr int count = 4;
		static constexpr fbxsdk::FbxLayerElement::EType enumType = fbxsdk::FbxLayerElement::eVertexColor;
	};

	template<>
	struct FLTVector<fbxsdk::FbxLayerElementTangent>
	{
		using type = flt::Vector3f;
		static constexpr int count = 3;
		static constexpr fbxsdk::FbxLayerElement::EType enumType = fbxsdk::FbxLayerElement::eTangent;
	};

	template<>
	struct FLTVector<fbxsdk::FbxLayerElementBinormal>
	{
		using type = flt::Vector3f;
		static constexpr int count = 3;
		static constexpr fbxsdk::FbxLayerElement::EType enumType = fbxsdk::FbxLayerElement::eBiNormal;
	};


	template<typename FbxLayerElementType>
	class VertexDataLayer
	{
		using VectorType = typename FLTVector<FbxLayerElementType>::type;


	public:
		VertexDataLayer(const fbxsdk::FbxMesh& mesh)
		{
			fbxsdk::FbxLayerElement::EType FbxLayerEnum = FLTVector<FbxLayerElementType>::enumType;
			const int VectorCount = FLTVector<FbxLayerElementType>::count;

			const int layerCount = mesh.GetLayerCount(FbxLayerEnum);
			_vertexData.resize(layerCount);

			for (int i = 0; i < layerCount; ++i)
			{
				auto layer = mesh.GetLayer(i, FbxLayerEnum);

				const fbxsdk::FbxLayerElementNormal* compareLayer = mesh.GetElementNormal(i);
				auto testLayer = mesh.GetLayer(i, fbxsdk::FbxLayerElement::eNormal);

				auto& element = layer->GetDirectArray();
				fbxsdk::FbxLayerElementArrayTemplate<int>* pIndices = nullptr;

				auto referenceMode = layer->GetReferenceMode();
				if (referenceMode == fbxsdk::FbxLayerElement::eIndexToDirect || referenceMode == fbxsdk::FbxLayerElement::eIndex)
				{
					pIndices = &layer->GetIndexArray();
				}

				auto mappingMode = layer->GetMappingMode();
				// ByPolygonVertex : 폴리곤의 각 정점 별로 매핑
				// ByControlPoint : 컨트롤 포인트 별로 매핑
				// ByPolygon : 폴리곤 별로 매핑
				// TODO : 매핑 방법별로 별도 처리를 해줘야 함. 일단은 ByPolygonVertex만 처리.
				ASSERT(mappingMode == fbxsdk::FbxLayerElement::eByPolygonVertex, "mappingMode is not ByPolygonVertex");

				int elementCount = element.GetCount();

				_vertexData[i].resize(elementCount);

				for (int j = 0; j < elementCount; ++j)
				{
					int index = j;
					if (pIndices != nullptr)
					{
						index = pIndices->GetAt(j);
					}

					auto data = element.GetAt(index);
					for (int i = 0; i < VectorCount; ++i)
					{
						_vertexData[i][j].m[i] = data.mData[i];
					}
				}
			}
		}

	private:
		std::vector<std::vector<VectorType>> _vertexData;
		std::vector<int> _indices;
	};
}
