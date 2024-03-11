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
	}

	void PixelShader::Initialize(ID3D11Device* device, const std::wstring& path)
	{
		CreatePixelShader(device, path);
		CreateLightBuffer(device);
	}

	ID3D11PixelShader* PixelShader::GetPixelShader() const
	{
		return _pixelShader.Get();
	}

	void PixelShader::CreatePixelShader(ID3D11Device* device, const std::wstring& path)
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
	}

	void PixelShader::CreateLightBuffer(ID3D11Device* device)
	{
		D3D11_BUFFER_DESC lightBufferDesc;
		lightBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
		lightBufferDesc.ByteWidth = sizeof(LightBufferType);
		lightBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		lightBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		lightBufferDesc.MiscFlags = 0;
		lightBufferDesc.StructureByteStride = 0;

		HR(device->CreateBuffer(&lightBufferDesc, NULL, &_lightBuffer));
	}

	ID3D11Buffer* PixelShader::GetLightBuffer() const
	{
		return _lightBuffer.Get();
	}

	ID3D11Buffer** PixelShader::GetAddressOfLightBuffer()
	{
		return _lightBuffer.GetAddressOf();
	}

}
