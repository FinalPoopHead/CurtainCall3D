#pragma once
#include "DLLExporter.h"
#include "UIRenderer.h"
#include <functional>

namespace Rocket
{
	class GameObject;

	class ROCKET_API Button : public Rocket::Core::UIRenderer
	{
	public:
		Button();

	protected:
		virtual void LateUpdate() override;

	protected:
		virtual void OnFocusEvent() override;

		/// Button Event.
	public:
		void SetOnClickEvent(std::function<void()> func);
		std::function<void()> GetOnClickEvent() const;

	private:
		std::function<void()> _onClickEvent;
	};
}
