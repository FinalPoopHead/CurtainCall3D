#pragma once
#define _SILENCE_CXX20_CISO646_REMOVED_WARNING
#pragma warning(push)
#pragma warning(disable: 26495 26451 6297 33010 4099)
#include <physx/PxPhysics.h>
#include <physx/PxPhysicsAPI.h>

// �����̴� ��ü ����� �Լ�
physx::PxRigidDynamic* createDynamic(const physx::PxTransform& t, const physx::PxGeometry& geometry, const physx::PxVec3& velocity);

// �ڽ� (���� ��) ����� �Լ�
void createStack(const physx::PxTransform& t, physx::PxU32 size, physx::PxReal halfExtent);
void initPhysics(bool interactive);
void stepPhysics(bool /*interactive*/);
void cleanupPhysics(bool /*interactive*/);
int snippetMain(physx::PxTransform& transform);

#pragma warning(pop)