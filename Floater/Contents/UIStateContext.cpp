#include "UIStateContext.h"
#include "../FloaterGameEngine/include/Input.h"

UIStateContext::UIStateContext()
	: _elements()
	, _downKeyCodes()
	, _isControllable(false)
{

}

void UIStateContext::Update(float deltaSecond)
{
	for (auto& element : _elements)
	{
		element.second.Update(deltaSecond);
	}
}

void UIStateContext::PostUpdate(float deltaSecond)
{
	for (auto& key : _downKeyCodes)
	{
		if (flt::GetKeyDown(key))
		{
			for (auto& element : _elements)
			{
				element.second.Select(key);
			}

		}
	}
}

void UIStateContext::OnEnable()
{
	for (auto& element : _elements)
	{
		element.second.Enable();
	}
}

void UIStateContext::OnDisable()
{
	for (auto& element : _elements)
	{
		element.second.Disable();
	}
}

UIStateContext::UIElement& UIStateContext::AddElement(const std::string& name, MenuItem* item)
{
	_elements.emplace(name, UIElement(name, item, this));

	return _elements[name];
}

UIStateContext::UIElement& UIStateContext::GetElement(const std::string& name)
{
	return _elements[name];
}

UIStateContext& UIStateContext::AddKey(flt::KeyCode keyCode)
{
	_downKeyCodes.emplace(keyCode);
	return *this;
}
