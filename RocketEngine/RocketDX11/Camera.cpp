#include <cmath>
#include "Camera.h"
#include "ResourceManager.h"
#include "VertexStruct.h"
#include "GraphicsMacro.h"
#include "../RocketCommon/RocketTransform.h"

namespace Rocket::Core
{
	Camera* Camera::_mainCamera;

	Camera::Camera()
		: _nearZ(0.01f), _farZ(1000.0f), _aspect(16.0f / 9.0f), _fovY(70.0f),
		_nearWindowHeight(), _farWindowHeight(),
		_viewMatrix(), _projectionMatrix()
		, _boundingFrustum()
	{
		_nearWindowHeight = 2.0f * _nearZ * std::tanf(DirectX::XMConvertToRadians(_fovY / 2));
		_farWindowHeight = 2.0f * _farZ * std::tanf(DirectX::XMConvertToRadians(_fovY / 2));
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

	void Camera::SetNearHeight(float height)
	{
		_nearWindowHeight = height;
		UpdateProjectionMatrix();
	}

	void Camera::SetFarHeight(float height)
	{
		_farWindowHeight = height;
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
		_mainCamera = this;
	}

	void Camera::BindTransform(RocketTransform* transform)
	{
		_transform = transform;
	}

	Camera* Camera::GetMainCamera()
	{
		return _mainCamera;
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

}
