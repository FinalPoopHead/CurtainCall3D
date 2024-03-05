#pragma once
#include "DLLExporter.h"
#include "Collider.h"
#include "MathHeader.h"

namespace physx
{
	class PxRigidStatic;
}

namespace Rocket
{
	class GameObject;

	class ROCKET_API StaticCollider : public Collider
	{
	public:
		StaticCollider();

	public:
		void SetPhysXRigid(physx::PxRigidStatic* rigid);

	public:
		virtual void UpdateToPhysics() override;

	public:
		physx::PxRigidStatic* GetRigidBody();

	public:
		void Collide();
		bool GetIsCollided() const;

		bool GetWasCollided() const;

		void Flush();

	private:
		physx::PxRigidStatic* _physXRigid;

		bool _isCollided;
		bool _wasCollided;
	};
}
