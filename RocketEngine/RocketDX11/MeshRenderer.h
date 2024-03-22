#pragma once
#include <d3d11.h>
#include <dxgi.h>
#include <DirectXMath.h>
#include <wrl.h>
#include <vector>

#include "..\\RocketCommon\\IMeshRenderer.h"
#include "../RocketCommon/GraphicsEnum.h"
#include "ModelStruct.h"
#include "StaticMesh.h"
#include "Material.h"

using Microsoft::WRL::ComPtr;

namespace Rocket::Core
{
	/// Static Mesh만을 그리는 컴포넌트.
	/// 애니메이션이 적용된 Mesh는 Model기준으로 그리는 DynamicModelRenderer를 사용.
	class MeshRenderer : public Rocket::Core::IMeshRenderer
	{
	public:
		MeshRenderer();

	public:
		virtual void SetWorldTM(const Matrix& worldTM) override;
		virtual void SetActive(bool isActive) override;
		virtual void LoadMesh(std::string fileName) override;
		virtual void LoadTexture(std::string fileName) override;

	public:
		void Render(ID3D11DeviceContext* deviceContext, const DirectX::XMMATRIX& view, const DirectX::XMMATRIX& proj);

	public:
		void SetMaterial(Material* val) { _material = val; }
		void SetVertexShader(VertexShader* shader);
		void SetPixelShader(PixelShader* shader);
		void SetRenderState(ID3D11RasterizerState* renderState);

	private:
		void SetNodeBuffer(Node* node, NodeBufferType* nodeBuffer);

	private:
		StaticMesh* _mesh;
		StaticModel* _model;
		Material* _material;
		DirectX::XMMATRIX _worldTM;
		bool _isActive;
	};
}
