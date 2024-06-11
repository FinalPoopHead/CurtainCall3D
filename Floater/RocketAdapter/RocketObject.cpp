#include "RocketObject.h"
#include "./RocketCommon/IMeshRenderer.h"
#include "./RocketCommon/IDynamicModelRenderer.h"
#include "./RocketCommon/IStaticModelRenderer.h"
#include "./RocketCommon/ITextRenderer.h"
#include "./RocketCommon/ISpriteRenderer.h"
#include "./RocketCommon/ILineRenderer.h"
#include "./RocketCommon/ICamera.h"
#include "./RocketCommon/RawModelStruct.h"


flt::RocketObject::~RocketObject()
{
	if (camera)
	{
		camera->Destroy();
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
}

void flt::RocketObject::RemoveRKNodeRecursive(Rocket::Core::RawNode* node)
{
	for (auto& child : node->children)
	{
		RemoveRKNodeRecursive(child);
	}

	delete node;
}
