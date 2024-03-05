#pragma once
#include <windows.h>
#include <string>
#include <vector>
#include "include/KeyCode.h"
#include "include/EnumType.h"
#include "../FloaterUtil/include/Timer.h"

namespace flt
{
	class IRenderer;


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
		KeyData GetGamePad(int playerNum);
		//virtual void OnClosed();
		void ShowCursor(bool isShow);

	private:
		void UpdateKeyState();

		void HandleKeyboardRawData(const RAWKEYBOARD& data);
		void HandleMouseRawData(const RAWMOUSE& data);
		void HandleGamePadRawData(const RAWHID& data);

		void SetKeyState(KeyCode code, const KeyData& data, bool isActive, bool isInActive);
		
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

	private:
		static unsigned char _keyCodeMap[256];
	};
}
