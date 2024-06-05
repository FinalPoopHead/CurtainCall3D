#include "./include/RocketAdapter.h"
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
#include "./RocketCommon/IStaticModelRenderer.h"
#include "./RocketCommon/ITextRenderer.h"
#include "./RocketCommon/ISpriteRenderer.h"
#include "./RocketCommon/ILineRenderer.h"

#include "./RocketCommon/IResourceManager.h"
#include "./RocketCommon/RawModelStruct.h"

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
	RECT rect{};
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
		if (obj->rkModel)
		{
			if (0 < obj->rkModel->animations.size())
			{
				obj->renderer->SetActive(obj->isDraw);
			}
			else
			{
				obj->staticModelRenderer->SetActive(obj->isDraw);
			}
		}


		Transform temp;
		temp.SetMatrix(obj->transform->GetWorldMatrix4f());

		Vector4f wPos = temp.GetLocalPosition();
		obj->rocketTransform.SetLocalPosition({ wPos.x, wPos.y, wPos.z });

		Quaternion wRot = temp.GetLocalRotation();
		obj->rocketTransform.SetLocalRotation({ wRot.x, wRot.y, wRot.z, wRot.w });

		Vector4f wScale = temp.GetLocalScale();
		obj->rocketTransform.SetLocalScale({ wScale.x, wScale.y, wScale.z });

		if (obj->camera)
		{
		}
	}

	_impl->Update(deltaTime);
	_impl->Render();

	return false;
}

flt::HOBJECT flt::RocketAdapter::RegisterObject(RendererObject& renderable)
{
	RocketObject* rocketObject = new(std::nothrow) RocketObject(renderable.isDraw);
	ASSERT(rocketObject, "Failed to create RocketObject");

	auto factory = Rocket::Core::CreateGraphicsObjectFactory();

	rocketObject->transform = renderable.transform;

	if (renderable.camera)
	{
		// 카메라 등록 로직
		rocketObject->camera = factory->CreateCamera();
		//rocketObject->camera->SetAsMainCamera();
		rocketObject->camera->AddToMainCamera();
		rocketObject->camera->BindTransform(&rocketObject->rocketTransform);
	}

	if (renderable.node != nullptr)
	{
		rocketObject->rkModel = ConvertModel(*renderable.node);
		Rocket::Core::IResourceManager* rsmgr = Rocket::Core::GetResourceManager();

		//std::string pointer = std::to_string(reinterpret_cast<uint64_t>(rocketObject->rkModel));
		std::string pointer = rocketObject->rkModel->name;
		rsmgr->LoadModel(pointer, rocketObject->rkModel);

		if (renderable.node->meshes.size() > 0)
		{
			if (0 < rocketObject->rkModel->animations.size())
			{
				rocketObject->renderer = factory->CreateDynamicModelRenderer();
				rocketObject->renderer->LoadModel(pointer);
				Rocket::Core::RocketTransform* rootBoneTransform = new Rocket::Core::RocketTransform();
				rootBoneTransform->SetParent(&rocketObject->rocketTransform, false);

				RawSkeleton*& skeleton = renderable.node->skeleton;
				GenerateTransformHierarchyRecursive(rootBoneTransform, &skeleton->bones[skeleton->rootBoneIndex].transform);
				rocketObject->renderer->BindTransform(&rocketObject->rocketTransform);

				// 하드코딩 일단 0번만 넣어주자.
				if (renderable.materials[0].textures[RawMaterial::TextureType::ALBEDO_OPACITY]->name != L"")
				{
					rocketObject->renderer->LoadBaseColorTexture(ToString(renderable.materials[0].textures[RawMaterial::TextureType::ALBEDO_OPACITY]->name));
				}

				if (renderable.materials[0].textures[RawMaterial::TextureType::AO]->name != L"")
				{
					rocketObject->renderer->LoadAOTexture(ToString(renderable.materials[0].textures[RawMaterial::TextureType::AO]->name));
				}

				if (renderable.materials[0].textures[RawMaterial::TextureType::METALLIC]->name != L"")
				{
					rocketObject->renderer->LoadMetallicTexture(ToString(renderable.materials[0].textures[RawMaterial::TextureType::METALLIC]->name));
				}

				if (renderable.materials[0].textures[RawMaterial::TextureType::ROUGHNESS]->name != L"")
				{
					rocketObject->renderer->LoadRoughnessTexture(ToString(renderable.materials[0].textures[RawMaterial::TextureType::ROUGHNESS]->name));
				}

				if (renderable.materials[0].textures[RawMaterial::TextureType::NORMAL]->name != L"")
				{
					rocketObject->renderer->LoadNormalTexture(ToString(renderable.materials[0].textures[RawMaterial::TextureType::NORMAL]->name));
				}
			}
			else
			{
				rocketObject->staticModelRenderer = factory->CreateStaticModelRenderer();
				rocketObject->staticModelRenderer->LoadModel(pointer);

				rocketObject->staticModelRenderer->BindTransform(&rocketObject->rocketTransform);

				// 하드코딩 일단 0번만 넣어주자.
				if (renderable.materials[0].textures[RawMaterial::TextureType::ALBEDO_OPACITY]->name != L"")
				{
					rocketObject->staticModelRenderer->LoadBaseColorTexture(ToString(renderable.materials[0].textures[RawMaterial::TextureType::ALBEDO_OPACITY]->name));
				}

				if (renderable.materials[0].textures[RawMaterial::TextureType::AO]->name != L"")
				{
					rocketObject->staticModelRenderer->LoadAOTexture(ToString(renderable.materials[0].textures[RawMaterial::TextureType::AO]->name));
				}

				if (renderable.materials[0].textures[RawMaterial::TextureType::METALLIC]->name != L"")
				{
					rocketObject->staticModelRenderer->LoadMetallicTexture(ToString(renderable.materials[0].textures[RawMaterial::TextureType::METALLIC]->name));
				}

				if (renderable.materials[0].textures[RawMaterial::TextureType::ROUGHNESS]->name != L"")
				{
					rocketObject->staticModelRenderer->LoadRoughnessTexture(ToString(renderable.materials[0].textures[RawMaterial::TextureType::ROUGHNESS]->name));
				}

				if (renderable.materials[0].textures[RawMaterial::TextureType::NORMAL]->name != L"")
				{
					rocketObject->staticModelRenderer->LoadNormalTexture(ToString(renderable.materials[0].textures[RawMaterial::TextureType::NORMAL]->name));
				}
			}
		}
	}


	Rocket::Core::ReleaseFactory(factory);

	_objects.insert(rocketObject);
	return (HOBJECT)rocketObject;
}

bool flt::RocketAdapter::DeregisterObject(HOBJECT renderable)
{
	RocketObject* rocketObject = (RocketObject*)renderable;

	if (rocketObject->rkModel)
	{
		if (0 < rocketObject->rkModel->animations.size())
		{
			rocketObject->renderer->Destroy();
		}
		else
		{
			rocketObject->staticModelRenderer->Destroy();
		}
	}

	auto iter = _objects.find(rocketObject);

	if (iter == _objects.end())
	{
		return false;
	}

	_objects.erase(iter);
	return true;
}
