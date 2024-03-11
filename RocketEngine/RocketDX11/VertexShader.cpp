#include <sstream>
#include <fstream>
#include <vector>
#include <DirectXMath.h>
#include <d3dcompiler.h>
#pragma comment(lib, "D3DCompiler.lib")

#include "VertexShader.h"
#include "GraphicsMacro.h"

namespace Rocket::Core
{
	VertexShader::VertexShader()
		: _vertexDesc(nullptr)
	{

	}

	VertexShader::~VertexShader()
	{
		_inputLayout.Reset();
		_matrixBuffer.Reset();
		_vertexShader.Reset();
	}

	void VertexShader::Initialize(ID3D11Device* device, const std::wstring& path)
	{
		CreateShaderAndInputLayout(device, path);
		CreateMatrixBuffer(device);
		CreateSamplerState(device);
	}

	ID3D11VertexShader* VertexShader::GetVertexShader() const
	{
		return _vertexShader.Get();
	}

	ID3D11Buffer* VertexShader::GetMatrixBuffer() const
	{
		return _matrixBuffer.Get();
	}

	ID3D11InputLayout* VertexShader::GetInputLayout() const
	{
		return _inputLayout.Get();
	}

	ID3D11Buffer** VertexShader::GetAddressOfMatrixBuffer()
	{
		return _matrixBuffer.GetAddressOf();
	}

	void VertexShader::CreateShaderAndInputLayout(ID3D11Device* device, const std::wstring& path)
	{
// 		std::ifstream vsFile(path, std::ios::binary);
// 		std::vector<char> vsData = { std::istreambuf_iterator<char>(vsFile), std::istreambuf_iterator<char>() };

// 		device->CreateVertexShader(vsData.data(), vsData.size(), nullptr, &_vertexShader);
// 
// 		assert(_vertexDesc != nullptr);
// 
// 		HR(device->CreateInputLayout(_vertexDesc, _numElements, vsData.data(), vsData.size(), &_inputLayout));

		UINT flags1 = 0;
#if defined(_DEBUG) || defined(_DEBUG)
		flags1 |= D3DCOMPILE_DEBUG;
		flags1 |= D3DCOMPILE_SKIP_OPTIMIZATION;
#endif

		ID3D10Blob* vertexShaderBlob = nullptr;
		HRESULT hResult = D3DCompileFromFile(path.c_str(), nullptr, nullptr, "main", "vs_5_0", flags1, 0, &vertexShaderBlob, nullptr);
		if (hResult != S_OK)
		{
			assert(false);
		}

		hResult = device->CreateVertexShader(vertexShaderBlob->GetBufferPointer(), vertexShaderBlob->GetBufferSize(), nullptr, &_vertexShader);
		if (hResult != S_OK)
		{
			assert(false);
		}

		HR(device->CreateInputLayout(_vertexDesc, _numElements, vertexShaderBlob->GetBufferPointer(), vertexShaderBlob->GetBufferSize(), &_inputLayout));
	}

	void VertexShader::CreateMatrixBuffer(ID3D11Device* device)
	{
		D3D11_BUFFER_DESC matrixBufferDesc;
		matrixBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
		matrixBufferDesc.ByteWidth = sizeof(MatrixBufferType);
		matrixBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		matrixBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		matrixBufferDesc.MiscFlags = 0;
		matrixBufferDesc.StructureByteStride = 0;

		HR(device->CreateBuffer(&matrixBufferDesc, NULL, &_matrixBuffer));
	}

	void VertexShader::SetVertexDesc(D3D11_INPUT_ELEMENT_DESC desc[], unsigned int numElements)
	{
		_vertexDesc = desc;
		_numElements = numElements;
	}

	void VertexShader::SetVertexType(eVertexType type)
	{
		_vertexType = type;
	}

	eVertexType VertexShader::GetVertexType() const
	{
		return _vertexType;
	}

	void VertexShader::CreateSamplerState(ID3D11Device* device)
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

	ID3D11SamplerState** VertexShader::GetAddressOfSampleState()
	{
		return &_sampleState;
	}
}
