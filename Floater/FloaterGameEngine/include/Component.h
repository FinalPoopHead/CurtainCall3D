#pragma once
#include <iostream>

namespace flt
{
	class Scene;
	class GameObject;

	class Component
	{
		friend class Scene;
		friend class GameObject;
	public:
		Component() : _isEnable(false), _gameObject(nullptr) {}
		virtual ~Component() {}

		void virtual Start() {}
		void virtual OnEnable() {}
		void virtual Update(float deltaSecond) {}
		void virtual EndDraw() {}
		void virtual OnDisable() {}
		void virtual OnDestroy() {}

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

	private:
		static int s_indexCounter;
		bool _isEnable;
		GameObject* _gameObject;
	};

	template<typename T>
	class ComponentBase : public Component
	{
		friend class GameObject;
	public:
		void PrintIndex()
		{
			std::cout << s_index << std::endl;
		}
	protected:
		static const inline int s_index = Component::GetComponentIndex();

	private:
		virtual int GetIndex() final
		{
			return s_index;
		}

	};
}
