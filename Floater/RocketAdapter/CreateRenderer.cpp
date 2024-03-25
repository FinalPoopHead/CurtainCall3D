#include "./include/CreateRenderer.h"
#include "./include/RocketAdapter.h"

extern "C" flt::IRenderer * flt::CreateRendererRocketDX11(HWND hwnd)
{
	RocketAdapter* rocketRenderer = new(std::nothrow) RocketAdapter();

	if (!rocketRenderer)
	{
		return rocketRenderer;
	}

	if (!rocketRenderer->Initialize(hwnd))
	{
		rocketRenderer->Finalize();
		delete rocketRenderer;
		return nullptr;
	}

	return rocketRenderer;
}

extern "C" bool flt::DestroyRendererRocketDX11(IRenderer* renderer)
{
	auto rocketRenderer = dynamic_cast<RocketAdapter*>(renderer);

	if (rocketRenderer == nullptr)
	{
		return false;
	}

	if (rocketRenderer->Finalize())
	{
		delete rocketRenderer;
		return true;
	}

	return false;
}


