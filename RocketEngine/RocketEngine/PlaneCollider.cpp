#include "PlaneCollider.h"

#include "DebugSystem.h"
#include "GameObject.h"
#include "Transform.h"

namespace Rocket
{
	PlaneCollider::PlaneCollider()
	{

	}

	/// 디버그 정보 출력.
	void PlaneCollider::Update()
	{
		Matrix worldTM = gameObject->transform.GetWorldTM();
		Vector3 WHD = { 0.001f,100.0f,100.0f};
		Vector4 color = { 0.0f,0.0f,1.0f,1.0f };
		Rocket::Core::DebugSystem::Instance().DrawDebugBox(worldTM, WHD, true, color);
	}

	Vector3 PlaneCollider::GetNormalVector() const
	{
		Vector3 axis = gameObject->transform.GetRight();
		axis.Normalize();
		return axis;
	}

	float PlaneCollider::GetDistance() const
	{
		return gameObject->transform.GetLocalPosition().Length();
	}
}

