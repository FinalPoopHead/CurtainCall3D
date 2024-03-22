#pragma once
#include <d3d11.h>
#include <dxgi.h>
#include <DirectXMath.h>
#include <wrl.h>
#include <vector>

#include "..\\RocketCommon\\IDynamicModelRenderer.h"
#include "../RocketCommon/GraphicsEnum.h"
#include "../RocketCommon/RocketTransform.h"
#include "ModelStruct.h"
#include "Material.h"

using Microsoft::WRL::ComPtr;

namespace Rocket::Core
{
	class DynamicModelRenderer : public Rocket::Core::IDynamicModelRenderer
	{
	public:
		DynamicModelRenderer();

	public:
		virtual void SetWorldTM(const Matrix& worldTM) override;
		virtual void SetActive(bool isActive) override;
		virtual void LoadModel(const std::string& fileName) override;
		virtual void LoadTexture(std::string fileName) override;
		virtual void BindTransform(RocketTransform* rootTransform) override;

	public:
		void UpdateAnimation(float deltaTime);			// 깊은 복사 해온 Node 데이터에 애니메이션 데이터를 적용한다.
		void Render(ID3D11DeviceContext* deviceContext, const DirectX::XMMATRIX& view, const DirectX::XMMATRIX& proj);

	public:
		void SetMaterial(Material* val) { _material = val; }
		void SetVertexShader(VertexShader* shader);
		void SetPixelShader(PixelShader* shader);
		void SetRenderState(ID3D11RasterizerState* renderState);

	private:
		void CalcNodeWorldMatrix(Node* node);		// TODO : 이거 여기서 이렇게 하는게 맞나? Node에서 알아서 하게끔 해야될거같은데..
		void SetNodeBuffer(Node* node, NodeBufferType* nodeBuffer);
		void SetBoneBuffer(Node* node, BoneBufferType* boneBuffer);
		Node* CopyNodeData(Node* originalRootNode);
		void CopyNodeDataRecur(Node* from, Node* to);
		void BindTransformRecur(RocketTransform* transform, Node* node);

	private:
		// TODO : 이거 상속구조 잘 만들던가.. 어떻게든 해서 ModelData와 SkinnedModelData 잘 나눠보자..ㅠㅠ
		// 지금은 DynamicModel에 애니메이션 정보도 다 때려박고 사용하는 중. 나중에 꼭 분리하자.
		DynamicModel* _model;
		Material* _material;
		DirectX::XMMATRIX _worldTM;
		bool _isActive;

	/// <summary>
	/// 애니메이션 관련 멤버 변수.
	/// 나중에는 따로 빼서 관리하는게 낫겠다.
	/// </summary>
	private:
		std::string _nowAnimationName;
		Node* _animatedRootNode;		// 애니메이션을 적용한 루트 노드 (원본에서 깊은 복사해옴)
		RocketTransform* _rootTransform;	// 앞단에서의 GameObject의 transform의 최상위 부모
		std::unordered_map<std::string, Node*> _animatedNodeMap;	// 애니메이션을 적용한 노드들의 맵 (원본에서 깊은 복사 해옴)
		double _animationTime;
		double _animationTick;
		bool _isLoop = true;		// TODO : 지금은 임시로 Looping하도록 해뒀음. 나중에는 외부에서 설정할 수 있게 바꿔야함.

	private:
		UINT testCount = 0;
	};
}
