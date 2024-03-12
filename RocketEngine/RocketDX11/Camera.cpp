#include <cmath>
#include "Camera.h"
#include "ResourceManager.h"
#include "VertexStruct.h"
#include "GraphicsMacro.h"

using namespace DirectX;

namespace Rocket::Core
{
	Camera* Camera::_mainCamera;

	Camera::Camera()
		: _position(0.0f, 2.0f, -10.0f),
		_rotation(0.0f, 0.0f, 0.0f, 1.0f),
		_nearZ(0.01f), _farZ(1000.0f), _aspect(16.0f / 9.0f), _fovY(70.0f),
		_nearWindowHeight(), _farWindowHeight(),
		_viewMatrix(), _projectionMatrix()
	{
		_nearWindowHeight = 2.0f * _nearZ * std::tanf(XMConvertToRadians(_fovY / 2));
		_farWindowHeight = 2.0f * _farZ * std::tanf(XMConvertToRadians(_fovY / 2));
		CreateCameraBuffer(ResourceManager::Instance().GetDevice());
	}

	Camera::~Camera()
	{

	}

	DirectX::XMFLOAT3 Camera::GetPosition() const
	{
		return _position;
	}

	void Camera::SetPosition(float x, float y, float z)
	{
		_position = { x,y,z };
	}

	void Camera::SetRotation(float w, float x, float y, float z)
	{
		_rotation = { x,y,z,w };
	}

	/// 카메라의 세팅을 설정한다.
	/// 들어온 값에 맞춰 여러 멤버들도 재설정해준다.
	/// 지금은 XMMatrixPerspectiveFovLH 함수를 이용해서 투영행렬을 만든다.
	/// 이 부분은 내가 직접 투영행렬을 만들어보고 싶다.
	/// 
	/// 23.04.20 강석원 인재원
// 	void Camera::SetFrustum(float fovY, float aspect, float nearZ, float farZ)
// 	{
// 		_fovY = fovY;
// 		_aspect = aspect;
// 		_nearZ = nearZ;
// 		_farZ = farZ;
// 
// 		_nearWindowHeight = 2.0f * _nearZ * std::tanf(XMConvertToRadians(_fovY / 2));
// 		_farWindowHeight = 2.0f * _farZ * std::tanf(XMConvertToRadians(_fovY / 2));
// 	}

	/// ViewMatrix를 갱신해준다.
	/// 교수님 코드에서는 잘못된 벡터가 되어있을 시 look을 기준으로 다시 점검해서 변경하는 듯 하다.
	/// 나는 그냥 viewMatrix만 갱신해줬다.
	/// 
	/// 그리고 position은 음수로 줘야한다! 왜냐하면 카메라 기준으로 옮기는거니까!
	/// 
	/// 23.04.20 강석원 인재원
	void Camera::UpdateViewMatrix()
	{
// 		XMMATRIX world = XMLoadFloat4x4(&_worldMatrix);
// 		XMVECTOR det = XMMatrixDeterminant(world);
// 		XMStoreFloat4x4(&_viewMatrix, XMMatrixInverse(&det, world));
// 
// 		return;

		XMVECTOR R = GetRight();
		XMVECTOR U = GetUp();
		XMVECTOR L = GetForward();
		XMVECTOR P = DirectX::XMLoadFloat3(&_position);

		// Keep camera's axes orthogonal to each other and of unit length.
		L = XMVector3Normalize(L);
		U = XMVector3Normalize(XMVector3Cross(L, R));

		// U, L already ortho-normal, so no need to normalize cross product.
		R = XMVector3Cross(U, L);

		// Fill in the view matrix entries.
		float x = -XMVectorGetX(XMVector3Dot(P, R));
		float y = -XMVectorGetX(XMVector3Dot(P, U));
		float z = -XMVectorGetX(XMVector3Dot(P, L));

		DirectX::XMFLOAT3 _right;
		DirectX::XMFLOAT3 _up;
		DirectX::XMFLOAT3 _look;

		DirectX::XMStoreFloat3(&_right, R);
		DirectX::XMStoreFloat3(&_up, U);
		DirectX::XMStoreFloat3(&_look, L);

		_viewMatrix(0, 0) = _right.x;
		_viewMatrix(1, 0) = _right.y;
		_viewMatrix(2, 0) = _right.z;
		_viewMatrix(3, 0) = x;

		_viewMatrix(0, 1) = _up.x;
		_viewMatrix(1, 1) = _up.y;
		_viewMatrix(2, 1) = _up.z;
		_viewMatrix(3, 1) = y;

		_viewMatrix(0, 2) = _look.x;
		_viewMatrix(1, 2) = _look.y;
		_viewMatrix(2, 2) = _look.z;
		_viewMatrix(3, 2) = z;

		_viewMatrix(0, 3) = 0.0f;
		_viewMatrix(1, 3) = 0.0f;
		_viewMatrix(2, 3) = 0.0f;
		_viewMatrix(3, 3) = 1.0f;
		// 
		// 	viewMatrix_ =
		// 	{
		// 		right_.x,		up_.x,			look_.x,		0.0f,
		// 		right_.y,		up_.y,			look_.y,		0.0f,
		// 		right_.z,		up_.z,			look_.z,		0.0f,
		// 		-position_.x,	-position_.y,	-position_.z,	1.0f
		// 	};
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

	DirectX::XMVECTOR Camera::GetForward() const
	{
		XMFLOAT3 forward = { 0.0f,0.0f,1.0f };
		auto rotationMatrix = XMMatrixRotationQuaternion(DirectX::XMLoadFloat4(&_rotation));
		auto result = DirectX::XMVector3Transform(DirectX::XMLoadFloat3(&forward), rotationMatrix);
		return result;
	}

	DirectX::XMVECTOR Camera::GetUp() const
	{
		XMFLOAT3 up = { 0.0f,1.0f,0.0f };
		auto rotationMatrix = XMMatrixRotationQuaternion(DirectX::XMLoadFloat4(&_rotation));
		auto result = DirectX::XMVector3Transform(DirectX::XMLoadFloat3(&up), rotationMatrix);
		return result;
	}

	DirectX::XMVECTOR Camera::GetRight() const
	{
		XMFLOAT3 right = { 1.0f,0.0f,0.0f };
		auto rotationMatrix = XMMatrixRotationQuaternion(DirectX::XMLoadFloat4(&_rotation));
		auto result = DirectX::XMVector3Transform(DirectX::XMLoadFloat3(&right), rotationMatrix);
		return result;
	}

	void Camera::SetWorldTM(const Matrix& matrix)
	{
		_worldMatrix = matrix;
	}

	void Camera::SetNearZ(float nearZ)
	{
		_nearZ = nearZ;
	}

	void Camera::SetFarZ(float farZ)
	{
		_farZ = farZ;
	}

	void Camera::SetAspect(float aspect)
	{
		_aspect = aspect;
	}

	void Camera::SetFOVY(float fov)
	{
		_fovY = fov;
	}

	void Camera::SetNearHeight(float height)
	{
		_nearWindowHeight = height;
	}

	void Camera::SetFarHeight(float height)
	{
		_farWindowHeight = height;
	}

	void Camera::UpdateProjectionMatrix()
	{
		XMMATRIX temp = XMMatrixPerspectiveFovLH(XMConvertToRadians(_fovY / 2), _aspect, _nearZ, _farZ);
		DirectX::XMStoreFloat4x4(&_projectionMatrix, temp);
	}

	void Camera::SetAsMainCamera()
	{
		_mainCamera = this;
	}

	Camera* Camera::GetMainCamera()
	{
		return _mainCamera;
	}

	void Camera::SetPositionAndRotation(const Vector3& pos, const Quaternion& rot)
	{
		SetPosition(pos.x, pos.y, pos.z);
		SetRotation(rot.w, rot.x, rot.y, rot.z);
	}

	void Camera::CreateCameraBuffer(ID3D11Device* device)
	{
		D3D11_BUFFER_DESC cameraBufferDesc;
		cameraBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
		cameraBufferDesc.ByteWidth = sizeof(CameraBufferType);
		cameraBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		cameraBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		cameraBufferDesc.MiscFlags = 0;
		cameraBufferDesc.StructureByteStride = 0;

		HR(device->CreateBuffer(&cameraBufferDesc, NULL, &_cameraBuffer));
	}

	ID3D11Buffer* Camera::GetCameraBuffer() const
	{
		return _cameraBuffer.Get();
	}

	ID3D11Buffer** Camera::GetAddressOfCameraBuffer()
	{
		return _cameraBuffer.GetAddressOf();
	}

}
