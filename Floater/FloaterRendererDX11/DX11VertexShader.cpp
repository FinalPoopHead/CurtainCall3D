#include "DX11VertexShader.h"
#include "DX11VSConstantBuffer.h"
#include "../FloaterUtil/include/FloaterMacro.h"
#include <d3dcompiler.h>
#include <wrl/client.h>

#if defined(DEBUG) || defined(_DEBUG)
#include <dxgidebug.h>
#endif


//#pragma comment(lib, "D3DCompiler.lib")
template<typename T>
using comptr = Microsoft::WRL::ComPtr<T>;

flt::DX11VertexShaderBuilder::~DX11VertexShaderBuilder()
{

}

flt::DX11VertexShader* flt::DX11VertexShaderBuilder::build() const
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

	comptr<ID3D10Blob> vertexShaderBlob = nullptr;
	HRESULT hResult = D3DCompileFromFile(filePath.c_str(), nullptr, nullptr, "main", "vs_5_0", flags1, 0, &vertexShaderBlob, nullptr);
	if (hResult != S_OK)
	{
		ASSERT(false, "버텍스 쉐이더 컴파일 실패");
		return nullptr;
	}

	ID3D11VertexShader* vertexShader = nullptr;
	hResult = pDevice->CreateVertexShader(vertexShaderBlob->GetBufferPointer(), vertexShaderBlob->GetBufferSize(), nullptr, &vertexShader);
	if (hResult != S_OK)
	{
		ASSERT(false, "버텍스 쉐이더 생성 실패");
		return nullptr;
	}

	ID3D11InputLayout* inputLayout = nullptr;
	hResult = pDevice->CreateInputLayout(&(inputLayoutDesc[0]), (UINT)inputLayoutDesc.size(),
		vertexShaderBlob->GetBufferPointer(), (UINT)(vertexShaderBlob->GetBufferSize()),
		&inputLayout);
	if (hResult != S_OK)
	{
		ASSERT(false, "인풋 레이아웃 생성 실패");
		vertexShader->Release();
		return nullptr;
	}

	ID3D11ShaderReflection* pVertexShaderReflection = nullptr;
	hResult = D3DReflect(vertexShaderBlob->GetBufferPointer(), vertexShaderBlob->GetBufferSize(), IID_ID3D11ShaderReflection, (void**)&pVertexShaderReflection);

	D3D11_SHADER_DESC shaderDesc = {};
	hResult = pVertexShaderReflection->GetDesc(&shaderDesc);
	if (hResult != S_OK)
	{
		ASSERT(false, "버텍스 쉐이더 디스크립션 실패");
		vertexShader->Release();
		inputLayout->Release();
		return nullptr;
	}

	DX11VertexShader* dx11VertexShader = new DX11VertexShader();

	int constantBufferCount = shaderDesc.ConstantBuffers;
	dx11VertexShader->pConstantBuffers.resize(constantBufferCount);
	for (int i = 0; i < constantBufferCount; ++i)
	{
		ID3D11ShaderReflectionConstantBuffer* pConstantBufferReflection = pVertexShaderReflection->GetConstantBufferByIndex(i);
		D3D11_SHADER_BUFFER_DESC ConstantBufferReflectionDesc = {};
		hResult = pConstantBufferReflection->GetDesc(&ConstantBufferReflectionDesc);

		D3D11_BUFFER_DESC constantBufferDesc = {};
		constantBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
		constantBufferDesc.ByteWidth = ConstantBufferReflectionDesc.Size;
		constantBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		constantBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		constantBufferDesc.MiscFlags = 0;
		constantBufferDesc.StructureByteStride = 0;

		ID3D11Buffer*& rpConstantBuffer = dx11VertexShader->pConstantBuffers[i].first;
		hResult = pDevice->CreateBuffer(&constantBufferDesc, nullptr, &rpConstantBuffer);
		dx11VertexShader->pConstantBuffers[i].second = ConstantBufferReflectionDesc.Size;
		ASSERT(hResult == S_OK, "상수 버퍼 생성 실패");
		rpConstantBuffer->SetPrivateData(WKPDID_D3DDebugObjectNameW, sizeof(L"DX11VertexShaderBuffer") - 1, L"DX11VertexShaderBuffer");
	}

	pVertexShaderReflection->Release();

	dx11VertexShader->pVertexShader = vertexShader;
	dx11VertexShader->pInputLayout = inputLayout;

	return dx11VertexShader;
}

void flt::DX11VertexShader::SetConstantBuffer(ID3D11DeviceContext* pContext, void** pDataArr, int arrSize)
{
	ASSERT(arrSize == pConstantBuffers.size(), "상수 버퍼 개수가 다름");

	D3D11_MAPPED_SUBRESOURCE mappedResource = { };
	for (int i = 0; i < arrSize; ++i)
	{
		HRESULT hResult = pContext->Map(pConstantBuffers[i].first, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
		if (hResult != S_OK)
		{
			ASSERT(false, "상수 버퍼 맵핑 실패");
			return;
		}
		memcpy(mappedResource.pData, pDataArr[i], pConstantBuffers[i].second);
		pContext->Unmap(pConstantBuffers[i].first, 0);

		pContext->VSSetConstantBuffers(i, 1, &pConstantBuffers[i].first);
	}
}
