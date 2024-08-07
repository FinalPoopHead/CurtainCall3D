﻿#include "./include/Transform.h"

#include <DirectXMath.h>
#include <cmath>


flt::Transform::~Transform()
{
	//for (auto& child : _children)
	//{
	//	child->SetParent(nullptr);
	//}
}

flt::Transform::Transform(const Transform& other) : 
	_position(other._position),
	_scale(other._scale),
	_rotation(other._rotation),
	_pParent(nullptr),
	_children(),
	_isDirty(true),
	_pOwner(nullptr)
{
}

flt::Transform& flt::Transform::operator=(const Transform& other)
{
	if (this == &other)
	{
		return *this;
	}

	_position = other._position;
	_scale = other._scale;
	_rotation = other._rotation;
	//_pParent = nullptr;
	//_children.clear();
	_isDirty = true;

	return *this;
}

void flt::Transform::SetMatrix(const Matrix4f& worldMatrix)
{
	MakeDirtyRecursive();

	DirectX::XMMATRIX matrix
	{
		worldMatrix.e[0][0], worldMatrix.e[0][1], worldMatrix.e[0][2], worldMatrix.e[0][3],
		worldMatrix.e[1][0], worldMatrix.e[1][1], worldMatrix.e[1][2], worldMatrix.e[1][3],
		worldMatrix.e[2][0], worldMatrix.e[2][1], worldMatrix.e[2][2], worldMatrix.e[2][3],
		worldMatrix.e[3][0], worldMatrix.e[3][1], worldMatrix.e[3][2], worldMatrix.e[3][3],
	};

	DirectX::XMMatrixDecompose(&_scale.m, &_rotation.m, &_position.m, matrix);
}

flt::Vector4f flt::Transform::GetWorldPosition() const noexcept
{
	Vector4f worldPos = _position;
	if (_pParent)
	{
		worldPos = worldPos * _pParent->GetWorldMatrix4f();
	}

	return worldPos;
}

void flt::Transform::SetPosition(float x, float y, float z)
{
	MakeDirtyRecursive();

	_position.x = x;
	_position.y = y;
	_position.z = z;
}

void flt::Transform::SetWorldPosition(const Vector4f& position)
{
	MakeDirtyRecursive();

	if (_pParent)
	{
		_position = position * _pParent->GetWorldMatrix4f().Inverse();
	}
	else
	{
		_position = position;
	}
}

void flt::Transform::SetWorldPosition(float x, float y, float z)
{
	SetWorldPosition({ x, y, z, 1.0f });
}

void flt::Transform::SetPosition(const Vector3f& position)
{
	SetPosition(position.x, position.y, position.z);
}

void flt::Transform::SetPosition(double x, double y, double z)
{
	SetPosition(static_cast<float>(x), static_cast<float>(y), static_cast<float>(z));
}

void flt::Transform::SetPosition(const Vector4f& position)
{
	MakeDirtyRecursive();

	_position = position;
}

flt::Quaternion flt::Transform::GetWorldRotation() const noexcept
{
	Quaternion worldRot = _rotation;
	if (_pParent)
	{
		worldRot = _pParent->GetWorldRotation() * worldRot;
	}

	worldRot.Normalize();
	return worldRot;
}

void flt::Transform::SetRotation(float degreeX, float degreeY, float degreeZ, Quaternion::AxisOrder order)
{
	MakeDirtyRecursive();

	_rotation.SetEuler(degreeX, degreeY, degreeZ, order);
}

void flt::Transform::SetWorldRotation(const Quaternion& q)
{
	MakeDirtyRecursive();

	if (_pParent)
	{
		// 회전 Quaternion은 단위 Quaternion이기 때문에 Inverse == Conjugate
		_rotation = q * _pParent->GetWorldRotation().Conjugate();
	}
	else
	{
		_rotation = q;
	}

	_rotation.Normalize();
}

void flt::Transform::SetRotation(Vector3f degree, Quaternion::AxisOrder order /*= Quaternion::AxisOrder::YXZ*/)
{
	SetRotation(degree.x, degree.y, degree.z, order);
}

void flt::Transform::SetRotation(float x, float y, float z, float w)
{
	SetRotation(Quaternion{ x, y, z, w });
}

void flt::Transform::SetRotation(double degreeX, double degreeY, double degreeZ, Quaternion::AxisOrder order)
{
	SetRotation(static_cast<float>(degreeX), static_cast<float>(degreeY), static_cast<float>(degreeZ), order);
}

void flt::Transform::SetRotation(const Vector3f& axis, float radian)
{
	SetRotation(Quaternion{ axis, radian });
}

void flt::Transform::SetRotation(const Quaternion& q)
{
	MakeDirtyRecursive();

	_rotation = q;
	_rotation.Normalize();
}


void flt::Transform::SetScale(float x, float y, float z)
{
	MakeDirtyRecursive();

	_scale.x = x;
	_scale.y = y;
	_scale.z = z;
}

void flt::Transform::SetScale(const Vector3f& scale)
{
	SetScale(scale.x, scale.y, scale.z);
}

void flt::Transform::SetScale(double x, double y, double z)
{
	SetScale(static_cast<float>(x), static_cast<float>(y), static_cast<float>(z));
}

void flt::Transform::SetScale(const Vector4f& scale)
{
	MakeDirtyRecursive();

	_scale = scale;
}

void flt::Transform::AddLocalPosition(float localX, float localY, float localZ)
{
	MakeDirtyRecursive();

	_position.x += localX;
	_position.y += localY;
	_position.z += localZ;
}

void flt::Transform::AddLocalPosition(const Vector3f& localPos)
{
	AddLocalPosition(localPos.x, localPos.y, localPos.z);
}

void flt::Transform::AddLocalPosition(const Vector4f& localPos)
{
	MakeDirtyRecursive();

	_position += localPos;
	_position.w = 1.0f;
}

void flt::Transform::AddWorldPosition(const Vector4f& worldPos)
{
	Matrix4f worldMatrix = Matrix4f::Identity();
	if (_pParent)
	{
		worldMatrix = _pParent->GetWorldMatrix4f();
	}
	Vector4f localPos = worldPos * worldMatrix.Inverse();
	AddLocalPosition(localPos);
}

void flt::Transform::AddLocalRotation(const Vector3f& localAxis, float radian)
{
	Vector4f worldAxis = Vector4f{localAxis, 0.0f} * GetWorldMatrix4f();

	worldAxis.Normalize();

	AddWorldRotation((Vector3f)worldAxis, radian);
}

void flt::Transform::AddWorldPosition(float worldX, float worldY, float worldZ)
{
	AddWorldPosition(Vector4f{ worldX, worldY, worldZ, 1.0f });
}

void flt::Transform::AddRotation(const Vector3f& worldAxis, float radian)
{
	//Quaternion q0{ DirectX::XMQuaternionRotationAxis(Vector4f{ axis, 0.0f }, radian) };
	Quaternion q(worldAxis, radian);
	//q.SetEuler(degreeX, degreeY, degree);
	AddRotation(q);
}

void flt::Transform::AddWorldRotation(const Vector3f& axis, float radian)
{
	Matrix4f worldMatrix = Matrix4f::Identity();
	if (_pParent)
	{
		worldMatrix = _pParent->GetWorldMatrix4f();
	}
	Vector4f worldAxis = { axis, 0.0f };
	Vector4f localAxis = worldAxis * worldMatrix.Inverse();

	AddRotation({localAxis.x, localAxis.y, localAxis.z}, radian);
}

void flt::Transform::AddRotation(const Quaternion& q)
{
	MakeDirtyRecursive();

	_rotation = q * _rotation;
	_rotation.Normalize();
}

void flt::Transform::AddScale(float x, float y, float z)
{
	MakeDirtyRecursive();

	_scale.x += x;
	_scale.y += y;
	_scale.z += z;
}

flt::Matrix4f flt::Transform::GetTranslateMatrix4f() const noexcept
{
	return Matrix4f
	{
		1.0f,			0.0f,			0.0f,			0.0f,
		0.0f,			1.0f,			0.0f,			0.0f,
		0.0f,			0.0f,			1.0f,			0.0f,
		_position.x,	_position.y,	_position.z,	1.0f
	};
}

flt::Matrix4f flt::Transform::GetRotationMatrix4f() const noexcept
{
	float x2 = _rotation.x * _rotation.x;
	float y2 = _rotation.y * _rotation.y;
	float z2 = _rotation.z * _rotation.z;
	float xy = _rotation.x * _rotation.y;
	float xz = _rotation.x * _rotation.z;
	float yz = _rotation.y * _rotation.z;
	float wx = _rotation.w * _rotation.x;
	float wy = _rotation.w * _rotation.y;
	float wz = _rotation.w * _rotation.z;

	return Matrix4f
	{
		1.f - 2.f * (y2 + z2),	2.f * (xy + wz),		2.f * (xz - wy),		0.f,
		2.f * (xy - wz),		1.f - 2.f * (x2 + z2),	2.f * (yz + wx),		0.f,
		2.f * (xz + wy),		2.f * (yz - wx),		1.f - 2.f * (x2 + y2),	0.f,
		0.f,					0.f,					0.f,					1.f
	};
}

flt::Matrix4f flt::Transform::GetScaleMatrix4f() const noexcept
{
	return Matrix4f
	{
		_scale.x,	0.0f,		0.0f,		0.0f,
		0.0f,		_scale.y,	0.0f,		0.0f,
		0.0f,		0.0f,		_scale.z,	0.0f,
		0.0f,		0.0f,		0.0f,		1.0f
	};
}

flt::Matrix4f flt::Transform::GetLocalMatrix4f() const noexcept
{
	float x2 = _rotation.x * _rotation.x;
	float y2 = _rotation.y * _rotation.y;
	float z2 = _rotation.z * _rotation.z;
	float xy = _rotation.x * _rotation.y;
	float xz = _rotation.x * _rotation.z;
	float yz = _rotation.y * _rotation.z;
	float wx = _rotation.w * _rotation.x;
	float wy = _rotation.w * _rotation.y;
	float wz = _rotation.w * _rotation.z;

	return Matrix4f
	{
		_scale.x * (1.f - 2.f * (y2 + z2)),	_scale.x * (2.f * (xy + wz)),		_scale.x * (2.f * (xz - wy)),		0.f,
		_scale.y * (2.f * (xy - wz)),		_scale.y * (1.f - 2.f * (x2 + z2)),	_scale.y * (2.f * (yz + wx)),		0.f,
		_scale.z * (2.f * (xz + wy)),		_scale.z * (2.f * (yz - wx)),		_scale.z * (1.f - 2.f * (x2 + y2)),	0.f,
		_position.x,						_position.y,						_position.z,						1.f
	};

	//return GetScaleMatrix4f() * GetRotationMatrix4f() * GetTransformMatrix4f();
}

flt::Matrix4f flt::Transform::GetWorldMatrix4f() const noexcept
{
	if (_isDirty)
	{
		CalcWorldMatrixRecursive();

		_isDirty = false;
	}

	return _worldMatrix;
}

void flt::Transform::LookAt(Vector4f target)
{
	MakeDirtyRecursive();

	// target을 바라보도록 회전
	Vector4f wantLook = target - _position;
	wantLook.Vector3Normalize();
	if (wantLook.NormPow() == 0.0f)
	{
		return;
	}

	Vector4f currLook = Vector4f(0.f, 0.f, 1.f, 1.f) * GetLocalMatrix4f() - _position;
	currLook.Vector3Normalize();
	Vector4f axis = currLook.Vector3Cross(wantLook);

	axis.Vector3Normalize();

	if (axis.NormPow() == 0.0f)
	{
		axis = Vector4f(0.f, 1.f, 0.f, 0.f);
	}

	float dot = wantLook.Vector3Dot(currLook);

	float angle = 0.0f;

	// 아크 코사인의 정의역이 -1 ~ 1이므로 그 이상일 경우 가장 가까운 각으로 보정해 준다.
	// 단, 1이아닌 0.99999같은 값으로 설정했을 경우에 연속으로 LookAt을 호출할 경우 오차가 누적되어 떨림 증상이 발생한다.
	if (dot > 1.f)
	{
		angle = 0.0f;
	}
	else if (dot < -1.f)
	{
		axis = Vector4f(0.f, 1.f, 0.f, 0.f);
		angle = PI<float>;
	}
	else
	{
		angle = acos(dot);
	}

	Quaternion q{ axis, angle };

	_rotation = q * _rotation;
	_rotation.Normalize();
}

void flt::Transform::LookAt(float x, float y, float z)
{
	LookAt({ x, y, z, 1.0f });
}

flt::Vector4f flt::Transform::Forward() const noexcept
{
	return (Vector4f(0.f, 0.f, 1.f, 0.f) * GetWorldMatrix4f()).Normalize();
}

flt::Vector4f flt::Transform::Right() const noexcept
{
	return (Vector4f(1.f, 0.f, 0.f, 0.f) * GetWorldMatrix4f()).Normalize();
}

flt::Vector4f flt::Transform::Up() const noexcept
{
	return (Vector4f(0.f, 1.f, 0.f, 0.f) * GetWorldMatrix4f()).Normalize();
}

bool flt::Transform::SetParent(Transform* pParent)
{
	// 순환 고리가 생길 수 없도록 체크
	Transform* checkRecursive = pParent;
	while (checkRecursive)
	{
		if (checkRecursive == this)
		{
			return false;
		}
		checkRecursive = checkRecursive->_pParent;
	}

	// 이미 어떤 자식일 경우에는 
	if (_pParent)
	{
		auto iter = std::find(_pParent->_children.begin(), _pParent->_children.end(), this);
		if (iter != _pParent->_children.end())
		{
			_pParent->_children.erase(iter);
		}
	}

	if (pParent)
	{
		pParent->_children.push_back(this);
	}

	_pParent = pParent;

	MakeDirtyRecursive();

	return true;
}

flt::Transform* flt::Transform::GetChild(size_t index) const noexcept
{
	if (index >= _children.size())
	{
		return nullptr;
	}

	return _children[index];
}

bool flt::Transform::AddChild(Transform* pChild)
{
	return pChild->SetParent(this);
}

void flt::Transform::MakeDirtyRecursive() const noexcept
{
	if (_isDirty)
	{
		return;
	}

	_isDirty = true;

	for (auto& child : _children)
	{
		child->MakeDirtyRecursive();
	}
}

void flt::Transform::CalcWorldMatrixRecursive() const noexcept
{
	if (_pParent)
	{
		Matrix4f localMatrix = GetLocalMatrix4f();
		Matrix4f parentWorldMatrix = _pParent->GetWorldMatrix4f();
		//const Matrix4f& parentWorldMatrix = _pParent->GetWorldMatrix4fRef();
		Matrix4fMul(localMatrix, _worldMatrix, parentWorldMatrix.v[0].m, parentWorldMatrix.v[1].m, parentWorldMatrix.v[2].m, parentWorldMatrix.v[3].m);
		//Matrix4fMuluseDot(localMatrix, parentWorldMatrix, _worldMatrix);

		//_worldMatrix = GetLocalMatrix4f() * _pParent->GetWorldMatrix4f();
	}
	else
	{
		_worldMatrix = GetLocalMatrix4f();
	}
}

const flt::Matrix4f& flt::Transform::GetWorldMatrix4fRef() const noexcept
{
	if (_isDirty)
	{
		CalcWorldMatrixRecursive();

		_isDirty = false;
	}

	return _worldMatrix;
}

const flt::Vector4f flt::Transform::WorldForward()
{
	return Vector4f(0.0f, 0.0f, 1.0f, 0.0f);
}

const flt::Vector4f flt::Transform::WorldRight()
{
	return Vector4f(1.0f, 0.0f, 0.0f, 0.0f);
}

const flt::Vector4f flt::Transform::WorldUp()
{
	return Vector4f(0.0f, 1.0f, 0.0f, 0.0f);
}
