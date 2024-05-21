#include "StaticModelRenderer.h"
#include "GraphicsMacro.h"
#include "VertexStruct.h"
#include "ResourceManager.h"
#include "ObjectManager.h"
#include "StaticMesh.h"
#include "Camera.h"
#include "DirectionalLight.h"
#include "../RocketCommon/RocketTransform.h"

namespace Rocket::Core
{
	StaticModelRenderer::StaticModelRenderer()
		: _model(nullptr)
		, _material(nullptr)
		, _isActive(true)
		, _worldTM(Matrix::Identity)
		, _boundingBox()
	{

	}

	StaticModelRenderer::~StaticModelRenderer()
	{
		// TODO : 원래는 ResourceManager에서 unique_ptr로 관리하고 있어야 하는데, 지금은 ObjectManager에서 생성할때 new Material()해서 받아온 것임.
		//			ResourceManager에서 받아오도록 수정해야함.
		delete _material;
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
		if (_model == nullptr)
		{
			MessageBox(NULL, TEXT("스태틱 모델이 없습니다."), TEXT("스태틱 모델 로드 실패"), MB_OK);
			return;
		}

		// _model = reinterpret_cast<StaticModel*>(ResourceManager::Instance().GetModel(fileName)); 원래는 모델기준으로 그렸었음.

		// BoundingBox 생성
		std::vector<DirectX::XMFLOAT3> points;

		std::vector<Node*> nodes;
		nodes.push_back(nullptr);
		MakeNodeVector(_model->rootNode, &nodes);

		for (auto& mesh : _model->meshes)
		{
			for (auto& vertex : mesh->GetVertices())
			{
				Vector3 temp = vertex.position;
				temp = DirectX::XMVector3Transform(temp, nodes[vertex.nodeIndex]->GetWorldMatrix());
				points.push_back(temp);
			}
		}

		DirectX::BoundingBox::CreateFromPoints(_boundingBox, points.size(), points.data(), sizeof(DirectX::XMFLOAT3));
	}

	void StaticModelRenderer::LoadBaseColorTexture(std::string fileName)
	{
		_material->SetBaseColorTexture(ResourceManager::Instance().GetTexture(fileName));
	}

	// TODO : 이거 분리할 필요가 있을거같은데.. 셰이더 데이터 따로, 메쉬데이터 따로, 그리는거 따로.. 아닌가.. 그냥 함수를 따로 만들까..
	void StaticModelRenderer::Render(ID3D11DeviceContext* deviceContext, const DirectX::XMMATRIX& view, const DirectX::XMMATRIX& proj)
	{
		if (!_isActive)
		{
			return;
		}

		// Shader deviceContext 이용해 연결.
		deviceContext->VSSetShader(_material->GetVertexShader()->GetVertexShader(), nullptr, 0);
		deviceContext->PSSetShader(_material->GetPixelShader()->GetPixelShader(), nullptr, 0);

		deviceContext->PSSetSamplers(0, 1, _material->GetPixelShader()->GetAddressOfSampleState());

		// 입력 배치 객체 셋팅
		deviceContext->IASetInputLayout(_material->GetVertexShader()->GetInputLayout());
		deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		// 상수 버퍼 세팅
		{
			// 버텍스 쉐이더
			unsigned int bufferNumber = 0;

			D3D11_MAPPED_SUBRESOURCE mappedResource;
			HR(deviceContext->Map(_material->GetVertexShader()->GetConstantBuffer(bufferNumber), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource));

			MatrixBufferType* matrixBufferDataPtr = (MatrixBufferType*)mappedResource.pData;

			// DX에서 HLSL 로 넘어갈때 자동으로 전치가 되서 넘어간다.
			// HLSL 에서도 Row Major 하게 작성하고 싶으므로 미리 전치를 시켜놓는다.
			// 총 전치가 2번되므로 HLSL에서도 Row Major한 Matrix로 사용한다.

			DirectX::XMVECTOR det = DirectX::XMMatrixDeterminant(_transform->GetWorldTM());
			DirectX::XMMATRIX worldInverse = DirectX::XMMatrixInverse(&det, _transform->GetWorldTM());

			DirectX::XMMATRIX w = DirectX::XMMatrixTranspose(_transform->GetWorldTM());
			DirectX::XMMATRIX wi = DirectX::XMMatrixTranspose(worldInverse);
			DirectX::XMMATRIX v = DirectX::XMMatrixTranspose(view);
			DirectX::XMMATRIX p = DirectX::XMMatrixTranspose(proj);

			matrixBufferDataPtr->world = w;
			matrixBufferDataPtr->worldInverse = wi;
			matrixBufferDataPtr->view = v;
			matrixBufferDataPtr->projection = p;

			deviceContext->Unmap(_material->GetVertexShader()->GetConstantBuffer(bufferNumber), 0);
			deviceContext->VSSetConstantBuffers(bufferNumber, 1, _material->GetVertexShader()->GetAddressOfConstantBuffer(bufferNumber));

			// NodeTransform 세팅
			bufferNumber = 1;
			HR(deviceContext->Map(_material->GetVertexShader()->GetConstantBuffer(bufferNumber), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource));
			NodeBufferType* nodeTRBufferDataPtr = (NodeBufferType*)mappedResource.pData;

			SetNodeBuffer(_model->rootNode, nodeTRBufferDataPtr);

			deviceContext->Unmap(_material->GetVertexShader()->GetConstantBuffer(bufferNumber), 0);
			deviceContext->VSSetConstantBuffers(bufferNumber, 1, _material->GetVertexShader()->GetAddressOfConstantBuffer(bufferNumber));

			/// 픽셀 쉐이더
			// PBR Data를 넘겨준다.
			{
				bufferNumber = 0;

				HR(deviceContext->Map(_material->GetPixelShader()->GetConstantBuffer(bufferNumber), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource));

				PBRBufferType* pbrBufferData = (PBRBufferType*)mappedResource.pData;

				pbrBufferData->metallic = _material->GetMetallic();
				pbrBufferData->roughness = _material->GetRoughness();
				pbrBufferData->useNormalMap = false;
				pbrBufferData->useMetallicMap = false;
				pbrBufferData->useRoughnessMap = false;
				pbrBufferData->useAOMap = false;

				if (_material->GetNormalTexture())
				{
					pbrBufferData->useNormalMap = true;
				}

				if (_material->GetMetallicTexture())
				{
					pbrBufferData->useMetallicMap = true;
				}

				if (_material->GetRoughnessTexture())
				{
					pbrBufferData->useRoughnessMap = true;
				}

				if (_material->GetAOTexture())
				{
					pbrBufferData->useAOMap = true;
				}

				deviceContext->Unmap(_material->GetPixelShader()->GetConstantBuffer(bufferNumber), 0);

				deviceContext->PSSetConstantBuffers(bufferNumber, 1, _material->GetPixelShader()->GetAddressOfConstantBuffer(bufferNumber));
			}
		}

		// 렌더스테이트
		deviceContext->RSSetState(_material->GetRenderState());

		/// 그린다
		// 인덱스버퍼와 버텍스버퍼 셋팅
		UINT stride = 0;
		UINT offset = 0;

		stride = sizeof(Vertex);

		/// Model 기반으로 그릴 때.

		for (auto& mesh : _model->meshes)
		{
			deviceContext->IASetVertexBuffers(0, 1, mesh->GetAddressOfVertexBuffer(), &stride, &offset);
			deviceContext->IASetIndexBuffer(mesh->GetIndexBuffer(), DXGI_FORMAT_R32_UINT, 0);

			if (_material->GetBaseColorTexture())
			{
				deviceContext->PSSetShaderResources(0, 1, _material->GetBaseColorTexture()->GetAddressOfSRV());
			}
			if (_material->GetNormalTexture())
			{
				deviceContext->PSSetShaderResources(1, 1, _material->GetNormalTexture()->GetAddressOfSRV());
			}
			if (_material->GetMetallicTexture())
			{
				deviceContext->PSSetShaderResources(2, 1, _material->GetMetallicTexture()->GetAddressOfSRV());
			}
			if (_material->GetRoughnessTexture())
			{
				deviceContext->PSSetShaderResources(3, 1, _material->GetRoughnessTexture()->GetAddressOfSRV());
			}
			if (_material->GetAOTexture())
			{
				deviceContext->PSSetShaderResources(4, 1, _material->GetAOTexture()->GetAddressOfSRV());
			}

			deviceContext->DrawIndexed(mesh->GetIndexCount(), 0, 0);
		}

		ComPtr<ID3D11ShaderResourceView> nullSRV = nullptr;
		deviceContext->PSSetShaderResources(0, 1, nullSRV.GetAddressOf());
		deviceContext->PSSetShaderResources(1, 1, nullSRV.GetAddressOf());
		deviceContext->PSSetShaderResources(2, 1, nullSRV.GetAddressOf());
		deviceContext->PSSetShaderResources(3, 1, nullSRV.GetAddressOf());
		deviceContext->PSSetShaderResources(4, 1, nullSRV.GetAddressOf());
	}

	// return : 사용하던 VertexShader
	Rocket::Core::VertexShader* StaticModelRenderer::SetVertexShader(VertexShader* shader)
	{
		assert(_material);
		auto temp = _material->GetVertexShader();
		_material->SetVertexShader(shader);
		return temp;
	}

	// return : 사용하던 PixelShader
	Rocket::Core::PixelShader* StaticModelRenderer::SetPixelShader(PixelShader* shader)
	{
		assert(_material);
		auto temp = _material->GetPixelShader();
		_material->SetPixelShader(shader);
		return temp;
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
		if (node->index > 0)
		{
			nodeBuffer->transformMatrix[node->index] = DirectX::XMMatrixTranspose(node->GetWorldMatrix());
		}

		for (int i = 0; i < node->children.size(); i++)
		{
			SetNodeBuffer(node->children[i], nodeBuffer);
		}
	}

	void StaticModelRenderer::BindTransform(RocketTransform* transform)
	{
		_transform = transform;
	}

	DirectX::BoundingBox StaticModelRenderer::GetBoundingBox() const
	{
		// WorldTM을 곱한 다음에 내보낸다.
		DirectX::BoundingBox transformedBox;
		_boundingBox.Transform(transformedBox, _transform->GetWorldTM());
		return transformedBox;
	}

	void StaticModelRenderer::SetMetallic(float metallic)
	{
		_material->SetMetallic(metallic);
	}

	void StaticModelRenderer::SetRoughness(float roughness)
	{
		_material->SetRoughness(roughness);
	}

	void StaticModelRenderer::LoadNormalTexture(std::string fileName)
	{
		_material->SetNormalTexture(ResourceManager::Instance().GetTexture(fileName));
	}

	void StaticModelRenderer::LoadMetallicTexture(std::string fileName)
	{
		_material->SetMetallicTexture(ResourceManager::Instance().GetTexture(fileName));
	}

	void StaticModelRenderer::LoadRoughnessTexture(std::string fileName)
	{
		_material->SetRoughnessTexture(ResourceManager::Instance().GetTexture(fileName));
	}

	void StaticModelRenderer::LoadAOTexture(std::string fileName)
	{
		_material->SetAmbientOcclusionTexture(ResourceManager::Instance().GetTexture(fileName));
	}

	void StaticModelRenderer::RenderShadowMap(ID3D11DeviceContext* deviceContext, const DirectX::XMMATRIX& view, const DirectX::XMMATRIX& proj, VertexShader* vs, PixelShader* ps)
	{
		if (!_isActive)
		{
			return;
		}

		// Shader deviceContext 이용해 연결.
		deviceContext->VSSetShader(vs->GetVertexShader(), nullptr, 0);
		//deviceContext->PSSetShader(ps->GetPixelShader(), nullptr, 0);
		deviceContext->PSSetShader(NULL, NULL, 0);

		// 입력 배치 객체 셋팅
		deviceContext->IASetInputLayout(vs->GetInputLayout());
		deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		// 상수 버퍼 세팅
		{
			/// 버텍스 쉐이더
			unsigned int bufferNumber = 0;

			D3D11_MAPPED_SUBRESOURCE mappedResource;
			HR(deviceContext->Map(vs->GetConstantBuffer(bufferNumber), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource));

			MatrixBufferType* matrixBufferDataPtr = (MatrixBufferType*)mappedResource.pData;

			// DX에서 HLSL 로 넘어갈때 자동으로 전치가 되서 넘어간다.
			// HLSL 에서도 Row Major 하게 작성하고 싶으므로 미리 전치를 시켜놓는다.
			// 총 전치가 2번되므로 HLSL에서도 Row Major한 Matrix로 사용한다.

			DirectX::XMVECTOR det = DirectX::XMMatrixDeterminant(_transform->GetWorldTM());
			DirectX::XMMATRIX worldInverse = DirectX::XMMatrixInverse(&det, _transform->GetWorldTM());

			DirectX::XMMATRIX w = DirectX::XMMatrixTranspose(_transform->GetWorldTM());
			DirectX::XMMATRIX wi = DirectX::XMMatrixTranspose(worldInverse);
			DirectX::XMMATRIX v = DirectX::XMMatrixTranspose(view);
			DirectX::XMMATRIX p = DirectX::XMMatrixTranspose(proj);

			matrixBufferDataPtr->world = w;
			matrixBufferDataPtr->worldInverse = wi;
			matrixBufferDataPtr->view = v;
			matrixBufferDataPtr->projection = p;

			deviceContext->Unmap(vs->GetConstantBuffer(bufferNumber), 0);
			deviceContext->VSSetConstantBuffers(bufferNumber, 1, vs->GetAddressOfConstantBuffer(bufferNumber));

			// NodeTransform 세팅
			bufferNumber = 1;
			HR(deviceContext->Map(_material->GetVertexShader()->GetConstantBuffer(bufferNumber), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource));
			NodeBufferType* nodeTRBufferDataPtr = (NodeBufferType*)mappedResource.pData;

			SetNodeBuffer(_model->rootNode, nodeTRBufferDataPtr);

			deviceContext->Unmap(_material->GetVertexShader()->GetConstantBuffer(bufferNumber), 0);
			deviceContext->VSSetConstantBuffers(bufferNumber, 1, _material->GetVertexShader()->GetAddressOfConstantBuffer(bufferNumber));
		}

		// 렌더스테이트 ShadowMap용으로 사용
		deviceContext->RSSetState(ResourceManager::Instance().GetRenderState(ResourceManager::eRenderState::SHADOWMAP));

		/// 그린다
		// 인덱스버퍼와 버텍스버퍼 셋팅
		UINT stride = 0;
		UINT offset = 0;

		stride = sizeof(Vertex);

		/// Model 기반으로 그릴 때.

		for (auto& mesh : _model->meshes)
		{
			deviceContext->IASetVertexBuffers(0, 1, mesh->GetAddressOfVertexBuffer(), &stride, &offset);
			deviceContext->IASetIndexBuffer(mesh->GetIndexBuffer(), DXGI_FORMAT_R32_UINT, 0);

			if (_material->GetBaseColorTexture())
			{
				deviceContext->PSSetShaderResources(0, 1, _material->GetBaseColorTexture()->GetAddressOfSRV());
			}
			if (_material->GetNormalTexture())
			{
				deviceContext->PSSetShaderResources(1, 1, _material->GetNormalTexture()->GetAddressOfSRV());
			}
			if (_material->GetMetallicTexture())
			{
				deviceContext->PSSetShaderResources(2, 1, _material->GetMetallicTexture()->GetAddressOfSRV());
			}
			if (_material->GetRoughnessTexture())
			{
				deviceContext->PSSetShaderResources(3, 1, _material->GetRoughnessTexture()->GetAddressOfSRV());
			}
			if (_material->GetAOTexture())
			{
				deviceContext->PSSetShaderResources(4, 1, _material->GetAOTexture()->GetAddressOfSRV());
			}

			deviceContext->DrawIndexed(mesh->GetIndexCount(), 0, 0);
		}
	}

	void StaticModelRenderer::MakeNodeVector(Node* node, std::vector<Node*>* vec)
	{
		if (node->index > 0)
		{
			vec->push_back(node);
		}

		for (int i = 0; i < node->children.size(); i++)
		{
			MakeNodeVector(node->children[i], vec);
		}
	}

}
