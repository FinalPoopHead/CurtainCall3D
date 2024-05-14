#include "LightPass.h"
#include "VertexShader.h"
#include "PixelShader.h"
#include "GraphicsMacro.h"
#include "DeferredBuffers.h"
#include "ObjectManager.h"
#include "DirectionalLight.h"
#include "Camera.h"		// 흠 Camera를 열어버리는 건 마음에 들지 않는 군.

namespace Rocket::Core
{

	LightPass::LightPass()
	{

	}

	LightPass::~LightPass()
	{

	}

	void LightPass::Render(ID3D11DeviceContext* deviceContext, DeferredBuffers* g_buffer)
	{
		deviceContext->IASetInputLayout(_vertexShader->GetInputLayout());
		deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

		deviceContext->VSSetShader(_vertexShader->GetVertexShader(), nullptr, 0);
		deviceContext->PSSetShader(_pixelShader->GetPixelShader(), nullptr, 0);

		deviceContext->PSSetSamplers(0, 1, _sampleState.GetAddressOf());
		deviceContext->PSSetSamplers(1, 1, ObjectManager::Instance().GetCubeMap()->GetSamplerState());
		deviceContext->PSSetSamplers(2, 1, _shadowMapSamplerState.GetAddressOf());

		// G-Buffer 세팅
		for (int i = 0; i < BUFFER_COUNT; i++)
		{
			deviceContext->PSSetShaderResources(i, 1, g_buffer->GetAddressOfShaderResourceView(i));
		}

			unsigned int textureNumber = BUFFER_COUNT;

		// IBL Texture 세팅
			deviceContext->PSSetShaderResources(textureNumber, 1, ObjectManager::Instance().GetCubeMap()->GetIrradianceTextureSRV());
			textureNumber++;
			deviceContext->PSSetShaderResources(textureNumber, 1, ObjectManager::Instance().GetCubeMap()->GetPrefilteredTextureSRV());
			textureNumber++;
			deviceContext->PSSetShaderResources(textureNumber, 1, ObjectManager::Instance().GetCubeMap()->GetBRDF2DLUTTextureSRV());
			textureNumber++;

		// ShadowMap 세팅
			deviceContext->PSSetShaderResources(textureNumber, 1, g_buffer->GetAddressOfShadowMapSRV());
			textureNumber++;


		/// PS 상수 버퍼 세팅
		{
			// 라이트 세팅
			D3D11_MAPPED_SUBRESOURCE mappedResource;
			unsigned int bufferNumber = 0;

			HR(deviceContext->Map(_pixelShader->GetConstantBuffer(bufferNumber), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource));

			LightPassBufferType* lightBufferDataPtr = (LightPassBufferType*)mappedResource.pData;

			// Directional Light
			for (auto& directionalLight : ObjectManager::Instance().GetDirectionalLightList())
			{
				lightBufferDataPtr->lightDirection = directionalLight->GetForward();
			}

			// TODO : 라이트가 없는경우. 임시입니다.
			if (ObjectManager::Instance().GetDirectionalLightList().size() == 0)
			{
				lightBufferDataPtr->lightDirection = { 0.0f,-1.0f,0.0f };
			}

			deviceContext->Unmap(_pixelShader->GetConstantBuffer(bufferNumber), 0);

			deviceContext->PSSetConstantBuffers(bufferNumber, 1, _pixelShader->GetAddressOfConstantBuffer(bufferNumber));
		

			// view Direction 세팅
			bufferNumber = 1;

			HR(deviceContext->Map(_pixelShader->GetConstantBuffer(bufferNumber), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource));

			CameraBufferType* cameraBufferDataPtr = (CameraBufferType*)mappedResource.pData;

			cameraBufferDataPtr->cameraPosition = Camera::GetMainCamera()->GetPosition();

			deviceContext->Unmap(_pixelShader->GetConstantBuffer(bufferNumber), 0);

			deviceContext->PSSetConstantBuffers(bufferNumber, 1, _pixelShader->GetAddressOfConstantBuffer(bufferNumber));


			// TODO : 일단 우겨넣은거임
			// Directional Light Shadow Matrix 세팅
			bufferNumber = 2;

			HR(deviceContext->Map(_pixelShader->GetConstantBuffer(bufferNumber), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource));

			ShadowBufferType* shadowBufferDataPtr = (ShadowBufferType*)mappedResource.pData;

			auto dirLight = ObjectManager::Instance().GetDirectionalLightList().front();
			Matrix VP = dirLight->GetViewMatrix() * dirLight->GetProjectionMatrix();
			VP = VP.Transpose();
			shadowBufferDataPtr->lightViewProjection = VP;

			deviceContext->Unmap(_pixelShader->GetConstantBuffer(bufferNumber), 0);

			deviceContext->PSSetConstantBuffers(bufferNumber, 1, _pixelShader->GetAddressOfConstantBuffer(bufferNumber));
		}
		 

		deviceContext->Draw(4, 0);

		ComPtr<ID3D11ShaderResourceView> nullSRV = nullptr;
		for (int i = 0; i < textureNumber; i++)
		{
			deviceContext->PSSetShaderResources(i, 1, nullSRV.GetAddressOf());
		}
	}

	void LightPass::Initialize(ID3D11Device* device, VertexShader* vertexShader, PixelShader* pixelShader)
	{
		_vertexShader = vertexShader;
		_pixelShader = pixelShader;

		CreateSamplerState(device);
	}

	void LightPass::CreateSamplerState(ID3D11Device* device)
	{
		// 텍스처 샘플러 상태 구조체를 생성 및 설정한다.
		D3D11_SAMPLER_DESC samplerDesc;
		samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
// 		samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
// 		samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
// 		samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
		samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
		samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
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
		HR(device->CreateSamplerState(&samplerDesc, _sampleState.GetAddressOf()));

		_sampleState->SetPrivateData(WKPDID_D3DDebugObjectNameW, sizeof(L"LightPassSampler") - 1, L"LightPassSampler");
	
		// ShadowMap Sampler
		D3D11_SAMPLER_DESC shadowSamplerDESC;
		shadowSamplerDESC.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		shadowSamplerDESC.AddressU = D3D11_TEXTURE_ADDRESS_BORDER;
		shadowSamplerDESC.AddressV = D3D11_TEXTURE_ADDRESS_BORDER;
		shadowSamplerDESC.AddressW = D3D11_TEXTURE_ADDRESS_BORDER;
		shadowSamplerDESC.MipLODBias = 0.0f;
		shadowSamplerDESC.MaxAnisotropy = 1;
		shadowSamplerDESC.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
		shadowSamplerDESC.BorderColor[0] = 1.0f;
		shadowSamplerDESC.BorderColor[1] = 1.0f;
		shadowSamplerDESC.BorderColor[2] = 1.0f;
		shadowSamplerDESC.BorderColor[3] = 1.0f;
		shadowSamplerDESC.MinLOD = 0;
		shadowSamplerDESC.MaxLOD = D3D11_FLOAT32_MAX;

		// 텍스처 샘플러 상태를 만듭니다.
		HR(device->CreateSamplerState(&shadowSamplerDESC, _shadowMapSamplerState.GetAddressOf()));
	}

}
