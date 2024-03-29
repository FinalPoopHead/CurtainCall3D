#include "./include/Renderer.h"
#include "./include/GameObject.h"
#include "./include/Scene.h"
#include "../FloaterRendererCommon/include/IRenderer.h"
#include "../FloaterRendererCommon/include/RawNode.h"

flt::Renderer::Renderer(GameObject& gameObject) : _gameObject(gameObject)
{

}

void flt::Renderer::SetRawNode(const RawNode& rawNode)
{
	RendererObject rendererObject{ _gameObject.transform, _isEnable};


}
