#pragma once
#include "Component.h"

/// <summary>
/// MeshRenderer들이 상속받을 Base Class
/// </summary>
namespace Rocket::Core
{
	class MeshRendererBase : public Component
	{
	public:
		MeshRendererBase();
		virtual ~MeshRendererBase() = default;
	};
}
