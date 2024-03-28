#pragma once
#include <d3d11.h>
#include <dxgi.h>
#include <DirectXMath.h>
#include <wrl.h>
#include <vector>

#include "IResource.h"
#include "VertexStruct.h"

using Microsoft::WRL::ComPtr;

namespace Rocket::Core
{
	struct Node;

	// TODO : 이거를 템플릿으로 빼서 staticMesh와 skinnedMesh를 구분하면 좋지 않을까?
	class Mesh : public IResource
	{
	public:
		Mesh();
		virtual ~Mesh();
		// Mesh(std::vector<Vertex> vertices, std::vector<UINT> indices);
		virtual void Initialize(ID3D11Device* device) {}
		
		virtual void CreateBuffers() = 0;

		int GetVertexCount() const;
		int GetIndexCount() const;

		ID3D11Buffer* GetVertexBuffer() const;
		ID3D11Buffer* GetIndexBuffer() const;
		ID3D11Buffer** GetAddressOfVertexBuffer();
		ID3D11Buffer** GetAddressOfIndexBuffer();

		void SetVertexType(eVertexType type);
		eVertexType GetVertexType() const;

		Node* GetNode() const { return _node; }
		virtual void SetNode(Node* val) { _node = val; }

	protected:
		// std::vector<Vertex> _vertices;		// VertexType은 자식클래스인 Static,Skinned에서 정의
		std::vector<UINT> _indices;
		ComPtr<ID3D11Buffer> _vertexBuffer;
		ComPtr<ID3D11Buffer> _indexBuffer;
		int _vertexCount;
		int _indexCount;
		eVertexType _vertexType;
		Node* _node;
	};
}
