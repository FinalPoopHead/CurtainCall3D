#include "AnimationController.h"
#include "GraphicsSystem.h"
#include "ModelRendererBase.h"
#include "StaticModelRenderer.h"
#include "DynamicModelRenderer.h"
#include "GameObject.h"
#include "Animator.h"
#include "State.h"
#include "TimeSystem.h"

namespace Rocket
{
	void AnimationController::SetCurrentState(std::string stateName)
	{
		if (_prevStateName == stateName)
		{
			return;
		}

		_prevStateName = _animator->GetCurrentState()->GetStateName();

		_elapsedTime = 0.0f;
		_animator->SetCurrentState(stateName);
		PlayCurrentAnim();

		if (onStateChanged != nullptr)
		{
			onStateChanged();
		}
	}

	AnimationController::AnimationController()
	{

	}

	AnimationController::~AnimationController()
	{

	}

	void AnimationController::Start()
	{
		_animator->Initialize();
	}

	void AnimationController::Update()
	{
		State* currentState = GetCurrentState();
		if (currentState->GetIsLoop())
		{
			if (currentState->GetDuration() < _elapsedTime)
			{
				_elapsedTime = 0.0f;
				PlayCurrentAnim();
			}
			_elapsedTime += Rocket::Core::TimeSystem::Instance().GetDeltaTime();
		}
	}

	// 임시
	void AnimationController::PlayCurrentAnim()
	{
		Rocket::Core::ModelRendererBase* renderer = gameObject->GetComponentDynamic<Rocket::Core::ModelRendererBase>();
		
		if (!renderer)
		{
			return;
		}

// 		Rocket::Core::AnimData currentAnimData = GetCurrentState()->GetAnimData();
// 		Rocket::Core::RenderSystem::Instance().PlayAnimation(renderer, currentAnimData);
	}

	void AnimationController::SetAnimator(Animator* animator)
	{
		_animator = animator;
		for (auto& iter : _animator->GetAllStates())
		{
			iter.second->SetAnimDescObjName(gameObject->objName);
		}
	}

	Rocket::Animator* AnimationController::GetAnimator()
	{
		return _animator;
	}

	void AnimationController::SetCondition(std::string conditionName, bool isOn)
	{
		if (_animator->SetCondition(conditionName, isOn))
		{
			PlayCurrentAnim();
		}
	}

	State* AnimationController::GetCurrentState()
	{
		return _animator->GetCurrentState();
	}
}
