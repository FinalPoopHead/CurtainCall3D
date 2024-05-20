#include "ShadowPass.h"
#include "VertexShader.h"
#include "PixelShader.h"
#include "DeferredBuffers.h"
#include "ObjectManager.h"
#include "DirectionalLight.h"
#include "IRenderable.h"
#include "MeshRenderer.h"
#include "DynamicModelRenderer.h"

namespace Rocket::Core
{

	ShadowPass::ShadowPass()
	{

	}

	ShadowPass::~ShadowPass()
	{

	}

	void ShadowPass::Initialize(VertexShader* staticMeshVS, VertexShader* dynamicModelVS, PixelShader* shadowMapPS)
	{
		_staticMeshVS = staticMeshVS;
		_dynamicModelVS = dynamicModelVS;
		_shadowMapPS = shadowMapPS;
	}

	void ShadowPass::GenerateShadowMap(ID3D11DeviceContext* deviceContext, DeferredBuffers* g_buffer)
	{
		auto& objMgr = ObjectManager::Instance();

		g_buffer->SetShadowMapRenderTarget(deviceContext);

		// TODO : 라이트별로 물체를 컬링해서.. 그림자를 만들어내는..? 음..
		// 그러면 ShadowMap이 라이트 갯수만큼있어야..?
		for (auto& light : objMgr.GetDirectionalLightList())
		{
			std::vector<MeshRenderer*> staticMeshList;
			staticMeshList.reserve(256);

			std::vector<DynamicModelRenderer*> dynamicModelList;
			dynamicModelList.reserve(256);

			for (auto meshRenderer : objMgr.GetStaticMeshRenderers())
			{
/*				staticMeshList.push_back(meshRenderer);*/

				if (light->FrustumCulling(meshRenderer->GetBoundingBox()))
				{
					staticMeshList.push_back(meshRenderer);
				}
			}

			for (auto dynamicModelRenderer : objMgr.GetDynamicModelRenderers())
			{
/*				dynamicModelList.push_back(dynamicModelRenderer);*/

				if (light->FrustumCulling(dynamicModelRenderer->GetBoundingBox()))
				{
					dynamicModelList.push_back(dynamicModelRenderer);
				}
			}

			// TODO : 지금 각각의 렌더러가 Render할때는 머터리얼의 정보도 전부 포함해서 그린다.
			//			하지만 ShadowMap 만들때는 필요가 없단말이다..
			//			우선은 임시로 함수 하나 만들어서 때웠다.
			// Draw On ShadowMap
			for (auto& meshRenderer : staticMeshList)
			{
				meshRenderer->RenderShadowMap(deviceContext, light->GetViewMatrix(), light->GetProjectionMatrix(), _staticMeshVS, _shadowMapPS);
			}

			for (auto& modelRenderer : dynamicModelList)
			{
				modelRenderer->RenderShadowMap(deviceContext, light->GetViewMatrix(), light->GetProjectionMatrix(), _dynamicModelVS, _shadowMapPS);
			}
		}
	}

}
