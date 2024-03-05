#pragma once
#include <unordered_map>
#include <typeinfo>
#include <string>
#include <vector>
#include "DLLExporter.h"

namespace Rocket::Core
{
	class ObjectSystem;
	class MeshRendererBase;
	class PhysicsSystem;
}

// __declspec(dllexport) std::string;
// template class __declspec(dllexport) std::unordered_map<std::string, Rocket::Core::IComponent*>;

namespace Rocket
{
	class Transform;
	class Component;
	class Scene;
}

/// <summary>
/// Component Based 엔진 디자인 패턴의 Entity인 GameObject 클래스.
/// 모든 GameObject는 Transform 정보를 기본적으로 갖고 있다.
/// 여러 Component를 붙여서 사용한다.
/// 
/// 23.06.26 강석원 인재원.
/// </summary>
namespace Rocket
{
	class ROCKET_API GameObject final
	{
		/// GameObject의 LifeCycle을 관리하는 클래스들에게 friend 걸어줌.
		friend class Scene;
		friend class Core::PhysicsSystem;
		friend class Core::ObjectSystem;

		/// 생성자, 소멸자
		/// Rule of Three..
	public:
		GameObject(std::string objName);
		~GameObject() = default;
		GameObject(const GameObject& rhs) = delete;
		GameObject(GameObject&& rhs) = delete;
		GameObject& operator=(const GameObject& rhs) = delete;
		GameObject& operator=(GameObject&& rhs) = delete;

		/// GameObject LifeCycle.
	private:
		void Start();
		void FixedUpdate();
		void Update();
		void LateUpdate();
		void Finalize();

		/// GameObject EventCycle.
	private:
		void OnCollisionEnter();
		void OnCollisionStay();
		void OnCollisionExit();
		void OnDestroy();

	public:
		void Destroy();		// 소멸자를 대신할 Destroy 함수.

		/// Transform 컴포넌트는 모든 게임오브젝트가 필수적으로 갖는다.
		/// 새로운 Transform으로 변경할 수 없도록 참조 멤버로 설정.
	public:
		std::string objName;
		Transform& transform;
	
		/// GameObject의 활성,비활성화 관리
	public:
		void Enable();
		void Disable();
		bool IsActive() const;

		void FlushEnable();
		void FlushDisable();

	private:
		bool _isActive;

	public:
		void SetRestart();

	private:
		bool _isStarted;

	public:
		void SetScene(Scene* scene) { _scene = scene; }
		Scene* GetScene() { return _scene; }

	private:
		Scene* _scene;

		/// 컴포넌트 관련
	public:
		// 컴포넌트를 추가 또는 획득
		template <typename T>
		T* AddComponent();

		template <typename T>
		T* GetComponent();

		template <typename T>
		std::vector<T*> GetComponents();

		template <typename T>
		T* GetComponentDynamic();
		
		template <typename T>
		std::vector<T*> GetComponentsDynamic();

		std::unordered_map<std::string, std::vector<Component*>>& GetAllComponents();

	private:
		std::unordered_map<std::string, std::vector<Component*>> _components;
	};

	/// 컴포넌트를 추가하는 함수.
	/// 이미 추가 되어있는 컴포넌트라면 그 녀석을 반환한다.
	/// 근데 내가 컴포넌트를 벡터로 받고있는데 그러면 여러개를 받을 수 있는거 아닌가?
	/// 
	/// 23.06.27 강석원 인재원.
	template <typename T>
	T* GameObject::AddComponent()
	{
		T* component = new T();
		component->gameObject = this;
		_components[typeid(T).name()].emplace_back(component);
		return component;
	}
	
	/// 찾는 컴포넌트의 포인터를 "하나" 반환하는 함수.
	/// 찾는 컴포넌트가 없다면 nullptr을 반환한다.
	/// 
	/// 23.06.27 강석원 인재원.
	template <typename T>
	T* GameObject::GetComponent()
	{
		auto iter = _components.find(typeid(T).name());
		if (iter != _components.end())
		{
			return dynamic_cast<T*>(iter->second.front());
		}

		return nullptr;
	}

	template <typename T>
	std::vector<T*>
		GameObject::GetComponents()
	{
		std::vector<T*> result;
		T* temp;

		auto iter = _components.find(typeid(T).name());
		if (iter != _components.end())
		{
			for (auto& component : iter->second)
			{
				temp = dynamic_cast<T*>(component);
				if (temp)
				{
					result.push_back(temp);
				}
			}
		}

		return result;
	}

	template <typename T>
	T* GameObject::GetComponentDynamic()
	{
		T* component;
		for (auto& iter : _components)
		{
			component = dynamic_cast<T*>(iter.second.front());
			if (component)
			{
				return component;
			}
		}

		return nullptr;
	}

	template <typename T>
	std::vector<T*>
		GameObject::GetComponentsDynamic()
	{
		std::vector<T*> result;
		T* temp;

		for (auto& iter : _components)
		{
			for (auto& component : iter.second)
			{
				temp = dynamic_cast<T*>(component);
				if (temp)
				{
					result.push_back(temp);
				}
			}
		}

		return result;
	}

}
