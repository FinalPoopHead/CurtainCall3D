#pragma once
#include <type_traits>

namespace flt
{
	class Scene;
	class GameObject;
	class Collider;

	class ComponentBase
	{
		friend class Scene;
		friend class GameObject;
	public:
		ComponentBase() : ComponentBase(nullptr) {}
		ComponentBase(GameObject* gameObject) : _isEnable(false), _gameObject(gameObject) {}
		virtual ~ComponentBase() {}

		virtual void Start() {}
		virtual void OnEnable() {}

		virtual void PrePhysics() {}
		virtual void OnCollisionEnter(Collider* other) {}
		virtual void OnCollisionStay(Collider* other) {}
		virtual void OnCollisionExit(Collider* other) {}
		virtual void PostPhysics() {}
		virtual void FixedUpdate() {}

		virtual void Update(float deltaSecond) {}
		virtual void EndDraw() {}
		virtual void OnDisable() {}
		virtual void OnDestroy() {}



	public:
		void Enable();
		void Disable();

	private:
		virtual int GetIndex() = 0;

	protected:
		static int GetComponentIndex()
		{
			return s_indexCounter++;
		}

	protected:
		GameObject* _gameObject;

	private:
		static int s_indexCounter;
		bool _isEnable;
	};

	template<typename T>
	class Component : public ComponentBase
	{
		friend class GameObject;
	public:
		Component() : ComponentBase() {}
		Component(GameObject* gameObject) : ComponentBase(gameObject) {}

		//void PrintIndex()
		//{
		//	std::cout << s_index << std::endl;
		//}
	protected:
		static const inline int s_index = ComponentBase::GetComponentIndex();

	private:
		virtual int GetIndex() final
		{
			return s_index;
		}
	};

	template <typename T>
	concept ComponentType = requires(T a)
	{
		std::is_base_of_v<ComponentBase, T>;
	};
}
