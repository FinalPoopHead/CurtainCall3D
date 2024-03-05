#pragma once
#define _SILENCE_CXX20_CISO646_REMOVED_WARNING
#include "Component.h"
#include <physx/PxPhysics.h>
#include <physx/PxPhysicsAPI.h>

namespace RocketEngine
{
	class ROCKET_API Raycast : public Component
	{
	public:
		Raycast(GameObject* owner);

	public:
		void UpdateRayInfo();
		void PerformRayCast();

	private:
		// Raycast ���� ���� �ӽ÷� ��������. ���� �ʼ�. 23.7.31.AJY.
		physx::PxVec3 _rayOrigin;
		physx::PxVec3 _rayDirection;
		physx::PxReal _rayLength;

		physx::PxScene* _scene;
	};
}