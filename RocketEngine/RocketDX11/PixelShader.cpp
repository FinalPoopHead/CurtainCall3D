#include <sstream>
#include <fstream>
#include <vector>
#include <DirectXMath.h>
#include <d3dcompiler.h>
#pragma comment(lib, "D3DCompiler.lib")

#include "PixelShader.h"
#include "GraphicsMacro.h"
#include "VertexStruct.h"

namespace Rocket::Core
{
	PixelShader::PixelShader()
	{

	}

	PixelShader::~PixelShader()
	{
		_pixelShader.Reset();
		for (auto& buffer : _constantBuffer)
		{
			buffer.Reset();
		}
		_sampleState.Reset();
	}

	void PixelShader::Initialize(ID3D11Device* device, const std::wstring& path)
	{
		CreateAndReflectShader(device, path);
		CreateSamplerState(device);
	}

	ID3D11PixelShader* PixelShader::GetPixelShader() const
	{
		return _pixelShader.Get();
	}

	void PixelShader::CreateAndReflectShader(ID3D11Device* device, const std::wstring& path)
	{
// 		std::ifstream psFile(path, std::ios::binary);
// 		std::vector<char> psData = { std::istreambuf_iterator<char>(psFile), std::istreambuf_iterator<char>() };
// 
// 		device->CreatePixelShader(psData.data(), psData.size(), nullptr, &_pixelShader);

		UINT flags1 = 0;
#if defined(_DEBUG) || defined(_DEBUG)
		flags1 |= D3DCOMPILE_DEBUG;
		flags1 |= D3DCOMPILE_SKIP_OPTIMIZATION;
#endif

		ID3D10Blob* pixelShaderBlob = nullptr;
		HRESULT hResult = D3DCompileFromFile(path.c_str(), nullptr, nullptr, "main", "ps_5_0", flags1, 0, &pixelShaderBlob, nullptr);
		if (hResult != S_OK)
		{
			assert(false);
		}

		hResult = device->CreatePixelShader(pixelShaderBlob->GetBufferPointer(), pixelShaderBlob->GetBufferSize(), nullptr, &_pixelShader);
		if (hResult != S_OK)
		{
			assert(false);
		}

		/// Shader Reflection
		ID3D11ShaderReflection* pReflector = nullptr;

		// Create Reflector..
		D3DReflect(pixelShaderBlob->GetBufferPointer(), pixelShaderBlob->GetBufferSize(), IID_ID3D11ShaderReflection, (void**)&pReflector);

		// Shader Refection
		D3D11_SHADER_DESC shaderDesc;
		pReflector->GetDesc(&shaderDesc);

		/// ConstantBuffer Reflection
		// Pixel Shader ConstantBuffer..
		_constantBuffer.resize(shaderDesc.ConstantBuffers);

		for (unsigned int cbindex = 0; cbindex < shaderDesc.ConstantBuffers; cbindex++)
		{
			ID3D11ShaderReflectionConstantBuffer* cBuffer = pReflector->GetConstantBufferByIndex(cbindex);
			D3D11_SHADER_BUFFER_DESC bufferDesc;

			if (SUCCEEDED(cBuffer->GetDesc(&bufferDesc)))
			{
				CD3D11_BUFFER_DESC cBufferDesc(bufferDesc.Size, D3D11_BIND_CONSTANT_BUFFER, D3D11_USAGE_DYNAMIC, D3D11_CPU_ACCESS_WRITE);

				D3D11_SHADER_INPUT_BIND_DESC bindDesc;
				pReflector->GetResourceBindingDescByName(bufferDesc.Name, &bindDesc);

				// 해당 Constant Buffer 생성..
				HR(device->CreateBuffer(&cBufferDesc, nullptr, &_constantBuffer[bindDesc.BindPoint]));

				// Constant Buffer Register Slot Number..
				//cbuffer_register_slot = bindDesc.BindPoint;
			}
		}
	}

	ID3D11Buffer* PixelShader::GetConstantBuffer(int registerSlot) const
{
		return _constantBuffer[registerSlot].Get();
	}

	ID3D11Buffer** PixelShader::GetAddressOfConstantBuffer(int registerSlot)
{
		return _constantBuffer[registerSlot].GetAddressOf();
	}

	void PixelShader::CreateSamplerState(ID3D11Device* device)
	{
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
		HR(device->CreateSamplerState(&samplerDesc, &_sampleState));
	}

	ID3D11SamplerState** PixelShader::GetAddressOfSampleState()
	{
		return _sampleState.GetAddressOf();
	}

}
