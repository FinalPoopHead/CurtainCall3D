#include "DX11VertexShader.h"
#include "DX11VSConstantBuffer.h"
#include "../FloaterUtil/include/FloaterType.h"
#include "../FloaterUtil/include/FloaterMacro.h"
#include <d3dcompiler.h>
#include <wrl/client.h>
#include <filesystem>

#if defined(DEBUG) || defined(_DEBUG)
#include <dxgidebug.h>
#endif

//#pragma comment(lib, "D3DCompiler.lib")


template<typename T>
using comptr = Microsoft::WRL::ComPtr<T>;

flt::DX11VertexShaderBuilder::DX11VertexShaderBuilder(const std::wstring filePath) : IBuilder<DX11VertexShader>(filePath), filePath(filePath), pDevice(nullptr)
{
	ASSERT(std::filesystem::exists(filePath), "파일을 찾을 수 없습니다.");
}

flt::DX11VertexShaderBuilder::~DX11VertexShaderBuilder()
{

}

flt::DX11VertexShader* flt::DX11VertexShaderBuilder::build() const
{
	ASSERT(pDevice != nullptr, "디바이스 세팅 안됨.");

	UINT compileFlag = 0;
#if defined(_DEBUG) || defined(_DEBUG)
	compileFlag |= D3DCOMPILE_DEBUG;
	compileFlag |= D3DCOMPILE_SKIP_OPTIMIZATION; // 코드 생성 중에 최적화 단계 건너 뜀
	compileFlag |= D3DCOMPILE_ENABLE_STRICTNESS; // 레거시 구문을 허용하지 않음.
	compileFlag |= D3DCOMPILE_WARNINGS_ARE_ERRORS; // 경고를 에러로 처리
	compileFlag |= D3DCOMPILE_PACK_MATRIX_ROW_MAJOR; // 행 우선으로 행렬 패킹 -> 일반적인 벡터 행렬곱이 벡터 * 행렬 순서가 됨.
				 //D3DCOMPILE_PACK_MATRIX_COLUMN_MAJOR // 열 우선으로 행렬 패킹 -> 벡터 행렬 곱이 행렬 * 벡터 순서가 됨.
#endif

	comptr<ID3D10Blob> vertexShaderBlob = nullptr;
	HRESULT hResult = D3DCompileFromFile(filePath.c_str(), nullptr, nullptr, "main", "vs_5_0", compileFlag, 0, &vertexShaderBlob, nullptr);
	ASSERT(hResult == S_OK, "버텍스 쉐이더 컴파일 실패");

	ID3D11VertexShader* vertexShader = nullptr;
	hResult = pDevice->CreateVertexShader(vertexShaderBlob->GetBufferPointer(), vertexShaderBlob->GetBufferSize(), nullptr, &vertexShader);
	ASSERT(hResult == S_OK, "버텍스 쉐이더 생성 실패");
	vertexShader->SetPrivateData(WKPDID_D3DDebugObjectNameW, (UINT)((filePath.size() + 1) * sizeof(wchar_t)), filePath.c_str());

	ID3D11ShaderReflection* pVertexShaderReflection = nullptr;
	hResult = D3DReflect(vertexShaderBlob->GetBufferPointer(), vertexShaderBlob->GetBufferSize(), IID_ID3D11ShaderReflection, (void**)&pVertexShaderReflection);

	D3D11_SHADER_DESC shaderDesc = {};
	hResult = pVertexShaderReflection->GetDesc(&shaderDesc);
	ASSERT(hResult == S_OK, "버텍스 쉐이더 디스크립션 실패");

	DX11VertexShader* dx11VertexShader = new(std::nothrow) DX11VertexShader();
	ASSERT(dx11VertexShader != nullptr, "메모리 할당 실패");

	uint32 inputParameterCount = shaderDesc.InputParameters;
	std::vector<D3D11_INPUT_ELEMENT_DESC> testinputLayoutDesc{ inputParameterCount };
	for (uint32 i = 0; i < inputParameterCount; ++i)
	{
		D3D11_SIGNATURE_PARAMETER_DESC inputParameterDesc;
		hResult = pVertexShaderReflection->GetInputParameterDesc(i, &inputParameterDesc);
		testinputLayoutDesc[i].SemanticName = inputParameterDesc.SemanticName;
		testinputLayoutDesc[i].SemanticIndex = inputParameterDesc.SemanticIndex;
		testinputLayoutDesc[i].InputSlot = 0;
		testinputLayoutDesc[i].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
		testinputLayoutDesc[i].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		testinputLayoutDesc[i].InstanceDataStepRate = 0;
		if (inputParameterDesc.Mask == 1)
		{
			if (inputParameterDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32) testinputLayoutDesc[i].Format = DXGI_FORMAT_R32_UINT;
			else if (inputParameterDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32) testinputLayoutDesc[i].Format = DXGI_FORMAT_R32_SINT;
			else if (inputParameterDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32) testinputLayoutDesc[i].Format = DXGI_FORMAT_R32_FLOAT;
		}
		else if (inputParameterDesc.Mask <= 3)
		{
			if (inputParameterDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32) testinputLayoutDesc[i].Format = DXGI_FORMAT_R32G32_UINT;
			else if (inputParameterDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32) testinputLayoutDesc[i].Format = DXGI_FORMAT_R32G32_SINT;
			else if (inputParameterDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32) testinputLayoutDesc[i].Format = DXGI_FORMAT_R32G32_FLOAT;
		}
		else if (inputParameterDesc.Mask <= 7)
		{
			if (inputParameterDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32) testinputLayoutDesc[i].Format = DXGI_FORMAT_R32G32B32_UINT;
			else if (inputParameterDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32) testinputLayoutDesc[i].Format = DXGI_FORMAT_R32G32B32_SINT;
			else if (inputParameterDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32) testinputLayoutDesc[i].Format = DXGI_FORMAT_R32G32B32_FLOAT;
		}
		else if (inputParameterDesc.Mask <= 15)
		{
			if (inputParameterDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32) testinputLayoutDesc[i].Format = DXGI_FORMAT_R32G32B32A32_UINT;
			else if (inputParameterDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32) testinputLayoutDesc[i].Format = DXGI_FORMAT_R32G32B32A32_SINT;
			else if (inputParameterDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32) testinputLayoutDesc[i].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		}
	}

	ID3D11InputLayout* inputLayout = nullptr;
	hResult = pDevice->CreateInputLayout(&(testinputLayoutDesc[0]), (UINT)testinputLayoutDesc.size(),
		vertexShaderBlob->GetBufferPointer(), (UINT)(vertexShaderBlob->GetBufferSize()),
		&inputLayout);
	ASSERT(hResult == S_OK, "인풋 레이아웃 생성 실패");

	uint32 constantBufferCount = shaderDesc.ConstantBuffers;
	dx11VertexShader->pConstantBuffers.resize(constantBufferCount);
	for (uint32 i = 0; i < constantBufferCount; ++i)
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
