#pragma once
#include <DirectXMath.h>

namespace Rocket::Core
{
	enum class VertexType
	{
		COLOR_VERTEX,
		TEXTURE_VERTEX,
		LIGHT_VERTEX,
		VERTEX,
		SKINNED_VERTEX,
	};

	struct ColorVertex
	{
		DirectX::XMFLOAT3 position;
		DirectX::XMFLOAT4 color;
	};

	struct TextureVertex
	{
		DirectX::XMFLOAT3 position;
		DirectX::XMFLOAT2 uv;
	};

	struct LightVertex
	{
		DirectX::XMFLOAT3 position;
		DirectX::XMFLOAT2 uv;
		DirectX::XMFLOAT3 normal;
	};

	struct MatrixBufferType
	{
		DirectX::XMMATRIX world;
		DirectX::XMMATRIX view;
		DirectX::XMMATRIX projection;
	};

	struct LightBufferType
	{
		DirectX::XMFLOAT4 ambientColor;
		DirectX::XMFLOAT4 diffuseColor;
		DirectX::XMFLOAT3 lightDirection;
		float specularPower;
		DirectX::XMFLOAT4 specularColor;
	};

	struct CameraBufferType
	{
		DirectX::XMFLOAT3 cameraPosition;
		float padding;
	};

	struct NodeBufferType
	{
		DirectX::XMMATRIX transformMatrix[256];
	};

	/// <summary>
	/// assimp 용 vertex 구조체?
	/// </summary>
	struct Vertex
	{
		DirectX::XMFLOAT3 position;	// The position of the vertex
		DirectX::XMFLOAT2 UV;		// UV Coordinate for texturing
		DirectX::XMFLOAT3 normal;	// Normal for lighting
		DirectX::XMFLOAT3 tangent;	// For normal mapping
		UINT nodeIndex;				// 노드 인덱스

		Vertex() {}
		Vertex(const DirectX::XMFLOAT3& p, const DirectX::XMFLOAT2& uv, const DirectX::XMFLOAT3& n, const DirectX::XMFLOAT3& t)
			: position(p), UV(uv), normal(n), tangent(t) {}
		Vertex(
			float px, float py, float pz,
			float u, float v,
			float nx, float ny, float nz,
			float tx, float ty, float tz)
			: position(px, py, pz), UV(u, v),
			normal(nx, ny, nz), tangent(tx, ty, tz) {}
		Vertex(const DirectX::XMFLOAT3& p, const DirectX::XMFLOAT2& uv)
			: position(p), UV(uv), normal{ 0, 0, 0 }, tangent{ 0, 0, 0 } {}
	};

	struct VertexSkinned
	{
		DirectX::XMFLOAT3 Position;
		DirectX::XMFLOAT3 Normal;
		DirectX::XMFLOAT3 Tangent;
		DirectX::XMFLOAT2 UV;
		DirectX::XMFLOAT4 Weights;
		DirectX::XMUINT4 BoneIndices;
	};

	struct PosColor		// For debug object
	{
		DirectX::XMFLOAT3 Position;
		DirectX::XMFLOAT4 Color;
	};
}
