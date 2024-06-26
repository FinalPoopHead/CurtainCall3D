﻿#include "MeshRenderer.h"
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
	MeshRenderer::MeshRenderer()
		: _mesh(nullptr)
		, _material(nullptr)
		, _isActive(true)
		, _boundingBox()
	{

	}

	MeshRenderer::~MeshRenderer()
	{
		// TODO : 원래는 ResourceManager에서 unique_ptr로 관리하고 있어야 하는데, 지금은 ObjectManager에서 생성할때 new Material()해서 받아온 것임.
		//			ResourceManager에서 받아오도록 수정해야함.
		delete _material;
	}

	void MeshRenderer::SetActive(bool isActive)
	{
		_isActive = isActive;
	}


	void MeshRenderer::SetMesh(eMeshType meshType)
	{
		_mesh = dynamic_cast<StaticMesh*>(ResourceManager::Instance().GetMesh(meshType));
	}

	void MeshRenderer::LoadMesh(std::string fileName)
	{
		// TODO : reinterpret_cast를 사용하지 않는 엑세렌또한 방법을 찾아보자.
		_mesh = reinterpret_cast<StaticMesh*>(ResourceManager::Instance().GetMesh(fileName));
		if (_mesh == nullptr)
		{
			MessageBox(NULL, TEXT("메쉬가 없습니다."), TEXT("메쉬 로드 실패"), MB_OK);
			return;
		}

		// _model = reinterpret_cast<StaticModel*>(ResourceManager::Instance().GetModel(fileName)); 원래는 모델기준으로 그렸었음.

		// BoundingBox 생성
		std::vector<DirectX::XMFLOAT3> points;

		for (auto& vertex : _mesh->GetVertices())
		{
			points.push_back(vertex.position);
		}

		DirectX::BoundingBox::CreateFromPoints(_boundingBox, points.size(), points.data(), sizeof(DirectX::XMFLOAT3));
	}

	void MeshRenderer::LoadBaseColorTexture(std::string fileName)
	{
		_material->SetBaseColorTexture(ResourceManager::Instance().GetTexture(fileName));
	}

	// TODO : 이거 분리할 필요가 있을거같은데.. 셰이더 데이터 따로, 메쉬데이터 따로, 그리는거 따로.. 아닌가.. 그냥 함수를 따로 만들까..
	void MeshRenderer::Render(ID3D11DeviceContext* deviceContext, const DirectX::XMMATRIX& view, const DirectX::XMMATRIX& proj)
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

			// 카메라 버퍼 세팅
// 			{
// 				Camera* mainCam = Camera::GetMainCamera();
// 				// 버텍스 쉐이더
// 				D3D11_MAPPED_SUBRESOURCE mappedResource;
// 				HR(deviceContext->Map(mainCam->GetCameraBuffer(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource));
// 
// 				CameraBufferType* cameraBufferDataPtr = (CameraBufferType*)mappedResource.pData;
// 
// 				cameraBufferDataPtr->cameraPosition = mainCam->GetPosition();
// 				cameraBufferDataPtr->padding = 0.0f;
// 
// 				deviceContext->Unmap(mainCam->GetCameraBuffer(), 0);
// 
// 				unsigned int bufferNumber = 1;
// 
// 				deviceContext->VSSetConstantBuffers(bufferNumber, 1, mainCam->GetAddressOfCameraBuffer());
// 			}

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

				if(_material->GetNormalTexture())
				{
					pbrBufferData->useNormalMap = true;
				}

				if(_material->GetMetallicTexture())
				{
					pbrBufferData->useMetallicMap = true;
				}

				if(_material->GetRoughnessTexture())
				{
					pbrBufferData->useRoughnessMap = true;
				}

				if(_material->GetAOTexture())
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

// 		if(_meshes == nullptr)
// 		{
// 			return;
// 		}

		stride = sizeof(Vertex);

		/// Mesh 기반으로 그릴 때.

		deviceContext->IASetVertexBuffers(0, 1, _mesh->GetAddressOfVertexBuffer(), &stride, &offset);
		deviceContext->IASetIndexBuffer(_mesh->GetIndexBuffer(), DXGI_FORMAT_R32_UINT, 0);

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

		deviceContext->DrawIndexed(_mesh->GetIndexCount(), 0, 0);

		ComPtr<ID3D11ShaderResourceView> nullSRV = nullptr;
		deviceContext->PSSetShaderResources(0, 1, nullSRV.GetAddressOf());
		deviceContext->PSSetShaderResources(1, 1, nullSRV.GetAddressOf());
		deviceContext->PSSetShaderResources(2, 1, nullSRV.GetAddressOf());
		deviceContext->PSSetShaderResources(3, 1, nullSRV.GetAddressOf());
		deviceContext->PSSetShaderResources(4, 1, nullSRV.GetAddressOf());

		/// Model 기반으로 그릴 때.
		/*
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
		*/
	}

	// return : 사용하던 VertexShader
	Rocket::Core::VertexShader* MeshRenderer::SetVertexShader(VertexShader* shader)
	{
		assert(_material);
		auto temp = _material->GetVertexShader();
		_material->SetVertexShader(shader);
		return temp;
	}

	// return : 사용하던 PixelShader
	Rocket::Core::PixelShader* MeshRenderer::SetPixelShader(PixelShader* shader)
	{
		assert(_material);
		auto temp = _material->GetPixelShader();
		_material->SetPixelShader(shader);
		return temp;
	}

	void MeshRenderer::SetRenderState(ID3D11RasterizerState* renderState)
	{
		assert(_material);
		_material->SetRenderState(renderState);
	}

	void MeshRenderer::SetNodeBuffer(Node* node, NodeBufferType* nodeBuffer)
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

	void MeshRenderer::BindTransform(RocketTransform* transform)
	{
		_transform = transform;
	}

	DirectX::BoundingBox MeshRenderer::GetBoundingBox() const
	{
		// WorldTM을 곱한 다음에 내보낸다.
		DirectX::BoundingBox transformedBox;
		_boundingBox.Transform(transformedBox, _transform->GetWorldTM());
		return transformedBox;
	}

	void MeshRenderer::SetMetallic(float metallic)
	{
		_material->SetMetallic(metallic);
	}

	void MeshRenderer::SetRoughness(float roughness)
	{
		_material->SetRoughness(roughness);
	}

	void MeshRenderer::LoadNormalTexture(std::string fileName)
	{
		_material->SetNormalTexture(ResourceManager::Instance().GetTexture(fileName));
	}

	void MeshRenderer::LoadMetallicTexture(std::string fileName)
	{
		_material->SetMetallicTexture(ResourceManager::Instance().GetTexture(fileName));
	}

	void MeshRenderer::LoadRoughnessTexture(std::string fileName)
	{
		_material->SetRoughnessTexture(ResourceManager::Instance().GetTexture(fileName));
	}

	void MeshRenderer::LoadAOTexture(std::string fileName)
	{
		_material->SetAmbientOcclusionTexture(ResourceManager::Instance().GetTexture(fileName));
	}

	void MeshRenderer::RenderShadowMap(ID3D11DeviceContext* deviceContext, const DirectX::XMMATRIX& view, const DirectX::XMMATRIX& proj, VertexShader* vs, PixelShader* ps)
	{
		if (!_isActive)
		{
			return;
		}

		// Shader deviceContext 이용해 연결.
		deviceContext->VSSetShader(vs->GetVertexShader(), nullptr, 0);
		//deviceContext->PSSetShader(ps->GetPixelShader(), nullptr, 0);
		deviceContext->PSSetShader(NULL, NULL, 0);

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
		}

		// 렌더스테이트 ShadowMap용으로 사용
		deviceContext->RSSetState(ResourceManager::Instance().GetRenderState(ResourceManager::eRenderState::SHADOWMAP));


		/// 그린다
		// 인덱스버퍼와 버텍스버퍼 셋팅
		UINT stride = 0;
		UINT offset = 0;

		stride = sizeof(Vertex);

		/// Mesh 기반으로 그릴 때.

		// 입력 배치 객체 셋팅
		deviceContext->IASetInputLayout(vs->GetInputLayout());
		deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		deviceContext->IASetVertexBuffers(0, 1, _mesh->GetAddressOfVertexBuffer(), &stride, &offset);
		deviceContext->IASetIndexBuffer(_mesh->GetIndexBuffer(), DXGI_FORMAT_R32_UINT, 0);

		deviceContext->DrawIndexed(_mesh->GetIndexCount(), 0, 0);
	}

	void MeshRenderer::Destroy()
	{
		ObjectManager::Instance().DestroyMeshRenderer(this);
	}

}
