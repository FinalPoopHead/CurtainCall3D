#pragma once
#include <DirectXMath.h>

namespace Rocket::Core
{
	enum class eVertexType
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
		DirectX::XMMATRIX worldInverse;
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

	struct LightPassBufferType
	{
		DirectX::XMFLOAT3 lightDirection;
		float padding;
	};

	struct CameraBufferType
	{
		DirectX::XMFLOAT3 cameraPosition;
		float padding;
	};

	struct NodeBufferType
	{
		DirectX::XMMATRIX transformMatrix[512];
	};

	struct BoneBufferType
	{
		DirectX::XMMATRIX transformMatrix[512];
	};

	struct CubeMapVertex
	{
		DirectX::XMFLOAT3 position;
	};

	struct CubeMapBufferType
	{
		DirectX::XMMATRIX view;
		DirectX::XMMATRIX projection;
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

		Vertex() 
		: position(), UV(), normal(), tangent(), nodeIndex() {}
		Vertex(const DirectX::XMFLOAT3& p, const DirectX::XMFLOAT2& uv, const DirectX::XMFLOAT3& n, const DirectX::XMFLOAT3& t)
			: position(p), UV(uv), normal(n), tangent(t),nodeIndex() {}
		Vertex(
			float px, float py, float pz,
			float u, float v,
			float nx, float ny, float nz,
			float tx, float ty, float tz)
			: position(px, py, pz), UV(u, v),
			normal(nx, ny, nz), tangent(tx, ty, tz), nodeIndex() {}
		Vertex(const DirectX::XMFLOAT3& p, const DirectX::XMFLOAT2& uv)
			: position(p), UV(uv), normal{ 0, 0, 0 }, tangent{ 0, 0, 0 }, nodeIndex() {}
		Vertex(const DirectX::XMFLOAT3& p, const DirectX::XMFLOAT3& n, const DirectX::XMFLOAT2& uv)
			: position(p), normal(n), UV(uv), tangent{ 0, 0, 0 }, nodeIndex() {}
	};

	struct VertexSkinned
	{
		DirectX::XMFLOAT3 position;
		DirectX::XMFLOAT2 UV;
		DirectX::XMFLOAT3 normal;
		DirectX::XMFLOAT3 tangent;
		UINT nodeIndex;
		DirectX::XMFLOAT4 weights;			// 최대 4개의 본에 대한 가중치
		DirectX::XMUINT4 boneIndices;		// 최대 4개의 본에 대한 인덱스
	};

	struct PosColor		// For debug object
	{
		DirectX::XMFLOAT3 Position;
		DirectX::XMFLOAT4 Color;
	};
}
