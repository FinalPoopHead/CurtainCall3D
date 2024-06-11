#pragma once
#include "RawNode.h"
#include "AnimState.h"
#include "../FloaterUtil/include/FloaterType.h"
#include <vector>
#include <string>


namespace flt
{
	class Transform;

	struct RendererObject
	{
		RendererObject(const bool& isDraw) : RendererObject(nullptr, isDraw) {}
		RendererObject(Transform* transform, const bool& isDraw) :
			transform(transform),
			isDraw(isDraw),
			node(nullptr),
			camera(nullptr),
			animState(),
			imgName(L""),
			text(L""),
			name(L"") {}

		~RendererObject();

		void SetRawNode(RawNode* rawNode);
		void SetMaterial(uint32 meshIndex, const std::wstring& path, RawMaterial::TextureType type);

		// transform
		// model
		// material, texture
		// shader
		Transform* transform;
		const bool& isDraw;
		RawNode* node;
		std::vector<RawMaterial> materials;
		Camera* camera;
		AnimState animState;
		std::wstring imgName;
		std::wstring text;
		std::wstring name;
	};
}
