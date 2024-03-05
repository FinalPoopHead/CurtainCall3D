#define _CRT_SECURE_NO_WARNINGS

#include "OsWindows.h"
#include <iostream>
#include <memory>
#include "../FloaterUtil/include/FloaterMacro.h"
#include "../FloaterRendererCommon/include/IRenderer.h"
#include "../FloaterRendererDX11/include/CreateRenderer.h"
#include "../FloaterUtil/include/ConvString.h"

#include "../FloaterRendererCommon/include/ResourceMgr.h"

#include <DbgHelp.h>
#include <chrono>

#if defined(DEBUG) || defined(_DEBUG)
#include <dxgidebug.h>
#include <dxgi1_6.h>

#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "dxguid.lib")
#endif


const std::wstring flt::OsWindows::s_name = L"Windows";

flt::OsWindows::OsWindows(bool useConsole) :
	_hwnd(NULL),
	_isActivated(false),
	_keyTimer(),
	_pKeyStates{ new(std::nothrow) bool[(int)KeyCode::MAX] },
	_pKeyDatas{ new(std::nothrow) KeyData[(int)KeyCode::MAX] },
	_keyUp(),
	_consoleHwnd(NULL),
	_isShowCursor(true)
{
	ASSERT(_pKeyDatas, "메모리 동적 할당 실패");
	ASSERT(_pKeyStates, "메모리 동적 할당 실패");

	memset(_pKeyStates, 0, sizeof(*_pKeyStates) * (int)KeyCode::MAX);
	memset(_pKeyDatas, 0, sizeof(*_pKeyDatas) * (int)KeyCode::MAX);

	_consoleHwnd = GetConsoleWindow();
	if (useConsole)
	{
		if (_consoleHwnd == NULL)
		{
			ASSERT(AllocConsole() != NULL, "콘솔 생성 실패");
			_consoleHwnd = GetConsoleWindow();
			ASSERT(_consoleHwnd != NULL, "콘솔 윈도우 핸들을 가져오지 못했습니다.");
		}

		ShowWindow(_consoleHwnd, SW_SHOW);
	}
	else
	{
		if (_consoleHwnd != NULL)
		{
			ShowWindow(_consoleHwnd, SW_HIDE);
			ASSERT(FreeConsole() != FALSE, "콘솔 해제 실패");
		}
	}
}

flt::OsWindows::~OsWindows()
{
	delete[] _pKeyStates;
	delete[] _pKeyDatas;

	if (_consoleHwnd != NULL)
	{
		ASSERT(FreeConsole() != FALSE, "콘솔 해제 실패");
	}
}

//typedef BOOL(WINAPI* MINIDUMPWRITEDUMP)(HANDLE hProcess, DWORD dwPid, HANDLE hFile, 
//	MINIDUMP_TYPE DumpType, 
//	CONST PMINIDUMP_EXCEPTION_INFORMATION ExceptionParam, 
//	CONST PMINIDUMP_USER_STREAM_INFORMATION UserStreamParam, 
//	CONST PMINIDUMP_CALLBACK_INFORMATION CallbackParam);

struct DumpParam
{
	DWORD threadID;
	LPEXCEPTION_POINTERS pExp;
};

ULONG CreateDump(void* param)
{
	using MINIDUMPWRITEDUMP = BOOL(WINAPI*)(HANDLE, DWORD, HANDLE,
		MINIDUMP_TYPE,
		CONST PMINIDUMP_EXCEPTION_INFORMATION,
		CONST PMINIDUMP_USER_STREAM_INFORMATION,
		CONST PMINIDUMP_CALLBACK_INFORMATION);

	LONG retval = EXCEPTION_CONTINUE_SEARCH;
	HMODULE hDll = NULL;
	hDll = ::LoadLibrary(L"DBGHELP.DLL");

	WCHAR filename[64];

	std::time_t currTime = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
	tm* TimeOfDay = std::localtime(&currTime);
	swprintf_s(filename, L"crash_%04d-%02d-%02d_%02dh%02dm%02ds.dmp",
		TimeOfDay->tm_year + 1900, TimeOfDay->tm_mon + 1, TimeOfDay->tm_mday,
		TimeOfDay->tm_hour, TimeOfDay->tm_min, TimeOfDay->tm_sec);

	if (hDll) {
		MINIDUMPWRITEDUMP pDump = (MINIDUMPWRITEDUMP)::GetProcAddress(hDll, "MiniDumpWriteDump");
		if (pDump)
		{
			HANDLE hFile = ::CreateFile(filename, GENERIC_WRITE, FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
			if (INVALID_HANDLE_VALUE != hFile)
			{
				_MINIDUMP_EXCEPTION_INFORMATION ExInfo;
				ExInfo.ThreadId = ((DumpParam*)param)->threadID;
				ExInfo.ExceptionPointers = ((DumpParam*)param)->pExp;
				ExInfo.ClientPointers = NULL;
				BOOL bOK = pDump(GetCurrentProcess(), GetCurrentProcessId(), hFile, MiniDumpWithFullMemory, &ExInfo, NULL, NULL);
				//BOOL bOK = pDump(GetCurrentProcess(), GetCurrentProcessId(), hFile, MiniDumpNormal, &ExInfo, NULL, NULL);
				if (bOK)
				{
					retval = EXCEPTION_EXECUTE_HANDLER;
				}
				::CloseHandle(hFile);
			}
		}
		::FreeLibrary(hDll);
	}
	return retval;
}



LONG TopExceptionFilter(LPEXCEPTION_POINTERS pExp)
{
	LONG retval = 0;
	DumpParam* param = new DumpParam();
	param->threadID = ::GetCurrentThreadId();
	param->pExp = pExp;
	if (pExp->ExceptionRecord->ExceptionCode == EXCEPTION_STACK_OVERFLOW)
	{
		HANDLE hThread = CreateThread(NULL, 102400, CreateDump, param, 0, NULL);
		if (hThread == NULL)
		{
			auto error = GetLastError();
			std::cout << "CreateThread Error : " << error << std::endl;
			return retval;
		}

		WaitForSingleObject(hThread, INFINITE);
		CloseHandle(hThread);
	}
	else
	{
		retval = CreateDump(param);
	}

	delete param;
	return retval;
}

bool flt::OsWindows::Initialize(int windowWidth, int windowHeight, const std::wstring& title, const std::wstring& imgPath)
{
	WNDCLASSEX wc
	{
		sizeof(WNDCLASSEX),		// cbSize
		CS_CLASSDC,				// style
		WinProc,				// lpfnWndProc
		0L,						// cbClsExtra
		0L,						// cbWndExtra
		GetModuleHandle(NULL),	// hInstance
		NULL,					// hIcon
		NULL,					// hCursor
		NULL,					// hbrBackground
		NULL,					// lpszMenuName
		L"fltOsWindows",		// lpszClassName
		NULL					// hIconSm
	};

	RegisterClassEx(&wc);

	RECT windowRect = { 0, 0, windowWidth, windowHeight };    // 윈도우 창 크기
	AdjustWindowRect(&windowRect, WS_OVERLAPPEDWINDOW, FALSE);

	_hwnd = CreateWindow
	(
		wc.lpszClassName,
		title.c_str(),
		WS_OVERLAPPEDWINDOW | WS_SIZEBOX,
		100, // 윈도우 좌측 상단의 x 좌표
		100, // 윈도우 좌측 상단의 y 좌표
		windowRect.right - windowRect.left, // 윈도우 가로 방향 해상도
		windowRect.bottom - windowRect.top, // 윈도우 세로 방향 해상도
		NULL,
		NULL,
		wc.hInstance,
		this
	);

	if (_hwnd == NULL)
	{
		return false;
	}

	ShowWindow(_hwnd, SW_SHOWDEFAULT);
	UpdateWindow(_hwnd);

	// 덤프 파일을 남기기 위한 세팅.
	// https://docs.microsoft.com/en-us/windows/win32/api/minidumpapiset/nf-minidumpapiset-minidumpwritedump
	// https://docs.microsoft.com/en-us/windows/win32/api/minidumpapiset/ns-minidumpapiset-minidump_exception_information

	SetUnhandledExceptionFilter((LPTOP_LEVEL_EXCEPTION_FILTER)TopExceptionFilter);

	return true;
}

bool flt::OsWindows::Finalize()
{
	return true;
}

bool flt::OsWindows::Update()
{
	// 한 프레임 내에 up, down이 있을 경우에도 한번은 키입력 처리를 위해
	UpdateKeyState();

	// 윈도우 메세지 처리
	MSG msg;
	msg.message = WM_NULL;

	while (PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);

		// 윈도우 창이 종료됐다면 렌더링 할 필요가 없으니 즉시 return
		if (msg.message == WM_QUIT)
		{
			return false;
		}
	}

	return true;
}

flt::IRenderer* flt::OsWindows::CreateRenderer(RendererType type)
{
	IRenderer* renderer = nullptr;

	switch (type)
	{
		case flt::RendererType::SOFTWARE:

			break;
		case flt::RendererType::DX11:
			renderer = CreateRendererDX11(_hwnd);
			break;
		case flt::RendererType::DX12:

			break;
		default:
			ASSERT(false, "RendererType이 잘못되었습니다.");
			break;
	}

	return renderer;
}

void flt::OsWindows::DestroyRenderer(IRenderer* renderer)
{
	DestroyRendererDX11(renderer);

	// 그래픽스 리소스가 해제되는지 체크하기 위한 코드.
#if defined(DEBUG) || defined(_DEBUG)
	IDXGIDebug1* dxgiDebug;
	DXGIGetDebugInterface1(0, __uuidof(IDXGIDebug1), (void**)&dxgiDebug);
	OutputDebugStringW(L"▽▽▽▽▽▽▽▽▽▽▽▽▽▽▽▽그래픽스 리소스 누수 검사▽▽▽▽▽▽▽▽▽▽▽▽▽▽▽▽\n");
	dxgiDebug->ReportLiveObjects(DXGI_DEBUG_ALL, DXGI_DEBUG_RLO_ALL);
	OutputDebugStringW(L"△△△△△△△△△△△△△△△△그래픽스 리소스 누수 검사△△△△△△△△△△△△△△△△\n");
	dxgiDebug->Release();
#endif
}

flt::KeyData flt::OsWindows::GetKey(KeyCode code)
{
	if (_pKeyStates[(int)code])
	{
		return _pKeyDatas[(int)code];
	}

	KeyData keydata;
	keydata.keyTime = 0x8000'0000'0000'0000;
	return keydata;
}

flt::KeyData flt::OsWindows::GetGamePad(int playerNum)
{
	return KeyData{};
}

void flt::OsWindows::ShowCursor(bool isShow)
{
	if (_isShowCursor == isShow)
	{
		return;
	}

	if (isShow)
	{
		while (::ShowCursor(isShow) < 0)
		{

		}
	}
	else
	{
		while (::ShowCursor(isShow) > 0)
		{

		}
	}

	_isShowCursor = isShow;
}

void flt::OsWindows::UpdateKeyState()
{
	// keyUp 처리
	for (const auto& index : _keyUp)
	{
		_pKeyStates[index] = false;
		_pKeyDatas[index].keyTime = 0;
	}
	_keyUp.clear();

	// 매 프레임 초기화 해줘야 하는 키들.
	// 현재 마우스 좌표, 휠업 다운.

	_pKeyStates[(int)KeyCode::mouseRelativePos] = false;
	_pKeyDatas[(int)KeyCode::mouseRelativePos].keyTime = 0;
	_pKeyDatas[(int)KeyCode::mouseRelativePos].x = 0;
	_pKeyDatas[(int)KeyCode::mouseRelativePos].y = 0;

	_pKeyStates[(int)KeyCode::mouseAbsolutePos] = false;
	_pKeyDatas[(int)KeyCode::mouseAbsolutePos].keyTime = 0;
	_pKeyDatas[(int)KeyCode::mouseAbsolutePos].x = 0;
	_pKeyDatas[(int)KeyCode::mouseAbsolutePos].y = 0;

	_pKeyStates[(int)KeyCode::mouseWheelUp] = false;
	_pKeyDatas[(int)KeyCode::mouseWheelUp].keyTime = 0;
	_pKeyDatas[(int)KeyCode::mouseWheelUp].x = 0;
	_pKeyDatas[(int)KeyCode::mouseWheelUp].y = 0;

	_pKeyStates[(int)KeyCode::mouseWheelDown] = false;
	_pKeyDatas[(int)KeyCode::mouseWheelDown].keyTime = 0;
	_pKeyDatas[(int)KeyCode::mouseWheelDown].x = 0;
	_pKeyDatas[(int)KeyCode::mouseWheelDown].y = 0;
}

void flt::OsWindows::HandleKeyboardRawData(const RAWKEYBOARD& data)
{
	auto code = _keyCodeMap[data.VKey];
	KeyData keyData;
	keyData.keyTime = _keyTimer.GetLabTimeMicroSeconds();
	keyData.x = 0;
	keyData.y = 0;
	SetKeyState((KeyCode)code, keyData, data.Flags == RI_KEY_MAKE, data.Flags && RI_KEY_BREAK);
}

void flt::OsWindows::HandleMouseRawData(const RAWMOUSE& data)
{
	int rawX = data.lLastX;
	int rawY = data.lLastY;

	POINT mouseAbsPos;
	GetCursorPos(&mouseAbsPos);
	ScreenToClient(_hwnd, &mouseAbsPos);

	// 마우스 클릭 처리.
	auto buttonFlags = data.usButtonFlags;
	int64 time = _keyTimer.GetLabTimeMicroSeconds();
	KeyData keydata{ time, mouseAbsPos.x, mouseAbsPos.y };


	SetKeyState(KeyCode::mouseLButton, keydata,
		buttonFlags & RI_MOUSE_BUTTON_1_DOWN,
		buttonFlags & RI_MOUSE_BUTTON_1_UP);

	SetKeyState(KeyCode::mouseRButton, keydata,
		buttonFlags & RI_MOUSE_BUTTON_2_DOWN,
		buttonFlags & RI_MOUSE_BUTTON_2_UP);

	SetKeyState(KeyCode::mouseMButton, keydata,
		buttonFlags & RI_MOUSE_BUTTON_3_DOWN,
		buttonFlags & RI_MOUSE_BUTTON_3_UP);

	SetKeyState(KeyCode::mouseButton3, keydata,
		buttonFlags & RI_MOUSE_BUTTON_4_DOWN,
		buttonFlags & RI_MOUSE_BUTTON_4_UP);

	SetKeyState(KeyCode::mouseButton4, keydata,
		buttonFlags & RI_MOUSE_BUTTON_5_DOWN,
		buttonFlags & RI_MOUSE_BUTTON_5_UP);


	// 마우스 세로 휠 굴림 처리
	if (buttonFlags & RI_MOUSE_WHEEL)
	{
		short wheelDelta = data.usButtonData;
		KeyData wheelData{ time, wheelDelta, wheelDelta };
		if (wheelDelta > 0)
		{
			wheelData.x += _pKeyDatas[(int)KeyCode::mouseWheelUp].x;
			wheelData.y += _pKeyDatas[(int)KeyCode::mouseWheelUp].y;
			SetKeyState(KeyCode::mouseWheelUp, wheelData, true, false);
		}
		else
		{
			wheelData.x += _pKeyDatas[(int)KeyCode::mouseWheelDown].x;
			wheelData.y += _pKeyDatas[(int)KeyCode::mouseWheelDown].y;
			SetKeyState(KeyCode::mouseWheelDown, wheelData, true, false);
		}
	}

	// 마우스 가로 휠 굴림 처리
	if (buttonFlags & RI_MOUSE_HWHEEL)
	{
		short wheelDelta = data.usButtonData;
	}

	if (rawX == 0 && rawY == 0)
	{
		return;
	}

	// 마우스 좌표 처리.
	auto mouseFlags = data.usFlags;
	if (mouseFlags & MOUSE_MOVE_ABSOLUTE)
	{
		ASSERT(false, "아직 제대로 구현 안함");
		RECT rect;

		if (mouseFlags & MOUSE_VIRTUAL_DESKTOP)
		{
			rect.left = GetSystemMetrics(SM_XVIRTUALSCREEN);
			rect.top = GetSystemMetrics(SM_YVIRTUALSCREEN);
			rect.right = GetSystemMetrics(SM_CXVIRTUALSCREEN);
			rect.bottom = GetSystemMetrics(SM_CYVIRTUALSCREEN);
		}
		else
		{
			rect.left = 0;
			rect.top = 0;
			rect.right = GetSystemMetrics(SM_CXSCREEN);
			rect.bottom = GetSystemMetrics(SM_CYSCREEN);
		}

		int absoluteX = MulDiv(rawX, rect.right, 65535) + rect.left;
		int absoluteY = MulDiv(rawY, rect.bottom, 65535) + rect.top;

		//std::cout << "absolute " << absoluteX << " " << absoluteY << std::endl;
	}
	else
	{
		// 상대 좌표 마우스 위치 세팅
		KeyData mouseMove;
		mouseMove.keyTime = 0;
		mouseMove.x = rawX + _pKeyDatas[(int)KeyCode::mouseRelativePos].x;
		mouseMove.y = rawY + _pKeyDatas[(int)KeyCode::mouseRelativePos].y;
		SetKeyState(KeyCode::mouseRelativePos, mouseMove, true, false);

		// 절대 좌표 마우스 위치 세팅
		mouseMove.keyTime = 0;
		mouseMove.x = mouseAbsPos.x;
		mouseMove.y = mouseAbsPos.y;
		SetKeyState(KeyCode::mouseAbsolutePos, mouseMove, true, false);
	}
}

void flt::OsWindows::SetKeyState(KeyCode code, const KeyData& data, bool isActive, bool isInActive)
{
	if (isActive)
	{
		if (_pKeyDatas[(int)code].keyTime == 0)
		{
			_pKeyDatas[(int)code] = data;
		}
		_pKeyStates[(int)code] = true;
	}

	if (isInActive)
	{
		_keyUp.push_back((int)code);
	}
}

LRESULT WINAPI flt::OsWindows::WinProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	static OsWindows* thisPtr = nullptr;
	switch (msg)
	{
		case WM_INPUT:
		{
			UINT dwSize = 0;
			GetRawInputData((HRAWINPUT)lParam, RID_INPUT, NULL, &dwSize, sizeof(RAWINPUTHEADER));

			LPBYTE lpb = new BYTE[dwSize];
			if (lpb == NULL)
			{
				ASSERT(false, "메모리 동적할당 실패");
				break;
			}

			if (GetRawInputData((HRAWINPUT)lParam, RID_INPUT, lpb, &dwSize, sizeof(RAWINPUTHEADER)) != dwSize)
			{
				ASSERT(false, "RawInputData 가져오기 실패");
				break;
			}

			RAWINPUT* raw = (RAWINPUT*)lpb;

			if (raw->header.dwType == RIM_TYPEKEYBOARD)
			{
				thisPtr->HandleKeyboardRawData(raw->data.keyboard);
			}
			else if (raw->header.dwType == RIM_TYPEMOUSE)
			{
				thisPtr->HandleMouseRawData(raw->data.mouse);
			}
			else if (raw->header.dwType == RIM_TYPEHID)
			{
				auto x = raw->data.hid.bRawData;
				//std::cout << "test " << x << std::endl;
			}
		}
		break;

		case WM_ACTIVATE:
		{
			if (LOWORD(wParam) == WA_INACTIVE)
			{
				thisPtr->_isActivated = false;
				memset(thisPtr->_pKeyStates, 0, sizeof(*(thisPtr->_pKeyStates)) * (int)KeyCode::MAX);
			}
			else
			{
				thisPtr->_isActivated = true;
			}

		}
		break;

		case WM_SIZE:
		{
			//aptoCore::Graphics::OnResize();
			//aptoCore::Graphics::Resize(LOWORD(lParam), HIWORD(lParam));
			return 0;
		}
		break;

		case WM_INPUT_DEVICE_CHANGE:
		{
			UINT deviceCount = 0;

			if (GetRawInputDeviceList(NULL, &deviceCount, sizeof(RAWINPUTDEVICELIST)) == -1)
			{
				ASSERT(false, "RawInputDeviceList 정보 가져오기 실패");
			}

			std::unique_ptr<RAWINPUTDEVICELIST[]> pDeviceList(new(std::nothrow) RAWINPUTDEVICELIST[deviceCount]);
			ASSERT(pDeviceList != nullptr, "메모리 동적 할당 실패");

			int result = GetRawInputDeviceList(pDeviceList.get(), &deviceCount, sizeof(RAWINPUTDEVICELIST));
			ASSERT(result != -1, "RawInputDeviceList 정보 가져오기 실패");

			for (unsigned int i = 0; i < deviceCount; ++i)
			{
				UINT bufferSize = 0;
				result = GetRawInputDeviceInfo(pDeviceList[i].hDevice, RIDI_DEVICENAME, NULL, &bufferSize);
				if (result == -1)
				{
					ASSERT(false, "메모리 동적 할당 실패");
					continue;
				}

				std::unique_ptr<wchar_t[]> buffer(new(std::nothrow) wchar_t[bufferSize]);
				if (buffer == nullptr)
				{
					ASSERT(false, "메모리 동적 할당 실패");
					continue;
				}

				result = GetRawInputDeviceInfo(pDeviceList[i].hDevice, RIDI_DEVICENAME, buffer.get(), &bufferSize);
				if (result == -1)
				{
					ASSERT(false, "RawInputDevice 정보 가져오기 실패");
					continue;
				}

				//std::wcout << L"TYPE : " << pDeviceList[i].dwType << L" | " << buffer << std::endl;
			}
		}
		break;

		case WM_CREATE:
		{
			// OsWindows객체의 thisPtr을 저장.
			thisPtr = (OsWindows*)(((LPCREATESTRUCT)lParam)->lpCreateParams);

			int result = 0;

			unsigned int rawDeviceCount = 0;
			result = GetRawInputDeviceList(NULL, &rawDeviceCount, sizeof(RAWINPUTDEVICELIST));
			if (result == -1)
			{
				ASSERT(false, "RawInputDeviceList 정보 가져오기 실패");
			}

			//RAWINPUTDEVICELIST* pDeviceList = (PRAWINPUTDEVICELIST)malloc(sizeof(RAWINPUTDEVICELIST) * rawDeviceCount);
			//RAWINPUTDEVICELIST* pDeviceList = new RAWINPUTDEVICELIST[rawDeviceCount];
			std::unique_ptr<RAWINPUTDEVICELIST[]> pDeviceList(new(std::nothrow) RAWINPUTDEVICELIST[rawDeviceCount]);
			ASSERT(pDeviceList != nullptr, "메모리 동적 할당 실패");

			result = GetRawInputDeviceList(pDeviceList.get(), &rawDeviceCount, sizeof(RAWINPUTDEVICELIST));
			ASSERT(result != -1, "RawInputDeviceList 정보 가져오기 실패");

			for (unsigned int i = 0; i < rawDeviceCount; ++i)
			{
				UINT bufferSize = 0;
				result = GetRawInputDeviceInfo(pDeviceList[i].hDevice, RIDI_DEVICENAME, NULL, &bufferSize);
				if (result == -1)
				{
					ASSERT(false, "메모리 동적 할당 실패");
					continue;
				}

				//wchar_t* buffer = new(std::nothrow) wchar_t[bufferSize];
				std::unique_ptr<wchar_t[]> buffer(new(std::nothrow) wchar_t[bufferSize]);
				if (buffer == nullptr)
				{
					ASSERT(false, "메모리 동적 할당 실패");
					continue;
				}

				result = GetRawInputDeviceInfo(pDeviceList[i].hDevice, RIDI_DEVICENAME, buffer.get(), &bufferSize);
				if (result == -1)
				{
					ASSERT(false, "RawInputDevice 정보 가져오기 실패");
					continue;
				}

				//std::wcout << L"TYPE : " << pDeviceList[i].dwType << L" | " << buffer << std::endl;

				RID_DEVICE_INFO rdi;
				rdi.cbSize = sizeof(RID_DEVICE_INFO);
				bufferSize = sizeof(RID_DEVICE_INFO);

				result = GetRawInputDeviceInfo(pDeviceList[i].hDevice, RIDI_DEVICEINFO, &rdi, &bufferSize);
				if (result == -1)
				{
					ASSERT(false, "RawInputDevice 정보 가져오기 실패");
					continue;
				}

				switch (rdi.dwType)
				{
					//case RIM_TYPEMOUSE:
					//	std::wcout << L"	Mouse" << std::endl;
					//	std::wcout << L"	ID: " << rdi.mouse.dwId << std::endl;
					//	std::wcout << L"	Number of Buttons: " << rdi.mouse.dwNumberOfButtons << std::endl;
					//	std::wcout << L"	Sample Rate: " << rdi.mouse.dwSampleRate << std::endl;
					//	break;
					//case RIM_TYPEKEYBOARD:
					//	std::wcout << L"	Keyboard" << std::endl;
					//	std::wcout << L"	Type: " << rdi.keyboard.dwType << std::endl;
					//	std::wcout << L"	Sub Type: " << rdi.keyboard.dwSubType << std::endl;
					//	std::wcout << L"	Keyboard Mode: " << rdi.keyboard.dwKeyboardMode << std::endl;
					//	std::wcout << L"	Number of Function Keys: " << rdi.keyboard.dwNumberOfFunctionKeys << std::endl;
					//	std::wcout << L"	Number of Indicators: " << rdi.keyboard.dwNumberOfIndicators << std::endl;
					//	std::wcout << L"	Number of Keys Total: " << rdi.keyboard.dwNumberOfKeysTotal << std::endl;
					//	break;
					//case RIM_TYPEHID:
					//	std::wcout << L"	HID" << std::endl;
					//	std::wcout << L"	Vendor ID: " << rdi.hid.dwVendorId << std::endl;
					//	std::wcout << L"	Product ID: " << rdi.hid.dwProductId << std::endl;
					//	std::wcout << L"	Version Number: " << rdi.hid.dwVersionNumber << std::endl;
					//	std::wcout << L"	Usage for the top-level collection: " << rdi.hid.usUsage << std::endl;
					//	std::wcout << L"	Usage Page for the top-level collection: " << rdi.hid.usUsagePage << std::endl;
					//	break;
					default:
						//std::wcout << L"	Unknown" << std::endl;
						break;
				}
			}

			// usUsagePage 1일 경우 usUsage 정보
			// 1-2 마우스 (인데 2를 일반 마우스로 받는듯.)
			// 4-5 게임 컨트롤러
			// 6-7 키보드, 키패드

			RAWINPUTDEVICE rid[4];

			rid[0].dwFlags = RIDEV_NOLEGACY;// | RIDEV_INPUTSINK;
			rid[0].usUsagePage = 1;
			rid[0].usUsage = 6;
			rid[0].hwndTarget = hwnd;

			rid[1].dwFlags = 0;
			rid[1].usUsagePage = 1;
			rid[1].usUsage = 2;
			rid[1].hwndTarget = hwnd;

			rid[2].dwFlags = NULL;
			rid[2].usUsagePage = 1;
			rid[2].usUsage = 5;
			rid[2].hwndTarget = hwnd;

			result = RegisterRawInputDevices(rid, 3, sizeof(rid[0]));
			if (result == FALSE)
			{
				std::wstring errorMsg;
				unsigned int errorCode;
				auto r = thisPtr->GetError(&errorMsg, &errorCode);
				ASSERT(false, "RawInputDevice 등록 실패");
			}
		}
		break;

		case WM_DESTROY:
		{
			::PostQuitMessage(0);
			return 0;
		}
		break;
	}

	return ::DefWindowProc(hwnd, msg, wParam, lParam);
}

bool flt::OsWindows::GetError(std::wstring* outErrorMsg, unsigned int* outErrorCode)
{
	ASSERT(outErrorCode, "nullptr");
	ASSERT(outErrorCode, "nullptr");


	*outErrorCode = GetLastError();
	const int size = 512;
	WCHAR msgBuffer[size] = { 0, };

	auto ret = FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL,
		*outErrorCode,
		NULL,
		msgBuffer,
		size,
		NULL);

	if (ret == NULL)
	{
		return false;
	}

	std::wstring temp{ msgBuffer };
	*outErrorMsg += temp;

	return true;
}

unsigned char flt::OsWindows::_keyCodeMap[256] =
{
	NULL,
	(unsigned char)KeyCode::mouseLButton,	// VK_LBUTTON        0x01
	(unsigned char)KeyCode::mouseRButton,	// VK_RBUTTON        0x02
	NULL,	// VK_CANCEL         0x03
	(unsigned char)KeyCode::mouseMButton,	// VK_MBUTTON        0x04    /* NOT contiguous with L & RBUTTON */
	(unsigned char)KeyCode::mouseButton3,	// VK_XBUTTON1       0x05    /* NOT contiguous with L & RBUTTON */
	(unsigned char)KeyCode::mouseButton4,	// VK_XBUTTON2       0x06    /* NOT contiguous with L & RBUTTON */
	NULL,	// 0x07 : reserved
	(unsigned char)KeyCode::backspace,	// VK_BACK           0x08
	(unsigned char)KeyCode::tab,	// VK_TAB            0x09
	NULL, // 0x0A : reserved
	NULL, // 0x0B : reserved
	NULL,	// VK_CLEAR          0x0C
	(unsigned char)KeyCode::enter,	// VK_RETURN         0x0D
	NULL, // 0x0E : reserved
	NULL, // 0x0F : reserved
	(unsigned char)KeyCode::lShift,	// VK_SHIFT          0x10
	(unsigned char)KeyCode::lCtrl,	// VK_CONTROL        0x11
	(unsigned char)KeyCode::lAlt,	// VK_MENU           0x12
	NULL,	// VK_PAUSE          0x13
	(unsigned char)KeyCode::capslock,	// VK_CAPITAL        0x14
	(unsigned char)KeyCode::RAlt,	// VK_HANGUL         0x15
	NULL,	// VK_IME_ON         0x16
	NULL,	// VK_JUNJA          0x17
	NULL,	// VK_FINAL          0x18
	NULL,	// VK_HANJA          0x19
	NULL,	// VK_IME_OFF        0x1A
	(unsigned char)KeyCode::esc,	// VK_ESCAPE         0x1B
	NULL,	// VK_CONVERT        0x1C
	NULL,	// VK_NONCONVERT     0x1D
	NULL,	// VK_ACCEPT         0x1E
	NULL,	// VK_MODECHANGE     0x1F
	(unsigned char)KeyCode::spacebar,	// VK_SPACE          0x20
	(unsigned char)KeyCode::pageUp,	// VK_PRIOR          0x21
	(unsigned char)KeyCode::pageDown,	// VK_NEXT           0x22
	(unsigned char)KeyCode::end,	// VK_END            0x23
	(unsigned char)KeyCode::home,	// VK_HOME           0x24
	(unsigned char)KeyCode::left,	// VK_LEFT           0x25
	(unsigned char)KeyCode::up,	// VK_UP             0x26
	(unsigned char)KeyCode::right,	// VK_RIGHT          0x27
	(unsigned char)KeyCode::down,	// VK_DOWN           0x28
	NULL,	// VK_SELECT         0x29
	NULL,	// VK_PRINT          0x2A
	NULL,	// VK_EXECUTE        0x2B
	(unsigned char)KeyCode::printScreen,	// VK_SNAPSHOT       0x2C
	(unsigned char)KeyCode::insert,	// VK_INSERT         0x2D
	(unsigned char)KeyCode::Delete,	// VK_DELETE         0x2E
	NULL,	// VK_HELP           0x2F
	(unsigned char)KeyCode::key0,	// 0
	(unsigned char)KeyCode::key1,	// 1
	(unsigned char)KeyCode::key2,	// 2
	(unsigned char)KeyCode::key3,	// 3
	(unsigned char)KeyCode::key4,	// 4
	(unsigned char)KeyCode::key5,	// 5
	(unsigned char)KeyCode::key6,	// 6
	(unsigned char)KeyCode::key7,	// 7
	(unsigned char)KeyCode::key8,	// 8
	(unsigned char)KeyCode::key9,	// 9
	NULL, // 0x3A : unassigned
	NULL, // 0x3B : unassigned
	NULL, // 0x3C : unassigned
	NULL, // 0x3D : unassigned
	NULL, // 0x3E : unassigned
	NULL, // 0x3F : unassigned
	NULL, // 0x40 : unassigned
	(unsigned char)KeyCode::a,	// A
	(unsigned char)KeyCode::b,	// B
	(unsigned char)KeyCode::c,	// C
	(unsigned char)KeyCode::d,	// D
	(unsigned char)KeyCode::e,	// E
	(unsigned char)KeyCode::f,	// F
	(unsigned char)KeyCode::g,	// G
	(unsigned char)KeyCode::h,	// H
	(unsigned char)KeyCode::i,	// I
	(unsigned char)KeyCode::j,	// J
	(unsigned char)KeyCode::k,	// K
	(unsigned char)KeyCode::l,	// L
	(unsigned char)KeyCode::m,	// M
	(unsigned char)KeyCode::n,	// N
	(unsigned char)KeyCode::o,	// O
	(unsigned char)KeyCode::p,	// P
	(unsigned char)KeyCode::q,	// Q
	(unsigned char)KeyCode::r,	// R
	(unsigned char)KeyCode::s,	// S
	(unsigned char)KeyCode::t,	// T
	(unsigned char)KeyCode::u,	// U
	(unsigned char)KeyCode::v,	// V
	(unsigned char)KeyCode::w,	// W
	(unsigned char)KeyCode::x,	// X
	(unsigned char)KeyCode::y,	// Y
	(unsigned char)KeyCode::z,	// Z
	NULL,	// VK_LWIN           0x5B
	NULL,	// VK_RWIN           0x5C
	NULL,	// VK_APPS           0x5D
	NULL, // 0x5E : reserved
	NULL,	// VK_SLEEP          0x5F
	(unsigned char)KeyCode::num0,	// VK_NUMPAD0        0x60
	(unsigned char)KeyCode::num1,	// VK_NUMPAD1        0x61
	(unsigned char)KeyCode::num2,	// VK_NUMPAD2        0x62
	(unsigned char)KeyCode::num3,	// VK_NUMPAD3        0x63
	(unsigned char)KeyCode::num4,	// VK_NUMPAD4        0x64
	(unsigned char)KeyCode::num5,	// VK_NUMPAD5        0x65
	(unsigned char)KeyCode::num6,	// VK_NUMPAD6        0x66
	(unsigned char)KeyCode::num7,	// VK_NUMPAD7        0x67
	(unsigned char)KeyCode::num8,	// VK_NUMPAD8        0x68
	(unsigned char)KeyCode::num9,	// VK_NUMPAD9        0x69
	NULL,	// VK_MULTIPLY       0x6A
	NULL,	// VK_ADD            0x6B
	NULL,	// VK_SEPARATOR      0x6C
	NULL,	// VK_SUBTRACT       0x6D
	NULL,	// VK_DECIMAL        0x6E
	NULL,	// VK_DIVIDE         0x6F
	(unsigned char)KeyCode::f1,		// VK_F1             0x70
	(unsigned char)KeyCode::f2,		// VK_F2             0x71
	(unsigned char)KeyCode::f3,		// VK_F3             0x72
	(unsigned char)KeyCode::f4,		// VK_F4             0x73
	(unsigned char)KeyCode::f5,		// VK_F5             0x74
	(unsigned char)KeyCode::f6,		// VK_F6             0x75
	(unsigned char)KeyCode::f7,		// VK_F7             0x76
	(unsigned char)KeyCode::f8,		// VK_F8             0x77
	(unsigned char)KeyCode::f9,		// VK_F9             0x78
	(unsigned char)KeyCode::f10,	// VK_F10            0x79
	(unsigned char)KeyCode::f11,	// VK_F11            0x7A
	(unsigned char)KeyCode::f12,	// VK_F12            0x7B
	NULL,	// VK_F13            0x7C
	NULL,	// VK_F14            0x7D
	NULL,	// VK_F15            0x7E
	NULL,	// VK_F16            0x7F
	NULL,	// VK_F17            0x80
	NULL,	// VK_F18            0x81
	NULL,	// VK_F19            0x82
	NULL,	// VK_F20            0x83
	NULL,	// VK_F21            0x84
	NULL,	// VK_F22            0x85
	NULL,	// VK_F23            0x86
	NULL,	// VK_F24            0x87
	NULL,	// VK_NAVIGATION_VIEW     0x88 // reserved
	NULL,	// VK_NAVIGATION_MENU     0x89 // reserved
	NULL,	// VK_NAVIGATION_UP       0x8A // reserved
	NULL,	// VK_NAVIGATION_DOWN     0x8B // reserved
	NULL,	// VK_NAVIGATION_LEFT     0x8C // reserved
	NULL,	// VK_NAVIGATION_RIGHT    0x8D // reserved
	NULL,	// VK_NAVIGATION_ACCEPT   0x8E // reserved
	NULL,	// VK_NAVIGATION_CANCEL   0x8F // reserved
	NULL,	// VK_NUMLOCK        0x90
	NULL,	// VK_SCROLL         0x91
	NULL,	// VK_OEM_NEC_EQUAL  0x92   // '=' key on numpad
	NULL,	// VK_OEM_FJ_JISHO   0x92   // 'Dictionary' key
	NULL,	// VK_OEM_FJ_MASSHOU 0x93   // 'Unregister word' key
	NULL,	// VK_OEM_FJ_TOUROKU 0x94   // 'Register word' key
	NULL,	// VK_OEM_FJ_LOYA    0x95   // 'Left OYAYUBI' key
	NULL,	// VK_OEM_FJ_ROYA    0x96   // 'Right OYAYUBI' key
	NULL, // 0x97 : unassigned
	NULL, // 0x98 : unassigned
	NULL, // 0x99 : unassigned
	NULL, // 0x9A : unassigned
	NULL, // 0x9B : unassigned
	NULL, // 0x9C : unassigned
	NULL, // 0x9D : unassigned
	NULL, // 0x9E : unassigned
	NULL, // 0x9F : unassigned
	// 
	// VK_L* & VK_R* - left and right Alt, Ctrl and Shift virtual keys.
	// Used only as parameters to GetAsyncKeyState() and GetKeyState().
	// No other API or message will distinguish left and right keys in this way.
	// 
	(unsigned char)KeyCode::lShift,	// VK_LSHIFT         0xA0
	(unsigned char)KeyCode::rShift,	// VK_RSHIFT         0xA1
	(unsigned char)KeyCode::lCtrl,	// VK_LCONTROL       0xA2
	(unsigned char)KeyCode::RCtrl,	// VK_RCONTROL       0xA3
	(unsigned char)KeyCode::lAlt,	// VK_LMENU          0xA4
	(unsigned char)KeyCode::RAlt,	// VK_RMENU          0xA5
	(unsigned char)KeyCode::mouseButton3,	// VK_BROWSER_BACK        0xA6
	(unsigned char)KeyCode::mouseButton4,	// VK_BROWSER_FORWARD     0xA7
	NULL,	// VK_BROWSER_REFRESH     0xA8
	NULL,	// VK_BROWSER_STOP        0xA9
	NULL,	// VK_BROWSER_SEARCH      0xAA
	NULL,	// VK_BROWSER_FAVORITES   0xAB
	NULL,	// VK_BROWSER_HOME        0xAC
	NULL,	// VK_VOLUME_MUTE         0xAD
	NULL,	// VK_VOLUME_DOWN         0xAE
	NULL,	// VK_VOLUME_UP           0xAF
	NULL,	// VK_MEDIA_NEXT_TRACK    0xB0
	NULL,	// VK_MEDIA_PREV_TRACK    0xB1
	NULL,	// VK_MEDIA_STOP          0xB2
	NULL,	// VK_MEDIA_PLAY_PAUSE    0xB3
	NULL,	// VK_LAUNCH_MAIL         0xB4
	NULL,	// VK_LAUNCH_MEDIA_SELECT 0xB5
	NULL,	// VK_LAUNCH_APP1         0xB6
	NULL,	// VK_LAUNCH_APP2         0xB7
	NULL, // 0xB8 : reserved
	NULL, // 0xB9 : reserved
	NULL,	// VK_OEM_1          0xBA   // ';:' for US
	NULL,	// VK_OEM_PLUS       0xBB   // '+' any country
	NULL,	// VK_OEM_COMMA      0xBC   // ',' any country
	NULL,	// VK_OEM_MINUS      0xBD   // '-' any country
	NULL,	// VK_OEM_PERIOD     0xBE   // '.' any country
	NULL,	// VK_OEM_2          0xBF   // '/?' for US
	NULL,	// VK_OEM_3          0xC0   // '`~' for US
	NULL, // 0xC1 : reserved
	NULL, // 0xC2 : reserved
	NULL,	// VK_GAMEPAD_A                         0xC3 // reserved
	NULL,	// VK_GAMEPAD_B                         0xC4 // reserved
	NULL,	// VK_GAMEPAD_X                         0xC5 // reserved
	NULL,	// VK_GAMEPAD_Y                         0xC6 // reserved
	NULL,	// VK_GAMEPAD_RIGHT_SHOULDER            0xC7 // reserved
	NULL,	// VK_GAMEPAD_LEFT_SHOULDER             0xC8 // reserved
	NULL,	// VK_GAMEPAD_LEFT_TRIGGER              0xC9 // reserved
	NULL,	// VK_GAMEPAD_RIGHT_TRIGGER             0xCA // reserved
	NULL,	// VK_GAMEPAD_DPAD_UP                   0xCB // reserved
	NULL,	// VK_GAMEPAD_DPAD_DOWN                 0xCC // reserved
	NULL,	// VK_GAMEPAD_DPAD_LEFT                 0xCD // reserved
	NULL,	// VK_GAMEPAD_DPAD_RIGHT                0xCE // reserved
	NULL,	// VK_GAMEPAD_MENU                      0xCF // reserved
	NULL,	// VK_GAMEPAD_VIEW                      0xD0 // reserved
	NULL,	// VK_GAMEPAD_LEFT_THUMBSTICK_BUTTON    0xD1 // reserved
	NULL,	// VK_GAMEPAD_RIGHT_THUMBSTICK_BUTTON   0xD2 // reserved
	NULL,	// VK_GAMEPAD_LEFT_THUMBSTICK_UP        0xD3 // reserved
	NULL,	// VK_GAMEPAD_LEFT_THUMBSTICK_DOWN      0xD4 // reserved
	NULL,	// VK_GAMEPAD_LEFT_THUMBSTICK_RIGHT     0xD5 // reserved
	NULL,	// VK_GAMEPAD_LEFT_THUMBSTICK_LEFT      0xD6 // reserved
	NULL,	// VK_GAMEPAD_RIGHT_THUMBSTICK_UP       0xD7 // reserved
	NULL,	// VK_GAMEPAD_RIGHT_THUMBSTICK_DOWN     0xD8 // reserved
	NULL,	// VK_GAMEPAD_RIGHT_THUMBSTICK_RIGHT    0xD9 // reserved
	NULL,	// VK_GAMEPAD_RIGHT_THUMBSTICK_LEFT     0xDA // reserved
	NULL,	// VK_OEM_4          0xDB  //  '[{' for US
	NULL,	// VK_OEM_5          0xDC  //  '\|' for US
	NULL,	// VK_OEM_6          0xDD  //  ']}' for US
	NULL,	// VK_OEM_7          0xDE  //  ''"' for US
	NULL,	// VK_OEM_8          0xDF
	NULL, // 0xE0 : reserved
	NULL,	// VK_OEM_AX         0xE1  //  'AX' key on Japanese AX kbd
	NULL,	// VK_OEM_102        0xE2  //  "<>" or "\|" on RT 102-key kbd.
	NULL,	// VK_ICO_HELP       0xE3  //  Help key on ICO
	NULL,	// VK_ICO_00         0xE4  //  00 key on ICO
	NULL,	// VK_PROCESSKEY     0xE5
	NULL,	// VK_ICO_CLEAR      0xE6
	NULL,	// VK_PACKET         0xE7
	NULL, // 0xE8 : reserved
	NULL,	// VK_OEM_RESET      0xE9
	NULL,	// VK_OEM_JUMP       0xEA
	NULL,	// VK_OEM_PA1        0xEB
	NULL,	// VK_OEM_PA2        0xEC
	NULL,	// VK_OEM_PA3        0xED
	NULL,	// VK_OEM_WSCTRL     0xEE
	NULL,	// VK_OEM_CUSEL      0xEF
	NULL,	// VK_OEM_ATTN       0xF0
	NULL,	// VK_OEM_FINISH     0xF1
	NULL,	// VK_OEM_COPY       0xF2
	NULL,	// VK_OEM_AUTO       0xF3
	NULL,	// VK_OEM_ENLW       0xF4
	NULL,	// VK_OEM_BACKTAB    0xF5
	NULL,	// VK_ATTN           0xF6
	NULL,	// VK_CRSEL          0xF7
	NULL,	// VK_EXSEL          0xF8
	NULL,	// VK_EREOF          0xF9
	NULL,	// VK_PLAY           0xFA
	NULL,	// VK_ZOOM           0xFB
	NULL,	// VK_NONAME         0xFC
	NULL,	// VK_PA1            0xFD
	NULL	// VK_OEM_CLEAR      0xFE
	// 0xFF : reserved
};
