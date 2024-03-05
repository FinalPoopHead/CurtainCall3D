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

	class ROCKET_API StaticBoxCollider final : public StaticCollider
	{
	public:
		StaticBoxCollider();

		/// ����� ���� ������.
	protected:
		virtual void Update() override;

	public:
		void SetVolume(float w, float h, float d);
		float GetWidth() const;
		float GetHeight() const;
		float GetDepth() const;

	private:
		float _width;
		float _height;
		float _depth;
	};
}
