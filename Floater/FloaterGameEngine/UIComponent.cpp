#include "./include/internal/UIComponent.h"
#include "../FloaterRendererCommon/include/IRenderer.h"
#include "./include/internal/GameObject.h"
#include "./include/internal/Scene.h"
#include "./include/internal/GameEngine.h"
#include "Image.h"
#include <filesystem>

flt::UIComponent::UIComponent()
	: _rendererObject(new RendererObject{ _isDraw })
	, _renderer(*GameEngine::Instance()->GetRenderer())
	, _hObject()
	, _isDraw(false)
	, _isRegisted(false)
	, _isOffsetMode(false)
	, _image(new flt::Image())
	, _size(0.0f, 0.0f)
	, _position(0.0f, 0.0f)
	, _zOrder(0.0f)
	, _textAlignment(eTextAlignment::LEFT)
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

void flt::UIComponent::PreRender()
{
	// 화면 해상도가 바뀔 수 있으니 렌더링 전에 반드시 위치를 업데이트 해줘야 함
	if (_isOffsetMode)
	{
		SetPositionUseOffset();
		UpdateGameObjectPosition();
	}
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
	_rendererObject->imgPath = filePath;

	_image->Set(filePath);

	int w = 0;
	int h = 0;
	_image->GetSize(w, h);
	_size.x = static_cast<float>(w);
	_size.y = static_cast<float>(h);

	Vector4f scale = _gameObject->tr.GetLocalScale();
	_size.x *= scale.x;
	_size.y *= scale.y;

	if (_isRegisted)
	{
		_renderer.DeregisterObject(_hObject);
		_hObject = _renderer.RegisterObject(*_rendererObject);
	}
}

void flt::UIComponent::SetImageColor(flt::Vector4f color)
{
	_rendererObject->imgColor = color;
	if (_isRegisted)
	{
		_renderer.DeregisterObject(_hObject);
		_hObject = _renderer.RegisterObject(*_rendererObject);
	}
}

void flt::UIComponent::SetPosition(flt::Vector2f pixelPos)
{
	_isOffsetMode = false;
	_position = pixelPos;
	UpdateGameObjectPosition();
}

flt::Vector2f flt::UIComponent::GetPosition()
{
	return _position;
}

void flt::UIComponent::SetOffsetPosition(flt::Vector2f offsetPos)
{
	_isOffsetMode = true;
	//ASSERT(offsetPos.x >= 0.0f && offsetPos.x <= 1.0f, "Offset x is out of range");
	//ASSERT(offsetPos.y >= 0.0f && offsetPos.y <= 1.0f, "Offset y is out of range");

	_offsetPosition = offsetPos;

	SetPositionUseOffset();
}

flt::Vector2f flt::UIComponent::GetOffsetPosition()
{
	return _offsetPosition;
}

bool flt::UIComponent::IsOffsetMode()
{
	return _isOffsetMode;
}

void flt::UIComponent::SetZOrder(float zOrder)
{
	_zOrder = zOrder;
	UpdateGameObjectPosition();
}

float flt::UIComponent::GetZOrder()
{
	return _zOrder;
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
	//Vector2f currSize = GetSize();
	int w = 0;
	int h = 0;
	_image->GetSize(w, h);

	_gameObject->tr.SetScale(imgSize.x / (float)w, imgSize.y / (float)h, 1.0f);

	_size = imgSize;

	UpdateGameObjectPosition();
}

flt::Vector2f flt::UIComponent::GetSize()
{
	return _size;
}

void flt::UIComponent::SetTextAlignment(eTextAlignment alignMode)
{
	_rendererObject->text.textAlign = alignMode;

	if (_isRegisted)
	{
		_renderer.DeregisterObject(_hObject);
		_hObject = _renderer.RegisterObject(*_rendererObject);
	}
}

void flt::UIComponent::SetText(const std::wstring& text)
{
	_rendererObject->text.data = text;

	if (_isRegisted)
	{
		_renderer.DeregisterObject(_hObject);
		_hObject = _renderer.RegisterObject(*_rendererObject);
	}
}

void flt::UIComponent::SetFont(const std::wstring& fontPath)
{
	std::filesystem::path p(fontPath);
	ASSERT(std::filesystem::exists(p), "File not found");

	_rendererObject->text.font = fontPath;
	if (_isRegisted)
	{
		_renderer.DeregisterObject(_hObject);
		_hObject = _renderer.RegisterObject(*_rendererObject);
	}
}

void flt::UIComponent::SetFontSize(int fontSize)
{
	_rendererObject->text.size = fontSize;
}

void flt::UIComponent::SetTextColor(flt::Vector4f color)
{
	_rendererObject->text.color = color;
	if (_isRegisted)
	{
		_renderer.DeregisterObject(_hObject);
		_hObject = _renderer.RegisterObject(*_rendererObject);
	}
}

void flt::UIComponent::SetTextColor(float r, float g, float b)
{
	SetTextColor({ r, g, b, 1.0f });
}

void flt::UIComponent::UpdateGameObjectPosition()
{
	_gameObject->tr.SetPosition(_position.x - _size.x / 2, _position.y - _size.y / 2, _zOrder);
}

void flt::UIComponent::SetPositionUseOffset()
{
	ASSERT(!_gameObject->tr.GetParent(), "Offset 모드는 계층구조의 자식이면 안됨");
	flt::Vector2f windowSize = GameEngine::Instance()->GetWindowSize();
	_position.x = _offsetPosition.x * windowSize.x;
	_position.y = _offsetPosition.y * windowSize.y;
}
