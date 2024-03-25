#pragma once

#ifdef FLOATERRENDERERDX11_EXPORTS
#define FLT_RENDERER __declspec(dllexport)
#else
#define FLT_RENDERER __declspec(dllimport)
#endif

#include <windows.h>

namespace flt
{
	class IRenderer;

	extern "C" FLT_RENDERER IRenderer* CreateRendererRocketDX11(HWND hwnd);
	extern "C" FLT_RENDERER bool DestroyRendererRocketDX11(IRenderer * renderer);
}
