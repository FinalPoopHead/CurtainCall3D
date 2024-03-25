#include "./include/RocketAdapter.h"
#include "RawNodeConverter.h"
#include "./RocketCommon/IDX11Renderer.h"
#include "./RocketCommon/IFactory.h"

#include "./RocketCommon/ICamera.h"

/// Renderable objects
#include "./RocketCommon/IMeshRenderer.h"
#include "./RocketCommon/IDynamicModelRenderer.h"
#include "./RocketCommon/ITextRenderer.h"
#include "./RocketCommon/ISpriteRenderer.h"
#include "./RocketCommon/ILineRenderer.h"

#include "./RocketCommon/IResourceManager.h"

#pragma comment(lib, "RocketDX11.lib")
#pragma comment(lib, "RocketCommon.lib")
#pragma comment(lib, "DirectXTK.lib")


flt::RocketAdapter::RocketAdapter()
{
	_impl = Rocket::Core::CreateGraphicsInstance();
}

flt::RocketAdapter::~RocketAdapter()
{
	Rocket::Core::ReleaseGraphicsInstance(_impl);
}

bool flt::RocketAdapter::Initialize(HWND hwnd, HWND debugHWnd /*= NULL*/)
{
	RECT rect;
	ASSERT(GetClientRect(hwnd, &rect) == TRUE, "Failed to get window rect");

	_impl->Initialize(hwnd, rect.right - rect.left, rect.bottom - rect.top);
	return true;
}

bool flt::RocketAdapter::Finalize()
{
	_impl->Finalize();

	return true;
}

bool flt::RocketAdapter::Render(float deltaTime)
{
	_impl->Update(deltaTime);
	_impl->Render();

	return false;
}

flt::HOBJECT flt::RocketAdapter::RegisterObject(RendererObject& renderable)
{
	Rocket::Core::RawModel* rkModel = ConvertModel(renderable.node);
	Rocket::Core::IResourceManager* rsmgr = Rocket::Core::GetResourceManager();
	
	std::string pointer = std::to_string(reinterpret_cast<uint64_t>(rkModel));
	rsmgr->LoadModel(pointer, rkModel);

	if (!renderable.camera)
	{
		// 카메라 등록 로직
	}

// 	Rocket::Core::IFactory* factory = Rocket::Core::CreateGraphicsObjectFactory();
// 	auto ret = factory->CreateDynamicModelRenderer();
// 	ret->l

	return 0;
}

bool flt::RocketAdapter::DeregisterObject(HOBJECT renderable)
{
	return false;
}

