#include "InputSystem.h"

#include "SceneSystem.h"
#include "Scene.h"
#include "Camera.h"

#include "TimeSystem.h"

namespace Rocket::Core
{
	InputSystem::InputSystem()
		: _hWnd(),
		_currentMousePosition(0,0),
		_previousMousePosition(0,0),
		_screenWidth(),
		_screenHeight(),
		_widthOffset(),
		_heightOffset(),
		_mousePoint(),
		_mouseSensitivity(0.5f),
		_currentMouseDelta(0,0),
		_previousMouseDelta(0,0),
		currentKeyState(),
		previousKeyState()
	{

	}

	void InputSystem::Initialize(HWND hWnd, int screenWidth, int screenHeight)
	{
		_hWnd = hWnd;
		_screenWidth = screenWidth;
		_screenHeight = screenHeight;

		_widthOffset = (screenWidth - 1920) / 2;
		_heightOffset = 0;

		_mouseSensitivity = 1.0f;
	}

	void InputSystem::Finalize()
	{

	}

	void InputSystem::InputUpdate()
	{
		if (_hWnd != GetFocus())
		{
			return;
		}

		for (int keyCode = 0; keyCode < 0xB0; keyCode++)
		{
			currentKeyState[keyCode] = GetAsyncKeyState(keyCode) & 0x8000;
		}
		POINT mousePos;
		GetCursorPos(&mousePos);
		_mousePoint = mousePos;
		ScreenToClient(_hWnd, &mousePos);
		_currentMousePosition = { static_cast<float>(mousePos.x - _widthOffset), static_cast<float>(mousePos.y - _heightOffset) };
	}

	void InputSystem::InputUpdateForEditor()
	{
		for (int keyCode = 0; keyCode < 0xB0; keyCode++)
		{
			currentKeyState[keyCode] = GetAsyncKeyState(keyCode) & 0x8000;
		}

		GetCursorPos(&_mousePoint);
		_currentMousePosition = { static_cast<float>(_mousePoint.x), static_cast<float>(_mousePoint.y) };
	}

	void InputSystem::Flush()
	{
		for (int i = 0; i < 0xB0; i++)
		{
			previousKeyState[i] = currentKeyState[i];
			currentKeyState[i] = false;
		}

		_previousMousePosition = _currentMousePosition;

		if (_hWnd != GetFocus())
		{
			return;
		}

		RECT windowRect;
		GetWindowRect(_hWnd, &windowRect);

		/// 마우스 위치 이동 방식
// 		POINT mousePoint;
// 
// 		LONG x = 0;
// 		LONG y = 0;
// 
// 		if (windowRect.right - 1 <= _mousePoint.x)
// 		{
// 			x = windowRect.left + 2;
// 			y = _mousePoint.y;
// 			mousePoint = { x, y };
// 			ScreenToClient(_hWnd, &mousePoint);
// 			_previousMousePosition = { static_cast<float>(mousePoint.x - _widthOffset), static_cast<float>(mousePoint.y - _heightOffset) };
// 			mouse_event(MOUSEEVENTF_MOVE | MOUSEEVENTF_ABSOLUTE, x * 65535 / GetSystemMetrics(SM_CXSCREEN), y * 65535 / GetSystemMetrics(SM_CYSCREEN), 0, 0);
// 		}
// 		else if (_mousePoint.x <= windowRect.left + 1)
// 		{
// 			x = windowRect.right - 2;
// 			y = _mousePoint.y;
// 			mousePoint = { x, y };
// 			ScreenToClient(_hWnd, &mousePoint);
// 			_previousMousePosition = { static_cast<float>(mousePoint.x - _widthOffset), static_cast<float>(mousePoint.y - _heightOffset) };
// 			mouse_event(MOUSEEVENTF_MOVE | MOUSEEVENTF_ABSOLUTE, x * 65535 / GetSystemMetrics(SM_CXSCREEN), y * 65535 / GetSystemMetrics(SM_CYSCREEN), 0, 0);
// 		}
// 		if (windowRect.bottom - 1 <= _mousePoint.y)
// 		{
// 			x = _mousePoint.x;
// 			y = windowRect.top + 2;
// 			mousePoint = { x, y };
// 			ScreenToClient(_hWnd, &mousePoint);
// 			_previousMousePosition = { static_cast<float>(mousePoint.x - _widthOffset), static_cast<float>(mousePoint.y - _heightOffset) };
// 			mouse_event(MOUSEEVENTF_MOVE | MOUSEEVENTF_ABSOLUTE, x * 65535 / GetSystemMetrics(SM_CXSCREEN), y * 65535 / GetSystemMetrics(SM_CYSCREEN), 0, 0);
// 		}
// 		else if (_mousePoint.y <= windowRect.top + 1)
// 		{
// 			x = _mousePoint.x;
// 			y = windowRect.bottom - 2;
// 			mousePoint = { x, y };
// 			ScreenToClient(_hWnd, &mousePoint);
// 			_previousMousePosition = { static_cast<float>(mousePoint.x - _widthOffset), static_cast<float>(mousePoint.y - _heightOffset) };
// 			mouse_event(MOUSEEVENTF_MOVE | MOUSEEVENTF_ABSOLUTE, x * 65535 / GetSystemMetrics(SM_CXSCREEN), y * 65535 / GetSystemMetrics(SM_CYSCREEN), 0, 0);
// 		}
	}

	void InputSystem::FlushForEditor()
	{
		for (int i = 0; i < 0xB0; i++)
		{
			previousKeyState[i] = currentKeyState[i];
			currentKeyState[i] = false;
		}

		_previousMousePosition = _currentMousePosition;
	}

	bool InputSystem::GetKeyDown(int keyCode)
	{
		if (!(0x00 <= keyCode && keyCode < 0xB0))
		{
			return false;
		}

		return !previousKeyState[keyCode] && currentKeyState[keyCode];
	}

	bool InputSystem::GetKeyUp(int keyCode)
	{
		if (!(0x00 <= keyCode && keyCode < 0xB0))
		{
			return false;
		}

		return previousKeyState[keyCode] && !currentKeyState[keyCode];
	}

	bool InputSystem::GetKey(int keyCode)
	{
		if (!(0x00 <= keyCode && keyCode < 0xB0))
		{
			return false;
		}

		return previousKeyState[keyCode] && currentKeyState[keyCode];
	}

	Vector2 InputSystem::GetMousePosition() const
	{
		return _currentMousePosition;
	}

	Vector2 InputSystem::GetMouseDelta() const
	{
		//return (_c urrentMousePosition - Rocket::Vector2(500.0f, 500.0f)) * TimeSystem::GetDeltaTime();
		return (_currentMousePosition - _previousMousePosition) * _mouseSensitivity;
	}

	Vector2 InputSystem::MouseDeltaSmooth()
	{
		_currentMouseDelta = _currentMousePosition - _previousMousePosition;

		Vector2 result = (_currentMouseDelta + _previousMouseDelta);
		result.x /= 2.0f;
		result.y /= 2.0f;

		_previousMouseDelta = _currentMouseDelta;

		return result;
	}

	float InputSystem::GetMouseSensitivity() const
	{
		return _mouseSensitivity;
	}

	void InputSystem::SetMouseSensitivity(float sensitivity)
	{
		if (sensitivity < 0.0674f)
		{
			sensitivity = 0.0674f;
		}

		if (sensitivity > 2.579f)
		{
			sensitivity = 2.579f;
		}

		_mouseSensitivity = sensitivity;
	}
}
