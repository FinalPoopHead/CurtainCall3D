#include "HelperObjectBase.h"
#include "VertexShader.h"
#include "PixelShader.h"
#include "GraphicsMacro.h"

namespace Rocket::Core
{
	HelperObjectBase::HelperObjectBase()
		: _vertexBuffer(nullptr), _indexBuffer(nullptr),
		_world(), _view(), _proj(), _renderState(), _indexCount(0)
	{

	}

	HelperObjectBase::~HelperObjectBase()
	{
		// ComPtr 덕분에 해제할 필요 없다.
		// RenderState는 준쪽에서 ComPtr로 관리하고 있다.
	}

	void HelperObjectBase::Update(const DirectX::XMMATRIX& world, const DirectX::XMMATRIX& view, const DirectX::XMMATRIX& proj)
	{
		_world = world;
		_view = view;
		_proj = proj;
	}

	void HelperObjectBase::Render(ID3D11DeviceContext* deviceContext)
	{
		// Grid가 쓰는 Shader deviceContext 이용해 연결.
		deviceContext->VSSetShader(_vertexShader->GetVertexShader(), nullptr, 0);
		deviceContext->PSSetShader(_pixelShader->GetPixelShader(), nullptr, 0);

		// 입력 배치 객체 셋팅
		deviceContext->IASetInputLayout(_vertexShader->GetInputLayout());
		deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);

		// 인덱스버퍼와 버텍스버퍼 셋팅
		UINT stride = sizeof(Vertex);
		UINT offset = 0;
		deviceContext->IASetVertexBuffers(0, 1, _vertexBuffer.GetAddressOf(), &stride, &offset);
		deviceContext->IASetIndexBuffer(_indexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);

		D3D11_MAPPED_SUBRESOURCE mappedResource;
		MatrixBufferType* dataPtr;
		unsigned int bufferNumber = 0;

		_world = DirectX::XMMatrixTranspose(_world);
		_view = DirectX::XMMatrixTranspose(_view);
		_proj = DirectX::XMMatrixTranspose(_proj);

		HR(deviceContext->Map(_vertexShader->GetConstantBuffer(bufferNumber), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource));

		dataPtr = (MatrixBufferType*)mappedResource.pData;

		dataPtr->world = _world;
		dataPtr->view = _view;
		dataPtr->projection = _proj;

		deviceContext->Unmap(_vertexShader->GetConstantBuffer(bufferNumber), 0);

		deviceContext->VSSetConstantBuffers(bufferNumber, 1, _vertexShader->GetAddressOfConstantBuffer(bufferNumber));

		// 렌더스테이트
		deviceContext->RSSetState(_renderState);

		deviceContext->DrawIndexed(_indexCount, 0, 0);
	}

	void HelperObjectBase::SetRenderState(ID3D11RasterizerState* renderState)
	{
		_renderState = renderState;
	}

	void HelperObjectBase::SetShader(VertexShader* vertexShader, PixelShader* pixelShader)
	{
		_vertexShader = vertexShader;
		_pixelShader = pixelShader;
	}

}
