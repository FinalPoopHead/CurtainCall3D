#pragma once
#include "Component.h"
#include "DLLExporter.h"
#include <string>
#include <functional>

// enum AnimState : int {
// 	ANIM_NONE = 0,
// 	ANIM_IDLE = 1,
// 	ANIM_DIE = 2,
// 	ANIM_JUMP = 3,
// 	ANIM_MOVE_FORWARD = 4,
// 	ANIM_MOVE_LEFT = 5,
// 	ANIM_MOVE_RIGHT = 6,
// 	ANIM_RELOAD = 7,
// 	ANIM_SHOOT = 8,
// };

/// <summary>
/// 임시로 사용할 애니메이션컨트롤러 컴포넌트.
/// 수안이가 만든게 도착하면 그것으로 대체할 예정.
/// 
/// 23.07.26 강석원 인재원
/// </summary>
namespace Rocket
{
	class GameObject;
	class Animator;
	class State;

	class ROCKET_API AnimationController final : public Component
	{
#pragma region 급한대로 하드코딩
		/// 급한대로 막 쓸 수 있도록 하드코딩
	public:
		void SetCurrentState(std::string stateName);
	
	private:
		float _elapsedTime = 0.0f;
		std::string _prevStateName = "";

	public:
		std::function<void()> onStateChanged = nullptr;

		///
#pragma endregion 급한대로 하드코딩

	public:
		AnimationController();
		~AnimationController();

	protected:
		virtual void Start() override;
		virtual void Update() override;

	public:
		void PlayCurrentAnim();

	public:
		void SetAnimator(Animator* animator);
		Animator* GetAnimator();

		void SetCondition(std::string conditionName, bool isOn);

		State* GetCurrentState();

	private:
		Animator* _animator = nullptr;




	};
}
