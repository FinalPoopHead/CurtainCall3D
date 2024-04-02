#pragma once
#include <string>

#include "MathHeader.h"
#include "Component.h"
#include "DLLExporter.h"
#include "../RocketCommon/IGraphicsObject.h"

namespace Rocket
{
	class GameObject;
}

namespace Rocket::Core
{
	class ROCKET_API UIRenderer : public Component
	{
	public:
		UIRenderer();

	protected:
		virtual void Start() override;

	public:
		virtual void OnFocusEvent() {};

	public:
		bool CheckFocus();

		/// UI Size.
	public:
		float GetWidth() const;
		float GetHeight() const;

	public:
		/// 스크린 기준 위치 (Scale도 곱해짐)
		float GetLeft() const;		// 스크린 기준 left (Scale도 곱해짐)
		float GetTop() const;		// 스크린 기준 up (Scale도 곱해짐)
		float GetRight() const;		// 스크린 기준 right (Scale도 곱해짐)
		float GetBottom() const;		// 스크린 기준 down (Scale도 곱해짐)
		int GetSortOrder() const;
		bool GetIsFocused() const;
		bool GetIgnoreFocus() const;

		void SetSize(float width, float height);
		void SetWidth(float width);
		void SetHeight(float height);
		void SetSortOrder(int sortOrder);
		void SetIsFocused(bool isFocused);
		void SetIgnoreFocus(bool isIgnore);

	protected:
		float _width;
		float _height;
		int _sortOrder;		// 숫자가 높을수록 앞으로 나옴.
		bool _isFocused;
		bool _ignoreFocus;

	public:
		float GetFillRatio() const;
		void SetFillRatio(float ratio);

	private:
		float _fillRatio = 100.0f;
	};
}


