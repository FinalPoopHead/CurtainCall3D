#include "State.h"

namespace Rocket
{
	State::State()
		: _transitions(),
		_stateName()
	{

	}

	State::~State()
	{
		for (auto& trans : _transitions)
		{
			delete trans;
		}
	}

	void State::AddTranstiion(State* to, bool* condition, bool comp)
	{
		Transition* trans = new Transition;
		trans->_stateNext = to;
		trans->_isConditionOn = condition;
		trans->comparison = comp;
		_transitions.push_back(trans);
	}

	std::vector<Transition*>& State::GetAllTransitions()
	{
		return _transitions;
	}

	std::string State::GetStateName()
	{
		return _stateName;
	}

	float State::GetDuration()
	{
		return 0.f;
	}

	bool State::GetIsLoop()
	{
		return false;
	}

	std::string State::GetAnimName()
	{
		return std::string();
	}

	std::string State::GetAnimPath()
	{
		return std::string();
	}

	std::string State::GetOwnerPath()
	{
		return std::string();
	}

	void State::SetStateName(std::string stateName)
	{
		_stateName = stateName;
	}

	void State::SetDuration(float duration)
	{

	}

	void State::SetLoop(bool isLoop)
	{

	}

	void State::SetAnimDescName(std::string name)
	{

	}

	void State::SetAnimDescAnimPath(std::string animPath)
	{

	}

	void State::SetAnimDescOwnerPath(std::string ownerPath)
	{

	}

	void State::SetAnimDescObjName(std::string objName)
	{

	}

	void State::SetAnimDataName(std::string name)
	{

	}

	void State::SetAnimDataLoop(bool isLoop)
	{

	}

}
