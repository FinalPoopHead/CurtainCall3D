#include "./include/BoxColliderComponent.h"
#include "./include/GameEngine.h"
#include "PhysicsEngine.h"
#include "./include/GameObject.h"
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


flt::BoxColliderComponent::BoxColliderComponent(GameObject* gameObject) :
	Component<BoxColliderComponent>(gameObject),
	_transform(&gameObject->transform),
	_physcx(*GameEngine::Instance()->GetPhysicsEngine()->GetPhysics()),
	_scene(*GameEngine::Instance()->GetPhysicsEngine()->GetScene()),
	_physXData(new PhysXData()),
	_size(10.0f, 10.0f, 10.0f),
	_offset(0.0f, 0.0f, 0.0f, 1.0f)
{
	Vector4f position = _transform->GetWorldPosition();
	_physXData->transform.p = physx::PxVec3(position.x, position.y, position.z);
	Quaternion rotation = _transform->GetWorldRotation();
	_physXData->transform.q = physx::PxQuat(rotation.x, rotation.y, rotation.z, rotation.w);

	_physXData->actor = _physcx.createRigidDynamic(_physXData->transform);
	_physXData->actor->setRigidBodyFlag(physx::PxRigidBodyFlag::eKINEMATIC, true);

	//_physXData->actor = _physcx.createRigidStatic(_physXData->transform);
	_physXData->shape = _physcx.createShape(physx::PxBoxGeometry(_size.x, _size.y, _size.z), *_physcx.createMaterial(0.5f, 0.5f, 0.6f));
	_physXData->actor->attachShape(*_physXData->shape);
}

flt::BoxColliderComponent::~BoxColliderComponent()
{
	_physXData->actor->release();
	_physXData->shape->release();

	delete _physXData;
}

void flt::BoxColliderComponent::OnEnable()
{
	_scene.addActor(*_physXData->actor);
}

void flt::BoxColliderComponent::OnDisable()
{
	_scene.removeActor(*_physXData->actor);
}

void flt::BoxColliderComponent::PrePhysics()
{
	UpdatePhysTransform();
}

void flt::BoxColliderComponent::PostPhysics()
{
	UpdateTransform();
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
	_offset = {offset , 1.0f};
}

flt::Vector3f flt::BoxColliderComponent::GetOffset() const
{
	return (flt::Vector3f)_offset;
}

void flt::BoxColliderComponent::UpdatePhysTransform()
{
	Transform temp;
	temp.SetMatrix(_transform->GetWorldMatrix4f());

	Vector3f position = (Vector3f)temp.GetWorldPosition();
	Vector4f worldOffset = _offset * temp.GetWorldMatrix4f();
	position += (Vector3f)worldOffset;
	_physXData->transform.p = physx::PxVec3(position.x , position.y , position.z );
	Quaternion rotation = temp.GetWorldRotation();
	_physXData->transform.q = physx::PxQuat(rotation.x, rotation.y, rotation.z, rotation.w);

	_physXData->actor->setKinematicTarget(_physXData->transform);

	Vector4f scale = temp.GetLocalScale();
	float sizeX = _size.x * scale.x;
	float sizeY = _size.y * scale.y;
	float sizeZ = _size.z * scale.z;

	sizeX = sizeX < 0 ? sizeX = 0 : sizeX;
	sizeY = sizeY < 0 ? sizeY = 0 : sizeY;
	sizeZ = sizeZ < 0 ? sizeZ = 0 : sizeZ;

	physx::PxBoxGeometry geometry(sizeX, sizeY, sizeZ);
	_physXData->actor->detachShape(*_physXData->shape);
	_physXData->shape->setGeometry(geometry);
	_physXData->actor->attachShape(*_physXData->shape);
}

void flt::BoxColliderComponent::UpdateTransform()
{
	//_physXData->transform = _physXData->actor->getGlobalPose();

	//Vector4f position = Vector4f(_physXData->transform.p.x, _physXData->transform.p.y, _physXData->transform.p.z, 1.0f);
	//Vector4f worldOffset = _offset * _transform->GetWorldMatrix4f();
	//position -= worldOffset;
	//position.w = 1.0f;
	//_transform->SetWorldPosition(position);

	//Quaternion rotation = Quaternion(_physXData->transform.q.x, _physXData->transform.q.y, _physXData->transform.q.z, _physXData->transform.q.w);
	//_transform->SetWorldRotation(rotation);
}
