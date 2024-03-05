#include "./include/CreateRenderer.h"
#include "RendererDX11.h"

#pragma comment(lib, "DirectXTK.lib")

extern "C" flt::IRenderer * flt::CreateRendererDX11(HWND hwnd)
{
	RendererDX11* renderer = new(std::nothrow) RendererDX11();
	if (renderer == nullptr)
	{
		return nullptr;
	}

	if (!renderer->Initialize(hwnd))
	{
		renderer->Finalize();
		delete renderer;
		return nullptr;
	}
	return renderer;
}

extern "C" bool flt::DestroyRendererDX11(IRenderer * renderer)
{
	auto rendererDX11 = dynamic_cast<RendererDX11*>(renderer);

	if (rendererDX11 == nullptr)
	{
		return false;
	}

	if (rendererDX11->Finalize())
	{
		delete rendererDX11;
		return true;
	}


	return false;
}
