#pragma once
#include "../FloaterRendererCommon/include/Transform.h"
#include "../FloaterRendererCommon/include/Camera.h"
#include "../FloaterRendererCommon/include/RawNode.h"
#include "DX11Mesh.h"

#include <string>
#include <vector>
#include <map>

namespace flt
{
	struct DX11Node
	{
#pragma region Nasted
		enum BoneIndex
		{
			NO_BONE = -1,
			HAS_BONE = -2
		};
#pragma endregion
		DX11Node(Transform& transform, const bool& isDraw);

		std::wstring name;
		Transform& transform;
		const bool& isDraw;
		std::vector<Resource<DX11Mesh>> meshes;
		Camera* camera;

		int boneIndex;
	};
}
