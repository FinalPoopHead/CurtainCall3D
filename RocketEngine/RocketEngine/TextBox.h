#pragma once
#include <string>
#include "UIRenderer.h"
#include "DLLExporter.h"
#include "MathHeader.h"
#include "../GraphicsInterface/ITextRenderer.h"

namespace Rocket::Core
{
	class InputSystem;
}

namespace Rocket
{
	class GameObject;
}

namespace Rocket
{
	/// <summary>
	/// Text 정보를 담고 출력을 담당하는 컴포넌트.
	/// TextBox의 역할도 한다.
	/// 
	/// 23.08.11 강석원
	/// </summary>
	class ROCKET_API TextBox : public Rocket::Core::UIRenderer
	{
	public:
		TextBox();

	protected:
		virtual void Update() override;
		virtual void LateUpdate() override;
		virtual void UpdateRenderData() override;

	public:
		std::string GetText() const;
		void SetText(std::string text);

		void AppendText(std::string text);
		void AppendText(int keyCode);
		void PopBackText();

	public:
		Color GetFontColor() const;
		void SetFontColor(Color color);

	private:
		Core::ITextRenderer* _textRenderer;

		Core::InputSystem& _inputSystem;
	};
}
