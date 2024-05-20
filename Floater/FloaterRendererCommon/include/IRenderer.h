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
		RendererObject(Transform& transform, const bool& isDraw) : 
			transform(transform),
			isDraw(isDraw),
			node(nullptr),
			camera(nullptr),
			name() {}
		// 1. 그릴때 필요한것들.
		// 2. 게임 엔진상에서 데이터를 바꾸고 싶은것들.

		// transform
		// model
		// material, texture
		// shader
		Transform& transform;
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
