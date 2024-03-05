#pragma once
#include "../../FloaterMath/include/Vector2f.h"
#include "../../FloaterMath/include/Matrix4f.h"
#include "../../FloaterMath/include/Common.h"
#include "Transform.h"

namespace flt
{
	class Camera
	{
	public:
		Camera(Transform* pTransform) : Camera(pTransform, DegToRad(60.f), 0.01f, 1000.f, 3.2f, 1.8f, false) {}
		Camera(Transform* pTransform, float fieldOfView, float nearZ, float farZ, float viewWidth, float viewHeight, bool isOrtho) noexcept :
			_pTransform(pTransform),
			_fieldOfView(fieldOfView),
			_near(nearZ),
			_far(farZ),
			_viewRect(viewWidth, viewHeight),
			_isOrtho(isOrtho),
			priority(0),
			_viewPortCoord(0.f, 0.f),
			_viewPortSize(1.0f, 1.0f) {}
		Matrix4f GetViewMatrix() const noexcept;
		Matrix4f GetProjectionMatrix() const noexcept;
		const Transform* GetTransform() const noexcept { return _pTransform; }

	public:
		int priority;

	private:
		Transform* _pTransform;

		float _fieldOfView;
		float _near;
		float _far;
		Vector2f _viewRect;

		bool _isOrtho;

		Vector2f _viewPortCoord;
		Vector2f _viewPortSize;
	};
}
