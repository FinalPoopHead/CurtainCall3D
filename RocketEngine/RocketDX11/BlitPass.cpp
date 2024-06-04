#include "BlitPass.h"
#include "VertexShader.h"
#include "PixelShader.h"
#include "GraphicsMacro.h"
#include "DeferredBuffers.h"
#include "ObjectManager.h"
#include "DirectionalLight.h"
#include "Camera.h"

namespace Rocket::Core
{

	BlitPass::BlitPass()
	{

	}

	BlitPass::~BlitPass()
	{

	}

	void BlitPass::Initialize(ID3D11Device* device, VertexShader* vertexShader, PixelShader* pixelShader)
	{
		_vertexShader = vertexShader;
		_pixelShader = pixelShader;

		CreateSamplerState(device);
	}

	void BlitPass::Render(ID3D11DeviceContext* deviceContext, ID3D11ShaderResourceView** srv)
	{
		deviceContext->IASetInputLayout(_vertexShader->GetInputLayout());
		deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

		deviceContext->VSSetShader(_vertexShader->GetVertexShader(), nullptr, 0);
		deviceContext->PSSetShader(_pixelShader->GetPixelShader(), nullptr, 0);

		deviceContext->PSSetSamplers(0, 1, _sampleState.GetAddressOf());

		deviceContext->PSSetShaderResources(0, 1, srv);

		deviceContext->Draw(4, 0);

		ComPtr<ID3D11ShaderResourceView> nullSRV = nullptr;
		deviceContext->PSSetShaderResources(0, 1, nullSRV.GetAddressOf());
	}

	void BlitPass::CreateSamplerState(ID3D11Device* device)
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
	}

}
