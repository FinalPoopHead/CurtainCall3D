#pragma once
#include "DLLExporter.h"
#include "Component.h"
#include "Transform.h"
#include "GameObject.h"
#include <vector>

#include "MathHeader.h"

namespace physx
{
	class PxScene;
}

namespace Rocket
{
	class GameObject;
}

namespace Rocket
{
	class ROCKET_API Collider : public Component
	{
	public:
		Collider();

	// Collider 자체적으로 들고 있는 TRS Offset과 그 getter, setter
	public:
		void SetPositionOffset(Vector3 position);
		void SetRotationOffset(Quaternion rotation);
		void SetRotationOffset(float angleX, float angleY, float angleZ);
		void SetScaleOffset(Vector3 scale);

		Vector3 GetPositionOffset() const;
		Quaternion GetRotationOffset() const;
		Vector3 GetScaleOffset() const;

		Matrix GetOffsetTranslateMatrix() const;
		Matrix GetOffsetRotationMatrix() const;
		Matrix GetOffsetScaleMatrix() const;

		Matrix GetOffsetTM() const;

	protected:
		Vector3 _positionOffset;
		Quaternion _rotationOffset;
		Vector3 _scaleOffset;

	public:
		virtual void UpdateToPhysics() abstract;

	protected:
		bool _wasTranslated = false;

	public:
		void SetPxScene(physx::PxScene* pxScene);
		physx::PxScene* GetPxScene();

	protected:
		physx::PxScene* _pxScene;
	};
}
