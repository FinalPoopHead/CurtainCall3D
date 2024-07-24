#pragma once
#include <type_traits>
#include <concepts>

namespace flt
{
	template<typename Derived>
	class FSM
	{
		using StateBase_t = Derived;
	public:
		template<std::derived_from<StateBase_t> State>
		void SetState(State state);

		virtual void OnEnter() = 0;
		virtual void OnExit() = 0;

	private:
		StateBase_t* _currentState;
	};

	template<typename Derived>
	template<std::derived_from<Derived> State>
	void flt::FSM<Derived>::SetState(State state)
	{
		_currentState.OnExit();
		_currentState = state;
		_currentState.OnEnter();
	}

}
