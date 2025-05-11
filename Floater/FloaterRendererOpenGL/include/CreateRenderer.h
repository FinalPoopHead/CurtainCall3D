#pragma once
#ifdef FLOATERRENDEREROPENGL_EXPORTS
#define FLT_RENDERER __declspec(dllexport)
#else
#define FLT_RENDERER __declspec(dllimport)
#endif

#include <windows.h>

namespace flt
{
	class IRenderer;

	extern "C" FLT_RENDERER IRenderer* CreateRendererOpenGl(HWND hwnd);
	extern "C" FLT_RENDERER bool DestroyRendererOpenGl(IRenderer* renderer);
	//extern "C" FLT_RENDERER IRenderer* CreateRendererDX12(HWND hwnd);
}
