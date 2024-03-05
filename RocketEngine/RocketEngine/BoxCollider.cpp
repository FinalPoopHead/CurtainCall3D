#include "BoxCollider.h"

#include "DebugSystem.h"
#include "GameObject.h"
#include "Transform.h"

namespace Rocket
{
	BoxCollider::BoxCollider()
		: _width(1.0f),
		_height(1.0f),
		_depth(1.0f)
	{

	}

	/// 디버그 정보 출력.
	void BoxCollider::Update()
	{
		Matrix worldTM = GetOffsetTM() * gameObject->transform.GetWorldTM();
		Vector3 WHD = { GetWidth(),GetHeight(),GetDepth() };
		Vector4 color = { 0.0f,1.0f,0.0f,1.0f };
		Rocket::Core::DebugSystem::Instance().DrawDebugBox(worldTM, WHD, true, color);
		if (gameObject->objName == "player")
		{
			Vector3 direction = gameObject->transform.GetForward();
			Vector3 beginPoint = {worldTM._41, worldTM._42, worldTM._43};
			Vector3 endPoint = beginPoint + direction * 10.0f;
			color = { 1.0f, 0.0f, 0.0f, 1.0f };
			Rocket::Core::DebugSystem::Instance().DrawDebugLine(beginPoint,endPoint);
		}
	}

	void BoxCollider::SetVolume(float w, float h, float d)
	{
		_width = w;
		_height = h;
		_depth = d;
	}

	float BoxCollider::GetWidth() const
	{
		return _width * _scaleOffset.x * gameObject->transform.GetScale().x;
	}

	float BoxCollider::GetHeight() const
	{
		return _height * _scaleOffset.y * gameObject->transform.GetScale().y;
	}

	float BoxCollider::GetDepth() const
	{
		return _depth *_scaleOffset.z * gameObject->transform.GetScale().z;
	}

}


