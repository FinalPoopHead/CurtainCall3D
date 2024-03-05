#include "CapsuleCollider.h"

#include "DebugSystem.h"
#include "GameObject.h"
#include "Transform.h"

namespace Rocket
{
	CapsuleCollider::CapsuleCollider()
		: _radius(1.0f),
		_halfHeight(1.0f)
	{

	}

	void CapsuleCollider::Update()
	{
		Matrix worldTM = GetOffsetTM() * gameObject->transform.GetWorldTM();
		// Collider와 Mesh 일치를 위한 조정 test. 23.8.9.AJY.
		//worldTM._42 += _height / 2;
		Vector3 WHD = { GetHeight(), GetWidth(), GetDepth()};
		Vector4 color = { 0.0f,1.0f,0.0f,1.0f };
		Rocket::Core::DebugSystem::Instance().DrawDebugBox(worldTM, WHD, true, color);
	}

	float CapsuleCollider::GetWidth() const
	{
		return (_radius + _halfHeight) * 2 * _scaleOffset.y * gameObject->transform.GetScale().y;
	}

	float CapsuleCollider::GetHeight() const
	{
		return _radius * 2 * _scaleOffset.x * gameObject->transform.GetScale().x;
	}

	float CapsuleCollider::GetDepth() const
	{
		return _radius * 2 * _scaleOffset.z * gameObject->transform.GetScale().z;
	}

	float CapsuleCollider::GetRadius() const
	{
		return _radius;
	}

	float CapsuleCollider::GetHalfHeight() const
	{
		return _halfHeight;
	}

	void CapsuleCollider::SetCapsuleInfo(float radius, float halfHeight)
	{
		_radius = radius;
		_halfHeight = halfHeight;
	}

	void CapsuleCollider::SetRadius(float rad)
	{
		_radius = rad;
	}

	void CapsuleCollider::SetHalfHeight(float val)
	{
		_halfHeight = val;
	}

}
