﻿#pragma once
#include <vector>
#include <d3d11.h>
#include <wrl.h>
#include <DirectXCollision.h>

#include "MathHeader.h"
#include "../RocketCommon/ICamera.h"

using Microsoft::WRL::ComPtr;

namespace Rocket::Core
{
/// <summary>
/// 카메라 클래스
/// </summary>
	class Camera : public ICamera
	{
	public:
		Camera();
		Camera(float width, float height);
		~Camera();

	public:
		virtual void SetActive(bool isActive) override;
		virtual void Destroy() override;
		virtual void SetWorldTM(const Matrix& worldTM) override {}
		virtual bool IsActive() override { return _isActive; }

		/// ICamera 인터페이스 구현
	public:
		virtual void SetNearZ(float nearZ) override;
		virtual void SetFarZ(float farZ) override;
		virtual void SetAspect(float aspect) override;
		virtual void SetFOVY(float fov) override;
		virtual void SetAsMainCamera() override;
		virtual void AddToMainCamera() override;
		virtual bool SetMainCameraIndex(int index) override;
		virtual void BindTransform(RocketTransform* transform) override;
		virtual void GetAllValues(float& outNearZ, float& outFarZ, float& outAspect, float& outFovY, DirectX::XMFLOAT4X4& outViewMatrix, DirectX::XMFLOAT4X4& outProjectionMatrix) override;

		/// Static
	public:
		static Camera* GetMainCamera(UINT index = 0);
		static std::vector<Camera*>& GetMainCameras();

	private:
		static std::vector<Camera*> _mainCamera;

	public:
		DirectX::XMFLOAT3 GetPosition() const;

		void Update();

		DirectX::XMMATRIX GetWorldMatrix() const;				// 카메라의 worldTM을 반환
		DirectX::XMMATRIX GetViewMatrix() const;				// 카메라의 로컬좌표'계'를 반환
		DirectX::XMMATRIX GetProjectionMatrix() const;		// 카메라의 투영행렬을 반환
		DirectX::XMMATRIX GetViewProjectionMatrix() const;	// 위의 두 행렬을 적절히 곱해서 반환(view의 역행렬을 곱해주겠지?)

		Vector3 GetForward() const;
		Vector3 GetUp() const;
		Vector3 GetRight() const;

		float GetLengthZ() const { return _farZ - _nearZ; }
		void HalftheAspect() { SetAspect(_aspect / 2.0f); }

	public:
		bool FrustumCulling(const DirectX::BoundingBox& boundingBox);
		bool FrustumCulling(const DirectX::BoundingOrientedBox& boundingOrientedBox);
		bool FrustumCulling(const DirectX::BoundingSphere& boundingSphere);

	private:
		void UpdateProjectionMatrix();
		void UpdateViewMatrix();

	private:
		bool _isActive;
		RocketTransform* _transform;
		DirectX::BoundingFrustum _boundingFrustum;

		float _nearZ;					// frustum의 가까운 평면까지의 거리
		float _farZ;					// frustum의 먼 평면까지의 거리
		float _aspect;					// 가로 / 세로 비율
		float _fovY;					// fovY각도를 60분법으로 갖고있음

		DirectX::XMFLOAT4X4 _viewMatrix;		// 카메라의 로컬좌표'계' 또는 카메라 worldTM의 역행렬
		DirectX::XMFLOAT4X4 _projectionMatrix;	// 카메라의 투영 행렬
	};
}
