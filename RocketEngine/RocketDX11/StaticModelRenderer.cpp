#include "StaticModelRenderer.h"
#include "GraphicsMacro.h"
#include "VertexStruct.h"
#include "ResourceManager.h"
#include "StaticMesh.h"

namespace Rocket::Core
{
	StaticModelRenderer::StaticModelRenderer()
		: _model(nullptr),
		_material(nullptr),
		_isActive(true),
		_worldTM(Matrix::Identity)
	{

	}

	void StaticModelRenderer::SetWorldTM(const Matrix& worldTM)
	{
		_worldTM = worldTM;
	}

	void StaticModelRenderer::SetActive(bool isActive)
	{
		_isActive = isActive;
	}

	void StaticModelRenderer::LoadModel(std::string fileName)
	{
		// TODO : reinterpret_cast를 사용하지 않는 엑세렌또한 방법을 찾아보자.
		_model = reinterpret_cast<StaticModel*>(ResourceManager::Instance().GetModel(fileName));
	}

	void StaticModelRenderer::LoadTexture(std::string fileName)
	{
		_material->SetTexture(ResourceManager::Instance().GetTexture(fileName));
	}

	void StaticModelRenderer::Render(ID3D11DeviceContext* deviceContext, const DirectX::XMMATRIX& view, const DirectX::XMMATRIX& proj)
	{
		if (!_isActive)
		{
			return;
		}

		// Grid가 쓰는 Shader deviceContext 이용해 연결.
		deviceContext->VSSetShader(_material->GetVertexShader()->GetVertexShader(), nullptr, 0);
		deviceContext->PSSetShader(_material->GetPixelShader()->GetPixelShader(), nullptr, 0);

		deviceContext->PSSetSamplers(0, 1, _material->GetVertexShader()->GetAddressOfSampleState());

		// 입력 배치 객체 셋팅
		deviceContext->IASetInputLayout(_material->GetVertexShader()->GetInputLayout());
		deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		// 상수 버퍼 세팅
		{
			// 버텍스 쉐이더
			D3D11_MAPPED_SUBRESOURCE mappedResource;
			HR(deviceContext->Map(_material->GetVertexShader()->GetMatrixBuffer(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource));

			MatrixBufferType* matrixBufferDataPtr = (MatrixBufferType*)mappedResource.pData;

			// DX에서 HLSL 로 넘어갈때 자동으로 전치가 되서 넘어간다.
			// HLSL 에서도 Row Major 하게 작성하고 싶으므로 미리 전치를 시켜놓는다.
			// 총 전치가 2번되므로 HLSL에서도 Row Major한 Matrix로 사용한다.

			DirectX::XMVECTOR det = DirectX::XMMatrixDeterminant(_worldTM);
			DirectX::XMMATRIX worldInverse = DirectX::XMMatrixInverse(&det, _worldTM);

			DirectX::XMMATRIX w = DirectX::XMMatrixTranspose(_worldTM);
			DirectX::XMMATRIX wi = DirectX::XMMatrixTranspose(worldInverse);
			DirectX::XMMATRIX v = DirectX::XMMatrixTranspose(view);
			DirectX::XMMATRIX p = DirectX::XMMatrixTranspose(proj);

			matrixBufferDataPtr->world = w;
			matrixBufferDataPtr->worldInverse = wi;
			matrixBufferDataPtr->view = v;
			matrixBufferDataPtr->projection = p;

			deviceContext->Unmap(_material->GetVertexShader()->GetMatrixBuffer(), 0);

			unsigned int bufferNumber = 0;

			deviceContext->VSSetConstantBuffers(bufferNumber, 1, _material->GetVertexShader()->GetAddressOfMatrixBuffer());

			///
			HR(deviceContext->Map(_model->nodeBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource));

			NodeBufferType* nodeBufferDataPtr = (NodeBufferType*)mappedResource.pData;

			SetNodeBuffer(_model->rootNode, nodeBufferDataPtr);
			
			deviceContext->Unmap(_model->nodeBuffer.Get(), 0);

			bufferNumber = 2;

			deviceContext->VSSetConstantBuffers(bufferNumber, 1, _model->nodeBuffer.GetAddressOf());
			///

			// 픽셀 쉐이더
			HR(deviceContext->Map(_material->GetPixelShader()->GetLightBuffer(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource));
		
			LightBufferType* lightBufferDataPtr = (LightBufferType*)mappedResource.pData;

			// TODO : LightDirection이 왜 Y축과 Z축이 바뀌어서 나오지? 해결해야한다.
			lightBufferDataPtr->ambientColor = { 0.15f,0.15f,0.15f,1.0f };
			lightBufferDataPtr->diffuseColor = { 1.0f,1.0f,1.0f,1.0f };
			lightBufferDataPtr->lightDirection = { 1.0f,0.0f,0.0f };
			lightBufferDataPtr->specularPower = 2.0f;
			lightBufferDataPtr->specularColor = { 0.5f,0.5f,0.5f,1.0f };

			deviceContext->Unmap(_material->GetPixelShader()->GetLightBuffer(), 0);

			bufferNumber = 0;

			deviceContext->PSSetConstantBuffers(bufferNumber, 1, _material->GetPixelShader()->GetAddressOfLightBuffer());
		}

		// 렌더스테이트
		deviceContext->RSSetState(_material->GetRenderState());


		/// 그린다
		// 인덱스버퍼와 버텍스버퍼 셋팅
		UINT stride = 0;
		UINT offset = 0;

// 		if(_meshes == nullptr)
// 		{
// 			return;
// 		}

		stride = sizeof(Vertex);

		for (auto& mesh : _model->meshes)
		{
// 			switch (mesh->GetVertexType())
// 			{
// 				case eVertexType::COLOR_VERTEX:
// 					stride = sizeof(ColorVertex);
// 					break;
// 				case eVertexType::TEXTURE_VERTEX:
// 					stride = sizeof(TextureVertex);
// 					break;
// 				case eVertexType::LIGHT_VERTEX:
// 					stride = sizeof(LightVertex);
// 					break;
// 				case eVertexType::VERTEX:
// 					stride = sizeof(Vertex);
// 					break;
// 				default:
// 					break;
// 			}

			deviceContext->IASetVertexBuffers(0, 1, mesh->GetAddressOfVertexBuffer(), &stride, &offset);
			deviceContext->IASetIndexBuffer(mesh->GetIndexBuffer(), DXGI_FORMAT_R32_UINT, 0);

			deviceContext->PSSetShaderResources(0, 1, _material->GetTexture()->GetAddressOfTextureView());

			deviceContext->DrawIndexed(mesh->GetIndexCount(), 0, 0);
		}
	}

	void StaticModelRenderer::SetVertexShader(VertexShader* shader)
	{
		assert(_material);
		_material->SetVertexShader(shader);
	}

	void StaticModelRenderer::SetPixelShader(PixelShader* shader)
	{
		assert(_material);
		_material->SetPixelShader(shader);
	}

	void StaticModelRenderer::SetRenderState(ID3D11RasterizerState* renderState)
	{
		assert(_material);
		_material->SetRenderState(renderState);
	}

	void StaticModelRenderer::SetNodeBuffer(Node* node, NodeBufferType* nodeBuffer)
	{
		// DX에서 HLSL 로 넘어갈때 자동으로 전치가 되서 넘어간다.
		// HLSL 에서도 Row Major 하게 작성하고 싶으므로 미리 전치를 시켜놓는다.
		// 총 전치가 2번되므로 HLSL에서도 Row Major한 Matrix로 사용한다.
		nodeBuffer->transformMatrix[node->index] = DirectX::XMMatrixTranspose(node->GetWorldMatrix());
		for (int i = 0; i < node->children.size(); i++)
		{
			SetNodeBuffer(node->children[i], nodeBuffer);
		}
	}



}
