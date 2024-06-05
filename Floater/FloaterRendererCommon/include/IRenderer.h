#pragma once
#include <string>
#include "RawNode.h"
#include "../../FloaterUtil/include/FloaterType.h"


namespace flt
{
	class Transform;

	enum class RendererFlag
	{
		SOFTWARE = 0x0000'0001,
		ACCELERATED = 0x0000'0002
	};

	struct RendererObject
	{
		RendererObject(const bool& isDraw) : RendererObject(nullptr, isDraw) {}
		RendererObject(Transform* transform, const bool& isDraw) : 
			transform(transform),
			isDraw(isDraw),
			node(nullptr),
			camera(nullptr),
			name() {}

		// transform
		// model
		// material, texture
		// shader
		Transform* transform;
		const bool& isDraw;
		RawNode* node;
		Camera* camera;
		std::wstring name;
	};

	using HOBJECT = uint64;

	class IRenderer
	{
	public:
		IRenderer() = default;
		virtual ~IRenderer() = default;

		virtual bool Render(float deltaTime) = 0;
		virtual HOBJECT RegisterObject(RendererObject& renderable) = 0;
		virtual bool DeregisterObject(HOBJECT renderable) = 0;


		virtual bool Test() { return false; };
	};
}
