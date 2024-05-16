#include <DirectXColors.h>	// 이미 많은 부분 DX11과 겹친다.
#include "CubeMesh.h"
#include "GraphicsMacro.h"
#include "VertexStruct.h"

namespace Rocket::Core
{
	CubeMesh::CubeMesh()
	{

	}

	CubeMesh::~CubeMesh()
	{
		// 		_vertexBuffer.Reset();
		// 		_indexBuffer.Reset();
	}

	void CubeMesh::Initialize(ID3D11Device* device)
	{
		BuildGeometryBuffers(device);
	}

	void CubeMesh::BuildGeometryBuffers(ID3D11Device* device)
	{
		_vertexType = eVertexType::VERTEX;

		// 각 면의 정점을 정의합니다.
		Vertex vertices[] =
		{
			// 윗면
			{DirectX::XMFLOAT3(-0.5f, 0.5f, 0.5f), DirectX::XMFLOAT2(0.0f, 0.0f), DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f), DirectX::XMFLOAT3(1.0f, 0.0f, 0.0f), DirectX::XMFLOAT3(0.0f, 0.0f, 1.0f), 0},
			{DirectX::XMFLOAT3(0.5f, 0.5f, 0.5f), DirectX::XMFLOAT2(1.0f, 0.0f), DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f), DirectX::XMFLOAT3(1.0f, 0.0f, 0.0f), DirectX::XMFLOAT3(0.0f, 0.0f, 1.0f), 0},
			{DirectX::XMFLOAT3(0.5f, 0.5f, -0.5f), DirectX::XMFLOAT2(1.0f, 1.0f), DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f), DirectX::XMFLOAT3(1.0f, 0.0f, 0.0f), DirectX::XMFLOAT3(0.0f, 0.0f, 1.0f), 0},
			{DirectX::XMFLOAT3(-0.5f, 0.5f, -0.5f), DirectX::XMFLOAT2(0.0f, 1.0f), DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f), DirectX::XMFLOAT3(1.0f, 0.0f, 0.0f), DirectX::XMFLOAT3(0.0f, 0.0f, 1.0f), 0},

			// 앞면
			{DirectX::XMFLOAT3(-0.5f, 0.5f, -0.5f), DirectX::XMFLOAT2(0.0f, 0.0f), DirectX::XMFLOAT3(0.0f, 0.0f, -1.0f), DirectX::XMFLOAT3(1.0f, 0.0f, 0.0f), DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f), 0},
			{DirectX::XMFLOAT3(0.5f, 0.5f, -0.5f), DirectX::XMFLOAT2(1.0f, 0.0f), DirectX::XMFLOAT3(0.0f, 0.0f, -1.0f), DirectX::XMFLOAT3(1.0f, 0.0f, 0.0f), DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f), 0},
			{DirectX::XMFLOAT3(0.5f, -0.5f, -0.5f), DirectX::XMFLOAT2(1.0f, 1.0f), DirectX::XMFLOAT3(0.0f, 0.0f, -1.0f), DirectX::XMFLOAT3(1.0f, 0.0f, 0.0f), DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f), 0},
			{DirectX::XMFLOAT3(-0.5f, -0.5f, -0.5f), DirectX::XMFLOAT2(0.0f, 1.0f), DirectX::XMFLOAT3(0.0f, 0.0f, -1.0f), DirectX::XMFLOAT3(1.0f, 0.0f, 0.0f), DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f), 0},

			// 오른면
			{DirectX::XMFLOAT3(0.5f, 0.5f, -0.5f), DirectX::XMFLOAT2(0.0f, 0.0f), DirectX::XMFLOAT3(1.0f, 0.0f, 0.0f), DirectX::XMFLOAT3(0.0f, 0.0f, 1.0f), DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f), 0},
			{DirectX::XMFLOAT3(0.5f, 0.5f, 0.5f), DirectX::XMFLOAT2(1.0f, 0.0f), DirectX::XMFLOAT3(1.0f, 0.0f, 0.0f), DirectX::XMFLOAT3(0.0f, 0.0f, 1.0f), DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f), 0},
			{DirectX::XMFLOAT3(0.5f, -0.5f, 0.5f), DirectX::XMFLOAT2(1.0f, 1.0f), DirectX::XMFLOAT3(1.0f, 0.0f, 0.0f), DirectX::XMFLOAT3(0.0f, 0.0f, 1.0f), DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f), 0},
			{DirectX::XMFLOAT3(0.5f, -0.5f, -0.5f), DirectX::XMFLOAT2(0.0f, 1.0f), DirectX::XMFLOAT3(1.0f, 0.0f, 0.0f), DirectX::XMFLOAT3(0.0f, 0.0f, 1.0f), DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f), 0},

			// 왼면
			{DirectX::XMFLOAT3(-0.5f, 0.5f, 0.5f), DirectX::XMFLOAT2(0.0f, 0.0f), DirectX::XMFLOAT3(-1.0f, 0.0f, 0.0f), DirectX::XMFLOAT3(0.0f, 0.0f, -1.0f), DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f), 0},
			{DirectX::XMFLOAT3(-0.5f, 0.5f, -0.5f), DirectX::XMFLOAT2(1.0f, 0.0f), DirectX::XMFLOAT3(-1.0f, 0.0f, 0.0f), DirectX::XMFLOAT3(0.0f, 0.0f, -1.0f), DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f), 0},
			{DirectX::XMFLOAT3(-0.5f, -0.5f, -0.5f), DirectX::XMFLOAT2(1.0f, 1.0f), DirectX::XMFLOAT3(-1.0f, 0.0f, 0.0f), DirectX::XMFLOAT3(0.0f, 0.0f, -1.0f), DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f), 0},
			{DirectX::XMFLOAT3(-0.5f, -0.5f, 0.5f), DirectX::XMFLOAT2(0.0f, 1.0f), DirectX::XMFLOAT3(-1.0f, 0.0f, 0.0f), DirectX::XMFLOAT3(0.0f, 0.0f, -1.0f), DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f), 0},

			// 뒷면
			{DirectX::XMFLOAT3(0.5f, 0.5f, 0.5f), DirectX::XMFLOAT2(0.0f, 0.0f), DirectX::XMFLOAT3(0.0f, 0.0f, 1.0f), DirectX::XMFLOAT3(-1.0f, 0.0f, 0.0f), DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f), 0},
			{DirectX::XMFLOAT3(-0.5f, 0.5f, 0.5f), DirectX::XMFLOAT2(1.0f, 0.0f), DirectX::XMFLOAT3(0.0f, 0.0f, 1.0f), DirectX::XMFLOAT3(-1.0f, 0.0f, 0.0f), DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f), 0},
			{DirectX::XMFLOAT3(-0.5f, -0.5f, 0.5f), DirectX::XMFLOAT2(1.0f, 1.0f), DirectX::XMFLOAT3(0.0f, 0.0f, 1.0f), DirectX::XMFLOAT3(-1.0f, 0.0f, 0.0f), DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f), 0},
			{DirectX::XMFLOAT3(0.5f, -0.5f, 0.5f), DirectX::XMFLOAT2(0.0f, 1.0f), DirectX::XMFLOAT3(0.0f, 0.0f, 1.0f), DirectX::XMFLOAT3(-1.0f, 0.0f, 0.0f), DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f), 0},

			// 아랫면
			{DirectX::XMFLOAT3(-0.5f, -0.5f, -0.5f), DirectX::XMFLOAT2(0.0f, 0.0f), DirectX::XMFLOAT3(0.0f, -1.0f, 0.0f), DirectX::XMFLOAT3(1.0f, 0.0f, 0.0f), DirectX::XMFLOAT3(0.0f, 0.0f, -1.0f), 0},
			{DirectX::XMFLOAT3(0.5f, -0.5f, -0.5f), DirectX::XMFLOAT2(1.0f, 0.0f), DirectX::XMFLOAT3(0.0f, -1.0f, 0.0f), DirectX::XMFLOAT3(1.0f, 0.0f, 0.0f), DirectX::XMFLOAT3(0.0f, 0.0f, -1.0f), 0},
			{DirectX::XMFLOAT3(0.5f, -0.5f, 0.5f), DirectX::XMFLOAT2(1.0f, 1.0f), DirectX::XMFLOAT3(0.0f, -1.0f, 0.0f), DirectX::XMFLOAT3(1.0f, 0.0f, 0.0f), DirectX::XMFLOAT3(0.0f, 0.0f, -1.0f), 0},
			{DirectX::XMFLOAT3(-0.5f, -0.5f, 0.5f), DirectX::XMFLOAT2(0.0f, 1.0f), DirectX::XMFLOAT3(0.0f, -1.0f, 0.0f), DirectX::XMFLOAT3(1.0f, 0.0f, 0.0f), DirectX::XMFLOAT3(0.0f, 0.0f, -1.0f), 0}
		};

		// 정점 버퍼를 생성합니다.
		D3D11_BUFFER_DESC vbd;
		vbd.Usage = D3D11_USAGE_IMMUTABLE;
		vbd.ByteWidth = sizeof(Vertex) * ARRAYSIZE(vertices);
		vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		vbd.CPUAccessFlags = 0;
		vbd.MiscFlags = 0;
		vbd.StructureByteStride = 0;

		D3D11_SUBRESOURCE_DATA vinitData;
		vinitData.pSysMem = vertices;
		HR(device->CreateBuffer(&vbd, &vinitData, &_vertexBuffer));

		// 각 면의 인덱스를 정의합니다.
		UINT indices[] =
		{
			0, 1, 2,
			2, 3, 0,

			4, 5, 6,
			6, 7, 4,

			8, 9, 10,
			10, 11, 8,

			12, 13, 14,
			14, 15, 12,

			16, 17, 18,
			18, 19, 16,

			20, 21, 22,
			22, 23, 20
		};

		// 인덱스 버퍼를 생성합니다.
		D3D11_BUFFER_DESC ibd;
		ibd.Usage = D3D11_USAGE_IMMUTABLE;
		ibd.ByteWidth = sizeof(UINT) * ARRAYSIZE(indices);
		ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
		ibd.CPUAccessFlags = 0;
		ibd.MiscFlags = 0;
		ibd.StructureByteStride = 0;
		D3D11_SUBRESOURCE_DATA iinitData;
		iinitData.pSysMem = indices;
		HR(device->CreateBuffer(&ibd, &iinitData, &_indexBuffer));

		// 버퍼의 정점 및 인덱스 개수를 설정합니다.
		_vertexCount = ARRAYSIZE(vertices);
		_indexCount = ARRAYSIZE(indices);

		// 버퍼에 디버그 이름을 설정합니다.
		_vertexBuffer->SetPrivateData(WKPDID_D3DDebugObjectNameW, sizeof(L"CUBE v-Buffer") - 1, L"CUBE v-Buffer");
		_indexBuffer->SetPrivateData(WKPDID_D3DDebugObjectNameW, sizeof(L"CUBE i-Buffer") - 1, L"CUBE i-Buffer");
	}

}

