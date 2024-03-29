﻿#include "./include/RocketAdapter.h"
#include "RocketObject.h"
#include "RawNodeConverter.h"

#pragma warning(push)
#pragma warning(disable : 26495)
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

#pragma warning(pop)

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
	for (auto& obj : _objects)
	{
		Vector4f wPos = obj->transform->GetWorldPosition();
		obj->rocketTransform.SetLocalPosition({ wPos.x, wPos.y, wPos.z });

		Quaternion wRot = obj->transform->GetWorldRotation();
		obj->rocketTransform.SetLocalRotation({ wRot.x, wRot.y, wRot.z, wRot.w });

		Vector4f wScale = obj->transform->GetWorldScale();
		obj->rocketTransform.SetLocalScale({ wScale.x, wScale.y, wScale.z });

		if (obj->camera)
		{
			Vector3f pos = (Vector3f)obj->transform->GetWorldPosition();
			Quaternion rot = obj->transform->GetWorldRotation();
			obj->camera->SetPositionAndRotation({ pos.x, pos.y, pos.z }, { rot.x, rot.y, rot.z, rot.w });
		}
	}

	_impl->Update(deltaTime);
	_impl->Render();

	return false;
}

flt::HOBJECT flt::RocketAdapter::RegisterObject(RendererObject& renderable)
{
	RocketObject* rocketObject = new(std::nothrow) RocketObject();
	ASSERT(rocketObject, "Failed to create RocketObject");

	rocketObject->transform = &renderable.transform;

	rocketObject->rkModel = ConvertModel(renderable.node);
	Rocket::Core::IResourceManager* rsmgr = Rocket::Core::GetResourceManager();

	std::string pointer = std::to_string(reinterpret_cast<uint64_t>(rocketObject->rkModel));
	rsmgr->LoadModel(pointer, rocketObject->rkModel);

	auto factory = Rocket::Core::CreateGraphicsObjectFactory();

	if (renderable.camera)
	{
		// 카메라 등록 로직
		rocketObject->camera = factory->CreateCamera();
		rocketObject->camera->SetAsMainCamera();
	}

	if (renderable.node.meshes.size() > 0)
	{
		rocketObject->renderer = factory->CreateDynamicModelRenderer();
		rocketObject->renderer->LoadModel(pointer);
		Rocket::Core::RocketTransform* rootBoneTransform = new Rocket::Core::RocketTransform();
		rootBoneTransform->SetParent(&rocketObject->rocketTransform, false);

		GenerateTransformHierarchyRecursive(rootBoneTransform, &renderable.node.skeleton->bones[renderable.node.skeleton->rootBoneIndex].transform);
		rocketObject->renderer->BindTransform(&rocketObject->rocketTransform);
	}

	Rocket::Core::ReleaseFactory(factory);

	_objects.insert(rocketObject);
	return 0;
}

bool flt::RocketAdapter::DeregisterObject(HOBJECT renderable)
{
	return false;
}
