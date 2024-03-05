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

		// visual debugger ����, ���ÿ� ����
		_pvd = PxCreatePvd(*_foundation);
		physx::PxPvdTransport* transport = physx::PxDefaultPvdSocketTransportCreate("127.0.0.1", 5425, 10);
		_pvd->connect(*transport, physx::PxPvdInstrumentationFlag::eDEBUG);

		// ����, ����, ���� ���� ������ ���� ����
		_physics = PxCreatePhysics(PX_PHYSICS_VERSION, *_foundation, physx::PxTolerancesScale(), true, _pvd);
		_physics = PxCreatePhysics(PX_PHYSICS_VERSION, *_foundation, physx::PxTolerancesScale(), true, nullptr);

		//PxInitExtensions(*_physics, _pvd);

		// ���� ���� ����
		CreatePhysXScene();

		// ��Ƽ���� ����(����)
		_material = _physics->createMaterial(0.25f, 0.2f, 0.4f);

		// collider ���� �����.
		MakeAllCollider();
	}

	void PhysicsSystem::Finalize()
	{
		// initPhysics���� �ʱ�ȭ���� ���� �������� release
		PX_RELEASE(_pxScene);
		PX_RELEASE(_dispatcher);
		PX_RELEASE(_physics);
		// visual debugger�� release
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
		// ���� ���� ����
		physx::PxSceneDesc sceneDesc(_physics->getTolerancesScale());
		// ���Ƿ� �߷��� 2��� �ߴ�. 23.8.16.AJY.		3��� �ø� ����
		sceneDesc.gravity = physx::PxVec3(0.0f, -29.43f, 0.0f);
		_dispatcher = physx::PxDefaultCpuDispatcherCreate(2);
		sceneDesc.cpuDispatcher = _dispatcher;
		sceneDesc.filterShader = physx::PxDefaultSimulationFilterShader;
		_pxScene = _physics->createScene(sceneDesc);

		// Pvd�� ���� ������
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

			// TODO : Scene�� ���� physX Scene �� ���������� ����°� ���� ��.

			for (auto& object : sceneIter.second->GetOriginalList())
			{
				// ������ ��ü�� ���ε��� �׸��� ���� �ƴ�
				// RenderSystem���� ������ ��ü�� ������ �������� Graphics���� �׸���� ��Ű�� ��
				// object->GetRenderData();

				MakePlaneCollider(object);
				MakeBoxCollider(object);
				MakeSphereCollider(object);
				MakeCapsuleCollider(object);
				MakeStaticBoxCollider(object);
			}
		// fixedJoint�� ��� �߰��غ���
		//_fixedJoints.push_back(MakeFixedJoint(player, plBody));
		//_fixedJoints.push_back(MakeFixedJoint(player, plHead));
		MakeFixedJoint();
		AddActorsToScene();
		}
	}

	void PhysicsSystem::MakePlaneCollider(Rocket::GameObject* object)
	{
		Rocket::Collider* temp = object->GetComponent<Rocket::PlaneCollider>();
		
		// PlaneCollider ����
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

		// boxCollider ����
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

		// SphereCollider ����
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

		// CapsuleCollider ����
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

		// StaticBoxCollider ����
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
		//	// StaticBoxCollider ����
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
		// actor1�� actor2�� collider�� �޾ƿ´�.
		//Rocket::BoxCollider* tempBox1 = static_cast<Rocket::BoxCollider*>(actor1->userData);
		Rocket::CapsuleCollider* tempBox1 = static_cast<Rocket::CapsuleCollider*>(actor1->userData);
		Rocket::DynamicCollider* tempBox2 = static_cast<Rocket::DynamicCollider*>(actor2->userData);
		// actor2�� y��ǥ�� (�ڽ��� localPosition + actor1�� ���� ����) ���� �����Ѵ�. ������ ������ Identity�� ����.
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

		// �¾Ҵ����� ����.
		physx::PxRaycastBuffer _hit;
		bool hitSomething = _pxScene->raycast(rayOrigin, rayDirection, length, _hit);

		// �¾��� ���� ����. ��Ʈ ����� ������, ��Ʈ ��ġ, ���ε���.
		if (hitSomething)
		{
			physx::PxRigidActor* hitActor = _hit.block.actor;	// �浹�� ��ü�� actor�� ������

			// �ٴ��̶� �浹�� ��쿡 ���� ���� ó��.
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

			// �ǰ� ������ userdata�� ����
			hitCol = static_cast<Rocket::Collider*>(hitActor->userData);

			//hitCol->Collide();

			// �´� ��ġ�� ������ ��ƼŬ �Ͷ߷��� ��.
			physx::PxVec3 hitPoint = _hit.block.position;
		}

		return hitCol;
	}

	void PhysicsSystem::PhysicsUpdate(float deltaTime)
	{
		// delta time ��� �ùķ��̼�.
		_pxScene->simulate(deltaTime);
		_pxScene->fetchResults(true);

		// ������ GameObject�鿡�� ���� �̺�Ʈ �߻�.
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

		// ���� ���� ����� ������Ʈ�� ����.
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
