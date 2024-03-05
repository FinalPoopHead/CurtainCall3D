#include "Raycast.h"
#include "SceneSystem.h"
#include "Scene.h"
#include "GameObject.h"
#include "Transform.h"
#include "Camera.h"
#include "PhysicsSystem.h"
#include "DynamicCollider.h"

RocketEngine::Raycast::Raycast(GameObject* owner)
	: Component(owner),
	_rayOrigin(), _rayDirection(), _rayLength(100.0f)
{
	_scene = RocketCore::PhysicsSystem::Instance().GetPxScene();
}

void RocketEngine::Raycast::UpdateRayInfo()
{
	//����ĳ��Ʈ ������ ������Ʈ. 23.7.31.AJY.

	RMFLOAT3 position = gameObject->transform.GetLocalPosition();
	_rayOrigin.x = position.x;
	_rayOrigin.y = position.y;
	_rayOrigin.z = position.z;

	RMFLOAT3 direction = gameObject->transform.GetForward();
	_rayDirection.x = direction.x;
	_rayDirection.y = direction.y;
	_rayDirection.z = direction.z;
}

void RocketEngine::Raycast::PerformRayCast()
{
	if (!_scene)
	{
		_scene = RocketCore::PhysicsSystem::Instance().GetPxScene();
	}

	// �¾Ҵ����� ����.
	physx::PxRaycastBuffer _hit;
	bool hitSomething = _scene->raycast(_rayOrigin, _rayDirection, _rayLength, _hit);
	// �¾��� ���� ����. ��Ʈ ����� ������, ��Ʈ ��ġ, ���ε���.
	if (hitSomething)
	{
		physx::PxRigidActor* hitActor = _hit.block.actor;	// �浹�� ��ü�� actor�� ������

		// �ٴ��̶� �浹�� ��쿡 ���� ���� ó��.
		if (hitActor->getType() == physx::PxActorType::eRIGID_STATIC)
		{
			return;
		}

		// �浹�� ��ü�� +y�������� ���� �ش�.
		physx::PxRigidDynamic* hitRigid = static_cast<physx::PxRigidDynamic*>(hitActor);
		//hitRigid->addForce({ 0.0f, 5.0f, 2.0f }, physx::PxForceMode::eIMPULSE);

		// �ǰ� ������ userdata�� ����
		DynamicCollider* hitCol = static_cast<DynamicCollider*>(hitRigid->userData);
		hitCol->Collide();

		physx::PxVec3 hitPoint = _hit.block.position;
	}
}
