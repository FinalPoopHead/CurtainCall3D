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
		:_vertexShader(),
		_constantBuffers(),
		_inputLayout(),
		_vertexType(),
		_numElements()
	{

	}

	VertexShader::~VertexShader()
	{
		_vertexShader.Reset();
		for (auto& buffer : _constantBuffers)
		{
			buffer.Reset();
		}
		_inputLayout.Reset();
	}

	void VertexShader::Initialize(ID3D11Device* device, const std::wstring& path)
	{
		CreateAndReflectShader(device, path);
	}

	ID3D11VertexShader* VertexShader::GetVertexShader() const
	{
		return _vertexShader.Get();
	}

	ID3D11Buffer* VertexShader::GetConstantBuffer(int registerSlot) const
	{
		return _constantBuffers[registerSlot].Get();
	}

	ID3D11InputLayout* VertexShader::GetInputLayout() const
	{
		return _inputLayout.Get();
	}

	ID3D11Buffer** VertexShader::GetAddressOfConstantBuffer(int registerSlot)
	{
		return _constantBuffers[registerSlot].GetAddressOf();
	}

	void VertexShader::ReflectShader(ID3D11Device* device, const std::wstring& path)
	{
		/*
		/// Shader Resource Reflection
		// Shader Resource..
		for (unsigned int rsindex = 0; rsindex < shaderDesc.BoundResources; rsindex++)
		{
			D3D11_SHADER_INPUT_BIND_DESC bindDesc;
			pReflector->GetResourceBindingDesc(rsindex, &bindDesc);

			// Resource Type에 맞는 해당 List에 삽입..
			switch (bindDesc.Type)
			{
				case D3D_SIT_TEXTURE:
				{
					// Texture Resource 삽입..
				}
				break;
				case D3D_SIT_SAMPLER:
				{
					// Sampler Resource 삽입..
				}
				break;
				default:
					break;
			}
		}
		*/
	}

	void VertexShader::CreateAndReflectShader(ID3D11Device* device, const std::wstring& path)
	{
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

		_vertexShader->SetPrivateData(WKPDID_D3DDebugObjectNameW, sizeof(L"vertexShader") - 1, L"vertexShader");

		/// Shader Reflection
		ID3D11ShaderReflection* pReflector = nullptr;

		// Create Reflector..
		D3DReflect(vertexShaderBlob->GetBufferPointer(), vertexShaderBlob->GetBufferSize(), IID_ID3D11ShaderReflection, (void**)&pReflector);

		// Shader Refection
		D3D11_SHADER_DESC shaderDesc;
		pReflector->GetDesc(&shaderDesc);

		/// Input Layout Reflection
		// TODO : 이 부분 함수로 빼거나 할까?
		// Shader Input Layout..
		std::vector<D3D11_INPUT_ELEMENT_DESC> inputLayoutDesc;
		for (unsigned inputIndex = 0; inputIndex < shaderDesc.InputParameters; inputIndex++)
		{
			D3D11_SIGNATURE_PARAMETER_DESC paramDesc;
			pReflector->GetInputParameterDesc(inputIndex, &paramDesc);

			// Shader Input Data를 기반으로 생성..
			D3D11_INPUT_ELEMENT_DESC elementDesc;
			elementDesc.SemanticName = paramDesc.SemanticName;
			elementDesc.SemanticIndex = paramDesc.SemanticIndex;
			elementDesc.InputSlot = 0;
			elementDesc.AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
// 			elementDesc.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
// 			elementDesc.InstanceDataStepRate = 0;
			elementDesc.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
			elementDesc.InstanceDataStepRate = 0;

			// Shader Data 기반으로 DXGI format 설정..
			if (paramDesc.Mask == 1)
			{
				if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32) elementDesc.Format = DXGI_FORMAT_R32_UINT;
				else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32) elementDesc.Format = DXGI_FORMAT_R32_SINT;
				else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32) elementDesc.Format = DXGI_FORMAT_R32_FLOAT;
			}
			else if (paramDesc.Mask <= 3)
			{
				if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32) elementDesc.Format = DXGI_FORMAT_R32G32_UINT;
				else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32) elementDesc.Format = DXGI_FORMAT_R32G32_SINT;
				else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32) elementDesc.Format = DXGI_FORMAT_R32G32_FLOAT;
			}
			else if (paramDesc.Mask <= 7)
			{
				if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32) elementDesc.Format = DXGI_FORMAT_R32G32B32_UINT;
				else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32) elementDesc.Format = DXGI_FORMAT_R32G32B32_SINT;
				else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32) elementDesc.Format = DXGI_FORMAT_R32G32B32_FLOAT;
			}
			else if (paramDesc.Mask <= 15)
			{
				if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32) elementDesc.Format = DXGI_FORMAT_R32G32B32A32_UINT;
				else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32) elementDesc.Format = DXGI_FORMAT_R32G32B32A32_SINT;
				else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32) elementDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
			}

			// 현 InputLayout 데이터 삽입..
			inputLayoutDesc.push_back(elementDesc);
		}

		// Shader InputLayout 생성..
		HR(device->CreateInputLayout(&inputLayoutDesc[0], (UINT)inputLayoutDesc.size(), vertexShaderBlob->GetBufferPointer(), vertexShaderBlob->GetBufferSize(), &_inputLayout));
		_inputLayout->SetPrivateData(WKPDID_D3DDebugObjectNameW, sizeof(L"vertexShaderInputLayout"), L"vertexShaderInputLayout");

		/// ConstantBuffer Reflection
		// Vertex Shader ConstantBuffer..e
		_constantBuffers.resize(shaderDesc.ConstantBuffers);

		for (unsigned int cbindex = 0; cbindex < shaderDesc.ConstantBuffers; cbindex++)
		{
			ID3D11ShaderReflectionConstantBuffer* cBuffer = pReflector->GetConstantBufferByIndex(cbindex);
			D3D11_SHADER_BUFFER_DESC bufferDesc;

			if (SUCCEEDED(cBuffer->GetDesc(&bufferDesc)))
			{
				CD3D11_BUFFER_DESC cBufferDesc(bufferDesc.Size, D3D11_BIND_CONSTANT_BUFFER, D3D11_USAGE_DYNAMIC, D3D11_CPU_ACCESS_WRITE);

				D3D11_SHADER_INPUT_BIND_DESC bindDesc;
				pReflector->GetResourceBindingDescByName(bufferDesc.Name, &bindDesc);
				auto str = bindDesc.Name;

				// 해당 Constant Buffer 생성..
				HR(device->CreateBuffer(&cBufferDesc, nullptr, &_constantBuffers[bindDesc.BindPoint]));

				_constantBuffers[bindDesc.BindPoint]->SetPrivateData(WKPDID_D3DDebugObjectNameW, sizeof(L"vertexCBuffer") - 1, L"vertexCBuffer");

				// Constant Buffer Register Slot Number..
				//cbuffer_register_slot = bindDesc.BindPoint;
			}
		}

		vertexShaderBlob->Release();
		pReflector->Release();
	}

	void VertexShader::SetVertexType(eVertexType type)
	{
		_vertexType = type;
	}

	eVertexType VertexShader::GetVertexType() const
	{
		return _vertexType;
	}
}
