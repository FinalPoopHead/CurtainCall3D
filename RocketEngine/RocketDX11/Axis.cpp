#include <DirectXColors.h>
#include "Axis.h"
#include "GraphicsMacro.h"

namespace Rocket::Core
{
	void Axis::Initialize(ID3D11Device* device)
	{
		BuildGeometryBuffers(device);
	}

	void Axis::BuildGeometryBuffers(ID3D11Device* device)
	{
		// 정점 버퍼를 생성한다. 
		// 각 축에 맞도록 6개의 정점을 만들었다.
		Vertex vertices[] =
		{
			{ DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f), DirectX::XMFLOAT4((const float*)&DirectX::Colors::Red)  },	// x축 (빨강)
			{ DirectX::XMFLOAT3(10.0f, 0.0f, 0.0f), DirectX::XMFLOAT4((const float*)&DirectX::Colors::Red)  },

			{ DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f), DirectX::XMFLOAT4((const float*)&DirectX::Colors::Green)},	// y축 (초록)
			{ DirectX::XMFLOAT3(0.0f, 10.0f, 0.0f), DirectX::XMFLOAT4((const float*)&DirectX::Colors::Green)},

			{ DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f), DirectX::XMFLOAT4((const float*)&DirectX::Colors::Blue)	},	// z축 (파랑)
			{ DirectX::XMFLOAT3(0.0f, 0.0f, 10.0f), DirectX::XMFLOAT4((const float*)&DirectX::Colors::Blue) }
		};

		D3D11_BUFFER_DESC vbd;
		vbd.Usage = D3D11_USAGE_IMMUTABLE;
		vbd.ByteWidth = sizeof(Vertex) * 6;
		vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		vbd.CPUAccessFlags = 0;
		vbd.MiscFlags = 0;
		vbd.StructureByteStride = 0;
		D3D11_SUBRESOURCE_DATA vinitData;
		vinitData.pSysMem = vertices;
		HR(device->CreateBuffer(&vbd, &vinitData, _vertexBuffer.GetAddressOf()));


		// 인덱스 버퍼를 생성한다.
		// 역시 3개의 축을 나타내도록 했다.

		UINT indices[] = {
			// x축
			0, 1,

			// y축
			2, 3,

			// z축
			4, 5,
		};

		D3D11_BUFFER_DESC ibd;
		ibd.Usage = D3D11_USAGE_IMMUTABLE;
		ibd.ByteWidth = sizeof(UINT) * 6;
		ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
		ibd.CPUAccessFlags = 0;
		ibd.MiscFlags = 0;
		ibd.StructureByteStride = 0;
		D3D11_SUBRESOURCE_DATA iinitData;
		iinitData.pSysMem = indices;
		HR(device->CreateBuffer(&ibd, &iinitData, _indexBuffer.GetAddressOf()));

		_indexCount = 6;

		_vertexBuffer->SetPrivateData(WKPDID_D3DDebugObjectNameW, sizeof(L"vertexBuffer") - 1, L"vertexBuffer");
		_indexBuffer->SetPrivateData(WKPDID_D3DDebugObjectNameW, sizeof(L"indexBuffer") - 1, L"indexBuffer");
	}
}
