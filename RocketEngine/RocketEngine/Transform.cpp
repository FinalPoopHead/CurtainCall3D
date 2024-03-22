#include "Transform.h"
#include "GameObject.h"
#include "../RocketCommon/RocketTransform.h"
#include "GraphicsSystem.h"

namespace Rocket
{
	Transform::Transform()
		: _rocketTransform(new Core::RocketTransform()),
		_parent(nullptr),
		_children()
	{

	}

	Vector3 Transform::GetPosition() const
	{
		return _rocketTransform->GetPosition();
	}

	Quaternion Transform::GetRotation() const
	{
		return _rocketTransform->GetRotation();
	}

	Vector3 Transform::GetEuler() const
	{
		return GetRotation().ToEuler();
	}

	Vector3 Transform::GetScale() const
	{
		return _rocketTransform->GetScale();
	}

	void Transform::SetPosition(const Vector3& position)
	{
		_rocketTransform->SetPosition(position);
	}

	void Transform::SetPosition(float x, float y, float z)
	{
		_rocketTransform->SetPosition({x,y,z});
	}

	void Transform::SetRotation(const Quaternion& quaternion)
	{
		_rocketTransform->SetRotation(quaternion);
	}

	void Transform::SetRotation(float x, float y, float z, float w)
	{
		_rocketTransform->SetRotation({ x,y,z,w });
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
		_rocketTransform->SetScale(scale);
	}

	void Transform::SetScale(float x, float y, float z)
	{
		_rocketTransform->SetScale({ x,y,z });
	}

	Vector3 Transform::GetLocalPosition() const
	{
		return _rocketTransform->GetLocalPosition();
	}

	Quaternion Transform::GetLocalRotation() const
	{
		return _rocketTransform->GetLocalRotation();
	}

	Vector3 Transform::GetLocalEuler() const
	{
		return _rocketTransform->GetLocalEuler();
	}

	Vector3 Transform::GetLocalScale() const
	{
		return _rocketTransform->GetLocalScale();
	}

	Vector3* Transform::GetLocalPositionPointer()
{
		return _rocketTransform->GetLocalPositionPtr();
	}

	Quaternion* Transform::GetLocalRotationPointer()
{
		return _rocketTransform->GetLocalRotationPtr();
	}

	Vector3* Transform::GetLocalScalePointer()
{
		return _rocketTransform->GetLocalScalePtr();
	}	

	void Transform::SetLocalPosition(const Vector3& position)
	{
		_rocketTransform->SetLocalPosition(position);
	}

	void Transform::SetLocalPosition(float x, float y, float z)
	{
		_rocketTransform->SetLocalPosition({ x,y,z });
	}

	void Transform::SetLocalRotation(const Quaternion& quaternion)
	{
		_rocketTransform->SetLocalRotation(quaternion);
	}

	void Transform::SetLocalRotation(float x, float y, float z, float w)
	{
		_rocketTransform->SetLocalRotation({ x,y,z,w });
	}

	void Transform::SetLocalRotationEuler(const Vector3& euler)
	{
		SetLocalRotationEuler(euler.x, euler.y, euler.z);
	}

	void Transform::SetLocalRotationEuler(float angleX, float angleY, float angleZ)
	{
		_rocketTransform->SetLocalRotation(Quaternion::CreateFromYawPitchRoll(angleY, angleX, angleZ));
	}

	void Transform::SetLocalRotationEulerXZConvert(float angleX, float angleY, float angleZ)
	{
		SetLocalRotationEuler(angleZ, angleY + 90.0f, angleX);
	}

	void Transform::SetLocalScale(const Vector3& scale)
	{
		_rocketTransform->SetLocalScale(scale);
	}
	
	void Transform::SetLocalScale(float x, float y, float z)
	{
		_rocketTransform->SetLocalScale({ x,y,z });
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
		return _rocketTransform->GetLocalScaleMatrix();
	}

	Matrix Transform::GetLocalRotationMatrix() const
	{
		return _rocketTransform->GetLocalRotationMatrix();
	}

	Matrix Transform::GetLocalTranslateMatrix() const
	{
		return _rocketTransform->GetLocalTranslationMatrix();
	}

	Matrix Transform::GetWorldScaleMatrix() const
	{
		return _rocketTransform->GetScaleMatrix();
	}

	Matrix Transform::GetWorldRotationMatrix() const
	{
		return _rocketTransform->GetRotationMatrix();
	}

	Matrix Transform::GetWorldTranslateMatrix() const
	{
		return _rocketTransform->GetTranslationMatrix();
	}

	Matrix Transform::GetWorldTM() const
	{
		return _rocketTransform->GetWorldTM();
	}

	void Transform::Translate(const Vector3& position)
	{
		_rocketTransform->Translate(position);
	}

	void Transform::Translate(float x, float y, float z)
	{
		_rocketTransform->Translate({ x,y,z });
	}

	void Transform::Rotate(float angleX, float angleY, float angleZ)
	{
		Quaternion rot = Quaternion::CreateFromYawPitchRoll({ angleX, angleY, angleZ });
		_rocketTransform->Rotate(rot);
	}

	void Transform::Rotate(Quaternion quaternion)
	{
		_rocketTransform->Rotate(quaternion);
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
		_rocketTransform->LookAt(target, up);
	}

	void Transform::SetParent(Transform* parent)
	{	
		if (_parent == nullptr)
		{
			ReleaseParent();
		}

		_rocketTransform->SetParent(parent->_rocketTransform);
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

		_rocketTransform->ReleaseParent();
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
