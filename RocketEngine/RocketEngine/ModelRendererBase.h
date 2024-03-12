#pragma once
#include "Component.h"

/// <summary>
/// MeshRenderer들이 상속받을 Base Class
/// </summary>
namespace Rocket::Core
{
	class ModelRendererBase : public Component
	{
	public:
		ModelRendererBase();
		virtual ~ModelRendererBase() = default;
	};
}
