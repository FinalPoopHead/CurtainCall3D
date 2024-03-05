#include "DX11Mesh.h"
#include "../FloaterRendererCommon/include/RawMesh.h"
#include "../FloaterUtil/include/FloaterMacro.h"
#include <directxtk/DDSTextureLoader.h>
#include <directxtk/WICTextureLoader.h>


flt::DX11Mesh* flt::DX11MeshBuilder::build() const
{
	ASSERT(pDevice, "디바이스 세팅 안함");
	ASSERT(pRawMesh, "메쉬 세팅 안함");
	ASSERT(pImmediateContext, "디바이스 컨텍스트 세팅 안함");

	std::vector<Vertex::type> vertices;
	vertices.resize(pRawMesh->vertices.size());
	for (int i = 0; i < pRawMesh->vertices.size(); ++i)
	{
		vertices[i].pos = pRawMesh->vertices[i].pos;
		//vertices[i].tex = pRawMesh->vertices[i].uvs[0];
		for (int j = 0; j < 10; ++j)
		{
			vertices[i].tex[j] = pRawMesh->vertices[i].uvs[j];
		}
		vertices[i].normal = pRawMesh->vertices[i].normal;
		vertices[i].binormal = pRawMesh->vertices[i].binormal;
		vertices[i].tangent = pRawMesh->vertices[i].tangent;
		for (int j = 0; j < pRawMesh->vertices[i].boneIndice.size(); ++j)
		{
			vertices[i].BoneIndices[j] = pRawMesh->vertices[i].boneIndice[j];
		}
		for (int j = 0; j < pRawMesh->vertices[i].boneWeights.size(); ++j)
		{
			vertices[i].BoneWeights[j] = pRawMesh->vertices[i].boneWeights[j];
		}
	}

	D3D11_BUFFER_DESC vertexBufferDesc;
	vertexBufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
	vertexBufferDesc.ByteWidth = (UINT)(sizeof(Vertex::type) * vertices.size());
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA vertexData;
	vertexData.pSysMem = &(vertices[0]);
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	ID3D11Buffer* vertexBuffer;
	HRESULT hResult = pDevice->CreateBuffer(&vertexBufferDesc, &vertexData, &vertexBuffer);
	if (hResult != S_OK)
	{
		return nullptr;
	}
	vertexBuffer->SetPrivateData(WKPDID_D3DDebugObjectNameW, sizeof(L"MeshvertexBuffer") - 1, L"MeshvertexBuffer");

	D3D11_BUFFER_DESC indexBufferDesc;
	indexBufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
	indexBufferDesc.ByteWidth = (UINT)(sizeof(int) * pRawMesh->indices.size());
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA indexData;
	indexData.pSysMem = &(pRawMesh->indices[0]);
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	ID3D11Buffer* indexBuffer;
	hResult = pDevice->CreateBuffer(&indexBufferDesc, &indexData, &indexBuffer);
	if (hResult != S_OK)
	{
		return nullptr;
	}
	indexBuffer->SetPrivateData(WKPDID_D3DDebugObjectNameW, sizeof(L"MeshIndexBuffer") - 1, L"MeshIndexBuffer");

	ID3D11ShaderResourceView** textureViewArr = new(std::nothrow) ID3D11ShaderResourceView * [10] {nullptr, };
	int srvCount = 0;
	ASSERT(textureViewArr, "텍스처 뷰 배열 생성 실패");

	for (int i = 0; i < 10; ++i)
	{
		if (pRawMesh->material.textures[i]->path == L"")
		{
			textureViewArr[i] = nullptr;
			break;
		}
		size_t pos = pRawMesh->material.textures[0]->path.find_last_of(L".");
		std::wstring extension = pRawMesh->material.textures[0]->path.substr(pos + 1);
		const wchar_t* texturePath = pRawMesh->material.textures[0]->path.c_str();

		ID3D11Resource* texture;
		if (extension == L"dds")
		{
			hResult = DirectX::CreateDDSTextureFromFile(pDevice, pImmediateContext, texturePath, &texture, &textureViewArr[i]);
		}
		else
		{
			hResult = DirectX::CreateWICTextureFromFile(pDevice, pImmediateContext, texturePath, &texture, &textureViewArr[i]);
		}
		//ASSERT(hResult == S_OK, "텍스처 생성 실패");
		if (texture)
		{
			texture->Release();
		}

		srvCount++;
	}

	D3D11_SAMPLER_DESC samplerDesc = {};
	samplerDesc.Filter = D3D11_FILTER_ANISOTROPIC; //D3D11_FILTER_ANISOTROPIC;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	samplerDesc.MinLOD = 0;
	samplerDesc.MaxLOD = 2; D3D11_FLOAT32_MAX;

	ID3D11SamplerState* samplerState = nullptr;
	pDevice->CreateSamplerState(&samplerDesc, &samplerState);

	DX11VertexShaderBuilder vsBuilder(L"../FloaterRendererDX11/VertexShader.hlsl");
	vsBuilder.pDevice = pDevice;
	int layoutElementCount = Vertex::numElements;
	for (int i = 0; i < layoutElementCount; ++i)
	{
		vsBuilder.inputLayoutDesc.push_back(Vertex::layout[i]);
	}

	DX11PixelShaderBuilder psBuilder(L"../FloaterRendererDX11/PixelShader.hlsl");
	psBuilder.pDevice = pDevice;

	DX11Mesh* pMesh = new DX11Mesh(vsBuilder, psBuilder);
	pMesh->vertexBuffer = vertexBuffer;
	pMesh->singleVertexSize = sizeof(Vertex::type);
	pMesh->indexBuffer = indexBuffer;
	pMesh->indexCount = (UINT)pRawMesh->indices.size();
	pMesh->srv = textureViewArr;
	pMesh->srvCount = srvCount;
	pMesh->sampler = samplerState;
	//if (!pRawMesh->pRootBone)
	//{
	//	pMesh->pAnimation = new(std::nothrow) DX11Animation();
	//	pMesh->pAnimation->pRootBoneTransform = pRawMesh->pRootBone;
	//}

	return pMesh;
}

void flt::DX11Mesh::Release()
{
	if (vertexBuffer)
	{
		vertexBuffer->Release();
		vertexBuffer = nullptr;
	}

	if (indexBuffer)
	{
		indexBuffer->Release();
		indexBuffer = nullptr;
	}

	for (unsigned int i = 0; i < srvCount; ++i)
	{
		if (srv[i])
		{
			srv[i]->Release();
		}
	}
	delete[] srv;
	srv = nullptr;
	srvCount = 0;

	if (sampler)
	{
		sampler->Release();
		sampler = nullptr;
	}
}

flt::DX11Mesh* flt::DX11CubeMeshBuilder::build() const
{
	ASSERT(pDevice, "디바이스 세팅 안함.");
	ASSERT(pImmediateContext, "디바이스 컨텍스트 세팅 안함.");

	const D3D11_INPUT_ELEMENT_DESC VertexElementDesc[] =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0}
	};

	struct VertexUV
	{
		DirectX::XMFLOAT3 Pos;
		DirectX::XMFLOAT2 Tex;
	};

	std::vector<VertexUV> vertices;
	vertices.reserve(24);

	float w = 0.5f;
	float h = 0.5f;
	float d = 0.5f;

	vertices.insert(vertices.begin(),
		{
			VertexUV{DirectX::XMFLOAT3{-w, -h, -d}, DirectX::XMFLOAT2{0.0f, 1.0f}},
			VertexUV{DirectX::XMFLOAT3{-w, +h, -d}, DirectX::XMFLOAT2{0.0f, 0.0f}},
			VertexUV{DirectX::XMFLOAT3{+w, +h, -d}, DirectX::XMFLOAT2{1.0f, 0.0f}},
			VertexUV{DirectX::XMFLOAT3{+w, -h, -d}, DirectX::XMFLOAT2{1.0f, 1.0f}},

			VertexUV{DirectX::XMFLOAT3{-w, -h, +d}, DirectX::XMFLOAT2{1.0f, 1.0f}},
			VertexUV{DirectX::XMFLOAT3{+w, -h, +d}, DirectX::XMFLOAT2{0.0f, 1.0f}},
			VertexUV{DirectX::XMFLOAT3{+w, +h, +d}, DirectX::XMFLOAT2{0.0f, 0.0f}},
			VertexUV{DirectX::XMFLOAT3{-w, +h, +d}, DirectX::XMFLOAT2{1.0f, 0.0f}},

			VertexUV{DirectX::XMFLOAT3{-w, +h, -d}, DirectX::XMFLOAT2{0.0f, 1.0f}},
			VertexUV{DirectX::XMFLOAT3{-w, +h, +d}, DirectX::XMFLOAT2{0.0f, 0.0f}},
			VertexUV{DirectX::XMFLOAT3{+w, +h, +d}, DirectX::XMFLOAT2{1.0f, 0.0f}},
			VertexUV{DirectX::XMFLOAT3{+w, +h, -d}, DirectX::XMFLOAT2{1.0f, 1.0f}},

			VertexUV{DirectX::XMFLOAT3{-w, -h, -d}, DirectX::XMFLOAT2{1.0f, 1.0f}},
			VertexUV{DirectX::XMFLOAT3{+w, -h, -d}, DirectX::XMFLOAT2{0.0f, 1.0f}},
			VertexUV{DirectX::XMFLOAT3{+w, -h, +d}, DirectX::XMFLOAT2{0.0f, 0.0f}},
			VertexUV{DirectX::XMFLOAT3{-w, -h, +d}, DirectX::XMFLOAT2{1.0f, 0.0f}},

			VertexUV{DirectX::XMFLOAT3{-w, -h, +d}, DirectX::XMFLOAT2{0.0f, 1.0f}},
			VertexUV{DirectX::XMFLOAT3{-w, +h, +d}, DirectX::XMFLOAT2{0.0f, 0.0f}},
			VertexUV{DirectX::XMFLOAT3{-w, +h, -d}, DirectX::XMFLOAT2{1.0f, 0.0f}},
			VertexUV{DirectX::XMFLOAT3{-w, -h, -d}, DirectX::XMFLOAT2{1.0f, 1.0f}},

			VertexUV{DirectX::XMFLOAT3{+w, -h, -d}, DirectX::XMFLOAT2{0.0f, 1.0f}},
			VertexUV{DirectX::XMFLOAT3{+w, +h, -d}, DirectX::XMFLOAT2{0.0f, 0.0f}},
			VertexUV{DirectX::XMFLOAT3{+w, +h, +d}, DirectX::XMFLOAT2{1.0f, 0.0f}},
			VertexUV{DirectX::XMFLOAT3{+w, -h, +d}, DirectX::XMFLOAT2{1.0f, 1.0f}}
		});

	std::vector<int> indices;
	indices.reserve(36);
	indices.insert(indices.begin(),
		{
			0, 1, 2,
			0, 2, 3,

			4, 5, 6,
			4, 6, 7,

			8, 9, 10,
			8, 10, 11,

			12, 13, 14,
			12, 14, 15,

			16, 17, 18,
			16, 18, 19,

			20, 21, 22,
			20, 22, 23
		});


	D3D11_BUFFER_DESC vertexBufferDesc;
	vertexBufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
	vertexBufferDesc.ByteWidth = (UINT)(sizeof(VertexUV) * vertices.size());
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA vertexData;
	vertexData.pSysMem = &(vertices[0]);
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	ID3D11Buffer* vertexBuffer;
	HRESULT hResult = pDevice->CreateBuffer(&vertexBufferDesc, &vertexData, &vertexBuffer);
	if (hResult != S_OK)
	{
		return nullptr;
	}
	vertexBuffer->SetPrivateData(WKPDID_D3DDebugObjectNameW, sizeof(L"CubeVertexBuffer") - 1, L"CubeVertexBuffer");

	D3D11_BUFFER_DESC indexBufferDesc;
	indexBufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
	indexBufferDesc.ByteWidth = (UINT)(sizeof(int) * indices.size());
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA indexData;
	indexData.pSysMem = &(indices[0]);
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	ID3D11Buffer* indexBuffer;
	hResult = pDevice->CreateBuffer(&indexBufferDesc, &indexData, &indexBuffer);
	if (hResult != S_OK)
	{
		return nullptr;
	}
	indexBuffer->SetPrivateData(WKPDID_D3DDebugObjectNameW, sizeof(L"CubeIndexBuffer") - 1, L"CubeIndexBuffer");

	D3D11_BUFFER_DESC cbDesc;
	cbDesc.ByteWidth = sizeof(DirectX::XMMATRIX);
	cbDesc.Usage = D3D11_USAGE_DYNAMIC;
	cbDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cbDesc.MiscFlags = 0;
	cbDesc.StructureByteStride = 0;

	ID3D11Resource* texture;
	ID3D11ShaderResourceView** textureView = new(std::nothrow) ID3D11ShaderResourceView * [1];
	ASSERT(textureView, "텍스처 뷰 생성 실패");
	hResult = DirectX::CreateDDSTextureFromFile(pDevice, pImmediateContext, L"../FloaterRendererDX11/testResource/WoodCrate01.dds", &texture, textureView);
	if (hResult != S_OK)
	{
		return nullptr;
	}
	texture->Release();

	D3D11_SAMPLER_DESC samplerDesc = {};
	samplerDesc.Filter = D3D11_FILTER_ANISOTROPIC; //D3D11_FILTER_ANISOTROPIC;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	samplerDesc.MinLOD = 0;
	samplerDesc.MaxLOD = 2; D3D11_FLOAT32_MAX;

	ID3D11SamplerState* samplerState = nullptr;
	pDevice->CreateSamplerState(&samplerDesc, &samplerState);

	DX11VertexShaderBuilder vsBuilder(L"../FloaterRendererDX11/CubeVS.hlsl");
	vsBuilder.pDevice = pDevice;
	int layoutElementCount = sizeof(VertexElementDesc) / sizeof(D3D11_INPUT_ELEMENT_DESC);
	for (int i = 0; i < layoutElementCount; ++i)
	{
		vsBuilder.inputLayoutDesc.push_back(VertexElementDesc[i]);
	}

	DX11PixelShaderBuilder psBuilder(L"../FloaterRendererDX11/CubePS.hlsl");
	psBuilder.pDevice = pDevice;

	DX11Mesh* pMesh = new DX11Mesh(vsBuilder, psBuilder);
	pMesh->vertexBuffer = vertexBuffer;
	pMesh->singleVertexSize = sizeof(VertexUV);
	pMesh->indexBuffer = indexBuffer;
	pMesh->indexCount = (UINT)indices.size();
	pMesh->srv = textureView;
	pMesh->srvCount = 1;
	pMesh->sampler = samplerState;

	return pMesh;
}

flt::DX11Mesh* flt::DX11ScreedQuadBuilder::build() const
{
	ASSERT(pDevice, "디바이스 세팅 안함.");
	ASSERT(pImmediateContext, "디바이스 컨텍스트 세팅 안함.");

	const D3D11_INPUT_ELEMENT_DESC inputLayoutDesc[] =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0}
	};

	struct Vertex
	{
		DirectX::XMFLOAT3 Pos;
		DirectX::XMFLOAT2 Tex;
	};

	std::vector<Vertex> vertices;
	vertices.reserve(4);

	vertices.push_back(Vertex{ DirectX::XMFLOAT3{-1.0f, -1.0f, 0.0f}, DirectX::XMFLOAT2{0.0f, 1.0f} });
	vertices.push_back(Vertex{ DirectX::XMFLOAT3{-1.0f, +1.0f, 0.0f}, DirectX::XMFLOAT2{0.0f, 0.0f} });
	vertices.push_back(Vertex{ DirectX::XMFLOAT3{+1.0f, +1.0f, 0.0f}, DirectX::XMFLOAT2{1.0f, 0.0f} });
	vertices.push_back(Vertex{ DirectX::XMFLOAT3{+1.0f, -1.0f, 0.0f}, DirectX::XMFLOAT2{1.0f, 1.0f} });

	std::vector<int> indices;
	indices.reserve(6);

	indices.push_back(0);
	indices.push_back(1);
	indices.push_back(2);

	indices.push_back(0);
	indices.push_back(2);
	indices.push_back(3);

	D3D11_BUFFER_DESC vertexBufferDesc;
	vertexBufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
	vertexBufferDesc.ByteWidth = (UINT)(sizeof(Vertex) * vertices.size());
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA vertexData;
	vertexData.pSysMem = &(vertices[0]);
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	ID3D11Buffer* vertexBuffer;
	HRESULT hResult = pDevice->CreateBuffer(&vertexBufferDesc, &vertexData, &vertexBuffer);
	if (hResult != S_OK)
	{
		return nullptr;
	}
	vertexBuffer->SetPrivateData(WKPDID_D3DDebugObjectNameW, sizeof(L"ScreenQuadvertexBuffer") - 1, L"ScreenQuadvertexBuffer");


	D3D11_BUFFER_DESC indexBufferDesc;
	indexBufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
	indexBufferDesc.ByteWidth = (UINT)(sizeof(int) * indices.size());
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA indexData;
	indexData.pSysMem = &(indices[0]);
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	ID3D11Buffer* indexBuffer;
	hResult = pDevice->CreateBuffer(&indexBufferDesc, &indexData, &indexBuffer);
	if (hResult != S_OK)
	{
		return nullptr;
	}
	indexBuffer->SetPrivateData(WKPDID_D3DDebugObjectNameW, sizeof(L"ScreenQuadIndexBuffer") - 1, L"ScreenQuadIndexBuffer");

	//D3D11_BUFFER_DESC cbDesc;
	//cbDesc.ByteWidth = sizeof(DirectX::XMMATRIX);
	//cbDesc.Usage = D3D11_USAGE_DYNAMIC;
	//cbDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	//cbDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	//cbDesc.MiscFlags = 0;
	//cbDesc.StructureByteStride = 0;

	//ID3D11Buffer* constantBuffer;
	//hResult = pDevice->CreateBuffer(&cbDesc, nullptr, &constantBuffer);
	//if (hResult != S_OK)
	//{
	//	return nullptr;
	//}
	//constantBuffer->SetPrivateData(WKPDID_D3DDebugObjectNameW, sizeof(L"ScreenQuadConstantBuffer")-1, L"ScreenQuadConstantBuffer");

	D3D11_SAMPLER_DESC samplerDesc = {};
	samplerDesc.Filter = D3D11_FILTER_ANISOTROPIC; //D3D11_FILTER_ANISOTROPIC;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	samplerDesc.MinLOD = 0;
	samplerDesc.MaxLOD = 2; D3D11_FLOAT32_MAX;

	ID3D11SamplerState* samplerState = nullptr;
	pDevice->CreateSamplerState(&samplerDesc, &samplerState);

	DX11VertexShaderBuilder vsBuilder(L"../FloaterRendererDX11/BackBufferVertexShader.hlsl");
	vsBuilder.pDevice = pDevice;

	int layoutElementCount = sizeof(inputLayoutDesc) / sizeof(D3D11_INPUT_ELEMENT_DESC);
	for (int i = 0; i < layoutElementCount; ++i)
	{
		vsBuilder.inputLayoutDesc.push_back(inputLayoutDesc[i]);
	}

	DX11PixelShaderBuilder psBuilder(L"../FloaterRendererDX11/BackBufferPixelShader.hlsl");
	psBuilder.pDevice = pDevice;

	DX11Mesh* pMesh = new DX11Mesh(vsBuilder, psBuilder);
	pMesh->vertexBuffer = vertexBuffer;
	pMesh->singleVertexSize = sizeof(Vertex);
	pMesh->indexBuffer = indexBuffer;
	pMesh->indexCount = (UINT)indices.size();
	pMesh->srv = nullptr;
	pMesh->srvCount = 0;
	pMesh->sampler = samplerState;

	return pMesh;
}

flt::DX11Mesh* flt::DX11GridMeshBuilder::build() const
{
	const D3D11_INPUT_ELEMENT_DESC vertexElementDesc[] =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0}
	};

	struct Vertex
	{
		DirectX::XMFLOAT3 Pos;
		DirectX::XMFLOAT4 Color;
	};

	std::vector<Vertex> vertices;

	vertices.reserve(gridCount * gridCount);

	for (int i = 0; i < gridCount; ++i)
	{
		for (int j = 0; j < gridCount; ++j)
		{
			vertices.push_back
			(
				Vertex
				{
					DirectX::XMFLOAT3{(float)(gridCount / 2 - j), 0.0f, (float)(gridCount / 2 - i)},
					DirectX::XMFLOAT4{0.0f, 0.0f, 0.0f, 1.0f}
				}
			);
		}
	}

	// 선으로 그리기 때문에 인덱스는 2개씩
	std::vector<int> indices;
	indices.reserve((gridCount - 1) * (gridCount - 1));
	for (int i = 0; i < gridCount - 1; ++i)
	{
		for (int j = 0; j < gridCount - 2; ++j)
		{
			indices.push_back(i * gridCount + j);
			indices.push_back(i * gridCount + (j + 1));
		}
	}

	for (int i = 0; i < gridCount - 2; ++i)
	{
		for (int j = 0; j < gridCount - 1; ++j)
		{
			indices.push_back(i * gridCount + j);
			indices.push_back((i + 1) * gridCount + j);
		}
	}

	D3D11_BUFFER_DESC vertexBufferDesc;
	vertexBufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
	vertexBufferDesc.ByteWidth = (UINT)(sizeof(Vertex) * vertices.size());
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA vertexData;
	vertexData.pSysMem = &(vertices[0]);
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	ID3D11Buffer* vertexBuffer;
	HRESULT hResult = pDevice->CreateBuffer(&vertexBufferDesc, &vertexData, &vertexBuffer);
	if (hResult != S_OK)
	{
		return nullptr;
	}
	vertexBuffer->SetPrivateData(WKPDID_D3DDebugObjectNameW, sizeof(L"CubeVertexBuffer") - 1, L"CubeVertexBuffer");

	D3D11_BUFFER_DESC indexBufferDesc;
	indexBufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
	indexBufferDesc.ByteWidth = (UINT)(sizeof(int) * indices.size());
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA indexData;
	indexData.pSysMem = &(indices[0]);
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	ID3D11Buffer* indexBuffer;
	hResult = pDevice->CreateBuffer(&indexBufferDesc, &indexData, &indexBuffer);
	if (hResult != S_OK)
	{
		return nullptr;
	}
	indexBuffer->SetPrivateData(WKPDID_D3DDebugObjectNameW, sizeof(L"CubeIndexBuffer") - 1, L"CubeIndexBuffer");

	D3D11_BUFFER_DESC cbDesc;
	cbDesc.ByteWidth = sizeof(DirectX::XMMATRIX);
	cbDesc.Usage = D3D11_USAGE_DYNAMIC;
	cbDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cbDesc.MiscFlags = 0;
	cbDesc.StructureByteStride = 0;

	//ID3D11Buffer* constantBuffer;
	//hResult = pDevice->CreateBuffer(&cbDesc, nullptr, &constantBuffer);
	//if (hResult != S_OK)
	//{
	//	return nullptr;
	//}
	//constantBuffer->SetPrivateData(WKPDID_D3DDebugObjectNameW, sizeof(L"CubeConstantBuffer")-1, L"CubeConstantBuffer");

	ID3D11Resource* texture;
	ID3D11ShaderResourceView** textureView = new(std::nothrow) ID3D11ShaderResourceView * [1];
	ASSERT(textureView, "텍스처 뷰 생성 실패");
	hResult = DirectX::CreateDDSTextureFromFile(pDevice, pImmediateContext, L"../FloaterRendererDX11/testResource/WoodCrate01.dds", &texture, textureView);
	if (hResult != S_OK)
	{
		return nullptr;
	}
	texture->Release();

	D3D11_SAMPLER_DESC samplerDesc = {};
	samplerDesc.Filter = D3D11_FILTER_ANISOTROPIC; //D3D11_FILTER_ANISOTROPIC;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	samplerDesc.MinLOD = 0;
	samplerDesc.MaxLOD = 2; D3D11_FLOAT32_MAX;

	ID3D11SamplerState* samplerState = nullptr;
	pDevice->CreateSamplerState(&samplerDesc, &samplerState);

	DX11VertexShaderBuilder vsBuilder(L"../FloaterRendererDX11/GridVS.hlsl");
	vsBuilder.pDevice = pDevice;
	int layoutElementCount = sizeof(vertexElementDesc) / sizeof(D3D11_INPUT_ELEMENT_DESC);
	for (int i = 0; i < layoutElementCount; ++i)
	{
		vsBuilder.inputLayoutDesc.push_back(vertexElementDesc[i]);
	}

	DX11PixelShaderBuilder psBuilder(L"../FloaterRendererDX11/GridPS.hlsl");
	psBuilder.pDevice = pDevice;

	//D3D11_RASTERIZER_DESC rasterizerDesc = {};
	//rasterizerDesc.FillMode = D3D11_FILL_WIREFRAME;
	//rasterizerDesc.CullMode = D3D11_CULL_NONE;
	//rasterizerDesc.FrontCounterClockwise = false;
	//rasterizerDesc.DepthBias = 0;
	//rasterizerDesc.DepthBiasClamp = 0.0f;
	//rasterizerDesc.SlopeScaledDepthBias = 0.0f;
	//rasterizerDesc.DepthClipEnable = true;
	//rasterizerDesc.ScissorEnable = false;
	//rasterizerDesc.MultisampleEnable = false;
	//rasterizerDesc.AntialiasedLineEnable = false;

	//ID3D11RasterizerState* rasterizerState;
	//pDevice->CreateRasterizerState(&rasterizerDesc, &rasterizerState);

	DX11Mesh* pMesh = new DX11Mesh(vsBuilder, psBuilder);
	pMesh->vertexBuffer = vertexBuffer;
	pMesh->singleVertexSize = sizeof(Vertex);
	pMesh->indexBuffer = indexBuffer;
	pMesh->indexCount = (UINT)indices.size();
	pMesh->srv = textureView;
	pMesh->srvCount = 1;
	pMesh->sampler = samplerState;

	return pMesh;
}
