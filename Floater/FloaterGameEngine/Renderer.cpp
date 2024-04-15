#include "./include/Renderer.h"
#include "./include/GameObject.h"
#include "./include/Scene.h"
#include "./include/GameEngine.h"
#include "../FloaterRendererCommon/include/RawNode.h"


flt::Renderer::Renderer(GameObject& gameObject) : 
	_gameObject(gameObject), 
	_renderer(*GameEngine::Instance()->GetRenderer()), 
	_hObject()
{
}

flt::Renderer::~Renderer()
{

}

void flt::Renderer::OnEnable()
{
	if (!_hObject)
	{
		RendererObject rendererObject{ _gameObject.transform, _isDraw };

		_renderer.RegisterObject(rendererObject);
	}
}

void flt::Renderer::Update(float deltaSecond)
{
	_isDraw = true;
}

void flt::Renderer::EndDraw()
{
	_isDraw = false;
}

void flt::Renderer::OnDestroy()
{
	if (_hObject)
	{
		_renderer.DeregisterObject(_hObject);
	}
}
