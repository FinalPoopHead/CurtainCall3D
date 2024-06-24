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
#include "./RocketCommon/IDirectionalLight.h"

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
	for (auto& [rkObj, pair] : _objects)
	{
		if (rkObj->rkModel)
		{
			if (0 < rkObj->rkModel->animations.size())
			{
				rkObj->renderer->SetActive(rkObj->isDraw);
			}
			else
			{
				rkObj->staticModelRenderer->SetActive(rkObj->isDraw);
			}
		}

		Transform temp;
		temp.SetMatrix(rkObj->transform->GetWorldMatrix4f());

		Vector4f wPos = temp.GetLocalPosition();
		rkObj->rocketTransform.SetLocalPosition({ wPos.x, wPos.y, wPos.z });

		Quaternion wRot = temp.GetLocalRotation();
		rkObj->rocketTransform.SetLocalRotation({ wRot.x, wRot.y, wRot.z, wRot.w });

		Vector4f wScale = temp.GetLocalScale();
		rkObj->rocketTransform.SetLocalScale({ wScale.x, wScale.y, wScale.z });

		Matrix4f worldTM = temp.GetWorldMatrix4f();
		DirectX::SimpleMath::Matrix dxWorldTM
		{
			worldTM.e[0][0], worldTM.e[0][1], worldTM.e[0][2], worldTM.e[0][3],
			worldTM.e[1][0], worldTM.e[1][1], worldTM.e[1][2], worldTM.e[1][3],
			worldTM.e[2][0], worldTM.e[2][1], worldTM.e[2][2], worldTM.e[2][3],
			worldTM.e[3][0], worldTM.e[3][1], worldTM.e[3][2], worldTM.e[3][3]
		};

		auto& rendererObject = pair.first;
		auto& animState = pair.second;

		if (rendererObject->animState.index != -1)
		{
			if (!(animState.index == rendererObject->animState.index && animState.isPlaying == rendererObject->animState.isPlaying))
			{
				animState = rendererObject->animState;

				if (animState.isPlaying)
				{
					if (rkObj->rkModel)
					{
						if (0 < rkObj->rkModel->animations.size())
						{
							rkObj->renderer->PlayAnimation(animState.index, animState.isLoop);
						}
					}
				}
				else
				{
					if (rkObj->rkModel)
					{
						if (0 < rkObj->rkModel->animations.size())
						{
							rkObj->renderer->ForceStopAnimation();
						}
					}
				}
			}
		}

		if (rkObj->spriteRenderer)
		{
			rkObj->spriteRenderer->SetActive(rkObj->isDraw);
			rkObj->spriteRenderer->SetWorldTM(dxWorldTM);
		}

		if (rkObj->textRenderer)
		{
			rkObj->textRenderer->SetActive(rkObj->isDraw);
			rkObj->textRenderer->SetWorldTM(dxWorldTM);
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

	// 카메라 등록 로직
	if (renderable.camera)
	{
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
				if (renderable.materials[0].textures[RawMaterial::TextureType::ALBEDO_OPACITY]->path != L"")
				{
					rocketObject->renderer->LoadBaseColorTexture(ToString(renderable.materials[0].textures[RawMaterial::TextureType::ALBEDO_OPACITY]->path));
				}

				if (renderable.materials[0].textures[RawMaterial::TextureType::AO]->path != L"")
				{
					rocketObject->renderer->LoadAOTexture(ToString(renderable.materials[0].textures[RawMaterial::TextureType::AO]->path));
				}

				if (renderable.materials[0].textures[RawMaterial::TextureType::METALLIC]->path != L"")
				{
					rocketObject->renderer->LoadMetallicTexture(ToString(renderable.materials[0].textures[RawMaterial::TextureType::METALLIC]->path));
				}

				if (renderable.materials[0].textures[RawMaterial::TextureType::ROUGHNESS]->path != L"")
				{
					rocketObject->renderer->LoadRoughnessTexture(ToString(renderable.materials[0].textures[RawMaterial::TextureType::ROUGHNESS]->path));
				}

				if (renderable.materials[0].textures[RawMaterial::TextureType::NORMAL]->path != L"")
				{
					rocketObject->renderer->LoadNormalTexture(ToString(renderable.materials[0].textures[RawMaterial::TextureType::NORMAL]->path));
				}
			}
			else
			{
				rocketObject->staticModelRenderer = factory->CreateStaticModelRenderer();
				rocketObject->staticModelRenderer->LoadModel(pointer);

				rocketObject->staticModelRenderer->BindTransform(&rocketObject->rocketTransform);

				// 하드코딩 일단 0번만 넣어주자.
				if (renderable.materials[0].textures[RawMaterial::TextureType::ALBEDO_OPACITY]->path != L"")
				{
					rocketObject->staticModelRenderer->LoadBaseColorTexture(ToString(renderable.materials[0].textures[RawMaterial::TextureType::ALBEDO_OPACITY]->path));
				}

				if (renderable.materials[0].textures[RawMaterial::TextureType::AO]->path != L"")
				{
					rocketObject->staticModelRenderer->LoadAOTexture(ToString(renderable.materials[0].textures[RawMaterial::TextureType::AO]->path));
				}

				if (renderable.materials[0].textures[RawMaterial::TextureType::METALLIC]->path != L"")
				{
					rocketObject->staticModelRenderer->LoadMetallicTexture(ToString(renderable.materials[0].textures[RawMaterial::TextureType::METALLIC]->path));
				}

				if (renderable.materials[0].textures[RawMaterial::TextureType::ROUGHNESS]->path != L"")
				{
					rocketObject->staticModelRenderer->LoadRoughnessTexture(ToString(renderable.materials[0].textures[RawMaterial::TextureType::ROUGHNESS]->path));
				}

				if (renderable.materials[0].textures[RawMaterial::TextureType::NORMAL]->path != L"")
				{
					rocketObject->staticModelRenderer->LoadNormalTexture(ToString(renderable.materials[0].textures[RawMaterial::TextureType::NORMAL]->path));
				}
			}
		}
	}

	if (renderable.light != nullptr)
	{
		switch (renderable.light->type)
		{
			case Light::Type::none:
				break;
			case Light::Type::directional:
			{
				Rocket::Core::IDirectionalLight* d = factory->CreateDirectionalLight();
				Light& light = *renderable.light;
				rocketObject->directionalLight = d;
				d->BindTransform(&rocketObject->rocketTransform);
				d->SetDiffuseColor({ light.diffuseColor.r, light.diffuseColor.g, light.diffuseColor.b});
				d->SetSpecularColor({ light.specularColor.r, light.specularColor.g, light.specularColor.b});
				d->SetAmbientColor({ light.ambientColor.r, light.ambientColor.g, light.ambientColor.b});
			}
			break;
			case Light::Type::point:
			{
				ASSERT(false, "Not implemented");
			}
			break;
			case Light::Type::spot:
			{
				ASSERT(false, "Not implemented");
			}
			break;
			default:
				break;
		}
	}

	if (renderable.imgPath != L"")
	{
		rocketObject->spriteRenderer = factory->CreateSpriteRenderer();
		rocketObject->spriteRenderer->SetImage(ToString(renderable.imgPath));
	}

	if (renderable.text.data != L"")
	{
		rocketObject->textRenderer = factory->CreateTextRenderer();
		rocketObject->textRenderer->SetText(ToString(renderable.text.data));

		rocketObject->textRenderer->SetFontwithPath(ToString(renderable.text.font));
		rocketObject->textRenderer->SetColor({ renderable.text.color.r, renderable.text.color.g, renderable.text.color.b, renderable.text.color.a });
	}

	Rocket::Core::ReleaseFactory(factory);

	_objects.insert({ rocketObject, {&renderable, AnimState{}} });
	return (HOBJECT)rocketObject;
}

bool flt::RocketAdapter::DeregisterObject(HOBJECT renderable)
{
	RocketObject* rocketObject = (RocketObject*)renderable;



	auto iter = _objects.find(rocketObject);

	if (iter == _objects.end())
	{
		return false;
	}

	delete rocketObject;
	_objects.erase(iter);
	return true;
}

bool flt::RocketAdapter::Resize(uint32 width, uint32 height)
{
	_impl->OnResize(width, height);
	return true;
}
