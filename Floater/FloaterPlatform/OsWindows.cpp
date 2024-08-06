#define _CRT_SECURE_NO_WARNINGS


#include "OsWindows.h"
#include "./include/GamePad.h"
#include "KeyState.h"
#include <iostream>
#include <algorithm>
#include <memory>
#include "../FloaterUtil/include/FloaterMacro.h"
#include "../FloaterRendererCommon/include/IRenderer.h"
#include "../FloaterRendererDX11/include/CreateRenderer.h"
#include "../FloaterUtil/include/ConvString.h"
#include "../FloaterUtil/include/Hash.h"

#include "../RocketAdapter/include/CreateRenderer.h"
#include "../FloaterRendererCommon/include/ResourceMgr.h"

#include <DbgHelp.h>
#include <chrono>

#if defined(DEBUG) || defined(_DEBUG)
#include <dxgidebug.h>
#include <dxgi1_6.h>

#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "dxguid.lib")
#endif

// 게임패드 입력을 받기 위한 헤더.
#include "XboxPad.h"

#include <hidsdi.h>
//#include <hidpddi.h>
#include <hidpi.h>

#pragma comment(lib, "hid.lib")
//#pragma comment(lib, "hidparse.lib")

#include <dbt.h>


const std::wstring flt::OsWindows::s_name = L"Windows";

flt::OsWindows::OsWindows(bool useConsole)
	: _hwnd(NULL)
	, _isActivated(false)
	, _isClosed(true)
	, _keyTimer()
	, _pKeyStates{ new(std::nothrow) KeyState[(int)KeyCode::MAX] }
	, _pKeyDatas{ new(std::nothrow) KeyData[(int)KeyCode::MAX] }
	, _keyUp()
	, _consoleHwnd(NULL)
	, _isShowCursor(true)
	, _exePath()
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

			FILE* console = freopen("CONOUT$", "w", stdout);
			ASSERT(console, "Failed to open console window");
			std::cout.clear();
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
	BOOL ret = AdjustWindowRect(&windowRect, WS_OVERLAPPEDWINDOW | WS_SIZEBOX, FALSE);
	if (ret == 0)
	{
		ASSERT(false, "사각형 크기 계산 실패");
		return false;
	}

	HWND hwnd = CreateWindowEx
	(
		0,
		wc.lpszClassName,
		title.c_str(),
		WS_OVERLAPPEDWINDOW | WS_SIZEBOX,
		100, // 윈도우 좌측 상단의 x 좌표
		100, // 윈도우 좌측 상단의 y 좌표
		windowRect.right - windowRect.left, // 윈도우 가로 방향 해상도
		windowRect.bottom - windowRect.top, // 윈도우 세로 방향 해상도
		NULL,
		NULL,	// hMenu
		wc.hInstance,
		this	// lpParaam
	);

	if (!Initialize((uint64)hwnd))
	{
		return false;
	}

	return true;
}

bool flt::OsWindows::Initialize(uint64 hwnd)
{
	_hwnd = (HWND)hwnd;

	if (_hwnd == NULL)
	{
		return false;
	}

	// 덤프 파일을 남기기 위한 세팅.
	// https://docs.microsoft.com/en-us/windows/win32/api/minidumpapiset/nf-minidumpapiset-minidumpwritedump
	// https://docs.microsoft.com/en-us/windows/win32/api/minidumpapiset/ns-minidumpapiset-minidump_exception_information

	SetUnhandledExceptionFilter((LPTOP_LEVEL_EXCEPTION_FILTER)TopExceptionFilter);

	bool ret = InitializeXBOXController(_hwnd);
	if(ret == false)
	{
		return false;
	}

	InitializeExePath();

	ShowWindow(_hwnd, SW_SHOWDEFAULT);
	UpdateWindow(_hwnd);

	return true;
}

bool flt::OsWindows::Finalize()
{
	_exePath.clear();
	return true;
}

bool flt::OsWindows::Update(float deltaSeconds)
{
	if(_hwnd == NULL)
	{
		return false;
	}

	if(_isClosed)
	{
		return false;
	}

	UpdateKeyState();

	// 윈도우 메세지 처리
	MSG msg;
	msg.message = WM_NULL;

	for (int i = 0; i < 16; ++i)
	{
		if (_pGamePads[i].isConnected == false)
		{
			continue;
		}

		flt::Xbox::Get(&_pGamePads[i]);
	}

	UpdateGamePadVibration(deltaSeconds);

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
		case flt::RendererType::ROCKET_DX11:
			renderer = CreateRendererRocketDX11(_hwnd);
			break;
		default:
			ASSERT(false, "RendererType이 잘못되었습니다.");
			break;
	}

	_rendererMap[renderer] = type;
	return renderer;
}

void flt::OsWindows::DestroyRenderer(IRenderer* renderer)
{
	RendererType type = _rendererMap[renderer];

	switch (type)
	{
		case flt::RendererType::SOFTWARE:
			break;
		case flt::RendererType::DX11:
			DestroyRendererDX11(renderer);
			break;
		case flt::RendererType::DX12:
			break;
		case flt::RendererType::ROCKET_DX11:
			DestroyRendererRocketDX11(renderer);
			break;
		default:
			break;
	}

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

uint32 flt::OsWindows::GetWindowHandle()
{
	return (uint32)_hwnd;
}

flt::Vector2f flt::OsWindows::GetWindowSize()
{
	RECT rect;
	GetClientRect(_hwnd, &rect);
	return Vector2f{ (float)(rect.right - rect.left), (float)(rect.bottom - rect.top) };
}

void flt::OsWindows::SetWindowTitle(const std::wstring& title)
{
	BOOL ret = SetWindowText(_hwnd, title.c_str());
	if (ret == 0)
	{
		ASSERT(false, "타이틀 변경 실패");
		DWORD error = GetLastError();
	}
}

void flt::OsWindows::SetWindowSize(uint32 width /*= 0*/, uint32 height /*= 0*/, WindowMode mode /*= WindowMode::WINDOWED*/, int monitorIndex /*= -1*/)
{
	MONITORINFOEX monitorInfo;
	monitorInfo.cbSize = sizeof(MONITORINFOEX);

	HMONITOR hMonitor = MonitorFromWindow(_hwnd, MONITOR_DEFAULTTOPRIMARY);
	GetMonitorInfo(hMonitor, &monitorInfo);

	bool useMonitorSize = false;
	if (width == 0 && height == 0)
	{
		width = monitorInfo.rcMonitor.right - monitorInfo.rcMonitor.left;
		height = monitorInfo.rcMonitor.bottom - monitorInfo.rcMonitor.top;
		useMonitorSize = true;
	}

	LONG_PTR style = WS_POPUP;
	if (mode == WindowMode::WINDOWED)
	{
		style = WS_OVERLAPPEDWINDOW | WS_SIZEBOX;

		RECT windowRect = { 0, 0, (LONG)width, (LONG)height };    // 윈도우 창 크기
		BOOL ret = AdjustWindowRect(&windowRect, WS_OVERLAPPEDWINDOW | WS_SIZEBOX, FALSE);
		ASSERT(ret != 0, "사각형 크기 계산 실패");

		width = windowRect.right - windowRect.left;
		height = windowRect.bottom - windowRect.top;
	}
	style = SetWindowLongPtr(_hwnd, GWL_STYLE, style);
	ASSERT(style != 0, "스타일 변경 실패");
	SetWindowPos(_hwnd, 0, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_FRAMECHANGED);

	if (mode == WindowMode::FULLSCREEN)
	{
		DISPLAY_DEVICE displayDevice;
		displayDevice.cb = sizeof(DISPLAY_DEVICE);

		if (monitorIndex < 0 || EnumDisplayDevices(NULL, (DWORD)monitorIndex, &displayDevice, 0))
		{
			DWORD displayNum = 0;
			while (EnumDisplayDevices(NULL, displayNum, &displayDevice, 0))
			{
				if (!(displayDevice.StateFlags & DISPLAY_DEVICE_ACTIVE))
				{
					++displayNum;
					continue;
				}

				if (wcscmp(monitorInfo.szDevice, displayDevice.DeviceName) == 0)
				{
					break;
				}

				/// 현재 디스플레이에서 가능한 모드 출력
				/*DEVMODE devMode;
				devMode.dmSize = sizeof(DEVMODE);
				DWORD modeIndex = 0;
				while(EnumDisplaySettings(displayDevice.DeviceName, modeIndex, &devMode) != 0)
				{
					std::cout << "Mode " << modeIndex << ": "
						<< devMode.dmPelsWidth << "x"
						<< devMode.dmPelsHeight << " @ "
						<< devMode.dmDisplayFrequency << "Hz, "
						<< devMode.dmBitsPerPel << " bits"
						<< std::endl;
					++modeIndex;
				}*/
				++displayNum;
			}
		}

		DEVMODE devMode;
		memset(&devMode, 0, sizeof(DEVMODE));
		devMode.dmSize = sizeof(DEVMODE);
		devMode.dmPelsWidth = width;
		devMode.dmPelsHeight = height;
		devMode.dmBitsPerPel = 32;
		devMode.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;


		LONG ret = ChangeDisplaySettingsEx(displayDevice.DeviceName, &devMode, NULL, CDS_FULLSCREEN, NULL);
		ASSERT(ret == DISP_CHANGE_SUCCESSFUL, "전체화면으로 변경 실패");

		///  TODO 전체화면을 하려면 렌더러에서도 해줘야한다.
		//for (auto& [renderer, type] : _rendererMap)
		//{
		//	renderer->SetFullScreen(true);
		//}
	}
	else
	{
		ChangeDisplaySettingsEx(NULL, NULL, NULL, 0, NULL);

		//for (auto& [renderer, type] : _rendererMap)
		//{
		//	renderer->SetFullScreen(false);
		//}
	}

	// 모니터 해상도를 바꿧다면 위치, 크기가 바뀔 수 있기때문에 모니터 크기를 다시 계산.
	hMonitor = MonitorFromWindow(_hwnd, MONITOR_DEFAULTTOPRIMARY);
	GetMonitorInfo(hMonitor, &monitorInfo);
	if (useMonitorSize)
	{
		width = monitorInfo.rcMonitor.right - monitorInfo.rcMonitor.left;
		height = monitorInfo.rcMonitor.bottom - monitorInfo.rcMonitor.top;
	}

	//SetWindowPos(_hwnd, HWND_TOP, 0, 0, width, height, SWP_SHOWWINDOW | SWP_FRAMECHANGED);
	SetWindowPos(_hwnd, HWND_TOP, monitorInfo.rcMonitor.left, monitorInfo.rcMonitor.top, width, height, SWP_SHOWWINDOW);
}

flt::KeyData flt::OsWindows::GetKey(KeyCode code)
{
	if (_pKeyStates[(int)code].isStay)
	{
		return _pKeyDatas[(int)code];
	}

	KeyData keydata;
	keydata.keyTime = 0x8000'0000'0000'0000;
	return keydata;
}

flt::KeyData flt::OsWindows::GetKeyDown(KeyCode code)
{
	if (_pKeyStates[(int)code].isDown)
	{
		return _pKeyDatas[(int)code];
	}

	KeyData keydata;
	keydata.keyTime = 0x8000'0000'0000'0000;
	return keydata;
}

flt::KeyData flt::OsWindows::GetKeyUp(KeyCode code)
{
	if (_pKeyStates[(int)code].isUp)
	{
		return _pKeyDatas[(int)code];
	}

	KeyData keydata;
	keydata.keyTime = 0x8000'0000'0000'0000;
	return keydata;
}

bool flt::OsWindows::GamePadConnected(int padIndex)
{
	return _pGamePads[padIndex].isConnected;
}

bool flt::OsWindows::GetGamePadState(int padIndex, GamePadState* outState)
{
	ASSERT(outState, "outState가 nullptr입니다.");

	if (padIndex < 0 || padIndex >= 16)
	{
		return false;
	}

	if (_pGamePads[padIndex].isConnected == false)
	{
		return false;
	}

	*outState = _pGamePads[padIndex].state;

	return true;
}

// MoterPower : 0 ~ 1
bool flt::OsWindows::SetGamePadVibration(int padIndex, float leftMotor, float rightMotor)
{
	ASSERT(leftMotor >= 0.0f && leftMotor <= 1.0f, "leftMotor 범위가 잘못되었습니다.");
	ASSERT(rightMotor >= 0.0f && rightMotor <= 1.0f, "rightMotor 범위가 잘못되었습니다.");

	if (padIndex < 0 || padIndex >= 16)
	{
		return false;
	}

	if (_pGamePads[padIndex].isConnected == false)
	{
		return false;
	}

	leftMotor = std::clamp(leftMotor, 0.0f, 1.0f);
	rightMotor = std::clamp(rightMotor, 0.0f, 1.0f);

	Xbox::Set(&_pGamePads[padIndex], (BYTE)(leftMotor * 255), (BYTE)(rightMotor * 255));
	return true;
}

// MoterPower : 0 ~ 1
bool flt::OsWindows::SetGamePadVibration(int padIndex, bool isRightMoter, float moterPower, float time)
{
	ASSERT(time > 0.0f, "time이 0보다 작습니다.");
	ASSERT(moterPower >= 0.0f && moterPower <= 1.0f, "moterPower 범위가 잘못되었습니다.");

	if (padIndex < 0 || padIndex >= 16)
	{
		return false;
	}

	if (_pGamePads[padIndex].isConnected == false)
	{
		return false;
	}

	moterPower = std::clamp(moterPower, 0.0f, 1.0f);
	if (isRightMoter)
	{
		_pGamePads[padIndex].vibration.rightMotorPower = (BYTE)(moterPower * 255);
		_pGamePads[padIndex].vibration.rightMotorTime = time;
	}
	else
	{
		_pGamePads[padIndex].vibration.leftMotorPower = (BYTE)(moterPower * 255);
		_pGamePads[padIndex].vibration.leftMotorTime = time;
	}

	return true;
}

bool flt::OsWindows::SetGamePadAnalogDeadZone(int padIndex, GamePadDeadZone* deadZone)
{
	if (padIndex < 0 || padIndex >= 16)
	{
		return false;
	}

	if (_pGamePads[padIndex].isConnected == false)
	{
		return false;
	}

	_pGamePads[padIndex].deadZone = *deadZone;
	return true;
}

std::vector<int> flt::OsWindows::GetGamePadIndexList()
{
	std::vector<int> indexList;
	for (int i = 0; i < 16; ++i)
	{
		if (_pGamePads[i].isConnected)
		{
			indexList.push_back(i);
		}
	}

	return indexList;
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

std::wstring flt::OsWindows::GetExePath()
{
	return _exePath;
}

std::wstring flt::OsWindows::GetAbsPath(std::wstring relativePath)
{
	ASSERT(!_exePath.empty(), "exePath가 비어있습니다.");
	ASSERT(false, "구현 안됨");
	return std::wstring{};
}

void flt::OsWindows::Exit()
{
	::PostQuitMessage(0);
}

bool flt::OsWindows::InitializeXBOXController(HWND hwnd)
{
	DEV_BROADCAST_DEVICEINTERFACE_W db =
	{
		.dbcc_size = sizeof(db),
		.dbcc_devicetype = DBT_DEVTYP_DEVICEINTERFACE,
		.dbcc_classguid = Xbox::guid,
	};
	HDEVNOTIFY ret = RegisterDeviceNotificationW(hwnd, &db, DEVICE_NOTIFY_WINDOW_HANDLE);

	if (ret == NULL)
	{
		return false;
	}

	return true;
}

void flt::OsWindows::InitializeExePath()
{
	wchar_t buffer[512];
	{
		DWORD result = GetModuleFileName(nullptr, buffer, 512);
		ASSERT(result != 0, "가져오기 실패");
	}

	_exePath = buffer;
	_exePath = _exePath.substr(0, _exePath.find_last_of(L"\\") + 1);
}

void flt::OsWindows::UpdateKeyState()
{
	for (int i = 0; i < (int)KeyCode::MAX; ++i)
	{
		_pKeyStates[i].isDown = false;
		_pKeyStates[i].isUp = false;
	}

	// keyUp 처리
	for (const auto& index : _keyUp)
	{
		_pKeyStates[index].isStay = false;
		_pKeyStates[index].isUp = true;
		_pKeyDatas[index].keyTime = 0;
	}
	_keyUp.clear();

	// 매 프레임 초기화 해줘야 하는 키들.
	// 현재 마우스 좌표, 휠업 다운.

	_pKeyStates[(int)KeyCode::mouseRelativePos].isStay = false;
	_pKeyDatas[(int)KeyCode::mouseRelativePos].keyTime = 0;
	_pKeyDatas[(int)KeyCode::mouseRelativePos].x = 0;
	_pKeyDatas[(int)KeyCode::mouseRelativePos].y = 0;

	_pKeyStates[(int)KeyCode::mouseAbsolutePos].isStay = false;
	_pKeyDatas[(int)KeyCode::mouseAbsolutePos].keyTime = 0;
	_pKeyDatas[(int)KeyCode::mouseAbsolutePos].x = 0;
	_pKeyDatas[(int)KeyCode::mouseAbsolutePos].y = 0;

	_pKeyStates[(int)KeyCode::mouseWheelUp].isStay = false;
	_pKeyDatas[(int)KeyCode::mouseWheelUp].keyTime = 0;
	_pKeyDatas[(int)KeyCode::mouseWheelUp].x = 0;
	_pKeyDatas[(int)KeyCode::mouseWheelUp].y = 0;

	_pKeyStates[(int)KeyCode::mouseWheelDown].isStay = false;
	_pKeyDatas[(int)KeyCode::mouseWheelDown].keyTime = 0;
	_pKeyDatas[(int)KeyCode::mouseWheelDown].x = 0;
	_pKeyDatas[(int)KeyCode::mouseWheelDown].y = 0;
}

void flt::OsWindows::UpdateGamePadVibration(float deltaSeconds)
{
	for (auto& pad : _pGamePads)
	{
		if (pad.isConnected == false)
		{
			continue;
		}

		if (pad.vibration.leftMotorPower <= 0 && pad.vibration.rightMotorPower <= 0)
		{
			continue;
		}

		if (pad.vibration.leftMotorTime > 0.0f)
		{
			pad.vibration.leftMotorTime -= deltaSeconds;
			if (pad.vibration.leftMotorTime <= 0.0f)
			{
				pad.vibration.leftMotorPower = 0;
				pad.vibration.leftMotorTime = 0.0f;
			}
		}
		if (pad.vibration.rightMotorTime > 0.0f)
		{
			pad.vibration.rightMotorTime -= deltaSeconds;
			if (pad.vibration.rightMotorTime <= 0.0f)
			{
				pad.vibration.rightMotorPower = 0;
				pad.vibration.rightMotorTime = 0.0f;
			}
		}

		Xbox::Set(&pad, pad.vibration.leftMotorPower, pad.vibration.rightMotorPower);
	}
}

void flt::OsWindows::HandleKeyboardRawData(const RAWKEYBOARD& data)
{
	auto code = _keyCodeMap[data.VKey];
	KeyData keyData;
	keyData.keyTime = _keyTimer.GetLabTimeMicroSeconds();
	keyData.x = 0;
	keyData.y = 0;

	// 확장키의경우 flag로 RI_KEY_E0 가 오는데 이때 KeyUp인지 Down인지 구분이 안됨.
	// 이경우에는 Message 값으로 판단을 해야하는데 확장 키가 아닐 경우에도 Message가 오기 때문에
	// 항상 Message로 판단.
	// SYS가 붙은경우 ALT와 함께 눌린 키를 의미함.
	//SetKeyState((KeyCode)code, keyData, data.Flags & RI_KEY_MAKE, data.Flags & RI_KEY_BREAK);
	SetKeyState((KeyCode)code,
		keyData,
		data.Message == WM_KEYDOWN || data.Message == WM_SYSKEYDOWN,
		data.Message == WM_KEYUP || data.Message == WM_SYSKEYUP
	);
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
		// RemoteDesktop으로 마우스를 작동 시에 절대 좌표로 넘어옴.
		//ASSERT(false, "아직 제대로 구현 안함");
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

void flt::OsWindows::HandleGamePadRawData(const RAWINPUT* raw)
{
	unsigned int dataSize = 32;
	RID_DEVICE_INFO deviceInfo{};
	UINT retval = GetRawInputDeviceInfo(raw->header.hDevice, RIDI_DEVICEINFO, &deviceInfo, &dataSize);
	if (retval == -1)
	{
		std::wstring error;
		unsigned int errorCode;
		this->GetError(&error, &errorCode);
		ASSERT(false, "GetRawInputDeviceInfo 실패");
	}

	// 현재 게임패드만 처리할 수 있기 때문에 게임패드가 아니라면 리턴.
	if (deviceInfo.hid.usUsagePage != 1 || deviceInfo.hid.usUsage != 0x05)
	{
		return;
	}

	GetRawInputDeviceInfo(raw->header.hDevice, RIDI_PREPARSEDDATA, NULL, &dataSize);

	std::unique_ptr<BYTE> pData(new(std::nothrow) BYTE[dataSize]);
	ASSERT(pData.get(), "메모리 동적할당 실패");

	if (GetRawInputDeviceInfo(raw->header.hDevice, RIDI_PREPARSEDDATA, pData.get(), &dataSize) == -1)
	{
		ASSERT(false, "RawInputDevice 정보 가져오기 실패");
	}

	// 게임패드가 맞기 때문에 게임패드라 생각하고 데이터 처리
	HIDP_CAPS caps;
	NTSTATUS ret = HidP_GetCaps((PHIDP_PREPARSED_DATA)pData.get(), &caps);
	ASSERT(ret == HIDP_STATUS_SUCCESS, "HidP_GetCaps 실패");
	std::unique_ptr<HIDP_BUTTON_CAPS[]> pButtonCaps(new HIDP_BUTTON_CAPS[caps.NumberInputButtonCaps]);

	unsigned short capsLength = caps.NumberInputButtonCaps;
	ret = HidP_GetButtonCaps(HidP_Input, pButtonCaps.get(), &capsLength, (PHIDP_PREPARSED_DATA)pData.get());
	ASSERT(ret == HIDP_STATUS_SUCCESS, "HidP_GetButtonCaps 실패");

	for (int i = 0; i < capsLength; ++i)
	{
		USHORT usageMin = pButtonCaps[i].Range.UsageMin;
		USHORT usageMax = pButtonCaps[i].Range.UsageMax;
		ULONG buttonNum = pButtonCaps[i].Range.UsageMax - usageMin + 1;
		ASSERT(buttonNum <= 16, "버튼 개수가 16개를 넘어갑니다.");

		USAGE usages[128];
		ret = HidP_GetUsages(HidP_Input, pButtonCaps[i].UsagePage, 0, usages, &buttonNum, (PHIDP_PREPARSED_DATA)pData.get(),
			(PCHAR)raw->data.hid.bRawData, raw->data.hid.dwSizeHid);

		ASSERT(ret == HIDP_STATUS_SUCCESS, "HidP_GetUsages 실패");

		bool isPressed[16] = { false, };
		for (unsigned int j = 0; j < buttonNum; ++j)
		{
			int index = usages[j] - usageMin;
			isPressed[index] = true;
		}

		for (int i = 0; i < 16; ++i)
		{
			KeyCode code;
			switch (i)
			{
				case 0:
				{
					code = KeyCode::gpadA;
				}
				break;
				case 1:
				{
					code = KeyCode::gpadB;
				}
				break;
				case 2:
				{
					code = KeyCode::gpadX;
				}
				break;
				case 3:
				{
					code = KeyCode::gpadY;
				}
				break;
				case 4:
				{
					code = KeyCode::gpadLB;
				}
				break;
				case 5:
				{
					code = KeyCode::gpadRB;
				}
				break;
				case 6:
				{
					code = KeyCode::gpadBack;
				}
				break;
				case 7:
				{
					code = KeyCode::gpadStart;
				}
				break;
				case 8:
				{
					code = KeyCode::gpadLStickClick;
				}
				break;
				case 9:
				{
					code = KeyCode::gpadRStickClick;
				}
				break;

				default:
					continue;
			}

			if (isPressed[i])
			{
				KeyData data = GetKey(code);
				if (!data)
				{
					data.keyTime = _keyTimer.GetLabTimeMicroSeconds();
					SetKeyState(code, data, true, false);
				}
			}
			else
			{
				KeyData data{};
				bool isUp = GetKey(code);
				SetKeyState(code, data, false, isUp);
			}
		}

		USHORT valueNum = caps.NumberInputValueCaps;
		std::unique_ptr<HIDP_VALUE_CAPS[]> pValueCaps(new HIDP_VALUE_CAPS[valueNum]);
		ret = HidP_GetValueCaps(HidP_Input, pValueCaps.get(), &valueNum, (PHIDP_PREPARSED_DATA)pData.get());
		ASSERT(ret == HIDP_STATUS_SUCCESS, "HidP_GetValueCaps 실패");

		for (int j = 0; j < valueNum; ++j)
		{
			ULONG value;
			ret = HidP_GetUsageValue(HidP_Input, pValueCaps[j].UsagePage, 0, pValueCaps[j].Range.UsageMin, &value, (PHIDP_PREPARSED_DATA)pData.get(),
				(PCHAR)raw->data.hid.bRawData, raw->data.hid.dwSizeHid);
			ASSERT(ret == HIDP_STATUS_SUCCESS, "HidP_GetUsageValue 실패");

			switch (ret)
			{
				case HIDP_STATUS_INCOMPATIBLE_REPORT_ID:
				{
					std::cout << "HIDP_INVALID_REPORT_LENGTH" << std::endl;
				}
				break;
				case HIDP_STATUS_INVALID_PREPARSED_DATA:
				{
					std::cout << "HIDP_INVALID_PREPARSED_DATA" << std::endl;
				}
				break;
				case HIDP_STATUS_USAGE_NOT_FOUND:
				{
					std::cout << "HIDP_STATUS_USAGE_NOT_FOUND" << std::endl;
				}
				break;
			}

			//std::cout << pValueCaps[j].LogicalMin << " " << pValueCaps[j].Range.UsageMax << ", " << pValueCaps[j].Range.DataIndexMax << std::endl;
			switch (pValueCaps[j].Range.UsageMin)
			{
				case 0x31:
				{
					// LStick Y
					std::cout << "0x31 : " << (LONG)value << std::endl;
				}
				break;
				case 0x30:
				{
					// LStick X
					std::cout << "0x30 : " << (LONG)value << std::endl;
				}
				break;

				case 0x34:
				{
					// RStick Y
					std::cout << "0x34 : " << (LONG)value << std::endl;
				}
				break;
				case 0x33:
				{
					// RStick X
					std::cout << "0x33 : " << (LONG)value << std::endl;
				}
				break;

				case 0x32:
				{
					// 128~32768 (RTrigger)
					// 32768 ~ 65408 (LTrigger)
					std::cout << "0x32 : " << value << std::endl;

				}
				break;

				case 0x39:
				{
					// Dpad 입력.
					// 0 -> 입력 없음
					// 1 -> ↑
					// 2 -> ↗
					// 3 -> →
					// 4 -> ↘
					// 5 -> ↓
					// 6 -> ↙
					// 7 -> ←
					// 8 -> ↖
					std::cout << "dpad : " << value << std::endl;
				}
				break;

				default:
				{
					std::cout << "default : " << pValueCaps[j].Range.UsageMin << " : " << (LONG)value << std::endl;
				}
				break;
			}
		}
	}
}

void flt::OsWindows::SetKeyState(KeyCode code, const KeyData& data, bool isKeyDown, bool isKeyUp)
{
	if (isKeyDown)
	{
		if (_pKeyDatas[(int)code].keyTime == 0)
		{
			_pKeyDatas[(int)code] = data;
			_pKeyStates[(int)code].isDown = true;
		}

		_pKeyStates[(int)code].isStay = true;
	}

	if (isKeyUp)
	{
		_keyUp.push_back((int)code);
	}

	ASSERT((isKeyDown != true || isKeyUp != true), "둘 다 True인 경우가 있는지 체크.");
}

void flt::OsWindows::SetKeyState(KeyCode code, const KeyData& data, bool isActive)
{
	if (isActive)
	{
		if (_pKeyDatas[(int)code].keyTime == 0)
		{
			_pKeyDatas[(int)code] = data;
			_pKeyStates[(int)code].isDown = true;
		}

		_pKeyStates[(int)code].isStay = true;
	}
	else
	{
		_keyUp.push_back((int)code);
	}
}

flt::WinGamePad* flt::OsWindows::FindEmptyGamePad(uint64 hash)
{
	//먼저 전에 연결한 적이 있는 패드면 해당 슬롯에 연결
	for (int i = 0; i < 16; ++i)
	{
		if (_pGamePads[i].isConnected == true)
		{
			continue;
		}

		if (_pGamePads[i].hash == hash)
		{
			return &_pGamePads[i];
		}
	}

	// 먼저 한번도 연결된적이 없는 슬롯을 찾아보자.
	for (int i = 0; i < 16; ++i)
	{
		if (_pGamePads[i].isConnected == true || _pGamePads[i].hash != 0)
		{
			continue;
		}

		return &_pGamePads[i];
	}

	// 가장 빠른 슬롯을 할당한다.
	for (int i = 0; i < 16; ++i)
	{
		if (_pGamePads[i].isConnected == true)
		{
			continue;
		}

		return &_pGamePads[i];
	}

	return nullptr;
}

LRESULT WINAPI flt::OsWindows::WinProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	static OsWindows* thisPtr = nullptr;	// WM_CREATE에서 할당

	switch (msg)
	{
		case WM_INPUT:
		{
			UINT dwSize = 0;
			GetRawInputData((HRAWINPUT)lParam, RID_INPUT, NULL, &dwSize, sizeof(RAWINPUTHEADER));

			std::unique_ptr<BYTE> lpb(new(std::nothrow) BYTE[dwSize]);
			ASSERT(lpb.get(), "메모리 동적할당 실패");

			if (GetRawInputData((HRAWINPUT)lParam, RID_INPUT, lpb.get(), &dwSize, sizeof(RAWINPUTHEADER)) != dwSize)
			{
				ASSERT(false, "RawInputData 가져오기 실패");
				break;
			}

			RAWINPUT* raw = (RAWINPUT*)lpb.get();


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
				//thisPtr->HandleGamePadRawData(raw);
				//std::cout << "test " << x << std::endl;
			}
		}
		break;

		// 창 메뉴에서 명령 선택이나 단추 클릭시 이 메시지 호출
		//case WM_SYSCOMMAND:
		//{
		//	if ((wParam & 0xFFF0) == SC_CLOSE)
		//	{
		//		::PostQuitMessage(0);
		//	}
		//}
		//break;

		case WM_SETCURSOR:
		{
			if (LOWORD(lParam) == HTCLIENT)
			{
				// 커서 숨기기
				//SetCursor(NULL);

				// 커서를 기본으로 변경
				SetCursor(LoadCursor(NULL, IDC_ARROW));
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
			int width = LOWORD(lParam);
			int height = HIWORD(lParam);
			for (auto& renderer : thisPtr->_rendererMap)
			{
				renderer.first->Resize(width, height);
			}
			return 0;
		}
		break;

		// RegisterRawInputDevices 한 디바이스가 변경 되면 호출
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

			std::cout << "Device Change : " << deviceCount << std::endl;
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

				std::wcout << L"TYPE : " << pDeviceList[i].dwType << L" | " << buffer << std::endl;
			}

			std::cout << std::endl;
		}
		break;

		// 키보드 등이 연결/해제 되면 호출
		case WM_DEVICECHANGE:
		{
			DEV_BROADCAST_HDR* hdr = (DEV_BROADCAST_HDR*)lParam;

			if (hdr == nullptr)
			{
				break;
			}

			if (hdr->dbch_devicetype == DBT_DEVTYP_DEVICEINTERFACE)
			{
				DEV_BROADCAST_DEVICEINTERFACE_W* dif = (DEV_BROADCAST_DEVICEINTERFACE_W*)hdr;
				// 디바이스가 연결되었을 때
				if (wParam == DBT_DEVICEARRIVAL)
				{
					std::wstring data = dif->dbcc_name;
					std::locale loc;
					std::transform(data.begin(), data.end(), data.begin(), [&loc](wchar_t c) { return std::tolower(c, loc); });
					uint64 dataLen = wcslen(dif->dbcc_name) * sizeof(wchar_t);
					uint64 hash = flt::hash::xxh64::hash((char*)data.data(), dataLen, 0);
					ASSERT(hash != 0, "해시값이 0이면 처음 연결되는것으로 세팅되어있음. 값이 0이면 구분못하기 때문에 0이면 안됨.");

					WinGamePad* emptyGamePad = thisPtr->FindEmptyGamePad(hash);

					if (emptyGamePad == nullptr)
					{
						ASSERT(false, "게임패드 슬롯이 부족합니다.");
					}
					else
					{
						emptyGamePad->path = std::move(data);
						flt::Xbox::Connect(emptyGamePad);
					}
				}
				else if (wParam == DBT_DEVICEREMOVECOMPLETE)
				{
					for (int i = 0; i < 16; ++i)
					{
						if (thisPtr->_pGamePads[i].isConnected == false)
						{
							continue;
						}

						if (_wcsicmp(thisPtr->_pGamePads[i].path.c_str(), dif->dbcc_name) != 0)
						{
							continue;
						}

						flt::Xbox::Disconnect(&thisPtr->_pGamePads[i]);
						break;
					}
				}
			}

			/*UINT deviceCount = 0;

			if (GetRawInputDeviceList(NULL, &deviceCount, sizeof(RAWINPUTDEVICELIST)) == -1)
			{
				ASSERT(false, "RawInputDeviceList 정보 가져오기 실패");
			}

			std::unique_ptr<RAWINPUTDEVICELIST[]> pDeviceList(new(std::nothrow) RAWINPUTDEVICELIST[deviceCount]);
			ASSERT(pDeviceList != nullptr, "메모리 동적 할당 실패");

			int result = GetRawInputDeviceList(pDeviceList.get(), &deviceCount, sizeof(RAWINPUTDEVICELIST));
			ASSERT(result != -1, "RawInputDeviceList 정보 가져오기 실패");

			std::cout << "Device Change : " << deviceCount << std::endl;
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

				std::wcout << L"TYPE : " << pDeviceList[i].dwType << L" | " << buffer << std::endl;
			}

			std::cout << std::endl;*/
		}
		break;

		case WM_CREATE:
		{
			// 앞으로 this를 사용할 수 있도록 OsWindows객체의 포인터를 thisPtr에 저장.
			thisPtr = (OsWindows*)(((LPCREATESTRUCT)lParam)->lpCreateParams);
			thisPtr->_isClosed = false;

			Xbox::Initialize(thisPtr->_pGamePads, sizeof(thisPtr->_pGamePads) / sizeof(thisPtr->_pGamePads[0]));

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

				/// usUsagePage
				// 1 - Generic Desktop Controls -> 보통은 이걸 사용.
				// 2 - Simulation Controls
				// 3 - VR
				// 4 - Sport
				// 5 - Game
				// 6 - Generic Device
				// 7 - Keyboard
				// 8 - LED
				// 9 - Button

				/// usUsagePage 1일 경우 usUsage 정보
				// 0 미정의
				// 1 포인터
				// 2 마우스
				// 3 예약됨
				// 4 조이스틱
				// 5 게임 패드
				// 6 키보드
				// 7 키패드
				// 8 멀티 축 컨트롤러
				// 9 태블릿 pc 컨트롤

				switch (rdi.dwType)
				{
					case RIM_TYPEMOUSE:
					{
						/// 디버깅용
						//std::wcout << L"	Mouse" << std::endl;
						//std::wcout << L"	ID: " << rdi.mouse.dwId << std::endl;
						//std::wcout << L"	Number of Buttons: " << rdi.mouse.dwNumberOfButtons << std::endl;
						//std::wcout << L"	Sample Rate: " << rdi.mouse.dwSampleRate << std::endl;

						RAWINPUTDEVICE rawInputDevice;

						rawInputDevice.dwFlags = 0;
						// RIDEV_NOLEGACY;	// 레거시 메시지 생성 금지, 마우스의 경우 윈도우 창에서 작업중 표시로 바뀌며 닫기 버튼등이 동작하지 않음.
						// RIDEV_CAPTUREMOUSE; // 마우스가 포커스가 없는 창 위에 있어도 메시지를 받음, RIDEV_NOLEGACY와 같이 사용해야함
						// RIDEV_INPUTSINK; // 백 그라운드에서도 인풋 받기
						rawInputDevice.usUsagePage = 1;
						rawInputDevice.usUsage = 2;
						rawInputDevice.hwndTarget = hwnd;

						result = RegisterRawInputDevices(&rawInputDevice, 1, sizeof(rawInputDevice));

						if (result == FALSE)
						{
							std::wstring errorMsg;
							unsigned int errorCode;
							auto r = thisPtr->GetError(&errorMsg, &errorCode);
							ASSERT(result != FALSE, "RawInputDevice 등록 실패");
						}
					}

					break;
					case RIM_TYPEKEYBOARD:
					{
						/// 디버깅용
						//std::wcout << L"	Keyboard" << std::endl;
						//std::wcout << L"	Type: " << rdi.keyboard.dwType << std::endl;
						//std::wcout << L"	Sub Type: " << rdi.keyboard.dwSubType << std::endl;
						//std::wcout << L"	Keyboard Mode: " << rdi.keyboard.dwKeyboardMode << std::endl;
						//std::wcout << L"	Number of Function Keys: " << rdi.keyboard.dwNumberOfFunctionKeys << std::endl;
						//std::wcout << L"	Number of Indicators: " << rdi.keyboard.dwNumberOfIndicators << std::endl;
						//std::wcout << L"	Number of Keys Total: " << rdi.keyboard.dwNumberOfKeysTotal << std::endl;

						RAWINPUTDEVICE rawInputDevice;

						rawInputDevice.dwFlags =
							RIDEV_NOLEGACY; // 마우스/키보드의 레거시 메시지를 생성하지 않음 -> alt f4등이 동작하지 않음. WM_KEYDOWN메시지가 생성되지 않음. 성능상 이득이 있음
						// RIDEV_NOHOTKEYS; // 윈도우 등 핫키 무시하기
						// RIDEV_INPUTSINK; // 백 그라운드에서도 인풋 받기, hwndTarget지정 필요
						// RIDEV_EXINPUTSINK; // 포 그라운드에서 처리하지 않는 경우에만 입력 수신(포 그라운드에서 RAWINPUT에 등록이 안된 경우)
						// RIDEV_DEVNOTIFY; // 장비 제거 등 메시지를 WM_INPUT_DEVICE_CHANGE 로 받음

						rawInputDevice.usUsagePage = 1;
						rawInputDevice.usUsage = 6;
						rawInputDevice.hwndTarget = hwnd;

						result = RegisterRawInputDevices(&rawInputDevice, 1, sizeof(rawInputDevice));

						if (result == FALSE)
						{
							std::wstring errorMsg;
							unsigned int errorCode;
							auto r = thisPtr->GetError(&errorMsg, &errorCode);
							ASSERT(result != FALSE, "RawInputDevice 등록 실패");
						}
					}
					break;
					case RIM_TYPEHID:
					{
						/// 디버깅용
						//std::wcout << L"	HID" << std::endl;
						//std::wcout << L"	Vendor ID: " << rdi.hid.dwVendorId << std::endl;
						//std::wcout << L"	Product ID: " << rdi.hid.dwProductId << std::endl;
						//std::wcout << L"	Version Number: " << rdi.hid.dwVersionNumber << std::endl;
						//std::wcout << L"	Usage for the top-level collection: " << rdi.hid.usUsage << std::endl;
						//std::wcout << L"	Usage Page for the top-level collection: " << rdi.hid.usUsagePage << std::endl;

						//if (rdi.hid.usUsagePage == 1 && rdi.hid.usUsage == 4)
						//{
						//	std::cout << "test joyStick" << std::endl;
						//}

						//// 게임 패드이면
						//if (rdi.hid.usUsagePage == 1 && (rdi.hid.usUsage == 5 || rdi.hid.usUsage == 6))
						//{
						//	RAWINPUTDEVICE rawInputDevice{};

						//	rawInputDevice.dwFlags = 0;
						//	rawInputDevice.usUsagePage = 1; // rdi.hid.usUsagePage;
						//	rawInputDevice.usUsage = 5; // rdi.hid.usUsage;
						//	rawInputDevice.hwndTarget = hwnd;

						//	result = RegisterRawInputDevices(&rawInputDevice, 1, sizeof(rawInputDevice));

						//	if (result == FALSE)
						//	{
						//		std::wstring errorMsg;
						//		unsigned int errorCode;
						//		auto r = thisPtr->GetError(&errorMsg, &errorCode);
						//		ASSERT(result != FALSE, "RawInputDevice 등록 실패");
						//	}
						//}
					}
					break;
					default:
						// std::wcout << L"	Unknown" << std::endl;
						break;
				}
			}
		}
		break;

		case WM_DESTROY:
		{
			thisPtr->_isClosed = true;
			::PostQuitMessage(0);
			//return 0;
		}
		break;

		case WM_CLOSE:
		{
			thisPtr->_isClosed = true;
			::PostQuitMessage(0);
			//return 0;
		}
		break;

		case SC_CLOSE:
		{
			thisPtr->_isClosed = true;
			::PostQuitMessage(0);
			//return 0;
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
	NULL,	//	0x5E : reserved
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
	(unsigned char)KeyCode::f13,	// VK_F13            0x7C
	(unsigned char)KeyCode::f14,	// VK_F14            0x7D
	(unsigned char)KeyCode::f15,	// VK_F15            0x7E
	(unsigned char)KeyCode::f16,	// VK_F16            0x7F
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
	(unsigned char)KeyCode::volumMute,	// VK_VOLUME_MUTE         0xAD
	(unsigned char)KeyCode::volumDown,	// VK_VOLUME_DOWN         0xAE
	(unsigned char)KeyCode::volumUp,	// VK_VOLUME_UP           0xAF
	(unsigned char)KeyCode::mediaNext,	// VK_MEDIA_NEXT_TRACK    0xB0
	(unsigned char)KeyCode::mediaPrev,	// VK_MEDIA_PREV_TRACK    0xB1
	(unsigned char)KeyCode::mediaStop,	// VK_MEDIA_STOP          0xB2
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
