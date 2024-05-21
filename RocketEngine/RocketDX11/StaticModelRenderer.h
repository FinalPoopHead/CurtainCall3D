#pragma once
#include <d3d11.h>
#include <dxgi.h>
#include <DirectXMath.h>
#include <wrl.h>
#include <vector>
#include <DirectXCollision.h>

#include "..\\RocketCommon\\IStaticModelRenderer.h"
#include "IRenderable.h"
#include "../RocketCommon/GraphicsEnum.h"
#include "ModelStruct.h"
#include "StaticMesh.h"
#include "Material.h"

using Microsoft::WRL::ComPtr;

namespace Rocket::Core
{
	class RocketTransform;
}

namespace Rocket::Core
{
	/// Static Mesh만을 그리는 컴포넌트.
	/// 애니메이션이 적용된 Mesh는 Model기준으로 그리는 DynamicModelRenderer를 사용.
	class StaticModelRenderer : public IStaticModelRenderer, public IRenderable
	{
	public:
		StaticModelRenderer();
		~StaticModelRenderer();

	public:
		virtual void SetWorldTM(const Matrix& worldTM) override;
		virtual void SetActive(bool isActive) override;
		virtual void LoadModel(std::string fileName) override;
		virtual void LoadBaseColorTexture(std::string fileName) override;
		virtual void LoadNormalTexture(std::string fileName) override;
		virtual void LoadMetallicTexture(std::string fileName) override;
		virtual void LoadRoughnessTexture(std::string fileName) override;
		virtual void LoadAOTexture(std::string fileName) override;
		virtual void BindTransform(RocketTransform* transform) override;

		// 임시
		virtual void SetMetallic(float metallic) override;
		virtual void SetRoughness(float roughness) override;

	public:
		virtual void Render(ID3D11DeviceContext* deviceContext, const DirectX::XMMATRIX& view, const DirectX::XMMATRIX& proj) override;
		void RenderShadowMap(ID3D11DeviceContext* deviceContext, const DirectX::XMMATRIX& view, const DirectX::XMMATRIX& proj, VertexShader* vs, PixelShader* ps);

	public:
		void SetMaterial(Material* val) { _material = val; }
		VertexShader* SetVertexShader(VertexShader* shader);
		PixelShader* SetPixelShader(PixelShader* shader);
		void SetRenderState(ID3D11RasterizerState* renderState);
		DirectX::BoundingBox GetBoundingBox() const;

	private:
		void SetNodeBuffer(Node* node, NodeBufferType* nodeBuffer);
		// Node 인덱스화 시켜서 Matrix쓰려고 만드는 재귀함수.. 임시
		void MakeNodeVector(Node* node, std::vector<Node*>* vec);

	private:
		RocketTransform* _transform;
		StaticModel* _model;
		Material* _material;
		DirectX::XMMATRIX _worldTM;
		bool _isActive;
		DirectX::BoundingBox _boundingBox;		// frustumCulling 용
	};
}
