#include "./include/internal/RendererComponent.h"
#include "../FloaterRendererCommon/include/IRenderer.h"
#include "./include/internal/GameObject.h"
#include "./include/internal/Scene.h"
#include "./include/internal/GameEngine.h"
#include "../FloaterRendererCommon/include/RawSkeleton.h"
#include <filesystem>


flt::RendererComponent::RendererComponent()
	: _rendererObject(new RendererObject{ _isDraw })
	, _renderer(*GameEngine::Instance()->GetRenderer())
	, _hObject()
	, _isDraw(false)
	, _isRegisted(false)
{

}

flt::RendererComponent::~RendererComponent()
{
	delete _rendererObject;
}

void flt::RendererComponent::OnCreate()
{
	_rendererObject->transform = &_gameObject->transform;

	if (!_hObject)
	{
		_hObject = _renderer.RegisterObject(*_rendererObject);
		_isRegisted = true;
	}
}

void flt::RendererComponent::OnEnable()
{
	_isDraw = true;
}

void flt::RendererComponent::OnDisable()
{
	_isDraw = false;
}

void flt::RendererComponent::OnDestroy()
{
	_isDraw = false;

	if (_hObject)
	{
		_renderer.DeregisterObject(_hObject);
		_isRegisted = false;
		_hObject = 0;
	}
}

void flt::RendererComponent::SetText(const std::wstring& text)
{
	_rendererObject->text.data = text;
}

void flt::RendererComponent::SetFont(const std::wstring& path)
{
	_rendererObject->text.font = path;
}

void flt::RendererComponent::SetTextSize(uint32 size)
{
	_rendererObject->text.size = size;
}

void flt::RendererComponent::SetImage(const std::wstring& path)
{
	std::filesystem::path p(path);
	ASSERT(std::filesystem::exists(p), "File not found");
	std::wstring absPath = std::filesystem::absolute(p);
	_rendererObject->imgPath = path;

	if (_isRegisted)
	{
		_renderer.DeregisterObject(_hObject);
		_hObject = _renderer.RegisterObject(*_rendererObject);
	}
}

void flt::RendererComponent::SetRawNode(RawNode* rawNode)
{
	_rendererObject->SetRawNode(rawNode);
}

void flt::RendererComponent::SetFilePath(const std::wstring& path)
{
	std::filesystem::path p(path);
	ASSERT(std::filesystem::exists(p), "File not found");
	RenderableBuilder builder(path);
	_resource.Set(builder);
	_rendererObject->SetRawNode(_resource.Get()->node);

	if (_isRegisted)
	{
		_renderer.DeregisterObject(_hObject);
		_hObject = _renderer.RegisterObject(*_rendererObject);
	}
}

void flt::RendererComponent::SetMaterial(uint32 meshIndex, const std::wstring& path, RawMaterial::TextureType type)
{
	_rendererObject->SetMaterial(meshIndex, path, type);

	if (_isRegisted)
	{
		_renderer.DeregisterObject(_hObject);
		_hObject = _renderer.RegisterObject(*_rendererObject);
	}
}

std::vector<std::wstring> flt::RendererComponent::GetAnimaionList() const
{
	std::vector<std::wstring> list;
	if (!_rendererObject->node->skeleton)
	{
		return list;
	}

	list.reserve(_rendererObject->node->skeleton->animations.size());

	for (const auto& animation : _rendererObject->node->skeleton->animations)
	{
		list.push_back(animation.name);
	}

	return list;
}

void flt::RendererComponent::SetCastShadow(bool isCast)
{
	_rendererObject->isCastShadow = isCast;

	if (_isRegisted)
	{
		_renderer.DeregisterObject(_hObject);
		_hObject = _renderer.RegisterObject(*_rendererObject);
	}
}

void flt::RendererComponent::PlayAnimation(uint32 index, bool isLoop)
{
	_rendererObject->animState.index = index;
	_rendererObject->animState.isLoop = isLoop;
	_rendererObject->animState.isPlaying = true;
}

void flt::RendererComponent::StopAnimation()
{
	_rendererObject->animState.isPlaying = false;
}
