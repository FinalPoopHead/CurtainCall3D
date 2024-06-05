#pragma once
#include "Component.h"
#include "Collider.h"
#include "../../../FloaterMath/include/Vector3f.h"
#include "../../../FloaterMath/include/Vector4f.h"

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

	class BoxColliderComponent : public Component<BoxColliderComponent>, public Collider
	{
	public:
		BoxColliderComponent();
		virtual ~BoxColliderComponent();

	public:
		virtual void OnCreate() override;
		virtual void OnEnable() override;
		virtual void OnDisable() override;

		virtual void PrePhysics() override;
		virtual void PostPhysics() override;

	public:
		void SetSize(const flt::Vector3f& size);
		flt::Vector3f GetSize() const;

		void SetOffset(const flt::Vector3f& offset);
		flt::Vector3f GetOffset() const;

		void UseKinematic(bool use);

	private:
		void UpdatePhysTransform();
		void UpdateTransform();

	private:
		Transform* _transform;

		physx::PxPhysics& _physcx;
		physx::PxScene& _scene;
		PhysXData* _physXData;
		
		flt::Vector3f _size;
		flt::Vector4f _offset;

		bool _isKinematic;
	};
}
