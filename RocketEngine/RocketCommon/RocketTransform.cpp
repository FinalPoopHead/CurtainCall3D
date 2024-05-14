#include "RocketTransform.h"

namespace Rocket::Core
{
	RocketTransform::RocketTransform()
		: _position(Vector3::Zero)
		, _rotation(Quaternion::Identity)
		, _scale(Vector3::One)
		, _parent(nullptr)
		, _children()
		, _isDirty(false)
		, _worldTM(Matrix::Identity)
	{

	}

	void RocketTransform::SetPosition(const Vector3& position)
	{
		Vector3 result = position;

		if (_parent)
		{
			result = Vector3::Transform(result, _parent->GetWorldTM().Invert());
		}

		_position = result;
		SetDirtyRecur(this);
	}

	void RocketTransform::SetRotation(const Quaternion& rotation)
	{
		Quaternion result = rotation;

		if (_parent)
		{
			Quaternion parentRot = _parent->GetRotation();
			parentRot.Conjugate();

			result = Quaternion::Concatenate(result, parentRot);
		}

		_rotation = result;
		SetDirtyRecur(this);
	}

	void RocketTransform::SetRotationEuler(const Vector3& euler)
	{
		SetRotation(Quaternion::CreateFromYawPitchRoll(euler));
	}

	void RocketTransform::SetScale(const Vector3& scale)
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
		SetDirtyRecur(this);
	}

	Vector3 RocketTransform::GetPosition() const
	{
		Vector3 result = { _position.x, _position.y, _position.z };

		if (_parent)
		{
			result = Vector3::Transform(result, _parent->GetWorldTM());
		}

		return result;
	}

	Quaternion RocketTransform::GetRotation() const
	{
		Quaternion result = _rotation;

		if (_parent)
		{
			result = Quaternion::Concatenate(result, _parent->GetRotation());
		}

		return result;
	}

	Vector3 RocketTransform::GetEuler() const
	{
		return GetRotation().ToEuler();
	}

	Vector3 RocketTransform::GetScale() const
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

	void RocketTransform::SetLocalPosition(const Vector3& position)
	{
		_position = position;
		SetDirtyRecur(this);
	}

	void RocketTransform::SetLocalRotation(const Quaternion& quaternion)
	{
		_rotation = quaternion;
		_rotation.Normalize();
		SetDirtyRecur(this);
	}

	void RocketTransform::SetLocalRotationEuler(const Vector3& euler)
	{
		_rotation = Quaternion::CreateFromYawPitchRoll({ euler.x, euler.y , euler.z });
		SetDirtyRecur(this);
	}

	void RocketTransform::SetLocalScale(const Vector3& scale)
	{
		_scale = scale;
		SetDirtyRecur(this);
	}

	Vector3 RocketTransform::GetLocalPosition() const
	{
		return _position;
	}

	Quaternion RocketTransform::GetLocalRotation() const
	{
		return _rotation;
	}

	Vector3 RocketTransform::GetLocalEuler() const
	{

		Vector3 radian = _rotation.ToEuler();

		float degreeX = DirectX::XMConvertToDegrees(radian.x);
		float degreeY = DirectX::XMConvertToDegrees(radian.y);
		float degreeZ = DirectX::XMConvertToDegrees(radian.z);

		return { degreeX,degreeY,degreeZ };
	}

	Vector3 RocketTransform::GetLocalScale() const
	{
		return _scale;
	}

	Vector3 RocketTransform::GetForward() const
	{
		return Vector3::Transform(Vector3::Backward, GetRotation());
	}

	Vector3 RocketTransform::GetUp() const
	{
		return Vector3::Transform(Vector3::Up, GetRotation());
	}

	Vector3 RocketTransform::GetRight() const
	{
		return Vector3::Transform(Vector3::Right, GetRotation());
	}

	Matrix RocketTransform::GetScaleMatrix() const
	{
		Matrix result = GetLocalScaleMatrix();

		if (_parent)
		{
			result *= _parent->GetScaleMatrix();
		}

		return result;
	}

	Matrix RocketTransform::GetRotationMatrix() const
	{
		Matrix result = GetLocalRotationMatrix();

		if (_parent)
		{
			result *= _parent->GetRotationMatrix();
		}

		return result;
	}

	Matrix RocketTransform::GetTranslationMatrix() const
	{
		Matrix result = GetLocalTranslationMatrix();

		if (_parent)
		{
			result *= _parent->GetTranslationMatrix();
		}

		return result;
	}

	Matrix RocketTransform::GetWorldTM()
	{
		// 이거 한번 써봐야 할듯?
		// return Matrix::CreateWorld(_position, GetForward(), GetUp());

		if(!_isDirty)
		{
			return _worldTM;
		}

		Matrix result = GetLocalTM();

		if (_parent)
		{
			result *= _parent->GetWorldTM();
		}

		_worldTM = result;
		_isDirty = false;

		return result;
	}

	Matrix RocketTransform::GetLocalScaleMatrix() const
	{
		return Matrix::CreateScale(_scale);
	}

	Matrix RocketTransform::GetLocalRotationMatrix() const
	{
		return Matrix::CreateFromQuaternion(_rotation);
	}

	Matrix RocketTransform::GetLocalTranslationMatrix() const
	{
		return Matrix::CreateTranslation(_position);
	}

	Matrix RocketTransform::GetLocalTM() const
	{
		// TODO : 나중에 최적화할때는 다 이걸로 바꾸면 되겠다
		// return DirectX::XMMatrixAffineTransformation(_scale, Vector3::Zero, _rotation, _position);

		Matrix result;
		result *= GetLocalScaleMatrix();
		result *= GetLocalRotationMatrix();
		result *= GetLocalTranslationMatrix();

		return result;
	}

	void RocketTransform::Translate(const Vector3& translation)
	{
		_position += translation;
		SetDirtyRecur(this);
	}

	void RocketTransform::Rotate(const Quaternion& quaternion)
	{
		_rotation = Quaternion::Concatenate(_rotation, quaternion);
		SetDirtyRecur(this);
	}

	void RocketTransform::LookAt(const Vector3& target, const Vector3& up /*= Vector3::Up*/)
	{
		Vector3 forward = target - GetPosition();
		forward.Normalize();
		Vector3 right = up.Cross(forward);
		right.Normalize();
		Vector3 upVec = forward.Cross(right);
		upVec.Normalize();

		_rotation = Quaternion::CreateFromRotationMatrix(Matrix::CreateWorld(GetPosition(), forward, upVec));
		SetDirtyRecur(this);
	}

	void RocketTransform::LookAt(const RocketTransform* target, const Vector3& up /*= Vector3::Up*/)
	{
		Vector3 targetPosition = target->GetPosition();

		Vector3 forward = targetPosition - GetPosition();
		forward.Normalize();
		Vector3 right = up.Cross(forward);
		right.Normalize();
		Vector3 upVec = forward.Cross(right);
		upVec.Normalize();

		_rotation = Quaternion::CreateFromRotationMatrix(Matrix::CreateWorld(GetPosition(), forward, upVec));
		SetDirtyRecur(this);
	}

	void RocketTransform::RotateAround(const Vector3& point, const Vector3& axis, float angle)
	{
		Vector3 position = GetPosition();
		Vector3 direction = position - point;
		Matrix rotation = Matrix::CreateFromAxisAngle(axis, angle);
		direction = Vector3::Transform(direction, rotation);
		SetPosition(point + direction);
		SetDirtyRecur(this);
	}

	void RocketTransform::Scale(const Vector3& scale)
	{
		_scale *= scale;
		SetDirtyRecur(this);
	}

	void RocketTransform::SetParent(RocketTransform* parent, bool keepWorldPosition)
	{
		if (parent == nullptr)
		{
			ReleaseParent();
			return;
		}

		if (keepWorldPosition)
		{
			_position = Vector3::Transform(_position, parent->GetWorldTM().Invert());
			Quaternion parentRot = parent->GetRotation();
			parentRot.Conjugate();
			_rotation = Quaternion::Concatenate(_rotation, parentRot);
			_scale = Vector3::Transform(_scale, parent->GetScaleMatrix().Invert());
		}

		_parent = parent;
		parent->AddChild(this);
		SetDirtyRecur(this);
	}

	RocketTransform* RocketTransform::GetParent() const
	{
		return _parent;
	}

	RocketTransform* RocketTransform::GetChild(int index) const
	{
		if (_children.size() <= index)
		{
			return nullptr;
		}

		return _children[index];
	}

	int RocketTransform::GetChildCount() const
	{
		return (int)_children.size();
	}

	void RocketTransform::AddChild(RocketTransform* child)
	{
		_children.emplace_back(child);
	}

	void RocketTransform::ReleaseChild(RocketTransform* child)
	{
		auto iter = std::find(_children.begin(), _children.end(), child);
		if (iter != _children.end())
		{
			_children.erase(iter);
		}
		SetDirtyRecur(this);
	}

	void RocketTransform::ReleaseParent()
	{
		if (!_parent)
		{
			return;
		}

		SetLocalPosition(Vector3::Transform(_position, _parent->GetWorldTM()));
		SetLocalRotation(Quaternion::Concatenate(_rotation, _parent->GetRotation()));
		SetLocalScale(Vector3::Transform(_scale, _parent->GetScaleMatrix()));

		_parent->ReleaseChild(this);
		_parent = nullptr;
		SetDirtyRecur(this);
	}

	std::vector<RocketTransform*>& RocketTransform::GetChildren()
	{
		return _children;
	}

	Vector3* RocketTransform::GetLocalPositionPtr()
	{
		return &_position;
	}

	Quaternion* RocketTransform::GetLocalRotationPtr()
	{
		return &_rotation;
	}

	Vector3* RocketTransform::GetLocalScalePtr()
	{
		return &_scale;
	}

	void RocketTransform::SetDirtyRecur(RocketTransform* transform)
	{
		_isDirty = true;

		for (auto& child : _children)
		{
			child->SetDirtyRecur(child);
		}
	}
}
