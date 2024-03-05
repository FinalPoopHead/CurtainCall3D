#pragma once
#include <vector>
#include "../GraphicsInterface/ITransform.h"

namespace Rocket::Core
{
	class Transform : public ITransform
	{
	public:
		Transform();

	public:
		/// World 기준 Getter Setter
		virtual void SetPosition(const Vector3& position) override;
		virtual void SetRotation(const Quaternion& rotation) override;
		virtual void SetRotationEuler(const Vector3& euler) override;		// x,y,z 축의 회전 값 (radian)
		virtual void SetScale(const Vector3& scale) override;

		virtual Vector3 GetPosition() const override;
		virtual Quaternion GetRotation() const override;
		virtual Vector3 GetEuler() const override;
		virtual Vector3 GetScale() const override;

		/// Local 기준 Getter Setter
		virtual void SetLocalPosition(const Vector3& position) override;
		virtual void SetLocalRotation(const Quaternion& quaternion) override;
		virtual void SetLocalRotationEuler(const Vector3& euler) override;
		virtual void SetLocalScale(const Vector3& scale) override;

		virtual Vector3 GetLocalPosition() const override;
		virtual Quaternion GetLocalRotation() const override;
		virtual Vector3 GetLocalEuler() const override;
		virtual Vector3 GetLocalScale() const override;

		/// Local 기준 포인터 반환 (Tween Animation 고려)
		virtual Vector3* GetLocalPositionPtr() override;
		virtual Quaternion* GetLocalRotationPtr() override;
		virtual Vector3* GetLocalScalePtr() override;

		/// World 기준 방향 벡터
		virtual Vector3 GetForward() const override;
		virtual Vector3 GetUp() const override;
		virtual Vector3 GetRight() const override;

		/// World 기준 행렬
		virtual Matrix GetScaleMatrix() const override;
		virtual Matrix GetRotationMatrix() const override;
		virtual Matrix GetTranslationMatrix() const override;
		virtual Matrix GetWorldTM() const override;

		/// Local 기준 행렬
		virtual Matrix GetLocalScaleMatrix() const override;
		virtual Matrix GetLocalRotationMatrix() const override;
		virtual Matrix GetLocalTranslationMatrix() const override;
		virtual Matrix GetLocalTM() const override;

		/// Transform 변환 함수
		virtual void Translate(const Vector3& translation) override;
		virtual void Rotate(const Quaternion& quaternion) override;
		virtual void LookAt(const Vector3& target, const Vector3& up = Vector3::Up) override;
		virtual void LookAt(const ITransform* target, const Vector3& up = Vector3::Up) override;
		virtual void RotateAround(const Vector3& point, const Vector3& axis, float angle) override;
		virtual void Scale(const Vector3& scale) override;

		/// 계층구조 함수
		virtual void SetParent(ITransform* parent) override;
		virtual ITransform* GetParent() const override;
		virtual ITransform* GetChild(int index) const override;
		virtual int GetChildCount() const override;
		virtual void ReleaseParent() override;
		virtual std::vector<ITransform*>& GetChildren() override;

	protected:
		virtual void AddChild(ITransform* child) override;
		virtual void ReleaseChild(ITransform* child) override;

	private:
		Vector3 _position;
		Quaternion _rotation;		// 쿼터니언
		Vector3 _scale;

		ITransform* _parent;
		std::vector<ITransform*> _children;
	};
}
