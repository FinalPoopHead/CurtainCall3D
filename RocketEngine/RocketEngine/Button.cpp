#include "Button.h"
#include "GameObject.h"
#include "Transform.h"
#include "InputSystem.h"
#include "DebugSystem.h"
#include "MathHeader.h"

namespace Rocket
{
	Button::Button()
		: _onClickEvent()
	{

	}

	void Button::LateUpdate()
	{
		// 디버그 정보 출력.
		Vector3 worldPos = gameObject->transform.GetPosition();
		Vector2 LT = { worldPos.x - _width / 2, worldPos.y - _height / 2 };
		Vector2 RB = { worldPos.x + _width / 2, worldPos.y + _height / 2 };
		Vector4 color = { 1.0f,0.0f,0.0f,1.0f };
		Rocket::Core::DebugSystem::Instance().DrawDebug2DBox(LT, RB, color);
	}

	void Button::OnFocusEvent()
	{
		if (!_onClickEvent)
		{
			return;
		}

		_onClickEvent();
	}

	void Button::SetOnClickEvent(std::function<void()> func)
	{
		_onClickEvent = func;
	}

	std::function<void()> Button::GetOnClickEvent() const
	{
		return _onClickEvent;
	}
}
