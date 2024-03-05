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
/// �ӽ÷� ����� �ִϸ��̼���Ʈ�ѷ� ������Ʈ.
/// �����̰� ����� �����ϸ� �װ����� ��ü�� ����.
/// 
/// 23.07.26 ������ �����
/// </summary>
namespace Rocket
{
	class GameObject;
	class Animator;
	class State;

	class ROCKET_API AnimationController final : public Component
	{
#pragma region ���Ѵ�� �ϵ��ڵ�
		/// ���Ѵ�� �� �� �� �ֵ��� �ϵ��ڵ�
	public:
		void SetCurrentState(std::string stateName);
	
	private:
		float _elapsedTime = 0.0f;
		std::string _prevStateName = "";

	public:
		std::function<void()> onStateChanged = nullptr;

		///
#pragma endregion ���Ѵ�� �ϵ��ڵ�

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
