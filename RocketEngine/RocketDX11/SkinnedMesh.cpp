#include "SkinnedMesh.h"
#include "ResourceManager.h"
#include "GraphicsMacro.h"
#include "ModelStruct.h"

namespace Rocket::Core
{
	SkinnedMesh::SkinnedMesh()
		: _vertices()
	{
		_vertexType = eVertexType::SKINNED_VERTEX;
	}

	SkinnedMesh::SkinnedMesh(std::vector<VertexSkinned> vertices, std::vector<UINT> indices)
		: _vertices(vertices)
	{
		_vertexCount = (int)vertices.size();
		_indexCount = (int)indices.size();
		_vertexType = eVertexType::SKINNED_VERTEX;
		_indices = indices;
	}

	void SkinnedMesh::CreateBuffers()
	{
		D3D11_BUFFER_DESC vertexBufferDesc = {};
		vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
		vertexBufferDesc.ByteWidth = sizeof(VertexSkinned) * _vertexCount;
		vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		vertexBufferDesc.CPUAccessFlags = 0;
		vertexBufferDesc.MiscFlags = 0;
		vertexBufferDesc.StructureByteStride = 0;

		D3D11_SUBRESOURCE_DATA vertexData = {};
		vertexData.pSysMem = _vertices.data();

		HR(ResourceManager::Instance().GetDevice()->CreateBuffer(&vertexBufferDesc, &vertexData, _vertexBuffer.GetAddressOf()));

		D3D11_BUFFER_DESC indexBufferDesc = {};
		indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
		indexBufferDesc.ByteWidth = sizeof(UINT) * _indexCount;
		indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		indexBufferDesc.CPUAccessFlags = 0;
		indexBufferDesc.MiscFlags = 0;
		indexBufferDesc.StructureByteStride = 0;

		D3D11_SUBRESOURCE_DATA indexData = {};
		indexData.pSysMem = _indices.data();

		HR(ResourceManager::Instance().GetDevice()->CreateBuffer(&indexBufferDesc, &indexData, _indexBuffer.GetAddressOf()));
	
		_vertexBuffer->SetPrivateData(WKPDID_D3DDebugObjectNameW, sizeof(L"SKINNED v-Buffer") - 1, L"SKINNED v-Buffer");
		_indexBuffer->SetPrivateData(WKPDID_D3DDebugObjectNameW, sizeof(L"SKINNED i-Buffer") - 1, L"SKINNED i-Buffer");
	}

	void SkinnedMesh::SetNode(Node* node)
	{
		_node = node;

		for (auto& vertex : _vertices)
		{
			vertex.nodeIndex = _node->index;
		}
	}
}
