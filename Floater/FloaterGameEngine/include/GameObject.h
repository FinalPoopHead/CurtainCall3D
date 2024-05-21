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

		virtual void Start() {}
		virtual void OnEnable() {}
		virtual void Update(float deltaSecond) {}
		virtual void EndDraw() {}
		virtual void OnDisable() {}
		virtual void OnDestroy() {}
		
		[[deprecated("Use Template Function")]] bool AddComponent(ComponentBase* component);
		void RemoveComponent(ComponentBase* component);

		template<ComponentType T>
		T* AddComponent(bool isEnabled = true);

		template <typename T>
		T* GetComponent();

	public:
		std::wstring name;

	private:
		Scene* _scene;
		std::vector<ComponentBase*> _components;
		bool _isEnable;
	};

	template<ComponentType T>
	T* flt::GameObject::AddComponent(bool isEnabled /*= true*/)
	{
		ComponentBase* component = new T(this);
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
		component->_isEnable = isEnabled;

		return static_cast<T*>(component);
	}

	template <typename T>
	T* flt::GameObject::GetComponent()
	{
		int index = T::s_index;
		if (index < 0 || _components.size() <= index)
		{
			return nullptr;
		}

		return static_cast<T*>(_components[index]);
	}

	template <typename T>
	concept GameObjectType = requires(T a)
	{
		std::is_base_of_v<GameObject, T>;
	};
}
