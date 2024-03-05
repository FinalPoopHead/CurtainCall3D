#include "Collider.h"

#include <physx/PxPhysics.h>
#include <physx/PxPhysicsAPI.h>

namespace Rocket
{

	Collider::Collider()
		: _positionOffset(0.0f,0.0f,0.0f),
		_rotationOffset(1.0f,0.0f,0.0f,0.0f),
		_scaleOffset(1.0f,1.0f,1.0f),
		_pxScene(nullptr)
	{

	}

	Vector3 Collider::GetPositionOffset() const
	{
		return _positionOffset;
	}

	Quaternion Collider::GetRotationOffset() const
	{
		return _rotationOffset;
	}

	Vector3 Collider::GetScaleOffset() const
	{
		return _scaleOffset;
	}

	void Collider::SetPositionOffset(Vector3 position)
	{
		_positionOffset = position;
	}

	void Collider::SetRotationOffset(Quaternion rotation)
	{
		_rotationOffset = rotation;
	}

	void Collider::SetRotationOffset(float angleX, float angleY, float angleZ)
	{
		_rotationOffset = Quaternion::CreateFromYawPitchRoll(angleX, angleY, angleZ);
	}

	void Collider::SetScaleOffset(Vector3 scale)
	{
		_scaleOffset = scale;
	}

	Matrix Collider::GetOffsetTranslateMatrix() const
	{
		return Matrix::CreateTranslation(_positionOffset);
	}

	Matrix Collider::GetOffsetScaleMatrix() const
	{
		return Matrix::CreateScale(_scaleOffset);
	}

	Matrix Collider::GetOffsetRotationMatrix() const
	{
		return Matrix::CreateFromQuaternion(_rotationOffset);
	}

	Matrix Collider::GetOffsetTM() const
	{
		Matrix result;
		result *= GetOffsetScaleMatrix();
		result *= GetOffsetRotationMatrix();
		result *= GetOffsetTranslateMatrix();
		
		return result;
	}

	void Collider::SetPxScene(physx::PxScene* pxScene)
	{
		_pxScene = pxScene;
	}

	physx::PxScene* Collider::GetPxScene()
	{
		return _pxScene;
	}

}
