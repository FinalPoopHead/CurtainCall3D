#pragma once
#include "../FloaterUtil/include/FloaterType.h"
#include "../FloaterMath/include/Vector3f.h"
#include "CollisionPair.h"
#include <vector>

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

class SimulationEventCallback;

namespace flt
{
	class Collider;

	class PhysicsEngine
	{
	public:
		PhysicsEngine();
		void Initialize();
		void Finalize();

		void Update(float deltaTime, std::vector<CollisionPair>& collsionPairs);

		Collider* Raycast(const Vector3f& origin, const Vector3f& direction, float distance);

		physx::PxPhysics* GetPhysics() { return _physics; }
		physx::PxScene* GetScene() { return _scene; }

	private:
		physx::PxDefaultAllocator* _allocator;
		physx::PxDefaultErrorCallback* _errorCallback;
		physx::PxFoundation* _foundation;
		physx::PxPhysics* _physics;
		physx::PxPvd* _pvd;
		physx::PxDefaultCpuDispatcher* _dispatcher;
		physx::PxScene* _scene;

		SimulationEventCallback* _callback;
		std::vector<CollisionPair> _collisionPairs;
		//physx::PxMaterial* _material;
		//physx::PxRigidActor* _actor;
	};
}


