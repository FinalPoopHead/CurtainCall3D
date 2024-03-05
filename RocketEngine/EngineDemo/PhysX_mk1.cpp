#pragma warning(push)
#pragma warning(disable: 26495 26451 6297 33010 4099)

#include "PhysX_mk1.h"
#include <ctype.h>
#include <windows.h>

static  physx::PxDefaultAllocator		gAllocator;
static  physx::PxDefaultErrorCallback	gErrorCallback;
static  physx::PxFoundation* gFoundation = NULL;
static  physx::PxPhysics* gPhysics = NULL;
static  physx::PxDefaultCpuDispatcher* gDispatcher = NULL;
static  physx::PxScene* gScene = NULL;
static  physx::PxMaterial* gMaterial = NULL;
static  physx::PxPvd* gPvd = NULL;

static  physx::PxReal stackZ = 10.0f;

// 본체와 transform 정보 연동 되는지 test 용
// 23.7.28.AJY
static physx::PxRigidDynamic* mokujinTemp = {};
static  physx::PxTransform mokujinTempTransform = {};

// 움직이는 물체 만드는 함수
physx::PxRigidDynamic* createDynamic(const  physx::PxTransform& t, const  physx::PxGeometry& geometry, const  physx::PxVec3& velocity)
{
	physx::PxRigidDynamic* dynamic = PxCreateDynamic(*gPhysics, t, geometry, *gMaterial, 10.0f);
	dynamic->setAngularDamping(0.5f);
	dynamic->setLinearVelocity(velocity);
	gScene->addActor(*dynamic);
	return dynamic;
}

// 박스 (여러 개) 만드는 함수
void createStack(const  physx::PxTransform& t, physx::PxU32 size, physx::PxReal halfExtent)
{
	physx::PxShape* shape = gPhysics->createShape(physx::PxBoxGeometry(halfExtent, halfExtent, halfExtent), *gMaterial);
	for (physx::PxU32 i = 0; i < size; i++)
	{
		for (physx::PxU32 j = 0; j < size - i; j++)
		{
			physx::PxTransform localTm(physx::PxVec3(physx::PxReal(j * 2) - physx::PxReal(size - i), physx::PxReal(i * 2 + 1), 0) * halfExtent);
			physx::PxRigidDynamic* body = gPhysics->createRigidDynamic(t.transform(localTm));
			body->attachShape(*shape);
			physx::PxRigidBodyExt::updateMassAndInertia(*body, 10.0f);
			gScene->addActor(*body);
		}
	}
	shape->release();
}

void initPhysics(bool interactive)
{
	// foundation 세팅
	gFoundation = PxCreateFoundation(PX_PHYSICS_VERSION, gAllocator, gErrorCallback);

	// visual debugger 세팅, 로컬에 연결
	gPvd = PxCreatePvd(*gFoundation);
	physx::PxPvdTransport* transport = physx::PxDefaultPvdSocketTransportCreate("127.0.0.1", 5425, 10);
	gPvd->connect(*transport, physx::PxPvdInstrumentationFlag::eALL);

	// 버전, 세팅, 단위 등의 정보를 담은 물리
	gPhysics = PxCreatePhysics(PX_PHYSICS_VERSION, *gFoundation, physx::PxTolerancesScale(), true, gPvd);

	// 씬에 대한 설정
	physx::PxSceneDesc sceneDesc(gPhysics->getTolerancesScale());
	sceneDesc.gravity = physx::PxVec3(0.0f, -9.81f, 0.0f);
	gDispatcher = physx::PxDefaultCpuDispatcherCreate(2);
	sceneDesc.cpuDispatcher = gDispatcher;
	sceneDesc.filterShader = physx::PxDefaultSimulationFilterShader;
	gScene = gPhysics->createScene(sceneDesc);

	// Pvd에 정보 보내기
	physx::PxPvdSceneClient* pvdClient = gScene->getScenePvdClient();
	if (pvdClient)
	{
		pvdClient->setScenePvdFlag( physx::PxPvdSceneFlag::eTRANSMIT_CONSTRAINTS, true);
		pvdClient->setScenePvdFlag( physx::PxPvdSceneFlag::eTRANSMIT_CONTACTS, true);
		pvdClient->setScenePvdFlag( physx::PxPvdSceneFlag::eTRANSMIT_SCENEQUERIES, true);
	}
	// 머티리얼 생성(임의)
	gMaterial = gPhysics->createMaterial(0.5f, 0.5f, 0.6f);

	// ground 생성 후, 임의로 shape 붙여주기
	physx::PxRigidStatic* groundPlane = PxCreatePlane(*gPhysics, physx::PxPlane(0, 1, 0, 0), *gMaterial);
	physx::PxShape* gpShape = gPhysics->createShape(physx::PxBoxGeometry(1.0f, 256.0f, 256.0f), *gMaterial);
	groundPlane->attachShape(*gpShape);
	gScene->addActor(*groundPlane);

	// rigid body + collider 만드는 부분
	// 위치, 모양과 크기, 속도 순으로 들어간다.
	if (!interactive)
		//mokujinTemp = createDynamic(PxTransform(PxVec3(10, 100, 10)), PxBoxGeometry(10, 40, 10), PxVec3(-20, 50, 0));
		mokujinTemp = createDynamic(physx::PxTransform(physx::PxVec3(5, 0, 0)), physx::PxBoxGeometry(1, 1, 1), physx::PxVec3(0, 0, 0));

	/*
	// 쌓여있는 상자 만드는 부분
	for (PxU32 i = 0; i < 5; i++)
		createStack(PxTransform(PxVec3(0, 0, stackZ -= 10.0f)), 10, 2.0f);

	// 날아가는 공 생성하는 부분
	if (!interactive)
		createDynamic(PxTransform(PxVec3(0, 40, 100)), PxSphereGeometry(10), PxVec3(0, -50, -100));
	*/
}

void stepPhysics(bool /*interactive*/)
{
	// 60 FPS로 시뮬레이션 진행, 결과 처리
	gScene->simulate(1.0f / 60.0f);
	gScene->fetchResults(true);
}

void cleanupPhysics(bool /*interactive*/)
{
	// initPhysics에서 초기화해준 전역 변수들을 release
	PX_RELEASE(gScene);
	PX_RELEASE(gDispatcher);
	PX_RELEASE(gPhysics);
	// visual debugger도 release
	if (gPvd)
	{
		physx::PxPvdTransport* transport = gPvd->getTransport();
		gPvd->release();	gPvd = NULL;
		PX_RELEASE(transport);
	}
	PX_RELEASE(gFoundation);

	printf("SnippetHelloWorld done.\n");
}

int snippetMain(physx::PxTransform& transform)
{
	// 1000프레임 동안 시뮬레이션
	static const  physx::PxU32 frameCount = 10;
	// 초기화, 단계 진행, 클린업
	initPhysics(false);
	for (physx::PxU32 i = 0; i < frameCount; i++)
	{
		stepPhysics(false);
	}
	// 본체에 transform 정보 연동 되는지 test
	// 함수 반환형을 int에서 PxTransform 으로 바꿈
	// 23.7.28.AJY
	mokujinTempTransform = mokujinTemp->getGlobalPose();
	transform = mokujinTempTransform;
	cleanupPhysics(false);

	return 0;
}

//	int main(int argc, char** argv)
//	{
//		return snippetMain(argc, argv);
//	}

#pragma warning(pop)