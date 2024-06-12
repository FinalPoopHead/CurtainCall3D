#include "./include/internal/UIComponent.h"
#include "../FloaterRendererCommon/include/IRenderer.h"
#include "./include/internal/GameObject.h"
#include "./include/internal/Scene.h"
#include "./include/internal/GameEngine.h"
#include "Image.h"

#include <filesystem>

flt::UIComponent::UIComponent() :
	_rendererObject(new RendererObject{ _isDraw }),
	_renderer(*GameEngine::Instance()->GetRenderer()),
	_hObject(),
	_isDraw(false),
	_isRegisted(false),
	_image(new flt::Image())
{
}

flt::UIComponent::~UIComponent()
{
	delete _rendererObject;
}

void flt::UIComponent::OnCreate()
{
	_rendererObject->transform = &_gameObject->transform;

	if (!_hObject)
	{
		_hObject = _renderer.RegisterObject(*_rendererObject);
		_isRegisted = true;
	}
}

void flt::UIComponent::OnEnable()
{
	_isDraw = true;
}

void flt::UIComponent::OnDisable()
{
	_isDraw = false;
}

void flt::UIComponent::OnDestroy()
{
	_isDraw = false;

	if (_hObject)
	{
		_renderer.DeregisterObject(_hObject);
		_isRegisted = false;
		_hObject = 0;
	}
}

void flt::UIComponent::SetImage(const std::wstring& filePath)
{
	std::filesystem::path p(filePath);
	ASSERT(std::filesystem::exists(p), "File not found");
	std::wstring absPath = std::filesystem::absolute(p);
	_rendererObject->imgName = filePath;

	_image->Set(filePath);

	int w = 0;
	int h = 0;
	_image->GetSize(w, h);
	_size.x = static_cast<float>(w);
	_size.y = static_cast<float>(h);

	if (_isRegisted)
	{
		_renderer.DeregisterObject(_hObject);
		_hObject = _renderer.RegisterObject(*_rendererObject);
	}
}

void flt::UIComponent::SetPosition(flt::Vector2f pixelPos)
{
	_position = pixelPos;
	_gameObject->tr.SetPosition(_position.x - _size.x / 2, _position.y - _size.x / 2, 0.0f);
}

flt::Vector2f flt::UIComponent::GetPosition()
{
	return _position;
}

flt::Vector2f flt::UIComponent::GetImageSize()
{
	int w = 0;
	int h = 0;
	_image->GetSize(w, h);

	return flt::Vector2f(static_cast<float>(w), static_cast<float>(h));
}


void flt::UIComponent::SetSize(flt::Vector2f imgSize)
{
	Vector2f currSize = GetSize();

	_gameObject->tr.SetScale(imgSize.x / currSize.x, imgSize.y / currSize.y, 1.0f);

	_size = imgSize;

	SetPosition(_position);
}

flt::Vector2f flt::UIComponent::GetSize()
{
	return _size;
}
