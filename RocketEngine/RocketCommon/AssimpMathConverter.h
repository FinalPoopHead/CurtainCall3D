#pragma once
#include <DirectXMath.h>
#include <assimp/scene.h>

inline DirectX::XMMATRIX AIMatrix4x4ToXMMatrix(const aiMatrix4x4& matrix)
{
	DirectX::XMMATRIX m;
	m.r[0].m128_f32[0] = matrix.a1;
	m.r[0].m128_f32[1] = matrix.a2;
	m.r[0].m128_f32[2] = matrix.a3;
	m.r[0].m128_f32[3] = matrix.a4;

	m.r[1].m128_f32[0] = matrix.b1;
	m.r[1].m128_f32[1] = matrix.b2;
	m.r[1].m128_f32[2] = matrix.b3;
	m.r[1].m128_f32[3] = matrix.b4;

	m.r[2].m128_f32[0] = matrix.c1;
	m.r[2].m128_f32[1] = matrix.c2;
	m.r[2].m128_f32[2] = matrix.c3;
	m.r[2].m128_f32[3] = matrix.c4;

	m.r[3].m128_f32[0] = matrix.d1;
	m.r[3].m128_f32[1] = matrix.d2;
	m.r[3].m128_f32[2] = matrix.d3;
	m.r[3].m128_f32[3] = matrix.d4;

	return m;
}

inline DirectX::XMFLOAT3 AIVec3ToXMFloat3(const aiVector3D& vector)
{
	DirectX::XMFLOAT3 v;

	v.x = vector.x;
	v.y = vector.y;
	v.z = vector.z;

	return v;
}

inline DirectX::XMFLOAT4 AIQuaternionToXMFloat4(const aiQuaternion& quaternion)
{
	DirectX::XMFLOAT4 v;

	v.x = quaternion.x;
	v.y = quaternion.y;
	v.z = quaternion.z;
	v.w = quaternion.w;

	return v;
}
