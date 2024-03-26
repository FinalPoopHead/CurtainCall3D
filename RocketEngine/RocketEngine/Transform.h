#pragma once
#include <vector>

#include "DLLExporter.h"
#include "Component.h"
#include "MathHeader.h"

namespace Rocket::Core
{
	class RocketTransform;
}

namespace Rocket
{
	class GameObject;
	class DynamicModelRenderer;
	class Light;
}

namespace Rocket
{
	/// <summary>
	/// RocketTransform이라는 실제 Transform 클래스를 갖고있는 클래스.
	/// 사실상 RocketTransform의 랩퍼 클래스다.
	/// </summary>
	class ROCKET_API Transform final : public Component
	{
		friend DynamicModelRenderer;
		friend Light;

	public:
		Transform();		// 생성할 때 내 Entity가 무엇인지 저장한다.

		/// Get World Position, Rotation, Scale
	public:
		Vector3 GetPosition() const;	// world 기준
		Quaternion GetRotation() const;	// world 기준 쿼터니언
		Vector3 GetEuler() const;		// world 기준 radians
		Vector3 GetScale() const;		// world 기준

		/// Set World Position, Rotation, Scale
	public:
		void SetPosition(const Vector3& position);
		void SetPosition(float x, float y, float z);
		void SetRotation(const Quaternion& quaternion);
		void SetRotation(float x, float y, float z, float w);
		void SetRotationEuler(const Vector3& euler);
		void SetRotationEuler(float angleX, float angleY, float angleZ);
		void SetScale(const Vector3& scale);
		void SetScale(float x, float y, float z);
		

		/// Tween에게 참조로 전달하기 위함.(임시)
		/// 참조로 전달하기 더 좋고 안전한 방법 생각해보셈.
		/// 23.07.26 강석원 && 오수안 인재원
	public:
		Vector3* GetLocalPositionPointer();
		Quaternion* GetLocalRotationPointer();
		Vector3* GetLocalScalePointer();

		/// Get Local Position, Rotation, Scale 정보
	public:
		Vector3 GetLocalPosition() const;
		Quaternion GetLocalRotation() const;
		Vector3 GetLocalEuler() const;
		Vector3 GetLocalScale() const;

	public:
		void SetLocalPosition(const Vector3& position);
		void SetLocalPosition(float x, float y, float z);
		void SetLocalRotation(const Quaternion& quaternion);
		void SetLocalRotation(float x, float y, float z, float w);
		void SetLocalRotationEuler(const Vector3& euler);
		void SetLocalRotationEuler(float angleX, float angleY, float angleZ);
		void SetLocalRotationEulerXZConvert(float angleX, float angleY, float angleZ);
		void SetLocalScale(const Vector3& scale);
		void SetLocalScale(float x, float y, float z);

	public:
		/// Forwar,Up,Right vector (World 기준)
		Vector3 GetForward() const;
		Vector3 GetUp() const;
		Vector3 GetRight() const;

		Matrix GetLocalScaleMatrix() const;
		Matrix GetLocalRotationMatrix() const;
		Matrix GetLocalTranslateMatrix() const;

		Matrix GetWorldScaleMatrix() const;
		Matrix GetWorldRotationMatrix() const;
		Matrix GetWorldTranslateMatrix() const;

		Matrix GetWorldTM() const;

	public:
		void Translate(const Vector3& position);
		void Translate(float x, float y, float z);
		void Rotate(Quaternion quaternion);					// 쿼터니언 기준 회전
		void Rotate(float angleX, float angleY, float angleZ);	// 오일러 각 기준 회전, radian

		void LookAt(const Vector3& target, const Vector3& up);

		/// 계층구조.
	public:
		void SetParent(Transform* parent, bool keepWorldPosition = true);
		void SetParent(GameObject* parentObj, bool keepWorldPosition = true);
		Transform* GetParent();
		Transform* GetChild(int index);

		void ReleaseParent();
	private:
		void ReleaseChild(Transform* child);
		void AddChild(Transform* child);
		
	public:
		std::vector<Transform*>& GetChildrenVec();

	private:
		Core::RocketTransform* _rocketTransform;
		Transform* _parent;
		std::vector<Transform*> _children;

	};
}
