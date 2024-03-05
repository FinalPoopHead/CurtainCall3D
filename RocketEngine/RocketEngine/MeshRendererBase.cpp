#include "MeshRendererBase.h"
#include "GraphicsSystem.h"

namespace Rocket::Core
{
	MeshRendererBase::MeshRendererBase()
	{
		GraphicsSystem::Instance().AddToList(this);
	}

}
