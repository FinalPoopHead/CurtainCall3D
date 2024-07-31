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
#include "../FloaterMath/include/Vector2f.h"


namespace flt
{
	class IRenderer;
	struct GamePadState;
	//struct WinGamePad;
	struct KeyState;

	class OsWindows
	{
	public:
		static const std::wstring s_name;
	public:
		OsWindows(bool useConsole);
		virtual ~OsWindows();
		bool Initialize(int windowWidth, int windowHeight, const std::wstring& title, const std::wstring& imgPath);
		bool Initialize(uint64 hwnd);
		bool Finalize();
		bool Update(float deltaSeconds);

		IRenderer* CreateRenderer(RendererType type);
		void DestroyRenderer(IRenderer* renderer);

		uint32 GetWindowHandle();

		Vector2f GetWindowSize();
		void SetWindowTitle(const std::wstring& title);
		void SetWindowSize(uint32 width = 0, uint32 height = 0, WindowMode mode = WindowMode::WINDOWED, int monitorIndex = -1);

		KeyData GetKey(KeyCode code);
		KeyData GetKeyDown(KeyCode code);
		KeyData GetKeyUp(KeyCode code);
		bool GamePadConnected(int padIndex);
		bool GetGamePadState(int padIndex, GamePadState* outState);
		[[deprecated("Use bool SetGamePadVibration(int padIndex, bool isRightMoter, float moterPower, float time)")]] bool SetGamePadVibration(int padIndex, float leftMotor, float rightMotor);
		bool SetGamePadVibration(int padIndex, bool isRightMoter, float moterPower, float time);
		bool SetGamePadAnalogDeadZone(int padIndex, GamePadDeadZone* deadZone);
		std::vector<int> GetGamePadIndexList();

		//virtual void OnClosed();
		void ShowCursor(bool isShow);

		std::wstring GetExePath();
		std::wstring GetAbsPath(std::wstring relativePath);

		void Exit();

	private:
		bool InitializeXBOXController(HWND hwnd);
		void InitializeExePath();

		void UpdateKeyState();
		void UpdateGamePadVibration(float deltaSeconds);

		void HandleKeyboardRawData(const RAWKEYBOARD& data);
		void HandleMouseRawData(const RAWMOUSE& data);
		void HandleGamePadRawData(const RAWINPUT* raw);

		void SetKeyState(KeyCode code, const KeyData& data, bool isKeyDown, bool isKeyUp);
		void SetKeyState(KeyCode code, const KeyData& data, bool isActive);

		WinGamePad* FindEmptyGamePad(uint64 hash);
		
		static LRESULT WINAPI WinProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
		bool GetError(std::wstring* outErrorMsg, unsigned int* outErrorCode);

	protected:
		HWND _hwnd;
		bool _isActivated;

		Timer _keyTimer;
		KeyState* _pKeyStates;
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
