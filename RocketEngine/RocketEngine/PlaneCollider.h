#pragma once
#include "DLLExporter.h"
#include "StaticCollider.h"
#include "MathHeader.h"

/// <summary>
/// BoxCollider 라는 이름이지만
/// 사실 Rigidbody도 포함되어있습니다.
/// 
/// 23.08.01 강석원&안준용 인재원.
/// </summary>
namespace Rocket
{
	class GameObject;

	class ROCKET_API PlaneCollider final : public StaticCollider
	{
	public:
		PlaneCollider();

		/// 디버그 정보 보내기.
	protected:
		virtual void Update() override;

	public:
		Vector3 GetNormalVector() const;
		float GetDistance() const;
	};
}
