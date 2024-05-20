#include "CubeMap.h"
#include "ResourceManager.h"
#include "VertexStruct.h"
#include "GraphicsMacro.h"
#include "Texture.h"
#include "VertexShader.h"
#include "PixelShader.h"
#include "Camera.h"
#include "MathHeader.h"

namespace Rocket::Core
{
	constexpr float IRRADIANCEMAPSIZE = 128.0f;
	constexpr float PREFILTEREDMAPSIZE = 512.0f;
	constexpr float BRDF2DLUTSIZE = 512.0f;


	CubeMap::CubeMap()
		:_cubeMapTexture(),
		_vertexBuffer(),
		_indexBuffer(),
		_cubeMapRenderState(),
		_samplerState(),
		_vertexCount(),
		_indexCount(),
		_cubeMapVS(),
		_cubeMapPS()
	{

	}

	CubeMap::~CubeMap()
	{
		_vertexBuffer.Reset();
		_indexBuffer.Reset();
		_cubeMapRenderState.Reset();
		_samplerState.Reset();
	}

	void CubeMap::Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext)
	{
		_device = device;
		_deviceContext = deviceContext;

		BuildGeometryBuffers(device);

		D3D11_RASTERIZER_DESC cubeMapDesc;
		ZeroMemory(&cubeMapDesc, sizeof(D3D11_RASTERIZER_DESC));
		cubeMapDesc.FillMode = D3D11_FILL_SOLID;
		cubeMapDesc.CullMode = D3D11_CULL_NONE;
		cubeMapDesc.FrontCounterClockwise = false;
		cubeMapDesc.DepthClipEnable = true;
		HR(device->CreateRasterizerState(&cubeMapDesc, _cubeMapRenderState.GetAddressOf()));

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

		_cubeMapRenderState->SetPrivateData(WKPDID_D3DDebugObjectNameW, sizeof(L"cubeMapRenderState") - 1, L"cubeMapRenderState");
		_samplerState->SetPrivateData(WKPDID_D3DDebugObjectNameW, sizeof(L"CubeMapsamplerState") - 1, L"CubeMapsamplerState");
	}

	void CubeMap::Render(ID3D11DeviceContext* deviceContext)
	{
		Camera* mainCam = Camera::GetMainCamera();

		deviceContext->IASetInputLayout(_cubeMapVS->GetInputLayout());
		deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		deviceContext->RSSetState(_cubeMapRenderState.Get());

		// Shader deviceContext 이용해 연결.
		deviceContext->VSSetShader(_cubeMapVS->GetVertexShader(), nullptr, 0);
		deviceContext->PSSetShader(_cubeMapPS->GetPixelShader(), nullptr, 0);

		// 버텍스 쉐이더
		{
			unsigned int bufferNumber = 0;

			D3D11_MAPPED_SUBRESOURCE mappedResource;
			HR(deviceContext->Map(_cubeMapVS->GetConstantBuffer(bufferNumber), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource));

			CubeMapBufferType* matrixBufferDataPtr = (CubeMapBufferType*)mappedResource.pData;

			DirectX::XMMATRIX v = DirectX::XMMatrixTranspose(mainCam->GetViewMatrix());
			DirectX::XMMATRIX p = DirectX::XMMatrixTranspose(mainCam->GetProjectionMatrix());

			matrixBufferDataPtr->view = v;
			matrixBufferDataPtr->projection = p;

			deviceContext->Unmap(_cubeMapVS->GetConstantBuffer(bufferNumber), 0);

			deviceContext->VSSetConstantBuffers(bufferNumber, 1, _cubeMapVS->GetAddressOfConstantBuffer(bufferNumber));
		}

		// 픽셀 쉐이더 세팅
		{
			// pixelShader->SetShaderResourceView("Texture", m_material->GetAlbedoMap()); 아래로 대체.
			//deviceContext->PSSetShaderResources(0, 1, _cubeMapTexture->GetAddressOfSRV());
			deviceContext->PSSetShaderResources(0, 1, _cubeMapTexture->GetAddressOfSRV());
			deviceContext->PSSetSamplers(0, 1, _samplerState.GetAddressOf());
		}

		// 인덱스버퍼와 버텍스버퍼 셋팅
		UINT stride = 0;
		UINT offset = 0;

		stride = sizeof(CubeMapVertex);

		deviceContext->IASetVertexBuffers(0, 1, _vertexBuffer.GetAddressOf(), &stride, &offset);
		deviceContext->IASetIndexBuffer(_indexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);

		deviceContext->DrawIndexed(_indexCount, 0, 0);

		ComPtr<ID3D11ShaderResourceView> nullSRV = nullptr;
		deviceContext->PSSetShaderResources(0, 1, nullSRV.GetAddressOf());
	}

	void CubeMap::LoadTexture(const std::string& fileName)
	{
		//_texture = ResourceManager::Instance().GetTexture(fileName);

		// IBL Baker를 통해 미리 구워둔 IBL 텍스쳐를 불러온다.
		// TODO : 미리 굽지 않은경우 만들어내야할텐데..
		// 아니면 IBL Baker 코드를 갖고와서 여기서 생성하도록 해도 될듯..?
		std::string pureName = fileName.substr(0, fileName.find_last_of("."));
		std::string extension = fileName.substr(fileName.find_last_of(".") + 1);
		//_cubeMapTexture = ResourceManager::Instance().GetTexture(fileName);
		_cubeMapTexture = ResourceManager::Instance().GetTexture(pureName + "EnvHDR" + "." + extension);

		//_irradianceTexture = std::make_unique<Texture>();

		// TODO : 아래 텍스쳐들 hlsl을 통해 직접 만들기.
		_irradianceTexture = ResourceManager::Instance().GetTexture(pureName + "DiffuseHDR" + "." + extension);
		_prefilteredTexture = ResourceManager::Instance().GetTexture(pureName + "SpecularHDR" + "." + extension);
		_BRDF2DLUTTexture = ResourceManager::Instance().GetTexture(pureName + "Brdf" + "." + extension);

		//GenerateIBLTextures();
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

		_vertexBuffer->SetPrivateData(WKPDID_D3DDebugObjectNameW, sizeof(L"CubeMapvertexBuffer") - 1, L"CubeMapvertexBuffer");
		_indexBuffer->SetPrivateData(WKPDID_D3DDebugObjectNameW, sizeof(L"CubeMapindexBuffer") - 1, L"CubeMapindexBuffer");
	}

	void CubeMap::SetShader(VertexShader* vertexShader, PixelShader* pixelShader)
	{
		_cubeMapVS = vertexShader;
		_cubeMapPS = pixelShader;
	}

	ID3D11ShaderResourceView** CubeMap::GetIrradianceTextureSRV()
	{
		return _irradianceTexture->GetAddressOfSRV();
	}

	ID3D11ShaderResourceView** CubeMap::GetPrefilteredTextureSRV()
	{
		return _prefilteredTexture->GetAddressOfSRV();
	}

	ID3D11ShaderResourceView** CubeMap::GetBRDF2DLUTTextureSRV()
	{
		return _BRDF2DLUTTexture->GetAddressOfSRV();
	}

	void CubeMap::GenerateIBLTextures()
	{
		DirectX::XMVECTOR forward[6] =
		{
			{ 1, 0, 0, 0 },
			{ -1, 0, 0, 0 },
			{ 0, 1, 0, 0 },
			{ 0, -1, 0, 0 },
			{ 0, 0, 1, 0 },
			{ 0, 0, -1, 0 },
		};

		DirectX::XMVECTOR up[6] =
		{
			{ 0, 1, 0, 0 },
			{ 0, 1, 0, 0 },
			{ 0, 0, -1, 0 },
			{ 0, 0, 1, 0 },
			{ 0, 1, 0,  0 },
			{ 0, 1, 0, 0 },
		};

		/// Generate Irradiance Map
		{
			D3D11_TEXTURE2D_DESC textureDesc;
			ZeroMemory(&textureDesc, sizeof(textureDesc));
			textureDesc.Width = IRRADIANCEMAPSIZE;
			textureDesc.Height = IRRADIANCEMAPSIZE;
			textureDesc.MipLevels = 1;
			textureDesc.ArraySize = 6;	// 6 faces
			textureDesc.Format = DXGI_FORMAT_R16G16B16A16_FLOAT;
			textureDesc.SampleDesc.Count = 1;
			textureDesc.Usage = D3D11_USAGE_DEFAULT;
			textureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
			textureDesc.CPUAccessFlags = 0;
			textureDesc.MiscFlags = D3D11_RESOURCE_MISC_TEXTURECUBE;

			ComPtr<ID3D11Texture2D> texture;
			HR(_device->CreateTexture2D(&textureDesc, nullptr, texture.GetAddressOf()));

			D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
			srvDesc.Format = DXGI_FORMAT_R16G16B16A16_FLOAT;
			srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBE;
			srvDesc.TextureCube.MostDetailedMip = 0;
			srvDesc.TextureCube.MipLevels = 1;

			HR(_device->CreateShaderResourceView(texture.Get(), &srvDesc, _irradianceTexture->GetAddressOfSRV()));

			ComPtr<ID3D11RenderTargetView> rtvArr[6];

			D3D11_RENDER_TARGET_VIEW_DESC rtvDesc;
			rtvDesc.Format = DXGI_FORMAT_R16G16B16A16_FLOAT;
			rtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2DARRAY;
			rtvDesc.Texture2DArray.MipSlice = 0;
			rtvDesc.Texture2DArray.ArraySize = 1;

			for (UINT i = 0; i < 6; ++i)
			{
				rtvDesc.Texture2DArray.FirstArraySlice = i;
				ID3D11RenderTargetView* rtv;
				HR(_device->CreateRenderTargetView(texture.Get(), &rtvDesc, &rtv));
				rtvArr[i] = rtv;				
			}

			_deviceContext->IASetInputLayout(_cubeMapVS->GetInputLayout());
			_deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
			_deviceContext->RSSetState(_cubeMapRenderState.Get());

			// Shader deviceContext 이용해 연결.
			_deviceContext->VSSetShader(_cubeMapVS->GetVertexShader(), nullptr, 0);
			_deviceContext->PSSetShader(_irradianceMapPS->GetPixelShader(), nullptr, 0);

			for (int i = 0; i < 6; ++i)
			{

				_deviceContext->OMSetRenderTargets(1, rtvArr[i].GetAddressOf(), nullptr);
				DirectX::XMVECTOR eyePos{ 0, 0, 0, 0 };
				DirectX::XMVECTOR lookAt = forward[i];
				DirectX::XMVECTOR upVec = up[i];
				float nearZ = 0.0f;
				float farZ = 10.0f;
				float viewWidth = 2.0f;
				float viewHeight = 2.0f;

				// 버텍스 쉐이더
				{
					unsigned int bufferNumber = 0;

					D3D11_MAPPED_SUBRESOURCE mappedResource;
					HR(_deviceContext->Map(_cubeMapVS->GetConstantBuffer(bufferNumber), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource));

					CubeMapBufferType* matrixBufferDataPtr = (CubeMapBufferType*)mappedResource.pData;

					DirectX::XMMATRIX viewMatrix = DirectX::XMMatrixLookAtLH(eyePos, lookAt, upVec);
					DirectX::XMMATRIX projMatrix = DirectX::XMMatrixOrthographicLH(viewWidth, viewHeight, nearZ, farZ);

					matrixBufferDataPtr->view = viewMatrix;
					matrixBufferDataPtr->projection = projMatrix;

					_deviceContext->Unmap(_cubeMapVS->GetConstantBuffer(bufferNumber), 0);
					_deviceContext->VSSetConstantBuffers(bufferNumber, 1, _cubeMapVS->GetAddressOfConstantBuffer(bufferNumber));
				}
				
				// 픽셀 쉐이더 세팅
				{
					_deviceContext->PSSetShaderResources(0, 1, _cubeMapTexture->GetAddressOfSRV());
					_deviceContext->PSSetSamplers(0, 1, _samplerState.GetAddressOf());
				}

				// 인덱스버퍼와 버텍스버퍼 셋팅
				UINT stride = 0;
				UINT offset = 0;

				stride = sizeof(CubeMapVertex);

				_deviceContext->IASetVertexBuffers(0, 1, _vertexBuffer.GetAddressOf(), &stride, &offset);
				_deviceContext->IASetIndexBuffer(_indexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);

				_deviceContext->DrawIndexed(_indexCount, 0, 0);
			}

			_irradianceTexture->SetResource(texture.Get());

			ComPtr<ID3D11ShaderResourceView> nullSRV = nullptr;
			_deviceContext->PSSetShaderResources(0, 1, nullSRV.GetAddressOf());
			ComPtr<ID3D11RenderTargetView> nullRTV = nullptr;
			_deviceContext->OMSetRenderTargets(1, nullRTV.GetAddressOf(), nullptr);
		}

		/// Generate Prefiltered Map
		{

		}

		/// Generate BRDF 2D LUT
		{

		}
	}

	void CubeMap::SetIBLGenShader(PixelShader* irradianceMapPS, PixelShader* prefilteredMapPS, PixelShader* BRDF2DLUTPS)
	{
		_irradianceMapPS = irradianceMapPS;
		_prefilteredMapPS = prefilteredMapPS;
		_BRDF2DLUTPS = BRDF2DLUTPS;
	}

}
