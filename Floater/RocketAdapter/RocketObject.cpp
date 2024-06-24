#include "RocketObject.h"
#include "./RocketCommon/IMeshRenderer.h"
#include "./RocketCommon/IDynamicModelRenderer.h"
#include "./RocketCommon/IStaticModelRenderer.h"
#include "./RocketCommon/ITextRenderer.h"
#include "./RocketCommon/ISpriteRenderer.h"
#include "./RocketCommon/ILineRenderer.h"
#include "./RocketCommon/ICamera.h"
#include "./RocketCommon/IDirectionalLight.h"
#include "./RocketCommon/RawModelStruct.h"


flt::RocketObject::~RocketObject()
{
	if (camera)
	{
		camera->Destroy();
	}

	for (auto& tr : rocketTransform.GetChildren())
	{
		RemoveRKTransformRecursive(tr);
	}

	if (rkModel)
	{
		if (0 < rkModel->animations.size())
		{
			renderer->Destroy();
		}
		else
		{
			staticModelRenderer->Destroy();
		}

		RemoveRKNodeRecursive(rkModel->rootNode);
		for (auto& mesh : rkModel->meshes)
		{
			for (auto& texture : mesh->material->textures)
			{
				delete texture;
			}
			delete mesh->material;

			delete mesh;
		}

		for (auto& [name, animation] : rkModel->animations)
		{
			for (auto& animData : animation->nodeAnimations)
			{
				delete animData;
			}

			delete animation;
		}
		delete rkModel;
	}

	if (spriteRenderer)
	{
		spriteRenderer->Destroy();
	}

	if (textRenderer)
	{
		textRenderer->Destroy();
	}

	if (directionalLight)
	{
		directionalLight->Destroy();
		delete lightTransform;
	}
}

void flt::RocketObject::RemoveRKNodeRecursive(Rocket::Core::RawNode* node)
{
	for (auto& child : node->children)
	{
		RemoveRKNodeRecursive(child);
	}

	delete node->bindedBone;
	delete node;
}

void flt::RocketObject::RemoveRKTransformRecursive(Rocket::Core::RocketTransform* transform)
{
	for (auto& child : transform->GetChildren())
	{
		RemoveRKTransformRecursive(child);
	}

	delete transform;
}
