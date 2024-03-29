#pragma once
#include "Component.h"
#include "../../FloaterRendererCommon/include/Transform.h"
#include "../../FloaterRendererCommon/include/TransformOwner.h"

#include <string>

namespace flt
{
	class Scene;

	class GameObject : public TransformOwner
	{
	public:
		GameObject();
		~GameObject();

		void AddComponent(Component* component);
		void RemoveComponent(Component* component);

		template <typename T>
		Component* GetComponent();

	private:
		Scene* scene;
		std::wstring name;

	};
}
