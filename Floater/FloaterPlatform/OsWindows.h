#pragma once
#include <windows.h>
#include <string>
#include <vector>
#include <unordered_map>
#include "include/KeyCode.h"
#include "include/EnumType.h"
#include "../FloaterUtil/include/Timer.h"
#include "./WinGamePadImpl.h"
#include "../FloaterUtil/include/FloaterType.h"


namespace flt
{
	class IRenderer;
	struct GamePadState;
	struct WinGamePad;

	class OsWindows
	{
	public:
		static const std::wstring s_name;
	public:
		OsWindows(bool useConsole);
		virtual ~OsWindows();
		bool Initialize(int windowWidth, int windowHeight, const std::wstring& title, const std::wstring& imgPath);
		bool Finalize();
		bool Update();

		IRenderer* CreateRenderer(RendererType type);
		void DestroyRenderer(IRenderer* renderer);

		KeyData GetKey(KeyCode code);
		bool GetGamePadState(int padIndex, GamePadState* outState);
		bool SetGamePadVibration(int padIndex, float leftMotor, float rightMotor);
		std::vector<int> GetGamePadIndexList();

		//virtual void OnClosed();
		void ShowCursor(bool isShow);

		std::wstring GetExePath();

	private:
		void UpdateKeyState();

		void HandleKeyboardRawData(const RAWKEYBOARD& data);
		void HandleMouseRawData(const RAWMOUSE& data);
		void HandleGamePadRawData(const RAWINPUT* raw);

		void SetKeyState(KeyCode code, const KeyData& data, bool isActive, bool isInActive);

		WinGamePad* FindEmptyGamePad(uint64 hash);
		
		static LRESULT WINAPI WinProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
		bool GetError(std::wstring* outErrorMsg, unsigned int* outErrorCode);

	protected:
		HWND _hwnd;
		bool _isActivated;

		Timer _keyTimer;
		bool* _pKeyStates;
		KeyData* _pKeyDatas;
		std::vector<int> _keyUp;
		bool _isShowCursor;

		HWND _consoleHwnd;
		std::wstring _exePath;

	private:
		static unsigned char _keyCodeMap[256];
		std::unordered_map<IRenderer*, RendererType> _rendererMap;

		WinGamePad _pGamePads[16];
	};
}
