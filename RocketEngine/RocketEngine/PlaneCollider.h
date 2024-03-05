#pragma once
#include "DLLExporter.h"
#include "StaticCollider.h"
#include "MathHeader.h"

/// <summary>
/// BoxCollider ��� �̸�������
/// ��� Rigidbody�� ���ԵǾ��ֽ��ϴ�.
/// 
/// 23.08.01 ������&���ؿ� �����.
/// </summary>
namespace Rocket
{
	class GameObject;

	class ROCKET_API PlaneCollider final : public StaticCollider
	{
	public:
		PlaneCollider();

		/// ����� ���� ������.
	protected:
		virtual void Update() override;

	public:
		Vector3 GetNormalVector() const;
		float GetDistance() const;
	};
}
