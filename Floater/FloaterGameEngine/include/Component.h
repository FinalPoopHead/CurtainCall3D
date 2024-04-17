#pragma once
#include <type_traits>

namespace flt
{
	class Scene;
	class GameObject;

	class Component
	{
		friend class Scene;
		friend class GameObject;
	public:
		Component() : Component(nullptr) {}
		Component(GameObject* gameObject) : _isEnable(false), _gameObject(gameObject) {}
		virtual ~Component() {}

		virtual void Start() {}
		virtual void OnEnable() {}
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
	class ComponentBase : public Component
	{
		friend class GameObject;
	public:
		ComponentBase() : Component() {}
		ComponentBase(GameObject* gameObject) : Component(gameObject) {}

		//void PrintIndex()
		//{
		//	std::cout << s_index << std::endl;
		//}
	protected:
		static const inline int s_index = Component::GetComponentIndex();

	private:
		virtual int GetIndex() final
		{
			return s_index;
		}

	};

	template <typename T>
	concept ComponentType = requires(T a)
	{
		std::is_base_of_v<Component, T>;
	};
}
