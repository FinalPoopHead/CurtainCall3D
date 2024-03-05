#include "Axis.h"
#include <DirectXColors.h>	// 이미 많은 부분 DX11과 겹친다.
#include "GraphicsMacro.h"
#include "ResourceManager.h"

namespace Rocket::Core
{
	Axis::Axis()
		: _vertexBuffer(nullptr), _indexBuffer(nullptr),
		_world(), _view(), _proj(), _renderState(ResourceManager::Instance().GetRenderState(ResourceManager::eRenderState::WIREFRAME))
	{

	}

	Axis::~Axis()
	{
		_vertexBuffer.Reset();
		_indexBuffer.Reset();
	}


	void Axis::Initialize(ID3D11Device* device)
	{
		BuildGeometryBuffers(device);
	}

	void Axis::Update(const DirectX::XMMATRIX& world, const DirectX::XMMATRIX& view, const DirectX::XMMATRIX& proj)
	{
		_world = world;
		_view = view;
		_proj = proj;
	}

	void Axis::Render(ID3D11DeviceContext* deviceContext, ID3D11VertexShader* vertexShader, ID3D11PixelShader* pixelShader, ID3D11Buffer* matrixBuffer, ID3D11InputLayout* inputLayout)
	{
		// Grid가 쓰는 Shader deviceContext 이용해 연결.
		deviceContext->VSSetShader(vertexShader, nullptr, 0);
		deviceContext->PSSetShader(pixelShader, nullptr, 0);

		// 입력 배치 객체 셋팅
		deviceContext->IASetInputLayout(inputLayout);
		deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);

		// 인덱스버퍼와 버텍스버퍼 셋팅
		UINT stride = sizeof(Vertex);
		UINT offset = 0;
		deviceContext->IASetVertexBuffers(0, 1, _vertexBuffer.GetAddressOf(), &stride, &offset);
		deviceContext->IASetIndexBuffer(_indexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);

		D3D11_MAPPED_SUBRESOURCE mappedResource;
		MatrixBufferType* dataPtr;
		unsigned int bufferNumber;

		_world = DirectX::XMMatrixTranspose(_world);
		_view = DirectX::XMMatrixTranspose(_view);
		_proj = DirectX::XMMatrixTranspose(_proj);

		HR(deviceContext->Map(matrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource));

		dataPtr = (MatrixBufferType*)mappedResource.pData;

		dataPtr->world = _world;
		dataPtr->view = _view;
		dataPtr->projection = _proj;

		deviceContext->Unmap(matrixBuffer, 0);

		bufferNumber = 0;

		deviceContext->VSSetConstantBuffers(bufferNumber, 1, &matrixBuffer);

		// 렌더스테이트
		deviceContext->RSSetState(_renderState.Get());

		deviceContext->DrawIndexed(40, 0, 0);
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
		HR(device->CreateBuffer(&vbd, &vinitData, &_vertexBuffer));


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
		HR(device->CreateBuffer(&ibd, &iinitData, &_indexBuffer));
	}
}
