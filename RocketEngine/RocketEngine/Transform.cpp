#include "Transform.h"
#include "GameObject.h"
#include "../GraphicsInterface/ITransform.h"
#include "GraphicsSystem.h"

namespace Rocket
{
	Transform::Transform()
		: _transform(Core::GraphicsSystem::Instance().GetFactory()->CreateTransform()),
		_parent(nullptr),
		_children()
	{

	}

	Vector3 Transform::GetPosition() const
	{
		return _transform->GetPosition();
	}

	Quaternion Transform::GetRotation() const
	{
		return _transform->GetRotation();
	}

	Vector3 Transform::GetEuler() const
	{
		return GetRotation().ToEuler();
	}

	Vector3 Transform::GetScale() const
	{
		return _transform->GetScale();
	}

	void Transform::SetPosition(const Vector3& position)
	{
		_transform->SetPosition(position);
	}

	void Transform::SetPosition(float x, float y, float z)
	{
		_transform->SetPosition({x,y,z});
	}

	void Transform::SetRotation(const Quaternion& quaternion)
	{
		_transform->SetRotation(quaternion);
	}

	void Transform::SetRotation(float x, float y, float z, float w)
	{
		_transform->SetRotation({ x,y,z,w });
	}

	void Transform::SetRotationEuler(const Vector3& euler)
	{
		SetRotationEuler(euler.x, euler.y, euler.z);
	}

	void Transform::SetRotationEuler(float angleX, float angleY, float angleZ)
	{
		SetRotation(Quaternion::CreateFromYawPitchRoll(angleY, angleX, angleZ));
	}

	void Transform::SetScale(const Vector3& scale)
	{
		_transform->SetScale(scale);
	}

	void Transform::SetScale(float x, float y, float z)
	{
		_transform->SetScale({ x,y,z });
	}

	Vector3 Transform::GetLocalPosition() const
	{
		return _transform->GetLocalPosition();
	}

	Quaternion Transform::GetLocalRotation() const
	{
		return _transform->GetLocalRotation();
	}

	Vector3 Transform::GetLocalEuler() const
	{
		return _transform->GetLocalEuler();
	}

	Vector3 Transform::GetLocalScale() const
	{
		return _transform->GetLocalScale();
	}

	Vector3* Transform::GetLocalPositionPointer()
{
		return _transform->GetLocalPositionPtr();
	}

	Quaternion* Transform::GetLocalRotationPointer()
{
		return _transform->GetLocalRotationPtr();
	}

	Vector3* Transform::GetLocalScalePointer()
{
		return _transform->GetLocalScalePtr();
	}	

	void Transform::SetLocalPosition(const Vector3& position)
	{
		_transform->SetLocalPosition(position);
	}

	void Transform::SetLocalPosition(float x, float y, float z)
	{
		_transform->SetLocalPosition({ x,y,z });
	}

	void Transform::SetLocalRotation(const Quaternion& quaternion)
	{
		_transform->SetLocalRotation(quaternion);
	}

	void Transform::SetLocalRotation(float x, float y, float z, float w)
	{
		_transform->SetLocalRotation({ x,y,z,w });
	}

	void Transform::SetLocalRotationEuler(const Vector3& euler)
	{
		SetLocalRotationEuler(euler.x, euler.y, euler.z);
	}

	void Transform::SetLocalRotationEuler(float angleX, float angleY, float angleZ)
	{
		_transform->SetLocalRotation(Quaternion::CreateFromYawPitchRoll(angleY, angleX, angleZ));
	}

	void Transform::SetLocalRotationEulerXZConvert(float angleX, float angleY, float angleZ)
	{
		SetLocalRotationEuler(angleZ, angleY + 90.0f, angleX);
	}

	void Transform::SetLocalScale(const Vector3& scale)
	{
		_transform->SetLocalScale(scale);
	}
	
	void Transform::SetLocalScale(float x, float y, float z)
	{
		_transform->SetLocalScale({ x,y,z });
	}

	Vector3 Transform::GetForward() const
	{
		return Vector3::Transform(Vector3::Backward, GetRotation());
	}

	Vector3 Transform::GetUp() const
	{
		return Vector3::Transform(Vector3::Up, GetRotation());
	}

	Vector3 Transform::GetRight() const
	{
		return Vector3::Transform(Vector3::Right, GetRotation());
	}

	Matrix Transform::GetLocalScaleMatrix() const
	{
		return _transform->GetLocalScaleMatrix();
	}

	Matrix Transform::GetLocalRotationMatrix() const
	{
		return _transform->GetLocalRotationMatrix();
	}

	Matrix Transform::GetLocalTranslateMatrix() const
	{
		return _transform->GetLocalTranslationMatrix();
	}

	Matrix Transform::GetWorldScaleMatrix() const
	{
		return _transform->GetScaleMatrix();
	}

	Matrix Transform::GetWorldRotationMatrix() const
	{
		return _transform->GetRotationMatrix();
	}

	Matrix Transform::GetWorldTranslateMatrix() const
	{
		return _transform->GetTranslationMatrix();
	}

	Matrix Transform::GetWorldTM() const
	{
		return _transform->GetWorldTM();
	}

	void Transform::Translate(const Vector3& position)
	{
		_transform->Translate(position);
	}

	void Transform::Translate(float x, float y, float z)
	{
		_transform->Translate({ x,y,z });
	}

	void Transform::Rotate(float angleX, float angleY, float angleZ)
	{
		Quaternion rot = Quaternion::CreateFromYawPitchRoll({ angleX, angleY, angleZ });
		_transform->Rotate(rot);
	}

	void Transform::Rotate(Quaternion quaternion)
	{
		_transform->Rotate(quaternion);
	}

	/// <summary>
	/// target과 up벡터를 주면 그것을 기준으로 카메라가 바라본다.
	/// 
	/// 23.04.20 강석원 인재원
	/// </summary>
	/// <param name="target">바라볼 타겟</param>
	/// <param name="up"> 카메라가 right vector를 구할때 사용할 up 벡터</param>
	void Transform::LookAt(const Vector3& target, const Vector3& up)
	{
		_transform->LookAt(target, up);
	}

	void Transform::SetParent(Transform* parent)
	{	
		if (_parent == nullptr)
		{
			ReleaseParent();
		}

		_transform->SetParent(parent->_transform);
		_parent = parent;
		_parent->AddChild(this);
	}

	void Transform::SetParent(GameObject* parentObj)
	{
		SetParent(&(parentObj->transform));
	}

	Transform* Transform::GetParent()
	{
		return _parent;
	}

	Transform* Transform::GetChild(int index)
	{
		if (_children.size() <= index)
		{
			return nullptr;
		}

		return _children[index];
	}

	void Transform::ReleaseParent()
	{
		if (!_parent)
		{
			return;
		}

		_transform->ReleaseParent();
		_parent->ReleaseChild(this);
		_parent = nullptr;
	}

	void Transform::ReleaseChild(Transform* child)
	{
		std::erase_if(_children, [child](Transform* tr) {return tr == child; });
	}

	std::vector<Transform*>& Transform::GetChildrenVec()
	{
		return _children;
	}

	void Transform::AddChild(Transform* child)
	{
		_children.push_back(child);
	}

}
