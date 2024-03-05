#include "SphereCollider.h"

#include "DebugSystem.h"
#include "GameObject.h"
#include "Transform.h"

namespace Rocket
{
	SphereCollider::SphereCollider()
	{

	}

	void SphereCollider::Update()
	{
		Matrix worldTM = GetOffsetTM() * gameObject->transform.GetWorldTM();
		Vector3 WHD = { GetWidth(), GetHeight(), GetDepth() };
		Vector4 color = { 0.0f,1.0f,0.0f,1.0f };
		Rocket::Core::DebugSystem::Instance().DrawDebugBox(worldTM, WHD, true, color);
	}

	float SphereCollider::GetWidth() const
	{
		return _radius * 2 * _scaleOffset.x * gameObject->transform.GetScale().x;
	}

	float SphereCollider::GetHeight() const
	{
		return _radius * 2 * _scaleOffset.y * gameObject->transform.GetScale().y;
	}

	float SphereCollider::GetDepth() const
	{
		return _radius * 2 * _scaleOffset.z * gameObject->transform.GetScale().z;
	}

	float SphereCollider::GetRadius() const
	{
		return _radius;
	}

	void SphereCollider::SetRadius(float radius)
	{
		_radius = radius;
	}

}


