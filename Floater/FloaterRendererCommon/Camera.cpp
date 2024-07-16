#include "./include/Camera.h"


flt::Matrix4f flt::Camera::GetViewMatrix() const noexcept
{
	Matrix4f worldMatrix = _pTransform->GetWorldMatrix4f();
	Vector3f u{ worldMatrix.e[0][0], worldMatrix.e[0][1], worldMatrix.e[0][2] };
	Vector3f v{ worldMatrix.e[1][0], worldMatrix.e[1][1], worldMatrix.e[1][2] };
	Vector3f n{ worldMatrix.e[2][0], worldMatrix.e[2][1], worldMatrix.e[2][2] };

	Vector3f eye = (Vector3f)_pTransform->GetWorldPosition();

	return Matrix4f
	{
		u.x, v.x, n.x, 0.0f,
		u.y, v.y, n.y, 0.0f,
		u.z, v.z, n.z, 0.0f,
		-(eye.Dot(u)), -(eye.Dot(v)), -(eye.Dot(n)), 1.0f
	};
}

flt::Matrix4f flt::Camera::GetProjectionMatrix() const noexcept
{
	if (_isOrtho)
	{
		float f = 1.0f / (_far - _near);

		return Matrix4f
		{
			2.0f / _viewRect.x,	0.0f,				0.0f,				0.0f,
			0.0f,				2.0f / _viewRect.y,	0.0f,				0.0f,
			0.0f,				0.0f,				f,					0.0f,
			0.0f,				0.0f,				-(f * _near),		1.0f
		};
	}
	else
	{
		float d = cos(_fieldOfView / 2) / sin(_fieldOfView / 2);
		float a = _viewRect.x / _viewRect.y;
		float div = _far / (_far - _near);

		return Matrix4f
		{
			d / a,		0.0f,		0.0f,			0.0f,
			0.0f,		d,			0.0f,			0.0f,
			0.0f,		0.0f,		div,			1.0f,
			0.0f,		0.0f,		-div * _near,	0.0f
		};
	}
}

flt::Transform* flt::Camera::SetTransform(Transform* transform)
{
	Transform* old = _pTransform;
	_pTransform = transform;
	return old;
}

float flt::Camera::SetFov(float fovRadY) noexcept
{
	float old = _fieldOfView;
	_fieldOfView = fovRadY;
	return old;
}

float flt::Camera::SetNearZ(float nearZ) noexcept
{
	float old = _near;
	_near = nearZ;
	return old;
}

float flt::Camera::SetFarZ(float farZ) noexcept
{
	float old = _far;
	_far = farZ;
	return old;
}

flt::Vector2f flt::Camera::SetViewRectAspect(float width, float height) noexcept
{
	Vector2f old = _viewRect;
	_viewRect = Vector2f(width, height);
	return old;
}
