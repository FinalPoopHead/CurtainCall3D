#pragma once
#include <d3d11.h>
#include <dxgi.h>
#include <DirectXMath.h>
#include <wrl.h>
#include <vector>

#include "..\\GraphicsInterface\\IMeshRenderer.h"
#include "../GraphicsInterface/GraphicsEnum.h"
#include "Mesh.h"
#include "Material.h"

using Microsoft::WRL::ComPtr;

namespace Rocket::Core
{
	struct ModelData;

	class MeshRenderer : public Rocket::Core::IMeshRenderer
	{
	public:
		MeshRenderer();



	public:
		virtual void SetWorldTM(const Matrix& worldTM) override;
		virtual void SetActive(bool isActive) override;
		virtual void LoadMesh(eMeshType meshType) override;
		virtual void LoadMesh(std::string fileName) override;
		virtual void LoadTexture(std::string fileName) override;

	public:
		void Render(ID3D11DeviceContext* deviceContext, const DirectX::XMMATRIX& view, const DirectX::XMMATRIX& proj);

	public:
		void LoadMesh(Mesh* mesh) { _mesh = mesh; }
		void SetMaterial(Material* val) { _material = val; }
		void SetTexture(Texture* texture);
		void SetVertexShader(VertexShader* shader);
		void SetPixelShader(PixelShader* shader);
		void SetRenderState(ID3D11RasterizerState* renderState);

	private:
		void SetNodeBuffer(Node* node, UINT& index, NodeBufferType* nodeBuffer);

	private:
		ModelData* _model;
		Mesh* _mesh;
		std::vector<Mesh*>* _meshes;
		Material* _material;
		DirectX::XMMATRIX _worldTM;
		bool _isActive;
	};
}
