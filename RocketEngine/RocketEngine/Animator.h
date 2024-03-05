#pragma once
#include "DLLExporter.h"
#include <string>
#include <vector>
#include <unordered_map>
#include <functional>

namespace Rocket
{
	class State;

	class ROCKET_API Animator
	{
		/// 급한대로 막 쓸 수 있도록 하드코딩
	public:
		void SetCurrentState(std::string stateName);

	public:
		Animator();
		~Animator();

	public:
		void Initialize();

	public:
		bool CheckAnimCycle(float deltaTime);

	private:
		float _elapsedTime = 0.0f;

	public:
		void SetInitState(std::string stateName);
		bool SetCondition(std::string condition, bool isOn);

		State* AddNewState(std::string stateName);
		bool* AddCondition(std::string name);

		State* GetCurrentState();

	public:
		std::unordered_map<std::string, State*>& GetAllStates();
		std::unordered_map<std::string, bool>& GetAllCondition();

	private:

		std::unordered_map<std::string, State*> _states;
		std::unordered_map<std::string, bool> _conditions;

		State* _initState = nullptr;
		State* _currentState = nullptr;
		State* _prevState = nullptr;

	public:
		std::function<void(void)> onStateChanged = nullptr;
	};
}
