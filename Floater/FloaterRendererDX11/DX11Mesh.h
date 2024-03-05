#pragma once
#include <d3d11.h>
#include "CommonMath.h"
#include "../FloaterRendererCommon/include/IBuilder.h"
#include "../FloaterRendererCommon/include/Resource.h"
#include "DX11PixelShader.h"
#include "DX11VertexShader.h"
#include "DX11Animation.h"

namespace flt
{
	struct RawMesh;
	class Transform;

	struct Vertex
	{
		using type = struct _Type
		{
			Vector3f pos;
			Vector2f tex[10];
			Vector3f normal;
			Vector3f binormal;
			Vector3f tangent;
			UINT BoneIndices[4] = { 0, };
			float BoneWeights[4] = { 0.0f, };
		};

		static constexpr unsigned int numElements = 22;

		static constexpr D3D11_INPUT_ELEMENT_DESC layout[numElements] =
		{
			{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
			{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
			{"TEXCOORD", 1, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
			{"TEXCOORD", 2, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
			{"TEXCOORD", 3, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
			{"TEXCOORD", 4, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
			{"TEXCOORD", 5, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
			{"TEXCOORD", 6, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
			{"TEXCOORD", 7, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
			{"TEXCOORD", 8, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
			{"TEXCOORD", 9, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
			{"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
			{"BINORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
			{"TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
			{"BLENDINDICES", 0, DXGI_FORMAT_R32_UINT , 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
			{"BLENDINDICES", 1, DXGI_FORMAT_R32_UINT , 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{"BLENDINDICES", 2, DXGI_FORMAT_R32_UINT , 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{"BLENDINDICES", 3, DXGI_FORMAT_R32_UINT , 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{"BLENDWEIGHT", 0, DXGI_FORMAT_R32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{"BLENDWEIGHT", 1, DXGI_FORMAT_R32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{"BLENDWEIGHT", 2, DXGI_FORMAT_R32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{"BLENDWEIGHT", 3, DXGI_FORMAT_R32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
		};
	};

	struct DX11Mesh
	{
		DX11Mesh(const DX11VertexShaderBuilder& vsBuilder, const DX11PixelShaderBuilder& psBuilder) :
			vertexShader(vsBuilder),
			pixelShader(psBuilder),
			vertexBuffer(nullptr),
			singleVertexSize(0),
			indexBuffer(nullptr),
			indexCount(0),
			srv(nullptr),
			srvCount(0),
			sampler(nullptr),
			pAnimation(nullptr)
		{}

		void Release();

		Resource<DX11VertexShader> vertexShader;
		Resource<DX11PixelShader> pixelShader;

		ID3D11Buffer* vertexBuffer;
		UINT singleVertexSize;

		ID3D11Buffer* indexBuffer;
		UINT indexCount;

		ID3D11ShaderResourceView** srv;
		UINT srvCount;
		ID3D11SamplerState* sampler;

		DX11Animation* pAnimation;
	};

	template struct Resource<DX11Mesh>;

	struct DX11MeshBuilder : public IBuilder<DX11Mesh>
	{
		DX11MeshBuilder(const std::wstring& name) : IBuilder<DX11Mesh>(name), pDevice(nullptr), pImmediateContext(nullptr), pRawMesh(nullptr), vsBuilder(), psBuilder() {}

		virtual DX11Mesh* build() const override;

		ID3D11Device* pDevice;
		ID3D11DeviceContext* pImmediateContext;
		RawMesh* pRawMesh;
		DX11VertexShaderBuilder vsBuilder;
		DX11PixelShaderBuilder psBuilder;
	};

	struct DX11CubeMeshBuilder : public DX11MeshBuilder
	{
		DX11CubeMeshBuilder() : DX11MeshBuilder(L"flt::CubeBuilder") {}

		virtual DX11Mesh* build() const override;
	};

	struct DX11ScreedQuadBuilder : public DX11MeshBuilder
	{
		DX11ScreedQuadBuilder() : DX11MeshBuilder(L"flt::ScreedQuadBuilder") {}

		virtual DX11Mesh* build() const override;
	};

	struct DX11GridMeshBuilder : public DX11MeshBuilder
	{
		DX11GridMeshBuilder() : DX11MeshBuilder(L"flt::GridMeshBuilder"), gridCount(0) {}

		virtual DX11Mesh* build() const override;
		int gridCount;
	};
}
