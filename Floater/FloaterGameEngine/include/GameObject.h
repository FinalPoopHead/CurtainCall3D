#pragma once
#include "Component.h"
#include "../../FloaterRendererCommon/include/Transform.h"
#include "../../FloaterRendererCommon/include/TransformOwner.h"
#include <vector>
#include <string>

namespace flt
{
	class Scene;

	class GameObject : public TransformOwner
	{
		friend class Scene;
		friend class RendererComponent;
	public:
		GameObject();
		~GameObject();

		bool AddComponent(Component* component);
		void RemoveComponent(Component* component);

		template <typename T>
		Component* GetComponent();

	private:
		Scene* _scene;
		std::wstring _name;
		std::vector<Component*> _components;
		bool _isEnable;
	};

	template <typename T>
	Component* flt::GameObject::GetComponent()
	{
		int index = T::s_index;
		if (index < 0 || _components.size() <= index)
		{
			return nullptr;
		}

		return _components[index];
	}

}
