#pragma once
#include "../../FloaterRendererCommon/include/IRenderer.h"
#include <windows.h>

namespace Rocket::Core
{
	class IDX11Renderer;
}

namespace flt
{
	class RocketAdapter : public IRenderer
	{
	public:
		RocketAdapter();
		virtual ~RocketAdapter();

		bool Initialize(HWND hwnd, HWND debugHWnd = NULL);
		bool Finalize();

		virtual bool Render(float deltaTime);
		virtual HOBJECT RegisterObject(RendererObject& renderable);
		virtual bool DeregisterObject(HOBJECT renderable);

		//virtual bool Test() { return false; };
	private:
		Rocket::Core::IDX11Renderer* _impl;
	};
}
