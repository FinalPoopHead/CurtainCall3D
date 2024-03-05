#include "Animator.h"
#include <iostream>
#include <cassert>
#include <string>
#include "State.h"

namespace Rocket
{

	void Animator::SetCurrentState(std::string stateName)
	{
		_prevState = _currentState;
		_currentState = _states.at(stateName);
	}

	Animator::Animator()
		:_states(),
		_conditions(),
		_currentState(),
		_prevState()
	{

	}

	Animator::~Animator()
	{

	}

	void Animator::Initialize()
	{
		_currentState = _initState;
		_prevState = _initState;

		for (auto& condition : _conditions)
		{
			condition.second = false;
		}
	}

	bool Animator::CheckAnimCycle(float deltaTime)
	{
		if (_currentState->GetIsLoop())
		{
			if (_currentState->GetDuration() < _elapsedTime)
			{
				_elapsedTime = 0.0f;
				return true;
			}
			_elapsedTime += deltaTime;
		}
		return false;
	}

	void Animator::SetInitState(std::string stateName)
	{
		auto i = _states.find(stateName);
		if (i != _states.end())
		{
			_initState = i->second;
			_currentState = i->second;
			_prevState = i->second; // 일단 초기값 설정 시에는 시간 제한이 없는 것으로 두기 위해...
		}
	}

	bool Animator::SetCondition(std::string condition, bool isOn)
	{
		auto i = _conditions.find(condition);
		if (i != _conditions.end())
		{
			i->second = isOn;
		}

		for (auto i : _currentState->GetAllTransitions())
		{
			if (*(i->_isConditionOn) == i->comparison)
			{
				if (onStateChanged != nullptr)
				{
					onStateChanged;
				}
				_prevState = _currentState;
				_currentState = i->_stateNext;
				_elapsedTime = 0.0f;

				return true;
			}
		}

		return false;
	}

	State* Animator::AddNewState(std::string stateName)
	{
		State* state = new State();
		state->SetStateName(stateName);
		_states.insert({ stateName,state });
		return state;
	}

	bool* Animator::AddCondition(std::string name)
	{
		_conditions[name] = false;
		return &_conditions[name];
	}

	State* Animator::GetCurrentState()
	{
		return _currentState;
	}

	std::unordered_map<std::string, State*>& Animator::GetAllStates()
	{
		return _states;
	}

	std::unordered_map<std::string, bool>& Animator::GetAllCondition()
	{
		return _conditions;
	}

}
