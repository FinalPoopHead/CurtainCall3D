#include "PhysicsSystem.h"
#include "SceneSystem.h"
#include "Scene.h"
#include "GameObject.h"
#include "Transform.h"
#include "Collider.h"
#include "DynamicCollider.h"
#include "StaticCollider.h"
#include "BoxCollider.h"
#include "SphereCollider.h"
#include "CapsuleCollider.h"
#include "PlaneCollider.h"
#include "StaticBoxCollider.h"
#include "MathHeader.h"
#include "Camera.h"

namespace Rocket::Core
{
	void PhysicsSystem::Initialize()
	{
		_foundation = PxCreateFoundation(PX_PHYSICS_VERSION, _allocator, _errorCallback);

		// visual debugger 세팅, 로컬에 연결
		_pvd = PxCreatePvd(*_foundation);
		physx::PxPvdTransport* transport = physx::PxDefaultPvdSocketTransportCreate("127.0.0.1", 5425, 10);
		_pvd->connect(*transport, physx::PxPvdInstrumentationFlag::eDEBUG);

		// 버전, 세팅, 단위 등의 정보를 담은 물리
		_physics = PxCreatePhysics(PX_PHYSICS_VERSION, *_foundation, physx::PxTolerancesScale(), true, _pvd);
		_physics = PxCreatePhysics(PX_PHYSICS_VERSION, *_foundation, physx::PxTolerancesScale(), true, nullptr);

		//PxInitExtensions(*_physics, _pvd);

		// 물리 씬을 생성
		CreatePhysXScene();

		// 머티리얼 생성(임의)
		_material = _physics->createMaterial(0.25f, 0.2f, 0.4f);

		// collider 전부 만들기.
		MakeAllCollider();
	}

	void PhysicsSystem::Finalize()
	{
		// initPhysics에서 초기화해준 전역 변수들을 release
		PX_RELEASE(_pxScene);
		PX_RELEASE(_dispatcher);
		PX_RELEASE(_physics);
		// visual debugger도 release
		if (_pvd)
		{
			physx::PxPvdTransport* transport = _pvd->getTransport();
			_pvd->release();	_pvd = NULL;
			PX_RELEASE(transport);
		}
		PX_RELEASE(_foundation);
	}

	void PhysicsSystem::CreatePhysXScene()
	{
		// 씬에 대한 설정
		physx::PxSceneDesc sceneDesc(_physics->getTolerancesScale());
		// 임의로 중력을 2배로 했다. 23.8.16.AJY.		3배로 늘림 ㅎㅎ
		sceneDesc.gravity = physx::PxVec3(0.0f, -29.43f, 0.0f);
		_dispatcher = physx::PxDefaultCpuDispatcherCreate(2);
		sceneDesc.cpuDispatcher = _dispatcher;
		sceneDesc.filterShader = physx::PxDefaultSimulationFilterShader;
		_pxScene = _physics->createScene(sceneDesc);

		// Pvd에 정보 보내기
		physx::PxPvdSceneClient* pvdClient = _pxScene->getScenePvdClient();
		if (pvdClient)
		{
			pvdClient->setScenePvdFlag(physx::PxPvdSceneFlag::eTRANSMIT_CONSTRAINTS, true);
			pvdClient->setScenePvdFlag(physx::PxPvdSceneFlag::eTRANSMIT_CONTACTS, true);
			pvdClient->setScenePvdFlag(physx::PxPvdSceneFlag::eTRANSMIT_SCENEQUERIES, true);
		}
	}


	void PhysicsSystem::MakeAllCollider()
	{
		for (auto& sceneIter : SceneSystem::Instance().GetAllScenes())
		{
			if (sceneIter.second == nullptr)
			{
				return;
			}

			// TODO : Scene에 따라 physX Scene 도 개별적으로 만드는게 좋을 듯.

			for (auto& object : sceneIter.second->GetOriginalList())
			{
				// 각각의 객체가 본인들을 그리는 것이 아닌
				// RenderSystem에서 각각의 객체의 정보를 바탕으로 Graphics에게 그리라고 시키는 것
				// object->GetRenderData();

				MakePlaneCollider(object);
				MakeBoxCollider(object);
				MakeSphereCollider(object);
				MakeCapsuleCollider(object);
				MakeStaticBoxCollider(object);
			}
		// fixedJoint를 잠깐 추가해보자
		//_fixedJoints.push_back(MakeFixedJoint(player, plBody));
		//_fixedJoints.push_back(MakeFixedJoint(player, plHead));
		MakeFixedJoint();
		AddActorsToScene();
		}
	}

	void PhysicsSystem::MakePlaneCollider(Rocket::GameObject* object)
	{
		Rocket::Collider* temp = object->GetComponent<Rocket::PlaneCollider>();
		
		// PlaneCollider 생성
		if (temp)
		{
			auto colvec = object->GetComponents<Rocket::PlaneCollider>();

			for (auto& collider : colvec)
			{
				Rocket::PlaneCollider* planeCol = dynamic_cast<Rocket::PlaneCollider*>(collider);
				Vector3 normal = planeCol->GetNormalVector();
				physx::PxPlane pxPlane = { normal.x,normal.y,normal.z, planeCol->GetDistance() };

				physx::PxRigidStatic* planeRigid = PxCreatePlane(*_physics, pxPlane, *_material);
				//_scene->addActor(*planeRigid);

				planeCol->SetPhysXRigid(planeRigid);
				planeRigid->userData = planeCol;
				_rigidStatics.push_back(planeRigid);
			}
		}
	}

	void PhysicsSystem::MakeBoxCollider(Rocket::GameObject* object)
	{
		Rocket::Collider* temp = object->GetComponent<Rocket::BoxCollider>();

		// boxCollider 생성
		if (temp)
		{
			auto colvec = object->GetComponents<Rocket::BoxCollider>();

			for (auto& collider : colvec)
			{
				Rocket::BoxCollider* box = dynamic_cast<Rocket::BoxCollider*>(collider);

				physx::PxShape* shape = _physics->createShape(physx::PxBoxGeometry(box->GetWidth() / 2, box->GetHeight() / 2, box->GetDepth() / 2), *_material);

				Vector3 pos = object->transform.GetPosition();
				physx::PxTransform localTm(physx::PxVec3(pos.x, pos.y, pos.z));
				physx::PxRigidDynamic* body = _physics->createRigidDynamic(localTm);
				body->attachShape(*shape);
				body->setAngularDamping(0.5f);
				body->setLinearDamping(0.5f);

				if (box->gameObject->objName == "player")
				{
					body->setRigidDynamicLockFlag(physx::PxRigidDynamicLockFlag::eLOCK_ANGULAR_X, true);
					body->setRigidDynamicLockFlag(physx::PxRigidDynamicLockFlag::eLOCK_ANGULAR_Y, true);
					body->setRigidDynamicLockFlag(physx::PxRigidDynamicLockFlag::eLOCK_ANGULAR_Z, true);
				}
				//body->setMass(4.5f);
				//physx::PxRigidBodyExt::updateMassAndInertia(*body, 1.0f);
				//_scene->addActor(*body);

				box->SetPhysXRigid(body);
				body->userData = box;
				_rigidDynamics.push_back(body);

				shape->release();
			}
		}
	}

	void PhysicsSystem::MakeSphereCollider(Rocket::GameObject* object)
	{
		Rocket::Collider* temp = object->GetComponent<Rocket::SphereCollider>();

		// SphereCollider 생성
		if (temp)
		{
			auto colvec = object->GetComponents<Rocket::SphereCollider>();

			for (auto& collider : colvec)
			{
				Rocket::SphereCollider* sphere = dynamic_cast<Rocket::SphereCollider*>(collider);

				physx::PxShape* shape = _physics->createShape(physx::PxSphereGeometry(sphere->GetRadius()), *_material);

				Vector3 pos = object->transform.GetPosition();
				physx::PxTransform localTm(physx::PxVec3(pos.x, pos.y, pos.z));
				physx::PxRigidDynamic* body = _physics->createRigidDynamic(localTm);
				body->attachShape(*shape);
				body->setAngularDamping(0.5f);
				body->setLinearDamping(10.0f);
				//physx::PxRigidBodyExt::updateMassAndInertia(*body, 1000.0f);
				//_scene->addActor(*body);

				if (sphere->gameObject->objName == "playerHead")
				{
					body->setRigidDynamicLockFlag(physx::PxRigidDynamicLockFlag::eLOCK_ANGULAR_X, true);
					body->setRigidDynamicLockFlag(physx::PxRigidDynamicLockFlag::eLOCK_ANGULAR_Y, true);
					body->setRigidDynamicLockFlag(physx::PxRigidDynamicLockFlag::eLOCK_ANGULAR_Z, true);
				}

				sphere->SetPhysXRigid(body);
				body->userData = sphere;
				_rigidDynamics.push_back(body);

				shape->release();
			}
		}
	}

	void PhysicsSystem::MakeCapsuleCollider(Rocket::GameObject* object)
	{
		Rocket::Collider* temp = object->GetComponent<Rocket::CapsuleCollider>();

		// CapsuleCollider 생성
		if (temp)
		{
			auto colvec = object->GetComponents<Rocket::CapsuleCollider>();

			for (auto& collider : colvec)
			{
				Rocket::CapsuleCollider* capsuleCol = dynamic_cast<Rocket::CapsuleCollider*>(collider);

				physx::PxShape* shape = _physics->createShape(physx::PxCapsuleGeometry(capsuleCol->GetRadius(), capsuleCol->GetHalfHeight()), *_material);

				Quaternion quat = object->transform.GetRotation();
				physx::PxTransform shapeTransform(physx::PxIdentity);
				shapeTransform.q = physx::PxQuat(quat.x, quat.y, quat.z, quat.w);
				shape->setLocalPose(shapeTransform);

				Vector3 pos = object->transform.GetPosition();
				physx::PxTransform localTm(physx::PxVec3(pos.x, pos.y, pos.z));
				physx::PxRigidDynamic* body = _physics->createRigidDynamic(localTm);
				body->setAngularDamping(0.5f);
				body->setLinearDamping(0.5f);
				body->attachShape(*shape);

				//_scene->addActor(*body);

				capsuleCol->SetPhysXRigid(body);
				body->userData = capsuleCol;
				_rigidDynamics.push_back(body);

				shape->release();
			}
		}
	}

	void PhysicsSystem::MakeStaticBoxCollider(Rocket::GameObject* object)
	{
		Rocket::Collider* temp = object->GetComponent<Rocket::StaticBoxCollider>();

		// StaticBoxCollider 생성
		if (temp)
		{
			auto colvec = object->GetComponents<Rocket::StaticBoxCollider>();

			for (auto& collider : colvec)
			{
				Rocket::StaticBoxCollider* staticBoxCol = dynamic_cast<Rocket::StaticBoxCollider*>(collider);

				physx::PxShape* shape = _physics->createShape(physx::PxBoxGeometry(staticBoxCol->GetWidth() / 2, staticBoxCol->GetHeight() / 2, staticBoxCol->GetDepth() / 2), *_material);

				Vector3 pos = object->transform.GetPosition();
				physx::PxTransform localTm(physx::PxVec3(pos.x, pos.y, pos.z));
				physx::PxRigidStatic* body = _physics->createRigidStatic(localTm);
				body->attachShape(*shape);

				//_scene->addActor(*body);

				staticBoxCol->SetPhysXRigid(body);
				body->userData = staticBoxCol;
				_rigidStatics.push_back(body);

				shape->release();

			}
		}

		//std::vector<Rocket::StaticBoxCollider*> colliders = object->GetComponents<Rocket::StaticBoxCollider>();
		//int i = 0;
		//if (colliders[0] != nullptr)
		//{
		//	// StaticBoxCollider 생성
		//	for (const auto& col : colliders)
		//	{
		//		Rocket::StaticBoxCollider* staticBoxCol = dynamic_cast<Rocket::StaticBoxCollider*>(colliders[i++]);

		//		physx::PxShape* shape = _physics->createShape(physx::PxBoxGeometry(staticBoxCol->GetWidth() / 2, staticBoxCol->GetHeight() / 2, staticBoxCol->GetDepth() / 2), *_material);

		//		Rocket::Vector3 pos = object->transform.GetLocalPosition();
		//		physx::PxTransform localTm(physx::PxVec3(pos.x, pos.y, pos.z));
		//		physx::PxRigidStatic* body = _physics->createRigidStatic(localTm);
		//		body->attachShape(*shape);

		//		//_scene->addActor(*body);

		//		staticBoxCol->SetPhysXRigid(body);
		//		body->userData = staticBoxCol;
		//		_rigidStatics.push_back(body);

		//		shape->release();
		//	}
		//}
	}

	physx::PxFixedJoint* PhysicsSystem::MakeFixedJoint(physx::PxRigidDynamic* actor1, physx::PxRigidDynamic* actor2)
	{
		// actor1과 actor2의 collider를 받아온다.
		//Rocket::BoxCollider* tempBox1 = static_cast<Rocket::BoxCollider*>(actor1->userData);
		Rocket::CapsuleCollider* tempBox1 = static_cast<Rocket::CapsuleCollider*>(actor1->userData);
		Rocket::DynamicCollider* tempBox2 = static_cast<Rocket::DynamicCollider*>(actor2->userData);
		// actor2의 y좌표를 (자신의 localPosition + actor1의 높이 절반) 으로 고정한다. 나머지 정보는 Identity로 고정.
		physx::PxTransform tempTransform(physx::PxIdentity);
		tempTransform.p.y = (*tempBox2).gameObject->transform.GetLocalPosition().y + (*tempBox1).GetHalfHeight()
			+ (*tempBox1).GetRadius();
		//tempTransform.q.z = (*tempBox2).gameObject->transform.GetLocalRotation().z;
		return physx::PxFixedJointCreate(*_physics, actor1, tempTransform,
			actor2, physx::PxTransform(physx::PxIdentity));
	}

	void PhysicsSystem::MakeFixedJoint()
	{
		for (auto& dynamics : _rigidDynamics)
		{
			Rocket::DynamicCollider* col = static_cast<Rocket::DynamicCollider*>(dynamics->userData);
			for (auto& servant : col->GetServants())
			{
				std::string test2 = col->gameObject->objName;
				std::string serv = servant->gameObject->objName;

				Vector3 masterPos = col->gameObject->transform.GetPosition();
				physx::PxTransform masterTR(physx::PxIdentity);
				masterTR.p.x = masterPos.x;
				masterTR.p.y = masterPos.y;
				masterTR.p.z = masterPos.z;

				Vector3 servantPos = servant->gameObject->transform.GetPosition();
				physx::PxTransform servantTR(physx::PxIdentity);
				servantTR.p.x = servantPos.x;
				servantTR.p.y = servantPos.y;
				servantTR.p.z = servantPos.z;

				physx::PxFixedJoint* result = physx::PxFixedJointCreate(*_physics, dynamics, servantTR,
					servant->GetRigidBody(), masterTR);

				_fixedJoints.push_back(result);
			}
		}
	}

	void PhysicsSystem::AddActorsToScene()
	{
		for (auto& dynamics : _rigidDynamics)
		{
			static_cast<Rocket::DynamicCollider*>(dynamics->userData)->UpdateToPhysics();
			_pxScene->addActor(*dynamics);
		}
		for (auto& statics : _rigidStatics)
		{
			static_cast<Rocket::StaticCollider*>(statics->userData)->UpdateToPhysics();
			_pxScene->addActor(*statics);
		}
	}

	Rocket::Collider* PhysicsSystem::RayCast(Vector3 original, Vector3 direction, float length, int* type /*= nullptr*/)
	{
		physx::PxVec3 rayOrigin;
		rayOrigin.x = original.x;
		rayOrigin.y = original.y;
		rayOrigin.z = original.z;

		physx::PxVec3 rayDirection;
		rayDirection.x = direction.x;
		rayDirection.y = direction.y;
		rayDirection.z = direction.z;

		Rocket::Collider* hitCol = nullptr;

		// 맞았는지를 판정.
		physx::PxRaycastBuffer _hit;
		bool hitSomething = _pxScene->raycast(rayOrigin, rayDirection, length, _hit);

		// 맞았을 때의 동작. 히트 대상의 포인터, 히트 위치, 세부동작.
		if (hitSomething)
		{
			physx::PxRigidActor* hitActor = _hit.block.actor;	// 충돌한 물체의 actor의 포인터

			// 바닥이랑 충돌한 경우에 대한 예외 처리.
			if (hitActor->getType() == physx::PxActorType::eRIGID_STATIC)
			{
				hitActor = static_cast<physx::PxRigidStatic*>(hitActor);
				if (type != nullptr)
				{
					*type = 1;
				}
			}
			else if (hitActor->getType() == physx::PxActorType::eRIGID_DYNAMIC)
			{
				hitActor = static_cast<physx::PxRigidDynamic*>(hitActor);
				if (type != nullptr)
				{
					*type = 2;
				}
			}

			//physx::PxRigidDynamic* hitRigid = static_cast<physx::PxRigidDynamic*>(hitActor);

			// 피격 정보를 userdata에 저장
			hitCol = static_cast<Rocket::Collider*>(hitActor->userData);

			//hitCol->Collide();

			// 맞는 위치도 보내서 파티클 터뜨려야 함.
			physx::PxVec3 hitPoint = _hit.block.position;
		}

		return hitCol;
	}

	void PhysicsSystem::PhysicsUpdate(float deltaTime)
	{
		// delta time 기반 시뮬레이션.
		_pxScene->simulate(deltaTime);
		_pxScene->fetchResults(true);

		// 각각의 GameObject들에게 물리 이벤트 발생.
		for (auto& rigid : _rigidDynamics)
		{
			Rocket::DynamicCollider* col = static_cast<Rocket::DynamicCollider*>(rigid->userData);
			Rocket::GameObject* gameObject = col->gameObject;

			if (!col->GetWasCollided() && col->GetIsCollided())
			{
				gameObject->OnCollisionEnter();
			}
			else if (col->GetWasCollided() && col->GetIsCollided())
			{
				gameObject->OnCollisionStay();
			}
			else if (col->GetWasCollided() && !col->GetIsCollided())
			{
				gameObject->OnCollisionExit();
			}
		}

		// 물리 연산 결과를 오브젝트에 적용.
		Vector3 pos;
		Quaternion quat;

		physx::PxTransform temp;
		for (auto& rigid : _rigidDynamics)
		{
			temp = rigid->getGlobalPose();

			pos.x = temp.p.x;
			pos.y = temp.p.y;
			pos.z = temp.p.z;

			quat.x = temp.q.x;
			quat.y = temp.q.y;
			quat.z = temp.q.z;
			quat.w = temp.q.w;

			static_cast<Rocket::DynamicCollider*>(rigid->userData)->UpdateFromPhysics(pos, quat);
		}
	}

	void PhysicsSystem::UpdateToPhysics()
	{
		for (auto& rigid : _rigidDynamics)
		{
			Rocket::DynamicCollider* col = static_cast<Rocket::DynamicCollider*>(rigid->userData);
			col->UpdateToPhysics();
		}

		for (auto& rigid : _rigidStatics)
		{
			Rocket::StaticCollider* col = static_cast<Rocket::StaticCollider*>(rigid->userData);
			col->UpdateToPhysics();
		}
	}

	void PhysicsSystem::Flush()
	{
		for (auto& rigid : _rigidDynamics)
		{
			static_cast<Rocket::DynamicCollider*>(rigid->userData)->Flush();
		}
	}

	physx::PxScene* PhysicsSystem::GetPxScene()
	{
		return _pxScene;
	}

	std::vector<physx::PxRigidDynamic*>& PhysicsSystem::GetRigidDynamicsList()
	{
		return _rigidDynamics;
	}

	std::vector<physx::PxRigidStatic*>& PhysicsSystem::GetRigidStaticsList()
	{
		return _rigidStatics;
	}

}
