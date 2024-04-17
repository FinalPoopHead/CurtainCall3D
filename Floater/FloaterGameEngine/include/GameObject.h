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

		
		[[deprecated("Use Template Function")]] bool AddComponent(Component* component);
		void RemoveComponent(Component* component);

		template<ComponentType T>
		T* AddComponent();

		template <typename T>
		Component* GetComponent();

	private:
		Scene* _scene;
		std::wstring _name;
		std::vector<Component*> _components;
		bool _isEnable;
	};

	template<ComponentType T>
	T* flt::GameObject::AddComponent()
	{
		Component* component = new T(this);
		int index = component->GetIndex();

		if (_components.size() <= index)
		{
			_components.resize(index + 1);
		}

		if (_components[index] != nullptr)
		{
			delete component;
			return static_cast<T*>(_components[index]);
		}

		_components[index] = component;
		component->_gameObject = this;

		return static_cast<T*>(component);
	}

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
