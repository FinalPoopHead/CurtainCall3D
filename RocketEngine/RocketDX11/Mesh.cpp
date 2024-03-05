#include "Mesh.h"
#include "ResourceManager.h"
#include "GraphicsMacro.h"
#include "Animation.h"

namespace Rocket::Core
{
	Mesh::Mesh()
		: _vertexBuffer(), _indexBuffer(),
		_vertexCount(0),
		_indexCount(0),
		_vertexType(VertexType::VERTEX),
		_vertices(),
		_indices()
	{

	}

	Mesh::Mesh(std::vector<Vertex> vertices, std::vector<UINT> indices)
		: _vertexBuffer(), _indexBuffer(),
		_vertexCount((int)vertices.size()),
		_indexCount((int)indices.size()),
		_vertexType(VertexType::VERTEX),
		_vertices(vertices),
		_indices(indices)
	{

	}

	ID3D11Buffer* Mesh::GetVertexBuffer() const
	{
		return _vertexBuffer.Get();
	}

	ID3D11Buffer* Mesh::GetIndexBuffer() const
	{
		return _indexBuffer.Get();
	}

	ID3D11Buffer** Mesh::GetAddressOfVertexBuffer()
	{
		return _vertexBuffer.GetAddressOf();
	}

	ID3D11Buffer** Mesh::GetAddressOfIndexBuffer()
	{
		return _indexBuffer.GetAddressOf();
	}

	int Mesh::GetVertexCount() const
	{
		return _vertexCount;
	}

	int Mesh::GetIndexCount() const
	{
		return _indexCount;
	}

	void Mesh::SetVertexType(VertexType type)
	{
		_vertexType = type;
	}

	VertexType Mesh::GetVertexType() const
	{
		return _vertexType;
	}

	void Mesh::CreateBuffers()
	{
		for (auto& vertex : _vertices)
		{
			vertex.nodeIndex = _node->bone.id;
		}

		D3D11_BUFFER_DESC vertexBufferDesc = {};
		vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
		vertexBufferDesc.ByteWidth = sizeof(Vertex) * _vertexCount;
		vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		vertexBufferDesc.CPUAccessFlags = 0;
		vertexBufferDesc.MiscFlags = 0;
		vertexBufferDesc.StructureByteStride = 0;

		D3D11_SUBRESOURCE_DATA vertexData = {};
		vertexData.pSysMem = _vertices.data();

		HR(ResourceManager::Instance().GetDevice()->CreateBuffer(&vertexBufferDesc, &vertexData, &_vertexBuffer));

		D3D11_BUFFER_DESC indexBufferDesc = {};
		indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
		indexBufferDesc.ByteWidth = sizeof(UINT) * _indexCount;
		indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		indexBufferDesc.CPUAccessFlags = 0;
		indexBufferDesc.MiscFlags = 0;
		indexBufferDesc.StructureByteStride = 0;

		D3D11_SUBRESOURCE_DATA indexData = {};
		indexData.pSysMem = _indices.data();

		HR(ResourceManager::Instance().GetDevice()->CreateBuffer(&indexBufferDesc, &indexData, &_indexBuffer));
	}
}
