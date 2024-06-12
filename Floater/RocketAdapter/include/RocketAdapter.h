#pragma once
#include "../../FloaterRendererCommon/include/IRenderer.h"
#include <windows.h>
#include <set>
#include <unordered_map>

namespace Rocket::Core
{
	class IDX11Renderer;
}

namespace flt
{
	struct RocketObject;

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

		virtual bool Resize(uint32 width, uint32 height);


	private:
		Rocket::Core::IDX11Renderer* _impl;

		std::unordered_map<RocketObject*, std::pair<RendererObject*, AnimState>> _objects;
	};
}
