#include "PhysicsEngine.h"

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


flt::PhysicsEngine::PhysicsEngine() : 
	_allocator(new physx::PxDefaultAllocator()),
	_errorCallback(new physx::PxDefaultErrorCallback),
	_foundation(nullptr),
	_physics(nullptr),
	_pvd(nullptr),
	_dispatcher(nullptr),
	_scene(nullptr),
	//_material(nullptr),
	//_actor(nullptr)
{

}

void flt::PhysicsEngine::Initialize()
{
	_foundation = PxCreateFoundation(PX_PHYSICS_VERSION, *_allocator, *_errorCallback);

	_pvd = physx::PxCreatePvd(*_foundation);
	constexpr char PVD_HOST[] = "127.0.0.1";
	physx::PxPvdTransport* transport = physx::PxDefaultPvdSocketTransportCreate(PVD_HOST, 5425, 10);
	_pvd->connect(*transport, physx::PxPvdInstrumentationFlag::eALL);

	_physics = PxCreatePhysics(PX_PHYSICS_VERSION, *_foundation, physx::PxTolerancesScale(), true, _pvd);

	physx::PxSceneDesc sceneDesc(_physics->getTolerancesScale());
	//sceneDesc.gravity = physx::PxVec3(0.0f, -9.81f, 0.0f);
	_dispatcher = physx::PxDefaultCpuDispatcherCreate(2);
	sceneDesc.cpuDispatcher = _dispatcher;
	sceneDesc.filterShader = physx::PxDefaultSimulationFilterShader;

	_scene = _physics->createScene(sceneDesc);
	_scene->setVisualizationParameter(physx::PxVisualizationParameter::eCOLLISION_SHAPES, 1.0f);
	_scene->setVisualizationParameter(physx::PxVisualizationParameter::eSCALE, 1.0f);

	physx::PxPvdSceneClient* pvdClient = _scene->getScenePvdClient();
	if (pvdClient)
	{
		pvdClient->setScenePvdFlag(physx::PxPvdSceneFlag::eTRANSMIT_CONSTRAINTS, true);
		pvdClient->setScenePvdFlag(physx::PxPvdSceneFlag::eTRANSMIT_CONTACTS, true);
		pvdClient->setScenePvdFlag(physx::PxPvdSceneFlag::eTRANSMIT_SCENEQUERIES, true);
	}

	//_material = _physics->createMaterial(0.5f, 0.5f, 0.6f);


	//// Ground plane
	//physx::PxRigidStatic* planeActor = _physics->createRigidStatic(physx::PxTransform(physx::PxQuat(PX_PIDIV2, physx::PxVec3(0, 0, 1))));
	//physx::PxRigidActorExt::createExclusiveShape(*planeActor, physx::PxPlaneGeometry(), *_material);
	//_scene->addActor(*planeActor);

	//_scene->setVisualizationParameter(physx::PxVisualizationParameter::eSCALE, 1.f);
	//_scene->setVisualizationParameter(physx::PxVisualizationParameter::eCOLLISION_SHAPES, 1.0f);
}

void flt::PhysicsEngine::Finalize()
{
	_scene->release();
	_scene = nullptr;

	_physics->release();
	_physics = nullptr;

	_pvd->release();
	_pvd = nullptr;
	
	_foundation->release();
	_foundation = nullptr;
}

void flt::PhysicsEngine::Update(float deltaTime)
{
	_scene->simulate(deltaTime);
}
