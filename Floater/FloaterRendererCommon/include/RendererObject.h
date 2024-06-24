#pragma once
#include "RawNode.h"
#include "Light.h"
#include "AnimState.h"
#include "Text.h"
#include "../../FloaterUtil/include/FloaterType.h"
#include <vector>
#include <string>


namespace flt
{
	class Transform;

	struct RendererObject
	{
		RendererObject(const bool& isDraw) : RendererObject(nullptr, isDraw) {}
		RendererObject(Transform* transform, const bool& isDraw)
			: transform(transform)
			, isDraw(isDraw)
			, node(nullptr)
			, camera(nullptr)
			, light(nullptr)
			, animState()
			, imgPath(L"")
			, text()
			, name(L"")
			, useRootMotion(false) {}

		~RendererObject();

		void SetRawNode(RawNode* rawNode);
		void SetMaterial(uint32 meshIndex, const std::wstring& path, RawMaterial::TextureType type);

		Transform* transform;
		const bool& isDraw;
		RawNode* node;
		std::vector<RawMaterial> materials;
		Camera* camera;
		Light* light;
		AnimState animState;
		std::wstring imgPath;
		Text text;
		std::wstring name;
		bool useRootMotion;
	};
}
