#pragma once

#include "../FloaterMath/include/Vector3f.h"
#pragma warning(push)
#pragma warning(disable: 26813 26495)
#define FBXSDK_SHARED
#define FBXSDK_NAMESPACE_USING 0
#include "fbxsdk.h"
#pragma warning (pop)
#include <Vector>

namespace flt
{
	struct VertexNormalData final
	{
	public:
		VertexNormalData(const fbxsdk::FbxMesh& mesh);

	public:
		std::vector<std::vector<Vector3f>> vertexNormal;
		std::vector<std::vector<int>> index;
	};
}
