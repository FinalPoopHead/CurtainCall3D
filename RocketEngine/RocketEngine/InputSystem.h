#pragma once
#include "Singleton.h"
#include <windows.h>
#include "MathHeader.h"

namespace Rocket::Core
{
	class InputSystem : public Singleton<InputSystem>
	{
		friend Singleton;
	private:
		InputSystem();		// 싱글턴이기 때문에 외부에서 생성할 수 없도록.

	public:
		void Initialize(HWND hWnd, int screenWidth, int screenHeight);
		void Finalize();

	public:
		void InputUpdate();
		void InputUpdateForEditor();
		void Flush();
		void FlushForEditor();

	private:
		HWND _hWnd;

	public:
		bool GetKeyDown(int keyCode);
		bool GetKeyUp(int keyCode);
		bool GetKey(int keyCode);

		/// 키는 0xFF, 총 256개까지 받을 수 있게 할 수도 있지만
		/// 그냥 빨리 돌리고 끝내고싶어서 갯수를 최적화 해봤음.
	private:
		bool currentKeyState[0xB0];
		bool previousKeyState[0xB0];

	public:
		Vector2 GetMousePosition() const;
		Vector2 GetMouseDelta() const;
		Vector2 MouseDeltaSmooth();
		float GetMouseSensitivity() const;
		void SetMouseSensitivity(float sensitivity);

	private:
		float _mouseSensitivity;
		POINT _mousePoint;
		Vector2 _currentMousePosition;
		Vector2 _previousMousePosition;
		Vector2 _currentMouseDelta;
		Vector2 _previousMouseDelta;

	private:
		int _screenWidth;
		int _screenHeight;
		int _widthOffset;
		int _heightOffset;
	};
}
