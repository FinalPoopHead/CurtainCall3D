#include "Transform.h"


namespace Rocket::Core
{
	Transform::Transform()
		: _position(Vector3::Zero)
		, _rotation(Quaternion::Identity)
		, _scale(Vector3::One)
		, _parent(nullptr)
		, _children()
	{

	}

	void Transform::SetPosition(const Vector3& position)
	{
		Vector3 result = position;

		if (_parent)
		{
			result = Vector3::Transform(result, _parent->GetWorldTM().Invert());
		}

		_position = result;
	}

	void Transform::SetRotation(const Quaternion& rotation)
	{
		Quaternion result = rotation;

		if (_parent)
		{
			Quaternion parentRot = _parent->GetRotation();
			parentRot.Conjugate();

			result = Quaternion::Concatenate(result, parentRot);
		}

		_rotation = result;
	}

	void Transform::SetRotationEuler(const Vector3& euler)
	{
		SetRotation(Quaternion::CreateFromYawPitchRoll(euler));
	}

	void Transform::SetScale(const Vector3& scale)
	{
		Vector3 result = scale;

		if (_parent)
		{
			Vector3 parentScale = _parent->GetScale();
			result.x /= parentScale.x;
			result.y /= parentScale.y;
			result.z /= parentScale.z;
		}

		_scale = result;
	}

	Vector3 Transform::GetPosition() const
	{
		Vector3 result = { _position.x, _position.y, _position.z };

		if (_parent)
		{
			result = Vector3::Transform(result, _parent->GetWorldTM());
		}

		return result;
	}

	Quaternion Transform::GetRotation() const
	{
		Quaternion result = _rotation;

		if (_parent)
		{
			result = Quaternion::Concatenate(result, _parent->GetRotation());
		}

		return result;
	}

	Vector3 Transform::GetEuler() const
	{
		return GetRotation().ToEuler();
	}

	Vector3 Transform::GetScale() const
	{
		Vector3 result = _scale;

		if (_parent)
		{
			Vector3 parentScale = _parent->GetScale();
			result.x *= parentScale.x;
			result.y *= parentScale.y;
			result.z *= parentScale.z;
		}

		return result;
	}

	void Transform::SetLocalPosition(const Vector3& position)
	{
		_position = position;
	}

	void Transform::SetLocalRotation(const Quaternion& quaternion)
	{
		_rotation = quaternion;
		_rotation.Normalize();
	}

	void Transform::SetLocalRotationEuler(const Vector3& euler)
	{
		_rotation = Quaternion::CreateFromYawPitchRoll({ euler.x, euler.y , euler.z });
	}

	void Transform::SetLocalScale(const Vector3& scale)
	{
		_scale = scale;
	}

	Vector3 Transform::GetLocalPosition() const
	{
		return _position;
	}

	Quaternion Transform::GetLocalRotation() const
	{
		return _rotation;
	}

	Vector3 Transform::GetLocalEuler() const
	{

		Vector3 radian = _rotation.ToEuler();

		float degreeX = DirectX::XMConvertToDegrees(radian.x);
		float degreeY = DirectX::XMConvertToDegrees(radian.y);
		float degreeZ = DirectX::XMConvertToDegrees(radian.z);

		return { degreeX,degreeY,degreeZ };
	}

	Vector3 Transform::GetLocalScale() const
	{
		return _scale;
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

	Matrix Transform::GetScaleMatrix() const
	{
		Matrix result = GetLocalScaleMatrix();

		if (_parent)
		{
			result *= _parent->GetScaleMatrix();
		}

		return result;
	}

	Matrix Transform::GetRotationMatrix() const
	{
		Matrix result = GetLocalRotationMatrix();

		if (_parent)
		{
			result *= _parent->GetRotationMatrix();
		}

		return result;
	}

	Matrix Transform::GetTranslationMatrix() const
	{
		Matrix result = GetLocalTranslationMatrix();

		if (_parent)
		{
			result *= _parent->GetTranslationMatrix();
		}

		return result;
	}

	Matrix Transform::GetWorldTM() const
	{
		// 이거 한번 써봐야 할듯?
		// return Matrix::CreateWorld(_position, GetForward(), GetUp());

		Matrix result = GetLocalTM();

		if (_parent)
		{
			result *= _parent->GetWorldTM();
		}

		return result;
	}

	Matrix Transform::GetLocalScaleMatrix() const
	{
		return Matrix::CreateScale(_scale);
	}

	Matrix Transform::GetLocalRotationMatrix() const
	{
		return Matrix::CreateFromQuaternion(_rotation);
	}

	Matrix Transform::GetLocalTranslationMatrix() const
	{
		return Matrix::CreateTranslation(_position);
	}

	Matrix Transform::GetLocalTM() const
	{
		Matrix result;
		result *= GetLocalScaleMatrix();
		result *= GetLocalRotationMatrix();
		result *= GetLocalTranslationMatrix();

		return result;
	}

	void Transform::Translate(const Vector3& translation)
	{
		_position += translation;
	}

	void Transform::Rotate(const Quaternion& quaternion)
	{
		_rotation = Quaternion::Concatenate(_rotation, quaternion);
	}

	void Transform::LookAt(const Vector3& target, const Vector3& up /*= Vector3::Up*/)
	{
		Vector3 forward = target - GetPosition();
		forward.Normalize();
		Vector3 right = up.Cross(forward);
		right.Normalize();
		Vector3 upVec = forward.Cross(right);
		upVec.Normalize();

		_rotation = Quaternion::CreateFromRotationMatrix(Matrix::CreateWorld(GetPosition(), forward, upVec));
	}

	void Transform::LookAt(const ITransform* target, const Vector3& up /*= Vector3::Up*/)
	{
		Vector3 targetPosition = target->GetPosition();

		Vector3 forward = targetPosition - GetPosition();
		forward.Normalize();
		Vector3 right = up.Cross(forward);
		right.Normalize();
		Vector3 upVec = forward.Cross(right);
		upVec.Normalize();

		_rotation = Quaternion::CreateFromRotationMatrix(Matrix::CreateWorld(GetPosition(), forward, upVec));
	}

	void Transform::RotateAround(const Vector3& point, const Vector3& axis, float angle)
	{
		Vector3 position = GetPosition();
		Vector3 direction = position - point;
		Matrix rotation = Matrix::CreateFromAxisAngle(axis, angle);
		direction = Vector3::Transform(direction, rotation);
		SetPosition(point + direction);
	}

	void Transform::Scale(const Vector3& scale)
	{
		_scale *= scale;
	}

	void Transform::SetParent(ITransform* parent)
	{
		if (parent == nullptr)
		{
			ReleaseParent();
			return;
		}

		_position = Vector3::Transform(_position, parent->GetWorldTM().Invert());
		Quaternion parentRot = parent->GetRotation();
		parentRot.Conjugate();
		_rotation = Quaternion::Concatenate(_rotation, parentRot);
		_scale = Vector3::Transform(_scale, parent->GetScaleMatrix().Invert());

		_parent = parent;
		dynamic_cast<Transform*>(parent)->AddChild(this);
	}

	ITransform* Transform::GetParent() const
	{
		return _parent;
	}

	ITransform* Transform::GetChild(int index) const
	{
		if (_children.size() <= index)
		{
			return nullptr;
		}

		return _children[index];
	}

	int Transform::GetChildCount() const
	{
		return _children.size();
	}

	void Transform::AddChild(ITransform* child)
	{
		_children.emplace_back(child);
	}

	void Transform::ReleaseChild(ITransform* child)
	{
		auto iter = std::find(_children.begin(), _children.end(), child);
		if (iter != _children.end())
		{
			_children.erase(iter);
		}
	}

	void Transform::ReleaseParent()
	{
		if (!_parent)
		{
			return;
		}

		SetLocalPosition(Vector3::Transform(_position, _parent->GetWorldTM()));
		SetLocalRotation(Quaternion::Concatenate(_rotation, _parent->GetRotation()));
		SetLocalScale(Vector3::Transform(_scale, _parent->GetScaleMatrix()));

		dynamic_cast<Transform*>(_parent)->ReleaseChild(this);
		_parent = nullptr;
	}

	std::vector<ITransform*>& Transform::GetChildren()
	{
		return _children;
	}

	Vector3* Transform::GetLocalPositionPtr()
	{
		return &_position;
	}

	Quaternion* Transform::GetLocalRotationPtr()
	{
		return &_rotation;
	}

	Vector3* Transform::GetLocalScalePtr()
	{
		return &_scale;
	}

}
