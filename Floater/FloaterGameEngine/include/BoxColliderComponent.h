#pragma once
#include "Component.h"
#include "../../FloaterMath/include/Vector3f.h"

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

	public:
		void SetSize(const flt::Vector3f& size);
		flt::Vector3f GetSize() const;

		void SetOffset(const flt::Vector3f& offset);
		flt::Vector3f GetOffset() const;

	private:
		void UpdatePhysTransform();
		void UpdateTransform();

	private:
		Transform* _transform;

		physx::PxPhysics& _physcx;
		physx::PxScene& _scene;
		PhysXData* _physXData;
		
		flt::Vector3f _size;
		flt::Vector3f _offset;
	};
}
