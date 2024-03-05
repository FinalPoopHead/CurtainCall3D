#pragma once
#include <d3d11.h>
#include <dxgi.h>
#include <DirectXMath.h>
#include <wrl.h>
#include <vector>

#include "IResource.h"
#include "GraphicsStruct.h"

using Microsoft::WRL::ComPtr;

namespace Rocket::Core
{
	struct Node;

	class Mesh : public IResource
	{
	public:
		Mesh();
		Mesh(std::vector<Vertex> vertices, std::vector<UINT> indices);
		virtual void Initialize(ID3D11Device* device) {}
		
		void CreateBuffers();

		int GetVertexCount() const;
		int GetIndexCount() const;

		ID3D11Buffer* GetVertexBuffer() const;
		ID3D11Buffer* GetIndexBuffer() const;
		ID3D11Buffer** GetAddressOfVertexBuffer();
		ID3D11Buffer** GetAddressOfIndexBuffer();

		void SetVertexType(VertexType type);
		VertexType GetVertexType() const;

		Node* GetNode() const { return _node; }
		void SetNode(Node* val) { _node = val; }

	protected:
		std::vector<Vertex> _vertices;
		std::vector<UINT> _indices;
		ComPtr<ID3D11Buffer> _vertexBuffer;
		ComPtr<ID3D11Buffer> _indexBuffer;
		int _vertexCount;
		int _indexCount;
		VertexType _vertexType;
		Node* _node;
	};
}
