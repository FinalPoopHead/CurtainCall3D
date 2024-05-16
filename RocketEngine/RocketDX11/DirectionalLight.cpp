#include "DirectionalLight.h"
#include "../RocketCommon/RocketTransform.h"
#include "Camera.h"

namespace Rocket::Core
{
	DirectionalLight::DirectionalLight()
		: _transform(nullptr),
		_diffuseColor(1.0f, 1.0f, 1.0f, 1.0f),
		_ambientColor(0.3f, 0.3f, 0.3f, 0.3f),
		_specularColor(1.0f, 1.0f, 1.0f, 1.0f),
		_specularPower(4.0f),
		_lengthZ(0.0f),
		_shadowRadius(50.0f),
		_lightPosForShadow(Vector3::Zero),
		_boundingFrustum(),
		_viewMatrix(),
		_projectionMatrix()
	{

	}

	Vector3 DirectionalLight::GetForward() const
	{
		return _transform->GetForward();
	}

	void DirectionalLight::Update()
	{
		// 카메라 근처의 적절한 위치로 위치 변경.
		auto mainCam = Camera::GetMainCamera();
		//_lengthZ = mainCam->GetLengthZ();
		_lightPosForShadow = mainCam->GetPosition() + (mainCam->GetForward() * _shadowRadius);	// frustum의 중앙으로 보냄
		_lightPosForShadow -= _transform->GetForward() * _shadowRadius;							// 빛 방향의 반대방향으로 lengthZ만큼 보냄

		UpdateViewMatrix();
		UpdateProjectionMatrix();
	}

	void DirectionalLight::UpdateViewMatrix()
	{
		auto temp = DirectX::XMMatrixLookAtLH(_lightPosForShadow, _lightPosForShadow + _transform->GetForward(), _transform->GetUp());
		//auto temp = DirectX::XMMatrixLookAtLH(_transform->GetPosition(), _transform->GetPosition() + _transform->GetForward(), _transform->GetUp());
		DirectX::XMStoreFloat4x4(&_viewMatrix, temp);

// 		DirectX::XMVECTOR R = _transform->GetRight();
// 		DirectX::XMVECTOR U = _transform->GetUp();
// 		DirectX::XMVECTOR L = _transform->GetForward();
// 		DirectX::XMVECTOR P = _lightPosForShadow;
// 
// 		// Keep camera's axes orthogonal to each other and of unit length.
// 		L = DirectX::XMVector3Normalize(L);
// 		U = DirectX::XMVector3Normalize(DirectX::XMVector3Cross(L, R));
// 
// 		// U, L already ortho-normal, so no need to normalize cross product.
// 		R = DirectX::XMVector3Cross(U, L);
// 
// 		// Fill in the view matrix entries.
// 		float x = -DirectX::XMVectorGetX(DirectX::XMVector3Dot(P, R));
// 		float y = -DirectX::XMVectorGetX(DirectX::XMVector3Dot(P, U));
// 		float z = -DirectX::XMVectorGetX(DirectX::XMVector3Dot(P, L));
// 
// 		DirectX::XMFLOAT3 _right;
// 		DirectX::XMFLOAT3 _up;
// 		DirectX::XMFLOAT3 _look;
// 
// 		DirectX::XMStoreFloat3(&_right, R);
// 		DirectX::XMStoreFloat3(&_up, U);
// 		DirectX::XMStoreFloat3(&_look, L);
// 
// 		_viewMatrix(0, 0) = _right.x;
// 		_viewMatrix(1, 0) = _right.y;
// 		_viewMatrix(2, 0) = _right.z;
// 		_viewMatrix(3, 0) = x;
// 
// 		_viewMatrix(0, 1) = _up.x;
// 		_viewMatrix(1, 1) = _up.y;
// 		_viewMatrix(2, 1) = _up.z;
// 		_viewMatrix(3, 1) = y;
// 
// 		_viewMatrix(0, 2) = _look.x;
// 		_viewMatrix(1, 2) = _look.y;
// 		_viewMatrix(2, 2) = _look.z;
// 		_viewMatrix(3, 2) = z;
// 
// 		_viewMatrix(0, 3) = 0.0f;
// 		_viewMatrix(1, 3) = 0.0f;
// 		_viewMatrix(2, 3) = 0.0f;
// 		_viewMatrix(3, 3) = 1.0f;
	}

	void DirectionalLight::UpdateProjectionMatrix()
	{
		// [in] float ViewWidth, [in] float ViewHeight, [in] float NearZ, [in] float FarZ

		//DirectX::XMMATRIX temp = DirectX::XMMatrixPerspectiveFovLH(DirectX::XMConvertToRadians(70.0f/2), 16.0f/9.0f , 0.01f, 1000.0f);
		//float tempSize = 64.0f;
		//DirectX::XMMATRIX temp = DirectX::XMMatrixOrthographicLH(tempSize, tempSize, -100.0f, 100.0f);
		//DirectX::XMMATRIX temp = DirectX::XMMatrixOrthographicLH(tempSize / 9.0f, tempSize / 16.0f, 0.001f, _lengthZ / 2);
		DirectX::XMMATRIX projMatrix = DirectX::XMMatrixOrthographicLH(_shadowRadius * 2, _shadowRadius * 2, 0.0001f, _shadowRadius * 2);
		//DirectX::XMMATRIX temp = DirectX::XMMatrixOrthographicOffCenterLH(-_shadowRadius, _shadowRadius, -_shadowRadius, _shadowRadius, -_shadowRadius, _shadowRadius);
		DirectX::XMStoreFloat4x4(&_projectionMatrix, projMatrix);

		DirectX::XMMATRIX boundingMatrix = DirectX::XMMatrixOrthographicLH(_shadowRadius * 8, _shadowRadius * 8, 0.0001f, _shadowRadius * 4);
		_boundingFrustum = DirectX::BoundingFrustum(boundingMatrix);		// boundingFrustum도 갱신해준다.
	}

	bool DirectionalLight::FrustumCulling(const DirectX::BoundingBox& boundingBox)
	{
		DirectX::BoundingFrustum transformedFrustum;
		auto TM = DirectX::XMMatrixAffineTransformation(_transform->GetScale(), Vector3::Zero, _transform->GetRotation(), _lightPosForShadow);
		_boundingFrustum.Transform(transformedFrustum, TM);
		return transformedFrustum.Intersects(boundingBox);
	}

	bool DirectionalLight::FrustumCulling(const DirectX::BoundingOrientedBox& boundingOrientedBox)
	{
		DirectX::BoundingFrustum transformedFrustum;
		auto TM = DirectX::XMMatrixAffineTransformation(_transform->GetScale(), Vector3::Zero, _transform->GetRotation(), _lightPosForShadow);
		_boundingFrustum.Transform(transformedFrustum, TM);
		return transformedFrustum.Intersects(boundingOrientedBox);
	}

	bool DirectionalLight::FrustumCulling(const DirectX::BoundingSphere& boundingSphere)
	{
		auto TM = DirectX::XMMatrixAffineTransformation(_transform->GetScale(), Vector3::Zero, _transform->GetRotation(), _lightPosForShadow);
		DirectX::BoundingFrustum transformedFrustum;
		_boundingFrustum.Transform(transformedFrustum, TM);
		return transformedFrustum.Intersects(boundingSphere);
	}

}
