#include "PhysicsEngine.h"
#include "./include/Collider.h"
#include "./include/GameEngine.h"
#include "../FloaterUtil/include/FloaterMacro.h"

#pragma warning(push)
#pragma warning(disable:26495 6297 33010)
#include "../External/include/physx/include/PxPhysicsAPI.h"
#pragma warning(pop)

#ifdef _DEBUG
#pragma comment(lib, "../External/lib/x64/debug/PhysX_64.lib")
#pragma comment(lib, "../External/lib/x64/debug/PhysXCommon_64.lib")
#pragma comment(lib, "../External/lib/x64/debug/PhysXFoundation_64.lib")
#pragma comment(lib, "../External/lib/x64/debug/PhysXCooking_64.lib")
#pragma comment(lib, "../External/lib/x64/debug/PhysXExtensions_static_64.lib")
#pragma comment(lib, "../External/lib/x64/debug/PhysXPvdSDK_static_64.lib")
#else
#pragma comment(lib, "../External/lib/x64/release/PhysX_64.lib")
#pragma comment(lib, "../External/lib/x64/release/PhysXCommon_64.lib")
#pragma comment(lib, "../External/lib/x64/release/PhysXFoundation_64.lib")
#pragma comment(lib, "../External/lib/x64/release/PhysXCooking_64.lib")
#pragma comment(lib, "../External/lib/x64/release/PhysXExtensions_static_64.lib")
#pragma comment(lib, "../External/lib/x64/release/PhysXPvdSDK_static_64.lib")
#endif

#include <iostream>


class SimulationEventCallback : public physx::PxSimulationEventCallback
{
public:
	SimulationEventCallback() : physx::PxSimulationEventCallback(), _gameEngine(flt::GameEngine::Instance())
	{
	}

	virtual void onConstraintBreak(physx::PxConstraintInfo* constraints, physx::PxU32 count) override
	{
		PX_UNUSED(constraints);
		PX_UNUSED(count);
	}

	virtual void onWake(physx::PxActor** actors, physx::PxU32 count) override
	{
		PX_UNUSED(actors);
		PX_UNUSED(count);
	}

	virtual void onSleep(physx::PxActor** actors, physx::PxU32 count) override
	{
		PX_UNUSED(actors);
		PX_UNUSED(count);
	}

	virtual void onContact(const physx::PxContactPairHeader& pairHeader, const physx::PxContactPair* pairs, physx::PxU32 nbPairs) override
	{
		//ASSERT(false, "onContact");
		std::cout << "onContact" << std::endl;
		for (physx::PxU32 i = 0; i < nbPairs; i++)
		{
			const physx::PxContactPair& cp = pairs[i];
			if (cp.events & physx::PxPairFlag::eNOTIFY_TOUCH_FOUND)
			{
				physx::PxActor* actor0 = pairHeader.actors[0];
				physx::PxActor* actor1 = pairHeader.actors[1];
				physx::PxShape* shape0;
				physx::PxShape* shape1;
				flt::Collider* collider0 = static_cast<flt::Collider*>(actor0->userData);
				flt::Collider* collider1 = static_cast<flt::Collider*>(actor1->userData);
				//ASSERT(collider0, "Collider0 is nullptr");
				//ASSERT(collider1, "Collider1 is nullptr");
				//ASSERT(false, "onContact");
			}
			else if (cp.events & physx::PxPairFlag::eNOTIFY_TOUCH_PERSISTS)
			{
				//ASSERT(false, "onContact");
				// 접촉 유지중

			}
			else if (cp.events & physx::PxPairFlag::eNOTIFY_TOUCH_LOST)
			{
				//ASSERT(false, "onContact");
				// 접촉 종료

			}
		}
	}
	
	virtual void onTrigger(physx::PxTriggerPair* pairs, physx::PxU32 count) override
	{
		PX_UNUSED(pairs); 
		PX_UNUSED(count);
	}

	virtual void onAdvance(const physx::PxRigidBody* const* bodyBuffer, const physx::PxTransform* poseBuffer, const physx::PxU32 count) override
	{
	}

private:
	flt::GameEngine* _gameEngine;
};

static physx::PxFilterFlags contactReportFilterShader(physx::PxFilterObjectAttributes attributes0, physx::PxFilterData filterData0,
	physx::PxFilterObjectAttributes attributes1, physx::PxFilterData filterData1,
	physx::PxPairFlags& pairFlags, const void* constantBlock, physx::PxU32 constantBlockSize)
{
	PX_UNUSED(attributes0);
	PX_UNUSED(attributes1);
	PX_UNUSED(filterData0);
	PX_UNUSED(filterData1);
	PX_UNUSED(constantBlockSize);
	PX_UNUSED(constantBlock);

	// all initial and persisting reports for everything, with per-point data
	pairFlags = physx::PxPairFlag::eSOLVE_CONTACT | physx::PxPairFlag::eDETECT_DISCRETE_CONTACT
		| physx::PxPairFlag::eNOTIFY_TOUCH_FOUND
		| physx::PxPairFlag::eNOTIFY_TOUCH_PERSISTS
		| physx::PxPairFlag::eNOTIFY_TOUCH_LOST
		| physx::PxPairFlag::eNOTIFY_CONTACT_POINTS;
	return physx::PxFilterFlag::eDEFAULT;
}


flt::PhysicsEngine::PhysicsEngine() :
	_allocator(new physx::PxDefaultAllocator()),
	_errorCallback(new physx::PxDefaultErrorCallback),
	_foundation(nullptr),
	_physics(nullptr),
	_pvd(nullptr),
	_dispatcher(nullptr),
	_scene(nullptr),
	_callback(nullptr)
	//_material(nullptr),
	//_actor(nullptr)
{

}

void flt::PhysicsEngine::Initialize()
{
	_foundation = PxCreateFoundation(PX_PHYSICS_VERSION, *_allocator, *_errorCallback);
#ifdef _DEBUG
	_pvd = physx::PxCreatePvd(*_foundation);
	constexpr char PVD_HOST[] = "127.0.0.1";
	physx::PxPvdTransport* transport = physx::PxDefaultPvdSocketTransportCreate(PVD_HOST, 5425, 10);
	_pvd->connect(*transport, physx::PxPvdInstrumentationFlag::eALL);
#else
	_pvd = nullptr;
#endif

	_physics = PxCreatePhysics(PX_PHYSICS_VERSION, *_foundation, physx::PxTolerancesScale(), true, _pvd);

	_callback = new SimulationEventCallback();

	physx::PxSceneDesc sceneDesc(_physics->getTolerancesScale());
	sceneDesc.gravity = physx::PxVec3(0.0f, -9.81f, 0.0f);
	_dispatcher = physx::PxDefaultCpuDispatcherCreate(2);
	sceneDesc.cpuDispatcher = _dispatcher;
	//sceneDesc.filterShader = physx::PxDefaultSimulationFilterShader;
	sceneDesc.filterShader = contactReportFilterShader;
	sceneDesc.simulationEventCallback = _callback;

	_scene = _physics->createScene(sceneDesc);
	_scene->setVisualizationParameter(physx::PxVisualizationParameter::eCOLLISION_SHAPES, 1.0f);
	_scene->setVisualizationParameter(physx::PxVisualizationParameter::eSCALE, 1.0f);

#ifdef _DEBUG
	physx::PxPvdSceneClient* pvdClient = _scene->getScenePvdClient();
	if (pvdClient)
	{
		pvdClient->setScenePvdFlag(physx::PxPvdSceneFlag::eTRANSMIT_CONSTRAINTS, true);
		pvdClient->setScenePvdFlag(physx::PxPvdSceneFlag::eTRANSMIT_CONTACTS, true);
		pvdClient->setScenePvdFlag(physx::PxPvdSceneFlag::eTRANSMIT_SCENEQUERIES, true);
	}
#endif // _DEBUG

	// Ground plane
	physx::PxMaterial* plainMaterial = _physics->createMaterial(0.5f, 0.5f, 0.6f);
	//physx::PxRigidStatic* planeActor = _physics->createRigidStatic(physx::PxTransform(physx::PxQuat(PX_PIDIV2, physx::PxVec3(0, 0, 1))));
	//physx::PxRigidActorExt::createExclusiveShape(*planeActor, physx::PxPlaneGeometry(), *plainMaterial);
	physx::PxRigidStatic* planeActor = PxCreatePlane(*_physics, physx::PxPlane(0, 1, 0, 0), *plainMaterial);
	_scene->addActor(*planeActor);
	plainMaterial->release();

	//_scene->setVisualizationParameter(physx::PxVisualizationParameter::eSCALE, 1.f);
	//_scene->setVisualizationParameter(physx::PxVisualizationParameter::eCOLLISION_SHAPES, 1.0f);


}

void flt::PhysicsEngine::Finalize()
{
	if (_callback)
	{
		_scene->setSimulationEventCallback(nullptr);
		delete _callback;
	}

	PX_RELEASE(_scene);
	PX_RELEASE(_dispatcher);
	PX_RELEASE(_physics);
	if (_pvd)
	{
		physx::PxPvdTransport* transport = _pvd->getTransport();
		_pvd->release();	_pvd = NULL;
		PX_RELEASE(transport);
	}
	PX_RELEASE(_foundation);

}

void flt::PhysicsEngine::Update(float deltaTime)
{
	_scene->simulate(deltaTime);
	_scene->fetchResults(true);
}

flt::Collider* flt::PhysicsEngine::Raycast(const Vector3f& origin, const Vector3f& direction, float distance)
{
	physx::PxRaycastBuffer hit;
	physx::PxQueryFilterData filterData;
	filterData.flags = physx::PxQueryFlag::eSTATIC | physx::PxQueryFlag::eDYNAMIC;
	filterData.data.word0 = 0;
	filterData.data.word1 = 0;
	filterData.data.word2 = 0;
	filterData.data.word3 = 0;
	_scene->raycast(physx::PxVec3(origin.x, origin.y, origin.z), physx::PxVec3(direction.x, direction.y, direction.z), distance, hit, physx::PxHitFlag::eDEFAULT, filterData);
	if (hit.hasBlock)
	{
		return static_cast<Collider*>(hit.block.actor->userData);
	}
	return nullptr;

}
