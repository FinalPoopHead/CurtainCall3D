#include "TextBox.h" 
#include "InputSystem.h"
#include "GameObject.h"
#include "Transform.h"
#include "DebugSystem.h"
#include "TimeSystem.h"
#include "GraphicsSystem.h"

namespace Rocket
{
	float downTime = 0.0f;

	TextBox::TextBox()
		: _textRenderer(Core::GraphicsSystem::Instance().GetFactory()->CreateTextRenderer()),
		_inputSystem(Core::InputSystem::Instance())
	{

	}

	void TextBox::Update()
	{
		if (!_isFocused)
		{
			return;
		}

		for (int keyCode = 0x30; keyCode <= 0x5A; keyCode++)
		{
			if (_inputSystem.GetKeyDown(keyCode))
			{
				AppendText(keyCode);
			}
		}

		if (_inputSystem.GetKeyDown(VK_BACK))
		{
			PopBackText();
		}

		if (_inputSystem.GetKey(VK_BACK))
		{
			downTime += Rocket::Core::TimeSystem::Instance().GetDeltaTime();

			if (downTime >= 0.5f)
			{
				PopBackText();
			}
		}

		if (_inputSystem.GetKeyUp(VK_BACK))
		{
			downTime = 0.0f;
		}

		if (_inputSystem.GetKeyDown(VK_DELETE))
		{
			PopBackText();
		}

		if (_inputSystem.GetKey(VK_DELETE))
		{
			downTime += Rocket::Core::TimeSystem::Instance().GetDeltaTime();

			if (downTime >= 0.5f)
			{
				PopBackText();
			}
		}

		if (_inputSystem.GetKeyUp(VK_DELETE))
		{
			downTime = 0.0f;
		}
	}

	void TextBox::LateUpdate()
	{
		// 디버그 정보 출력.
		Vector3 worldPos = gameObject->transform.GetPosition();
		Vector2 LT = { worldPos.x - _width / 2, worldPos.y - _height / 2 };
		Vector2 RB = { worldPos.x + _width / 2, worldPos.y + _height / 2 };
		Vector4 color = { 0.0f,0.0f,1.0f,1.0f };
		Rocket::Core::DebugSystem::Instance().DrawDebug2DBox(LT, RB, color);
	}

	void TextBox::UpdateRenderData()
	{
		_textRenderer->SetWorldTM(gameObject->transform.GetWorldTM());
	}

	void TextBox::SetText(std::string text)
	{
		_textRenderer->SetText(text);
	}

	void TextBox::AppendText(std::string text)
	{
		std::string temp = _textRenderer->GetText();
		temp += text;
		_textRenderer->SetText(temp);
	}

	void TextBox::AppendText(int keyCode)
	{
		std::string temp = _textRenderer->GetText();
		temp += keyCode;
		_textRenderer->SetText(temp);
	}

	void TextBox::PopBackText()
	{
		std::string& temp = _textRenderer->GetText();
		if (temp.size() > 0)
		{
			temp.pop_back();
		}
	}

	std::string TextBox::GetText() const
	{
		return _textRenderer->GetText();
	}

	Color TextBox::GetFontColor() const
	{
		return _textRenderer->GetColor();
	}

	void TextBox::SetFontColor(Color color)
	{
		_textRenderer->SetColor(color);
	}

}
