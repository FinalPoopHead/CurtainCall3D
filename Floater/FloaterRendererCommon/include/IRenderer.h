#pragma once
#include <string>
#include "RendererObject.h"
#include "../../FloaterUtil/include/FloaterType.h"

namespace flt
{
	//enum class RendererFlag
	//{
	//	SOFTWARE = 0x0000'0001,
	//	ACCELERATED = 0x0000'0002
	//};
	using HOBJECT = uint64;

	class IRenderer
	{
	public:
		IRenderer() = default;
		virtual ~IRenderer() = default;

		virtual bool Render(float deltaTime) = 0;
		virtual HOBJECT RegisterObject(RendererObject& renderable) = 0;
		virtual bool DeregisterObject(HOBJECT renderable) = 0;

		virtual bool SetFullScreen(bool isFullScreen) = 0;
		virtual bool Resize(uint32 width, uint32 height) = 0;

		virtual bool Test() { return false; };
	};
}
