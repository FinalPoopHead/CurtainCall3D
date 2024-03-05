#pragma once
#include "DLLExporter.h"
#include <functional>
#include <vector>
#include <string>

namespace Rocket
{
	class State;

	class ROCKET_API Transition
	{
	public:
		bool* _isConditionOn;  // ���� bool ��
		bool comparison; // ���ؾ� �� bool ��
		State* _stateNext; // �Ѿ �� �ִ� ���� State
	};

	class ROCKET_API State
	{
	public:
		State();
		~State();

	public:
		void AddTranstiion(State* to, bool* condition, bool comp);

	public:
		std::vector<Transition*>& GetAllTransitions();

	private:
		std::vector<Transition*> _transitions;

	public:
		std::string GetStateName();
		float GetDuration();
		bool GetIsLoop();
		std::string GetAnimName();
		std::string GetAnimPath();
		std::string GetOwnerPath();

	public:
		void SetStateName(std::string stateName);
		void SetDuration(float duration);
		void SetLoop(bool isLoop);

	private:
		std::string _stateName;

	public:
		void SetAnimDescName(std::string name);
		void SetAnimDescAnimPath(std::string animPath);
		void SetAnimDescOwnerPath(std::string ownerPath);
		void SetAnimDescObjName(std::string objName);

		void SetAnimDataName(std::string name);
		void SetAnimDataLoop(bool isLoop);

	};
}

