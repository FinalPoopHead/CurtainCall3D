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
	//레이캐스트 정보의 업데이트. 23.7.31.AJY.

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

	// 맞았는지를 판정.
	physx::PxRaycastBuffer _hit;
	bool hitSomething = _scene->raycast(_rayOrigin, _rayDirection, _rayLength, _hit);
	// 맞았을 때의 동작. 히트 대상의 포인터, 히트 위치, 세부동작.
	if (hitSomething)
	{
		physx::PxRigidActor* hitActor = _hit.block.actor;	// 충돌한 물체의 actor의 포인터

		// 바닥이랑 충돌한 경우에 대한 예외 처리.
		if (hitActor->getType() == physx::PxActorType::eRIGID_STATIC)
		{
			return;
		}

		// 충돌한 물체에 +y방향으로 힘을 준다.
		physx::PxRigidDynamic* hitRigid = static_cast<physx::PxRigidDynamic*>(hitActor);
		//hitRigid->addForce({ 0.0f, 5.0f, 2.0f }, physx::PxForceMode::eIMPULSE);

		// 피격 정보를 userdata에 저장
		DynamicCollider* hitCol = static_cast<DynamicCollider*>(hitRigid->userData);
		hitCol->Collide();

		physx::PxVec3 hitPoint = _hit.block.position;
	}
}
