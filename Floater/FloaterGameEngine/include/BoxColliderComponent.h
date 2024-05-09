#pragma once
#include "Component.h"

namespace physx
{
	class PxPhysics;
	class PxScene;
}

namespace flt
{
	class PhysicsEngine;
	struct PhysXData;
	class Transform;

	class BoxColliderComponent : public Component<BoxColliderComponent>
	{
	public:
		BoxColliderComponent(GameObject* gameObject);
		virtual ~BoxColliderComponent();

	public:
		virtual void OnEnable() override;
		virtual void OnDisable() override;

		virtual void PrePhysics() override;
		virtual void PostPhysics() override;

	private:
		void UpdatePhysTransform();
		void UpdateTransform();

	private:
		Transform* _transform;

		physx::PxPhysics& _physcx;
		physx::PxScene& _scene;
		PhysXData* _physXData;
	};
}
