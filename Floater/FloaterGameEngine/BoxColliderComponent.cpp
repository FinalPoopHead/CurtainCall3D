#include "./include/internal/BoxColliderComponent.h"
#include "./include/internal/GameEngine.h"
#include "PhysicsEngine.h"
#include "./include/internal/GameObject.h"
#include "../FloaterRendererCommon/include/Transform.h"
#pragma warning(push)
#pragma warning(disable:26495 6297 33010)
#include "../External/include/physx/include/PxPhysicsAPI.h"
#pragma warning(pop)


namespace flt
{
	struct PhysXData
	{
		physx::PxTransform transform;
		physx::PxRigidDynamic* actor;
		physx::PxShape* shape;
	};
}

flt::BoxColliderComponent::BoxColliderComponent() :
	_transform(nullptr),
	_physx(*GameEngine::Instance()->GetPhysicsEngine()->GetPhysics()),
	_physxScene(*GameEngine::Instance()->GetPhysicsEngine()->GetScene()),
	_physXData(new PhysXData()),
	_size(10.0f, 10.0f, 10.0f),
	_offset(0.0f, 0.0f, 0.0f, 0.0f),
	_isKinematic(false)
{
	_physXData->transform.p.x = 0.0f;
	_physXData->transform.p.y = 0.0f;
	_physXData->transform.p.z = 0.0f;

	_physXData->transform.q.x = 0.0f;
	_physXData->transform.q.y = 0.0f;
	_physXData->transform.q.z = 0.0f;
	_physXData->transform.q.w = 1.0f;

	_physXData->actor = _physx.createRigidDynamic(_physXData->transform);
	_physXData->actor->setRigidBodyFlag(physx::PxRigidBodyFlag::eKINEMATIC, _isKinematic);
	_physXData->actor->setSleepThreshold(5e-4f);
	_physXData->actor->setWakeCounter(0.0f);

	_physXData->actor->userData = static_cast<Collider*>(this);

	_physXData->shape = _physx.createShape(physx::PxBoxGeometry(_size.x, _size.y, _size.z), *_physx.createMaterial(0.5f, 0.5f, 0.6f));
	_physXData->actor->attachShape(*_physXData->shape);
}

flt::BoxColliderComponent::~BoxColliderComponent()
{
	_physXData->actor->release();
	_physXData->shape->release();

	delete _physXData;
}

void flt::BoxColliderComponent::OnCreate()
{
	_transform = &Component::_gameObject->tr;
	Collider::_gameObject = Component::_gameObject;

	auto contactOffset = _physXData->shape->getContactOffset();


	physx::PxDominanceGroup group = 0;

	_physXData->actor->setDominanceGroup(group);
}

void flt::BoxColliderComponent::OnEnable()
{
	_physxScene.addActor(*_physXData->actor);
}

void flt::BoxColliderComponent::OnDisable()
{
	_physxScene.removeActor(*_physXData->actor);
}

void flt::BoxColliderComponent::PrePhysics()
{
	UpdatePhysTransform();
}

void flt::BoxColliderComponent::PostPhysics()
{
	UpdateTransform();
}

void flt::BoxColliderComponent::UseKinematic(bool use)
{
	_isKinematic = use;
	_physXData->actor->setRigidBodyFlag(physx::PxRigidBodyFlag::eKINEMATIC, _isKinematic);
}

void flt::BoxColliderComponent::SetSize(const flt::Vector3f& size)
{
	_size = size;
}

flt::Vector3f flt::BoxColliderComponent::GetSize() const
{
	return _size;
}

void flt::BoxColliderComponent::SetOffset(const flt::Vector3f& offset)
{
	_offset = { offset , 0.0f };
}

flt::Vector3f flt::BoxColliderComponent::GetOffset() const
{
	return (flt::Vector3f)_offset;
}

void flt::BoxColliderComponent::UpdatePhysTransform()
{
	bool isKinematic = _physXData->actor->getRigidBodyFlags().isSet(physx::PxRigidBodyFlag::eKINEMATIC);

	if (_physXData->actor->isSleeping())
	{
		int i = 0;
	}

	Transform temp;
	temp.SetMatrix(_transform->GetWorldMatrix4f());
	Vector4f position = temp.GetWorldPosition();
	Quaternion rotation = temp.GetWorldRotation();

	Vector4f physxPos;
	physxPos.x = _physXData->transform.p.x;
	physxPos.y = _physXData->transform.p.y;
	physxPos.z = _physXData->transform.p.z;
	physxPos.w = 1.0f;

	Quaternion physxRot;
	physxRot.x = _physXData->transform.q.x;
	physxRot.y = _physXData->transform.q.y;
	physxRot.z = _physXData->transform.q.z;
	physxRot.w = _physXData->transform.q.w;

	float eps = FLOAT_EPSILON * 10000.0f;
	if (position.isSimiller(physxPos, eps) && rotation.isSimiller(physxRot, eps))
	{
		return;
	}


	//Vector4f worldOffset = _offset * temp.GetWorldMatrix4f();
	//position += (Vector3f)worldOffset;
	_physXData->transform.p = physx::PxVec3(position.x, position.y, position.z);
	_physXData->transform.q = physx::PxQuat(rotation.x, rotation.y, rotation.z, rotation.w);

	if (_isKinematic)
	{
		_physXData->actor->setKinematicTarget(_physXData->transform);
	}
	else
	{
		_physXData->actor->setGlobalPose(_physXData->transform);
	}

	//static bool testBool = true;
	//if (testBool)
	//{
	//	_physXData->actor->setGlobalPose(_physXData->transform);
	//	testBool = false;
	//}


	Vector4f scale = temp.GetLocalScale();
	float sizeX = std::fabsf(_size.x * scale.x);
	float sizeY = std::fabsf(_size.y * scale.y);
	float sizeZ = std::fabsf(_size.z * scale.z);

	physx::PxBoxGeometry geometry(sizeX, sizeY, sizeZ);
	_physXData->actor->detachShape(*_physXData->shape);
	_physXData->shape->setGeometry(geometry);
	_physXData->actor->attachShape(*_physXData->shape);
}

void flt::BoxColliderComponent::UpdateTransform()
{
	auto tr = _physXData->actor->getGlobalPose();
	_physXData->transform;// = _physXData->actor->getGlobalPose();

	Vector4f position = Vector4f(tr.p.x, tr.p.y, tr.p.z, 1.0f);
	//Vector4f worldOffset = _offset * _transform->GetWorldMatrix4f();
	//position -= worldOffset;
	_transform->SetWorldPosition(position);
	//_transform->AddLocalPosition(-_offset);

	Quaternion rotation = Quaternion(tr.q.x, tr.q.y, tr.q.z, tr.q.w);
	_transform->SetWorldRotation(rotation);
}
