#pragma once
#include "DLLExporter.h"
#include "DynamicCollider.h"
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

	class ROCKET_API BoxCollider final : public DynamicCollider
	{
	public:
		BoxCollider();


		/// ����� ���� ������.
	protected:
		virtual void Update() override;

	public:
		void SetVolume(float w, float h, float d);
		virtual float GetWidth() const override;
		virtual float GetHeight() const override;
		virtual float GetDepth() const override;

	private:
		float _width;
		float _height;
		float _depth;
	};
}
