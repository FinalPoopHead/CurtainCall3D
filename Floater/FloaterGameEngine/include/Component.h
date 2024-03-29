#pragma once
#include <iostream>

namespace flt
{
	class GameObject;

	class Component
	{
	public:
		void virtual Start() {}
		void virtual OnEnable() {}
		void virtual Update() {}
		void virtual OnDisable() {}
		void virtual OnDestroy() {}

	protected:
		static int GetComponentIndex() { return s_index++; }

	protected:
		bool _isEnable;

		static int s_index;
	};

	template<typename T>
	class ComponentBase : public Component
	{
	public:
		void PrintIndex()
		{
			std::cout << s_index << std::endl;
		}
	protected:
		static inline int s_index = Component::GetComponentIndex();

	private:

	};
}


