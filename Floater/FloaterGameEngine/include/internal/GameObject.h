#pragma once
#include "Component.h"
#include "../../../FloaterRendererCommon/include/Transform.h"
#include "../../../FloaterRendererCommon/include/TransformOwner.h"
#include <type_traits>
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

		// 씬에 추가되었을 때 비활성화 여부와 상관 없이 호출
		virtual void OnCreate() {}
		// 활성화 되었을때 호출
		virtual void OnEnable() {}

		virtual void FixedUpdate() {}

		virtual void PreUpdate(float deltaSecond) {}
		virtual void Update(float deltaSecond) {}
		virtual void PostUpdate(float deltaSecond) {}
		virtual void EndDraw() {}
		// 비활성화 되었을때 호출 제거시 활성화 되어있다면 호출
		virtual void OnDisable() {}
		// 씬에 제거되었을 때 비활성화 여부와 상관 없이 호출
		virtual void OnDestroy() {}
		
	public:
		void Enable();
		void Disable();
		void Destroy();

	public:
		[[deprecated("Use Template Function")]] bool AddComponent(ComponentBase* component);
		void RemoveComponent(ComponentBase* component);

		template<ComponentType T, typename... TArgs>
		T* AddComponent(bool isEnabled, TArgs&&... args);

		template <typename T>
		T* GetComponent();

	public:
		std::wstring name;

	private:
		Scene* _scene;
		std::vector<ComponentBase*> _components;
		bool _isEnable;
	};


	template<ComponentType T, typename... TArgs>
	T* flt::GameObject::AddComponent(bool isEnabled, TArgs&&... args)
	{
		ComponentBase* component = new T(std::forward<TArgs>(args)...);
		int index = component->GetIndex();
		//component->_gameObject = this;

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
	concept GameObjectDerived = requires(T a)
	{
		std::is_base_of_v<flt::GameObject, T>;
	};
}
