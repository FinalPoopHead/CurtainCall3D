#pragma once
#include "MathHeader.h"

namespace Rocket::Core
{
	class ITransform
	{
	public:
		virtual ~ITransform() {};

		/// World 기준 Getter Setter
		virtual void SetPosition(const Vector3& position) = 0;
		virtual void SetRotation(const Quaternion& rotation) = 0;
		virtual void SetRotationEuler(const Vector3& euler) = 0;		// world 기준 (degrees)
		virtual void SetScale(const Vector3& scale) = 0;

		virtual Vector3 GetPosition() const = 0;			// world 기준
		virtual Quaternion GetRotation() const = 0;			// world 기준
		virtual Vector3 GetEuler() const = 0;				// world 기준 (radians)
		virtual Vector3 GetScale() const = 0;				// world 기준

		/// Local 기준 Getter Setter
		virtual void SetLocalPosition(const Vector3& position) = 0;
		virtual void SetLocalRotation(const Quaternion& rotation) = 0;
		virtual void SetLocalRotationEuler(const Vector3& euler) = 0;		// world 기준 (degrees)
		virtual void SetLocalScale(const Vector3& scale) = 0;

		virtual Vector3 GetLocalPosition() const = 0;		// local 기준
		virtual Quaternion GetLocalRotation() const = 0;	// local 기준
		virtual Vector3 GetLocalEuler() const = 0;			// local 기준 (radians)
		virtual Vector3 GetLocalScale() const = 0;			// local 기준

		/// Local 기준 포인터 반환 (Tween Animation 고려)
		virtual Vector3* GetLocalPositionPtr() = 0;
		virtual Quaternion* GetLocalRotationPtr() = 0;
		virtual Vector3* GetLocalScalePtr() = 0;

		/// World 기준 방향 벡터
		virtual Vector3 GetForward() const = 0;			// world 기준
		virtual Vector3 GetUp() const = 0;				// world 기준
		virtual Vector3 GetRight() const = 0;				// world 기준

		/// World 기준 행렬
		virtual Matrix GetScaleMatrix() const = 0;
		virtual Matrix GetRotationMatrix() const = 0;
		virtual Matrix GetTranslationMatrix() const = 0;
		virtual Matrix GetWorldTM() const = 0;

		/// Local 기준 행렬
		virtual Matrix GetLocalScaleMatrix() const = 0;
		virtual Matrix GetLocalRotationMatrix() const = 0;
		virtual Matrix GetLocalTranslationMatrix() const = 0;
		virtual Matrix GetLocalTM() const = 0;

		/// Transform 변환 함수
		virtual void Translate(const Vector3& translation) = 0;
		virtual void Rotate(const Quaternion& rotation) = 0;

		virtual void LookAt(const Vector3& target, const Vector3& up = Vector3::Up) = 0;
		virtual void LookAt(const ITransform* target, const Vector3& up = Vector3::Up) = 0;

		virtual void RotateAround(const Vector3& point, const Vector3& axis, float angle) = 0;	// world 기준 (degrees)

		virtual void Scale(const Vector3& scale) = 0;

		/// 계층구조 함수
		virtual void SetParent(ITransform* parent) = 0;
		virtual ITransform* GetParent() const = 0;

		virtual void ReleaseParent() = 0;

		virtual ITransform* GetChild(int index) const = 0;
		virtual std::vector<ITransform*>& GetChildren() = 0;
		virtual int GetChildCount() const = 0;

	protected:
		virtual void AddChild(ITransform* child) = 0;
		virtual void ReleaseChild(ITransform* child) = 0;
	};
}
