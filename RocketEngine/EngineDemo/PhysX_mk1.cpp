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

// ��ü�� transform ���� ���� �Ǵ��� test ��
// 23.7.28.AJY
static physx::PxRigidDynamic* mokujinTemp = {};
static  physx::PxTransform mokujinTempTransform = {};

// �����̴� ��ü ����� �Լ�
physx::PxRigidDynamic* createDynamic(const  physx::PxTransform& t, const  physx::PxGeometry& geometry, const  physx::PxVec3& velocity)
{
	physx::PxRigidDynamic* dynamic = PxCreateDynamic(*gPhysics, t, geometry, *gMaterial, 10.0f);
	dynamic->setAngularDamping(0.5f);
	dynamic->setLinearVelocity(velocity);
	gScene->addActor(*dynamic);
	return dynamic;
}

// �ڽ� (���� ��) ����� �Լ�
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
	// foundation ����
	gFoundation = PxCreateFoundation(PX_PHYSICS_VERSION, gAllocator, gErrorCallback);

	// visual debugger ����, ���ÿ� ����
	gPvd = PxCreatePvd(*gFoundation);
	physx::PxPvdTransport* transport = physx::PxDefaultPvdSocketTransportCreate("127.0.0.1", 5425, 10);
	gPvd->connect(*transport, physx::PxPvdInstrumentationFlag::eALL);

	// ����, ����, ���� ���� ������ ���� ����
	gPhysics = PxCreatePhysics(PX_PHYSICS_VERSION, *gFoundation, physx::PxTolerancesScale(), true, gPvd);

	// ���� ���� ����
	physx::PxSceneDesc sceneDesc(gPhysics->getTolerancesScale());
	sceneDesc.gravity = physx::PxVec3(0.0f, -9.81f, 0.0f);
	gDispatcher = physx::PxDefaultCpuDispatcherCreate(2);
	sceneDesc.cpuDispatcher = gDispatcher;
	sceneDesc.filterShader = physx::PxDefaultSimulationFilterShader;
	gScene = gPhysics->createScene(sceneDesc);

	// Pvd�� ���� ������
	physx::PxPvdSceneClient* pvdClient = gScene->getScenePvdClient();
	if (pvdClient)
	{
		pvdClient->setScenePvdFlag( physx::PxPvdSceneFlag::eTRANSMIT_CONSTRAINTS, true);
		pvdClient->setScenePvdFlag( physx::PxPvdSceneFlag::eTRANSMIT_CONTACTS, true);
		pvdClient->setScenePvdFlag( physx::PxPvdSceneFlag::eTRANSMIT_SCENEQUERIES, true);
	}
	// ��Ƽ���� ����(����)
	gMaterial = gPhysics->createMaterial(0.5f, 0.5f, 0.6f);

	// ground ���� ��, ���Ƿ� shape �ٿ��ֱ�
	physx::PxRigidStatic* groundPlane = PxCreatePlane(*gPhysics, physx::PxPlane(0, 1, 0, 0), *gMaterial);
	physx::PxShape* gpShape = gPhysics->createShape(physx::PxBoxGeometry(1.0f, 256.0f, 256.0f), *gMaterial);
	groundPlane->attachShape(*gpShape);
	gScene->addActor(*groundPlane);

	// rigid body + collider ����� �κ�
	// ��ġ, ���� ũ��, �ӵ� ������ ����.
	if (!interactive)
		//mokujinTemp = createDynamic(PxTransform(PxVec3(10, 100, 10)), PxBoxGeometry(10, 40, 10), PxVec3(-20, 50, 0));
		mokujinTemp = createDynamic(physx::PxTransform(physx::PxVec3(5, 0, 0)), physx::PxBoxGeometry(1, 1, 1), physx::PxVec3(0, 0, 0));

	/*
	// �׿��ִ� ���� ����� �κ�
	for (PxU32 i = 0; i < 5; i++)
		createStack(PxTransform(PxVec3(0, 0, stackZ -= 10.0f)), 10, 2.0f);

	// ���ư��� �� �����ϴ� �κ�
	if (!interactive)
		createDynamic(PxTransform(PxVec3(0, 40, 100)), PxSphereGeometry(10), PxVec3(0, -50, -100));
	*/
}

void stepPhysics(bool /*interactive*/)
{
	// 60 FPS�� �ùķ��̼� ����, ��� ó��
	gScene->simulate(1.0f / 60.0f);
	gScene->fetchResults(true);
}

void cleanupPhysics(bool /*interactive*/)
{
	// initPhysics���� �ʱ�ȭ���� ���� �������� release
	PX_RELEASE(gScene);
	PX_RELEASE(gDispatcher);
	PX_RELEASE(gPhysics);
	// visual debugger�� release
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
	// 1000������ ���� �ùķ��̼�
	static const  physx::PxU32 frameCount = 10;
	// �ʱ�ȭ, �ܰ� ����, Ŭ����
	initPhysics(false);
	for (physx::PxU32 i = 0; i < frameCount; i++)
	{
		stepPhysics(false);
	}
	// ��ü�� transform ���� ���� �Ǵ��� test
	// �Լ� ��ȯ���� int���� PxTransform ���� �ٲ�
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