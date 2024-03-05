#include "StaticBoxCollider.h"

#include "DebugSystem.h"
#include "GameObject.h"
#include "Transform.h"

namespace Rocket
{
	StaticBoxCollider::StaticBoxCollider()
	{

	}

	/// 디버그 정보 출력.
	void StaticBoxCollider::Update()
	{
		Matrix worldTM = GetOffsetTM() * gameObject->transform.GetWorldTM();
		Vector3 WHD = { GetWidth(),GetHeight(),GetDepth() };
		Vector4 color = { 0.0f,0.0f,1.0f,1.0f };
		Rocket::Core::DebugSystem::Instance().DrawDebugBox(worldTM, WHD, true, color);
	}

	void StaticBoxCollider::SetVolume(float w, float h, float d)
	{
		_width = w;
		_height = h;
		_depth = d;
	}

	float StaticBoxCollider::GetWidth() const
	{
		return _width * _scaleOffset.x * gameObject->transform.GetScale().x;
	}

	float StaticBoxCollider::GetHeight() const
	{
		return _height * _scaleOffset.y * gameObject->transform.GetScale().y;
	}

	float StaticBoxCollider::GetDepth() const
	{
		return _depth * _scaleOffset.z * gameObject->transform.GetScale().z;
	}
}

