#include "StaticCollider.h"
#include "GameObject.h"
#include "Transform.h"

#include <physx/PxPhysics.h>
#include <physx/PxPhysicsAPI.h>

namespace Rocket
{
	StaticCollider::StaticCollider()
		: _isCollided(false),
		_wasCollided(false),
		_physXRigid(nullptr)
	{

	}

	void StaticCollider::SetPhysXRigid(physx::PxRigidStatic* rigid)
	{
		_physXRigid = rigid;
		_pxScene = rigid->getScene();
	}

	void StaticCollider::UpdateToPhysics()
	{
		if (_physXRigid == nullptr)
		{
			return;
		}

		_wasTranslated = true;

		Vector3 pos = gameObject->transform.GetPosition();
		Quaternion rot = gameObject->transform.GetRotation();

		physx::PxTransform pxTransform;

		pxTransform.p.x = pos.x;
		pxTransform.p.y = pos.y;
		pxTransform.p.z = pos.z;

		pxTransform.q.x = rot.x;
		pxTransform.q.y = rot.y;
		pxTransform.q.z = rot.z;
		pxTransform.q.w = rot.w;

		_physXRigid->setGlobalPose(pxTransform);
	}

	physx::PxRigidStatic* StaticCollider::GetRigidBody()
{
		return _physXRigid;
	}

	void StaticCollider::Collide()
	{
		_isCollided = true;
	}

	bool StaticCollider::GetIsCollided() const
	{
		return _isCollided;
	}

	bool StaticCollider::GetWasCollided() const
	{
		return _wasCollided;
	}

	void StaticCollider::Flush()
	{
		_wasCollided = _isCollided;
		_isCollided = false;
	}

}
