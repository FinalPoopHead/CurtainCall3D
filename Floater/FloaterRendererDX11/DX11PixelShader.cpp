#include "DX11PixelShader.h"
#include "DX11PSConstantBuffer.h"
#include "../FloaterUtil/include/FloaterMacro.h"
#include <d3dcompiler.h>
#include <wrl/client.h>


#if defined(DEBUG) || defined(_DEBUG)
#include <dxgidebug.h>
#endif

//#pragma comment(lib, "D3DCompiler.lib")

template<typename T>
using comptr = Microsoft::WRL::ComPtr<T>;

flt::DX11PixelShader* flt::DX11PixelShaderBuilder::build() const 
{
	if (pDevice == nullptr)
	{
		ASSERT(false, "디바이스 세팅 안됨.");
		return nullptr;
	}

	UINT flags1 = 0;
#if defined(_DEBUG) || defined(_DEBUG)
	flags1 |= D3DCOMPILE_DEBUG;
#endif

	comptr<ID3D10Blob> pixelShaderBlob;

	HRESULT hResult = D3DCompileFromFile(filePath.c_str(), nullptr, nullptr, "main", "ps_5_0", flags1, 0, &pixelShaderBlob, nullptr);
	if (hResult != S_OK)
	{
		ASSERT(false, "픽셀 쉐이더 컴파일 실패");
		return nullptr;
	}

	ID3D11PixelShader* pixelShader;
	hResult = pDevice->CreatePixelShader(pixelShaderBlob->GetBufferPointer(), pixelShaderBlob->GetBufferSize(), nullptr, &pixelShader);
	if (hResult != S_OK)
	{
		ASSERT(false, "픽셀 쉐이더 생성 실패");
		pixelShaderBlob->Release();
		return nullptr;
	}

	ID3D11Buffer* constantBuffer = nullptr;
	D3D11_BUFFER_DESC constantBufferDesc = {};
	constantBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	constantBufferDesc.ByteWidth = sizeof(PSConstantBuffer);
	constantBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	constantBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	constantBufferDesc.MiscFlags = 0;
	constantBufferDesc.StructureByteStride = 0;

	hResult = pDevice->CreateBuffer(&constantBufferDesc, nullptr, &constantBuffer);
	if (hResult != S_OK)
	{
		ASSERT(false, "픽셀 쉐이더 상수 버퍼 생성 실패");
		pixelShader->Release();
		return nullptr;
	}
	constantBuffer->SetPrivateData(WKPDID_D3DDebugObjectNameW, sizeof(L"PixelShaderBuffer") - 1, L"PixelShaderBuffer");


	DX11PixelShader* pPixelShader = new DX11PixelShader();
	pPixelShader->pPixelShader = pixelShader;
	pPixelShader->pConstantBuffer = constantBuffer;
	return pPixelShader;
}

void flt::DX11PixelShader::Release()
{
	pPixelShader->Release();
	pConstantBuffer->Release();
}
