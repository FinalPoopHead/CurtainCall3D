#include "UIStateContext.h"
#include "../FloaterGameEngine/include/Input.h"

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
