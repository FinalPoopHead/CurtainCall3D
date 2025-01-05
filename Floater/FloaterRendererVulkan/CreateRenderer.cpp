#include "./include/CreateRenderer.h"
#include "RendererVulkan.h"

extern "C" flt::IRenderer* flt::CreateRendererVulkan(HWND hwnd)
{
	RendererVulkan* renderer = new(std::nothrow) RendererVulkan();
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

extern "C" bool flt::DestroyRendererVulkan(IRenderer* renderer)
{
	auto rendererVulkan = dynamic_cast<RendererVulkan*>(renderer);

	if (rendererVulkan == nullptr)
	{
		return false;
	}

	if (!rendererVulkan->Finalize())
	{
		return false;
	}

	delete rendererVulkan;
	return true;
}
