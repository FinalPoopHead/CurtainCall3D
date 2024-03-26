#pragma once
#include <vector>

#include "MathHeader.h"

namespace Rocket::Core
{
	class RocketTransform
	{
	public:
		RocketTransform();

	public:
		/// World 기준 Getter Setter
		void SetPosition(const Vector3& position);
		void SetRotation(const Quaternion& rotation);
		void SetRotationEuler(const Vector3& euler);		// x,y,z 축의 회전 값 (radian)
		void SetScale(const Vector3& scale);

		Vector3 GetPosition() const;
		Quaternion GetRotation() const;
		Vector3 GetEuler() const;
		Vector3 GetScale() const;

		/// Local 기준 Getter Setter
		void SetLocalPosition(const Vector3& position);
		void SetLocalRotation(const Quaternion& quaternion);
		void SetLocalRotationEuler(const Vector3& euler);
		void SetLocalScale(const Vector3& scale);

		Vector3 GetLocalPosition() const;
		Quaternion GetLocalRotation() const;
		Vector3 GetLocalEuler() const;
		Vector3 GetLocalScale() const;

		/// Local 기준 포인터 반환 (Tween Animation 고려)
		Vector3* GetLocalPositionPtr();
		Quaternion* GetLocalRotationPtr();
		Vector3* GetLocalScalePtr();

		/// World 기준 방향 벡터
		Vector3 GetForward() const;
		Vector3 GetUp() const;
		Vector3 GetRight() const;

		/// World 기준 행렬
		Matrix GetScaleMatrix() const;
		Matrix GetRotationMatrix() const;
		Matrix GetTranslationMatrix() const;
		Matrix GetWorldTM() const;

		/// Local 기준 행렬
		Matrix GetLocalScaleMatrix() const;
		Matrix GetLocalRotationMatrix() const;
		Matrix GetLocalTranslationMatrix() const;
		Matrix GetLocalTM() const;

		/// Transform 변환 함수
		void Translate(const Vector3& translation);
		void Rotate(const Quaternion& quaternion);
		void LookAt(const Vector3& target, const Vector3& up = Vector3::Up);
		void LookAt(const RocketTransform* target, const Vector3& up = Vector3::Up);
		void RotateAround(const Vector3& point, const Vector3& axis, float angle);
		void Scale(const Vector3& scale);

		/// 계층구조 함수
		void SetParent(RocketTransform* parent, bool keepWorldPosition);
		RocketTransform* GetParent() const;
		RocketTransform* GetChild(int index) const;
		int GetChildCount() const;
		void ReleaseParent();
		std::vector<RocketTransform*>& GetChildren();

	protected:
		void AddChild(RocketTransform* child);
		void ReleaseChild(RocketTransform* child);

	private:
		Vector3 _position;
		Quaternion _rotation;		// 쿼터니언
		Vector3 _scale;

		RocketTransform* _parent;
		std::vector<RocketTransform*> _children;
	};
}
