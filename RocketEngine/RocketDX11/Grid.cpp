#include <DirectXColors.h>
#include "Grid.h"
#include "GraphicsMacro.h"

namespace Rocket::Core
{
	void Grid::Initialize(ID3D11Device* device)
	{
		BuildGeometryBuffers(device);
	}

	void Grid::BuildGeometryBuffers(ID3D11Device* device)
	{
		// 정점 버퍼를 생성한다. 
		// 40개의 정점을 만들었다.
		Vertex vertices[100];
		for (int i = 0; i < 100; i++)
		{
			vertices[i].Pos = DirectX::XMFLOAT3((float)(i % 10) - 5.0f, 0.0f, (float)(i / 10) - 5.0f);
			vertices[i].Color = DirectX::XMFLOAT4((const float*)&DirectX::Colors::DarkGray);	// (어두운 회색)
		}

		D3D11_BUFFER_DESC vbd;
		vbd.Usage = D3D11_USAGE_IMMUTABLE;
		vbd.ByteWidth = sizeof(Vertex) * 100;
		vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		vbd.CPUAccessFlags = 0;
		vbd.MiscFlags = 0;
		vbd.StructureByteStride = 0;

		D3D11_SUBRESOURCE_DATA vinitData;
		vinitData.pSysMem = vertices;
		HR(device->CreateBuffer(&vbd, &vinitData, &_vertexBuffer));


		// 인덱스 버퍼를 생성한다.
		// 역시 40개의 라인을 나타내도록 했다.
		UINT indices[40];
		for (int i = 0; i < 10; i++)
		{
			indices[i * 2] = i;
			indices[i * 2 + 1] = i + 90;
		}

		for (int i = 0; i < 10; i++)
		{
			indices[20 + (i * 2)] = i * 10;
			indices[20 + (i * 2) + 1] = i * 10 + 9;
		}

		D3D11_BUFFER_DESC ibd;
		ibd.Usage = D3D11_USAGE_IMMUTABLE;
		ibd.ByteWidth = sizeof(UINT) * 40;
		ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
		ibd.CPUAccessFlags = 0;
		ibd.MiscFlags = 0;
		ibd.StructureByteStride = 0;
		D3D11_SUBRESOURCE_DATA iinitData;
		iinitData.pSysMem = indices;
		HR(device->CreateBuffer(&ibd, &iinitData, &_indexBuffer));

		_indexCount = 40;
	}
}
