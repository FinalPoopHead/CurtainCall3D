﻿#pragma once


#ifdef FLT_WINDOWS
#pragma comment(lib, "FloaterPlatform_Windows.lib")
#elif  FLT_LINUX
#pragma comment(lib, "");
#endif

#include <string>
#include "KeyCode.h"
#include "GamePad.h"
#include "EnumType.h"
#include "../FloaterRendererCommon/include/IRenderer.h"
#include "../FloaterUtil/include/FloaterType.h"
#include "../FloaterMath/include/Vector2f.h"


namespace flt
{
	class IRenderer;

	class Platform
	{
		class impl;

	public:
		Platform();
		Platform(bool useConsole);
		~Platform();

	public:
		[[nodiscard]] bool Initialize(int pixelWidth, int pixelHeight, std::wstring title, std::wstring imgPath);
		[[nodiscard]] bool Finalize();
		[[nodiscard]] bool Update(float deltaSeconds);

		[[nodiscard]] IRenderer* CreateRenderer(RendererType type);
		void DestroyRenderer(IRenderer* renderer);

		[[nodiscard]] Vector2f GetWindowSize();
		void SetWindowTitle(const std::wstring& title);

		[[nodiscard]] KeyData GetKey(KeyCode code);
		[[nodiscard]] KeyData GetKeyDown(KeyCode code);
		[[nodiscard]] KeyData GetKeyUp(KeyCode code);

		[[nodiscard]] bool GamePadConnected(int padIndex);
		[[nodiscard]] bool GetGamePadState(int padIndex, GamePadState* outState);
		[[nodiscard]] bool SetGamePadVibration(int padIndex, bool isRightMoter, float moterPower, float time);

		[[nodiscard]] bool SetGamePadAnalogDeadZone(int padIndex, GamePadDeadZone* deadZone);
		std::vector<int> GetGamePadIndexList();

		void ShowCursor(bool isShow);

		std::wstring GetExePath();
		std::wstring GetAbsPath(std::wstring relativePath);

		void Exit();

		//virtual void OnResize() {};
		//virtual void OnFullScreen() {};
		//virtual void OnClosed() {};

	private:
		impl* _pOsImpl;
	};
}
