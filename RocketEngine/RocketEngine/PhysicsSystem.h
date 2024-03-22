#pragma once
#include <PxPhysics.h>
#include <PxPhysicsAPI.h>
#include <extensions/PxDefaultAllocator.h>
#include <extensions/PxDefaultErrorCallback.h>
#include <vector>
#include "Singleton.h"

#include "MathHeader.h"

namespace Rocket
{
	class GameObject;
	class Collider;
	class DynamicCollider;
	class BoxCollider;
	class SphereCollider;
	class Raycast;
}

namespace Rocket::Core
{
	class PhysicsSystem : public Singleton<PhysicsSystem>
	{
		friend Singleton;
	private:
		PhysicsSystem();		// 싱글턴이기 때문에 외부에서 생성할 수 없도록.

	public:
		void Initialize();
		void Finalize();

	public:
		void PhysicsUpdate(float deltaTime);
		void UpdateToPhysics();
		void Flush();

	public:
		void CreatePhysXScene();

	public:
		void MakeAllCollider();
		void MakePlaneCollider(Rocket::GameObject* object);
		void MakeBoxCollider(Rocket::GameObject* object);
		void MakeSphereCollider(Rocket::GameObject* object);
		void MakeCapsuleCollider(Rocket::GameObject* object);
		void MakeStaticBoxCollider(Rocket::GameObject* object);
		physx::PxFixedJoint* MakeFixedJoint(physx::PxRigidDynamic* actor1, physx::PxRigidDynamic* actor2);
		void MakeFixedJoint();
		void AddActorsToScene();

	public:
		Rocket::Collider* RayCast(Vector3 original, Vector3 direction, float length, int* type = nullptr);

	public:
		physx::PxScene* GetPxScene();

	public:
		std::vector<physx::PxRigidDynamic*>& GetRigidDynamicsList();
		std::vector<physx::PxRigidStatic*>& GetRigidStaticsList();

	private:
		std::vector<physx::PxRigidDynamic*> _rigidDynamics;
		std::vector<physx::PxRigidStatic*> _rigidStatics;
		std::vector<physx::PxFixedJoint*> _fixedJoints;

	private:
		physx::PxDefaultAllocator		_allocator;
		physx::PxDefaultErrorCallback	_errorCallback;
		physx::PxFoundation* _foundation;
		physx::PxPhysics* _physics;
		physx::PxDefaultCpuDispatcher* _dispatcher;
		physx::PxScene* _pxScene;
		physx::PxMaterial* _material;
		physx::PxPvd* _pvd;
	};
}
