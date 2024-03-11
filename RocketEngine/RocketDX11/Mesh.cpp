#include "Mesh.h"
#include "ResourceManager.h"
#include "GraphicsMacro.h"

namespace Rocket::Core
{
	Mesh::Mesh()
		: _vertexBuffer(), _indexBuffer(),
		_vertexCount(0),
		_indexCount(0),
		_vertexType(eVertexType::VERTEX),
		_indices()
	{

	}

// 	Mesh::Mesh(std::vector<Vertex> vertices, std::vector<UINT> indices)
// 		: _vertexBuffer(), _indexBuffer(),
// 		_vertexCount((int)vertices.size()),
// 		_indexCount((int)indices.size()),
// 		_vertexType(eVertexType::VERTEX),
// 		_indices(indices)
// 	{
// 
// 	}

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

	void Mesh::SetVertexType(eVertexType type)
	{
		_vertexType = type;
	}

	eVertexType Mesh::GetVertexType() const
	{
		return _vertexType;
	}
}
