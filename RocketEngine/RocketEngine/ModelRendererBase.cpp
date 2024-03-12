#include "ModelRendererBase.h"
#include "GraphicsSystem.h"

namespace Rocket::Core
{
	ModelRendererBase::ModelRendererBase()
	{
		GraphicsSystem::Instance().AddToList(this);
	}

}
