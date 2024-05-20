#include "Camera.h"
#include <cmath>
#include "GameObject.h"
#include "Transform.h"
#include "MathHeader.h"
#include <windows.h>	// 디버그용으로 열었음
#include "GraphicsSystem.h"

namespace Rocket
{
	Camera::Camera()
		: _nearZ(0.01f)
		, _farZ(1000.0f)
		, _aspect(16.0f / 9.0f)
		, _fovY(70.0f)
		, _nearWindowHeight()
		, _farWindowHeight()
		, _isShakeCameraOnHit(false), _isShakeCameraOnShoot(false)
		, _shakeDurationOnHit(0.1f), _shakeDurationOnShoot(0.1f)
		, _distX(0.0f), _distY(0.0f), _distYOnShoot(0.0f)
		, _camera(Core::GraphicsSystem::Instance().GetFactory()->CreateCamera())
		, _ray()
	{
		_camera->SetNearZ(0.01f);
		_camera->SetFarZ(1000.0f);
		_camera->SetAspect(16.0f / 9.0f);
		_camera->SetFOVY(70.0f);
	}

	void Camera::Start()
	{

	}

	void Camera::BindTransform()
	{
		_camera->BindTransform(gameObject->transform._rocketTransform);
	}

	void Camera::SetAsMainCamera()
	{
		_camera->SetAsMainCamera();
	}

	float Camera::GetNearZ() const
	{
		return _nearZ;
	}

	float Camera::GetFarZ() const
	{
		return _farZ;
	}

	float Camera::GetAspect() const
	{
		return _aspect;
	}

	float Camera::GetFovX() const
	{
		return DirectX::XMConvertToDegrees(GetRadianFovX());
	}

	float Camera::GetRadianFovX() const
	{
		float halfWidth = GetNearWindowWidth() / 2;
		return 2.0f * atan(halfWidth / _nearZ);
	}

	float Camera::GetFovY() const
	{
		return _fovY;
	}

	float Camera::GetRadianFovY() const
	{
		return DirectX::XMConvertToRadians(_fovY);
	}

	float Camera::GetNearWindowWidth() const
	{
		return _aspect * _nearWindowHeight;
	}

	float Camera::GetNearWindowHeight() const
	{
		return _nearWindowHeight;
	}

	float Camera::GetFarWindowWidth() const
	{
		return _aspect * _farWindowHeight;
	}

	float Camera::GetFarWindowHeight() const
	{
		return _farWindowHeight;
	}

	Matrix Camera::GetViewMatrix() const
	{
		return _viewMatrix;
	}

	Matrix Camera::GetProjMatrix() const
	{
		return _projMatrix;
	}

	void Camera::SetNearZ(float nearZ)
	{
		_nearZ = nearZ;
		//_cameraData.nearZ = nearZ;
	}

	void Camera::SetFarZ(float farZ)
	{
		//_cameraData.farZ = farZ;
	}

	void Camera::SetAspect(float aspect)
	{	
		//_cameraData.aspect = aspect;
	}

	void Camera::SetFovY(float fovY)
	{
		//_cameraData.fovY = fovY;
	}

	void Camera::Strafe(float delta)
	{
		Vector3 rightVec = gameObject->transform.GetRight();
		rightVec.x *= delta;
		rightVec.y *= delta;
		rightVec.z *= delta;

		gameObject->transform.Translate(rightVec);
	}

	void Camera::Walk(float delta)
	{
		Vector3 moveVec = gameObject->transform.GetForward() * delta;
		gameObject->transform.Translate(moveVec);
	}

	void Camera::WorldUpDown(float delta)
	{
		gameObject->transform.Translate(Vector3::Up * delta);
	}

	void Camera::Yaw(float angle)
	{
		// 아직 구현하지 않았음.
		assert(false);
	}

	void Camera::Pitch(float angle)
	{
		gameObject->transform.Rotate(angle, 0.0f, 0.0f);
	}

	void Camera::Roll(float angle)
	{
		// 아직 구현하지 않았음.
		assert(false);
	}

	void Camera::RotateY(float angle)
	{
		Quaternion rotQuat = Quaternion::CreateFromAxisAngle({ 0.0f,1.0f,0.0f }, angle);
		Quaternion result = Quaternion::Concatenate(rotQuat, gameObject->transform.GetLocalRotation());
		gameObject->transform.SetLocalRotation(result);
	}

	void Camera::ShakeCameraUpdateOnHit(float deltaTime)
	{
		if (!_isShakeCameraOnHit)
		{
			return;
		}

		float shakeIntensity = 0.01f;
		float shakeFrequency = 120.0f;

		if (_shakeDurationOnHit > 0.0f)
		{
			gameObject->transform.Translate(-_distX, -_distY, 0.0f);
			//_viewMatrix._41 += shakeIntensity * sin(shakeFrequency * _shakeDuration);
			//_viewMatrix._42 += shakeIntensity * cos(shakeFrequency * _shakeDuration);
			_distX = shakeIntensity * sin(shakeFrequency * _shakeDurationOnHit);
			_distY = shakeIntensity * cos(shakeFrequency * _shakeDurationOnHit);
			gameObject->transform.Translate(_distX, _distY, 0.0f);

			_shakeDurationOnHit -= deltaTime;
		}
		else
		{
			gameObject->transform.Translate(-_distX, -_distY, 0.0f);
			_shakeDurationOnHit = 0.1f;
			_isShakeCameraOnHit = false;
			_distX = 0.0f;
			_distY = 0.0f;
		}
	}

	void Camera::ShakeCameraUpdateOnShoot(float deltaTime)
	{
		if (!_isShakeCameraOnShoot)
		{
			return;
		}

		float shakeIntensity = 0.03f;
		float shakeFrequency = 90.0f;

		if (_shakeDurationOnShoot > 0.0f)
		{
			gameObject->transform.Translate(0.0f, -_distYOnShoot, 0.0f);
			//_viewMatrix._41 += shakeIntensity * sin(shakeFrequency * _shakeDuration);
			//_viewMatrix._42 += shakeIntensity * cos(shakeFrequency * _shakeDuration);
			_distYOnShoot = shakeIntensity * cos(shakeFrequency * _shakeDurationOnShoot);
			gameObject->transform.Translate(0.0f, _distYOnShoot, 0.0f);

			_shakeDurationOnShoot -= deltaTime;
		}
		else
		{
			gameObject->transform.Translate(0.0f, -_distYOnShoot, 0.0f);
			_shakeDurationOnShoot = 0.1f;
			_isShakeCameraOnShoot = false;
			_distYOnShoot = 0.0f;
		}
	}

	void Camera::SetIsCameraShakeOnHit(bool cameraShake)
	{
		_isShakeCameraOnHit = cameraShake;
	}

	void Camera::SetIsCameraShakeOnShoot(bool cameraShake)
	{
		_isShakeCameraOnShoot = cameraShake;
	}

}
