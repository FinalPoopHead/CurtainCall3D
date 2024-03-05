#pragma once
#include "DLLExporter.h"
#include "Collider.h"
#include <vector>
#include "MathHeader.h"

namespace Rocket
{
	enum class eForceMode : int
	{
		eFORCE,				//!< parameter has unit of mass * length / time^2, i.e., a force
		eIMPULSE,			//!< parameter has unit of mass * length / time, i.e., force * time
		eVELOCITY_CHANGE,	//!< parameter has unit of length / time, i.e., the effect is mass independent: a velocity change.
		eACCELERATION		//!< parameter has unit of length/ time^2, i.e., an acceleration. It gets treated just like a force except the mass is not divided out before integration.
	};
}

namespace physx
{
	class PxRigidDynamic;
}

namespace Rocket
{
	class GameObject;

	class ROCKET_API DynamicCollider : public Collider
	{
	public:
		DynamicCollider();

	protected:
		virtual void Start() override;

	public:
		void Collide();
		bool GetIsCollided() const;

		bool GetWasCollided() const;

		void Flush();

	public:
		void DisableSimulation();
		void EnableSimulation();

	public:
		void SetPhysXRigid(physx::PxRigidDynamic* rigid);

	public:
		std::vector<DynamicCollider*>& GetServants();
		void SetFixedJoint(DynamicCollider* rhs);

	private:
		std::vector<DynamicCollider*> _servant;

	public:
		void UpdateFromPhysics(Vector3 pos, Quaternion quat);
		virtual void UpdateToPhysics() override;
		void LockRotationXZ();

	public:
		virtual float GetWidth() const abstract;
		virtual float GetHeight() const abstract;
		virtual float GetDepth() const abstract;

	public:
		void AddForce(Vector3 direction, eForceMode forceMode);

	public:
		Vector3 GetVelocity() const;
		void SetVelocity(Vector3 velocity);
		void AddVelocity(Vector3 velocity);
		void SetIsJumping(bool jump);
		bool GetIsJumping() const;
		physx::PxRigidDynamic* GetRigidBody();


	private:
		physx::PxRigidDynamic* _physXRigid;

		// 점프 안할 때는 y좌표 잠궈버리기 위해 추가. 23.8.1.AJY.
		bool _isJumping;
		bool _isCollided;
		bool _wasCollided;

	private:
		Matrix _prevParentWorldTM;
		Quaternion _prevParentRot;
	};
}
