#include <cmath>
#include "Camera.h"
#include "ResourceManager.h"
#include "VertexStruct.h"
#include "GraphicsMacro.h"
#include "../RocketCommon/RocketTransform.h"

namespace Rocket::Core
{
	Camera* Camera::_mainCamera[2];

	Camera::Camera()
		: _nearZ(0.01f), _farZ(1500.0f), _aspect(16.0f / 9.0f), _fovY(70.0f),
		_viewMatrix(), _projectionMatrix()
		, _boundingFrustum()
	{
		UpdateProjectionMatrix();
	}

	Camera::~Camera()
	{

	}

	DirectX::XMFLOAT3 Camera::GetPosition() const
	{
		return _transform->GetPosition();
	}

	void Camera::UpdateViewMatrix()
	{
		auto temp = DirectX::XMMatrixLookAtLH(_transform->GetPosition(), _transform->GetPosition() + _transform->GetForward(), _transform->GetUp());
		DirectX::XMStoreFloat4x4(&_viewMatrix, temp);
	}

	DirectX::XMMATRIX Camera::GetViewMatrix() const
	{
		return DirectX::XMLoadFloat4x4(&_viewMatrix);
	}

	DirectX::XMMATRIX Camera::GetProjectionMatrix() const
	{
		return DirectX::XMLoadFloat4x4(&_projectionMatrix);
	}

	DirectX::XMMATRIX Camera::GetViewProjectionMatrix() const
	{
		return XMMatrixMultiply(GetViewMatrix(), GetProjectionMatrix());
	}

	void Camera::SetNearZ(float nearZ)
	{
		_nearZ = nearZ;
		UpdateProjectionMatrix();
	}

	void Camera::SetFarZ(float farZ)
	{
		_farZ = farZ;
		UpdateProjectionMatrix();
	}

	void Camera::SetAspect(float aspect)
	{
		_aspect = aspect;
		UpdateProjectionMatrix();
	}

	void Camera::SetFOVY(float fov)
	{
		_fovY = fov;
		UpdateProjectionMatrix();
	}

	void Camera::UpdateProjectionMatrix()
	{
		DirectX::XMMATRIX temp = DirectX::XMMatrixPerspectiveFovLH(DirectX::XMConvertToRadians(_fovY / 2), _aspect, _nearZ, _farZ);
		DirectX::XMStoreFloat4x4(&_projectionMatrix, temp);

		DirectX::XMMATRIX boundingFrustumMatrix = DirectX::XMMatrixPerspectiveFovLH(DirectX::XMConvertToRadians(_fovY / 2 * 1.0f), _aspect * 1.0f, _nearZ, _farZ);
		_boundingFrustum = DirectX::BoundingFrustum(boundingFrustumMatrix);		// boundingFrustum도 갱신해준다.
	}

	void Camera::SetAsMainCamera()
	{
		_mainCamera[0] = this;
		_mainCamera[1] = nullptr;
	}

	void Camera::BindTransform(RocketTransform* transform)
	{
		_transform = transform;
	}

	Camera* Camera::GetMainCamera(UINT index /*= 0*/)
	{
		return _mainCamera[index];
	}

	DirectX::XMMATRIX Camera::GetWorldMatrix() const
	{
		return _transform->GetWorldTM();
	}

	bool Camera::FrustumCulling(const DirectX::BoundingBox& boundingBox)
	{
		DirectX::BoundingFrustum transformedFrustum;
		_boundingFrustum.Transform(transformedFrustum, _transform->GetWorldTM());
		return transformedFrustum.Intersects(boundingBox);
	}

	bool Camera::FrustumCulling(const DirectX::BoundingOrientedBox& boundingOrientedBox)
	{
		DirectX::BoundingFrustum transformedFrustum;
		_boundingFrustum.Transform(transformedFrustum, _transform->GetWorldTM());
		return transformedFrustum.Intersects(boundingOrientedBox);
	}

	bool Camera::FrustumCulling(const DirectX::BoundingSphere& boundingSphere)
	{
		DirectX::BoundingFrustum transformedFrustum;
		_boundingFrustum.Transform(transformedFrustum, _transform->GetWorldTM());
		return transformedFrustum.Intersects(boundingSphere);
	}

	void Camera::Update()
	{
		UpdateViewMatrix();
	}

	Vector3 Camera::GetForward() const
	{
		return _transform->GetForward();
	}

	Vector3 Camera::GetUp() const
	{
		return _transform->GetUp();
	}

	Vector3 Camera::GetRight() const
	{
		return _transform->GetRight();
	}

	void Camera::AddToMainCamera()
	{
		if (_mainCamera[0] == nullptr)
		{
			_mainCamera[0] = this;
		}
		else if (_mainCamera[1] == nullptr)
		{
			_mainCamera[1] = this;
		}
	}

	Camera** Camera::GetMainCamArr()
	{
		return _mainCamera;
	}

}
