#include "CubeMap.h"
#include "ResourceManager.h"
#include "VertexStruct.h"
#include "GraphicsMacro.h"
#include "Texture.h"
#include "VertexShader.h"
#include "PixelShader.h"
#include "Camera.h"

namespace Rocket::Core
{

	CubeMap::CubeMap()
		:_texture(),
		_vertexBuffer(),
		_indexBuffer(),
		_cubeMapRenderState(),
		_samplerState(),
		_vertexCount(),
		_indexCount(),
		_vertexShader(),
		_pixelShader()
	{

	}

	CubeMap::~CubeMap()
	{
		_vertexBuffer.Reset();
		_indexBuffer.Reset();
		_cubeMapRenderState.Reset();
		_samplerState.Reset();
	}

	void CubeMap::Initialize(ID3D11Device* device)
	{
		BuildGeometryBuffers(device);

		D3D11_RASTERIZER_DESC cubeMapDesc;
		ZeroMemory(&cubeMapDesc, sizeof(D3D11_RASTERIZER_DESC));
		cubeMapDesc.FillMode = D3D11_FILL_SOLID;
		cubeMapDesc.CullMode = D3D11_CULL_NONE;
		cubeMapDesc.FrontCounterClockwise = false;
		cubeMapDesc.DepthClipEnable = true;
		ID3D11RasterizerState* cubemapRS;
		HR(device->CreateRasterizerState(&cubeMapDesc, &cubemapRS));
		_cubeMapRenderState = cubemapRS;

		// TODO : 이것도 얘가 리소스매니저한테 접근해서 받아오지말고 리소스매니저가 얘한테 부여하게끔 해야하나?
		_vertexShader = ResourceManager::Instance().GetVertexShader("CubeMapVS");
		_pixelShader = ResourceManager::Instance().GetPixelShader("CubeMapPS");

		/// 샘플러 만들기
		// 텍스처 샘플러 상태 구조체를 생성 및 설정한다.
		D3D11_SAMPLER_DESC samplerDesc;
		samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.MipLODBias = 0.0f;
		samplerDesc.MaxAnisotropy = 1;
		samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
		samplerDesc.BorderColor[0] = 0;
		samplerDesc.BorderColor[1] = 0;
		samplerDesc.BorderColor[2] = 0;
		samplerDesc.BorderColor[3] = 0;
		samplerDesc.MinLOD = 0;
		samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

		// 텍스처 샘플러 상태를 만듭니다.
		HR(device->CreateSamplerState(&samplerDesc, &_samplerState));
	}

	void CubeMap::Render(ID3D11DeviceContext* deviceContext)
	{
		Camera* mainCam = Camera::GetMainCamera();

		deviceContext->IASetInputLayout(_vertexShader->GetInputLayout());
		deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		deviceContext->RSSetState(_cubeMapRenderState.Get());

		// Shader deviceContext 이용해 연결.
		deviceContext->VSSetShader(_vertexShader->GetVertexShader(), nullptr, 0);
		deviceContext->PSSetShader(_pixelShader->GetPixelShader(), nullptr, 0);

		// 버텍스 쉐이더
		{
			unsigned int bufferNumber = 0;

			D3D11_MAPPED_SUBRESOURCE mappedResource;
			HR(deviceContext->Map(_vertexShader->GetConstantBuffer(bufferNumber), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource));

			CubeMapBufferType* matrixBufferDataPtr = (CubeMapBufferType*)mappedResource.pData;

			DirectX::XMMATRIX v = DirectX::XMMatrixTranspose(mainCam->GetViewMatrix());
			DirectX::XMMATRIX p = DirectX::XMMatrixTranspose(mainCam->GetProjectionMatrix());

			matrixBufferDataPtr->view = v;
			matrixBufferDataPtr->projection = p;

			deviceContext->Unmap(_vertexShader->GetConstantBuffer(bufferNumber), 0);

			deviceContext->VSSetConstantBuffers(bufferNumber, 1, _vertexShader->GetAddressOfConstantBuffer(bufferNumber));
		}

		// 픽셀 쉐이더 세팅
		{
			// pixelShader->SetShaderResourceView("Texture", m_material->GetAlbedoMap()); 아래로 대체.
			deviceContext->PSSetShaderResources(0, 1, _texture->GetAddressOfTextureView());
			deviceContext->PSSetSamplers(0, 1, _samplerState.GetAddressOf());
		}

		// 인덱스버퍼와 버텍스버퍼 셋팅
		UINT stride = 0;
		UINT offset = 0;

		stride = sizeof(CubeMapVertex);

		deviceContext->IASetVertexBuffers(0, 1, _vertexBuffer.GetAddressOf(), &stride, &offset);
		deviceContext->IASetIndexBuffer(_indexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);

		deviceContext->DrawIndexed(_indexCount, 0, 0);
	}

	void CubeMap::LoadTexture(const std::string& fileName)
	{
		_texture = ResourceManager::Instance().GetTexture(fileName);
	}

	void CubeMap::SetTexture(ID3D11Resource* texture, ID3D11ShaderResourceView* textureView)
	{
		if (_texture)
		{
			delete _texture;
			_texture = nullptr;
		}

		_texture = new Texture(texture, textureView);
	}

	void CubeMap::BuildGeometryBuffers(ID3D11Device* device)
	{
		/*
		Vertex vertices[] =
		{
			{ DirectX::XMFLOAT3(-1.0f, -1.0f, -1.0f), DirectX::XMFLOAT3(0.0f,  0.0f, -1.0f), DirectX::XMFLOAT2(0.0f, 0.0f) },
			{ DirectX::XMFLOAT3(1.0f, -1.0f, -1.0f), DirectX::XMFLOAT3(0.0f,  0.0f, -1.0f), DirectX::XMFLOAT2(1.0f, 0.0f) },
			{ DirectX::XMFLOAT3(1.0f,  1.0f, -1.0f), DirectX::XMFLOAT3(0.0f,  0.0f, -1.0f), DirectX::XMFLOAT2(1.0f, 1.0f) },
			{ DirectX::XMFLOAT3(-1.0f,  1.0f, -1.0f), DirectX::XMFLOAT3(0.0f,  0.0f, -1.0f), DirectX::XMFLOAT2(0.0f, 1.0f) },

			{ DirectX::XMFLOAT3(1.0f, -1.0f,  1.0f), DirectX::XMFLOAT3(0.0f,  0.0f,  1.0f), DirectX::XMFLOAT2(0.0f, 0.0f) },
			{ DirectX::XMFLOAT3(-1.0f, -1.0f,  1.0f), DirectX::XMFLOAT3(0.0f,  0.0f,  1.0f), DirectX::XMFLOAT2(1.0f, 0.0f) },
			{ DirectX::XMFLOAT3(-1.0f,  1.0f,  1.0f), DirectX::XMFLOAT3(0.0f,  0.0f,  1.0f), DirectX::XMFLOAT2(1.0f, 1.0f) },
			{ DirectX::XMFLOAT3(1.0f,  1.0f,  1.0f), DirectX::XMFLOAT3(0.0f,  0.0f,  1.0f), DirectX::XMFLOAT2(0.0f, 1.0f) },

			{ DirectX::XMFLOAT3(-1.0f, -1.0f,  1.0f), DirectX::XMFLOAT3(-1.0f,  0.0f,  0.0f), DirectX::XMFLOAT2(0.0f, 0.0f) },
			{ DirectX::XMFLOAT3(-1.0f, -1.0f, -1.0f), DirectX::XMFLOAT3(-1.0f,  0.0f,  0.0f), DirectX::XMFLOAT2(1.0f, 0.0f) },
			{ DirectX::XMFLOAT3(-1.0f,  1.0f, -1.0f), DirectX::XMFLOAT3(-1.0f,  0.0f,  0.0f), DirectX::XMFLOAT2(1.0f, 1.0f) },
			{ DirectX::XMFLOAT3(-1.0f,  1.0f,  1.0f), DirectX::XMFLOAT3(-1.0f,  0.0f,  0.0f), DirectX::XMFLOAT2(0.0f, 1.0f) },

			{ DirectX::XMFLOAT3(1.0f, -1.0f, -1.0f), DirectX::XMFLOAT3(1.0f,  0.0f,  0.0f), DirectX::XMFLOAT2(0.0f, 0.0f) },
			{ DirectX::XMFLOAT3(1.0f, -1.0f,  1.0f), DirectX::XMFLOAT3(1.0f,  0.0f,  0.0f), DirectX::XMFLOAT2(1.0f, 0.0f) },
			{ DirectX::XMFLOAT3(1.0f,  1.0f,  1.0f), DirectX::XMFLOAT3(1.0f,  0.0f,  0.0f), DirectX::XMFLOAT2(1.0f, 1.0f) },
			{ DirectX::XMFLOAT3(1.0f,  1.0f, -1.0f), DirectX::XMFLOAT3(1.0f,  0.0f,  0.0f), DirectX::XMFLOAT2(0.0f, 1.0f) },

			{ DirectX::XMFLOAT3(-1.0f, -1.0f, -1.0f), DirectX::XMFLOAT3(0.0f, -1.0f,  0.0f), DirectX::XMFLOAT2(0.0f, 0.0f) },
			{ DirectX::XMFLOAT3(-1.0f, -1.0f,  1.0f), DirectX::XMFLOAT3(0.0f, -1.0f,  0.0f), DirectX::XMFLOAT2(1.0f, 0.0f) },
			{ DirectX::XMFLOAT3(1.0f, -1.0f,  1.0f), DirectX::XMFLOAT3(0.0f, -1.0f,  0.0f), DirectX::XMFLOAT2(1.0f, 1.0f) },
			{ DirectX::XMFLOAT3(1.0f, -1.0f, -1.0f), DirectX::XMFLOAT3(0.0f, -1.0f,  0.0f), DirectX::XMFLOAT2(0.0f, 1.0f) },

			{ DirectX::XMFLOAT3(-1.0f,  1.0f,  1.0f), DirectX::XMFLOAT3(0.0f,  1.0f,  0.0f), DirectX::XMFLOAT2(0.0f, 0.0f) },
			{ DirectX::XMFLOAT3(-1.0f,  1.0f, -1.0f), DirectX::XMFLOAT3(0.0f,  1.0f,  0.0f), DirectX::XMFLOAT2(1.0f, 0.0f) },
			{ DirectX::XMFLOAT3(1.0f,  1.0f, -1.0f), DirectX::XMFLOAT3(0.0f,  1.0f,  0.0f), DirectX::XMFLOAT2(1.0f, 1.0f) },
			{ DirectX::XMFLOAT3(1.0f,  1.0f,  1.0f), DirectX::XMFLOAT3(0.0f,  1.0f,  0.0f), DirectX::XMFLOAT2(0.0f, 1.0f) }
		};
		*/

		CubeMapVertex vertices[] =
		{
			{ DirectX::XMFLOAT3(-1.0f, -1.0f, -1.0f)},
			{ DirectX::XMFLOAT3(1.0f, -1.0f, -1.0f)},
			{ DirectX::XMFLOAT3(1.0f,  1.0f, -1.0f)},
			{ DirectX::XMFLOAT3(-1.0f,  1.0f, -1.0f)},

			{ DirectX::XMFLOAT3(1.0f, -1.0f,  1.0f)},
			{ DirectX::XMFLOAT3(-1.0f, -1.0f,  1.0f)},
			{ DirectX::XMFLOAT3(-1.0f,  1.0f,  1.0f)},
			{ DirectX::XMFLOAT3(1.0f,  1.0f,  1.0f)},

			{ DirectX::XMFLOAT3(-1.0f, -1.0f,  1.0f)},
			{ DirectX::XMFLOAT3(-1.0f, -1.0f, -1.0f)},
			{ DirectX::XMFLOAT3(-1.0f,  1.0f, -1.0f)},
			{ DirectX::XMFLOAT3(-1.0f,  1.0f,  1.0f)},

			{ DirectX::XMFLOAT3(1.0f, -1.0f, -1.0f)},
			{ DirectX::XMFLOAT3(1.0f, -1.0f,  1.0f)},
			{ DirectX::XMFLOAT3(1.0f,  1.0f,  1.0f)},
			{ DirectX::XMFLOAT3(1.0f,  1.0f, -1.0f)},

			{ DirectX::XMFLOAT3(-1.0f, -1.0f, -1.0f)},
			{ DirectX::XMFLOAT3(-1.0f, -1.0f,  1.0f)},
			{ DirectX::XMFLOAT3(1.0f, -1.0f,  1.0f)},
			{ DirectX::XMFLOAT3(1.0f, -1.0f, -1.0f)},

			{ DirectX::XMFLOAT3(-1.0f,  1.0f,  1.0f)},
			{ DirectX::XMFLOAT3(-1.0f,  1.0f, -1.0f)},
			{ DirectX::XMFLOAT3(1.0f,  1.0f, -1.0f)},
			{ DirectX::XMFLOAT3(1.0f,  1.0f,  1.0f)}
		};

		D3D11_BUFFER_DESC vbd;
		vbd.Usage = D3D11_USAGE_IMMUTABLE;
		vbd.ByteWidth = sizeof(CubeMapVertex) * ARRAYSIZE(vertices);
		vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		vbd.CPUAccessFlags = 0;
		vbd.MiscFlags = 0;
		vbd.StructureByteStride = 0;

		D3D11_SUBRESOURCE_DATA vinitData;
		vinitData.pSysMem = vertices;
		HR(device->CreateBuffer(&vbd, &vinitData, _vertexBuffer.GetAddressOf()));

		UINT indices[] =
		{
			0,  1,  2,  0,  2,  3,
			4,  5,  6,  4,  6,  7,
			8,  9, 10,  8, 10, 11,
		   12, 13, 14, 12, 14, 15,
		   16, 17, 18, 16, 18, 19,
		   20, 21, 22, 20, 22, 23
		};

		D3D11_BUFFER_DESC ibd;
		ibd.Usage = D3D11_USAGE_IMMUTABLE;
		ibd.ByteWidth = sizeof(UINT) * ARRAYSIZE(indices);
		ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
		ibd.CPUAccessFlags = 0;
		ibd.MiscFlags = 0;
		ibd.StructureByteStride = 0;
		D3D11_SUBRESOURCE_DATA iinitData;
		iinitData.pSysMem = indices;
		HR(device->CreateBuffer(&ibd, &iinitData, _indexBuffer.GetAddressOf()));

		_vertexCount = ARRAYSIZE(vertices);
		_indexCount = ARRAYSIZE(indices);
	}

}
