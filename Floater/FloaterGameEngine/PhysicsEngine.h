#pragma once
#include "../FloaterUtil/include/FloaterType.h"

namespace physx
{
	class PxDefaultAllocator;
	class PxDefaultErrorCallback;
	class PxFoundation;
	class PxPhysics;
	class PxPvd;
	class PxDefaultCpuDispatcher;
	class PxScene;
	class PxMaterial;
	class PxRigidActor;
	class PxCustomGeometry;
}

namespace flt
{
	class PhysicsEngine
	{
	public:
		PhysicsEngine();
		void Initialize();
		void Finalize();

		void Update(float deltaTime);

	private:
		physx::PxDefaultAllocator* _allocator;
		physx::PxDefaultErrorCallback* _errorCallback;
		physx::PxFoundation* _foundation;
		physx::PxPhysics* _physics;
		physx::PxPvd* _pvd;
		physx::PxDefaultCpuDispatcher* _dispatcher;
		physx::PxScene* _scene;
		//physx::PxMaterial* _material;
		//physx::PxRigidActor* _actor;
	};
}


