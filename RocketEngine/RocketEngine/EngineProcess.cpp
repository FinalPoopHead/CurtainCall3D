#define _SILENCE_CXX20_CISO646_REMOVED_WARNING
#include "EngineProcess.h"
#include "ObjectSystem.h"
#include "GraphicsSystem.h"
#include "SceneSystem.h"
#include "TimeSystem.h"
#include "InputSystem.h"
#include "TweenSystem.h"
#include "PhysicsSystem.h"
#include "SoundSystem.h"
#include "DebugSystem.h"
#include "UISystem.h"
#include "DataSystem.h"
#include "ResourceSystem.h"

constexpr int WIDTH = 1920;
constexpr int HEIGHT = 1080;

static Rocket::Core::GraphicsSystem* RENDERSYSTEM;
static bool isInitialized = false;

namespace Rocket
{
	/// dll관련
	IEngineProcess* CreateEngine()
	{
		return new EngineProcess();
	}

	void ReleaseEngine(IEngineProcess* instance)
	{
		delete instance;
	}

	/// 초기화 관련
	EngineProcess::EngineProcess()
		: _objectSystem(Rocket::Core::ObjectSystem::Instance()),
		_graphicsSystem(Rocket::Core::GraphicsSystem::Instance()),
		_sceneSystem(Rocket::Core::SceneSystem::Instance()),
		_timeSystem(Rocket::Core::TimeSystem::Instance()),
		_inputSystem(Rocket::Core::InputSystem::Instance()),
		_tweenSystem(Rocket::Core::TweenSystem::Instance()),
		_physicsSystem(Rocket::Core::PhysicsSystem::Instance()),
		_soundSystem(Rocket::Core::SoundSystem::Instance()),
		_debugSystem(Rocket::Core::DebugSystem::Instance()),
		_uiSystem(Rocket::Core::UISystem::Instance()),
		_resourceSystem(Rocket::Core::ResourceSystem::Instance()),
		//_dataSystem(Rocket::Core::DataSystem::Instance()),
		_hWnd(), _msg(), _screenWidth(WIDTH),_screenHeight(HEIGHT),
		CLASSNAME(L"V-Gun"), WINDOWNAME(L"V-Gun"),
		_isDebugMode(false)
	{
		RENDERSYSTEM = &_graphicsSystem;

		int screenWidth = GetSystemMetrics(SM_CXSCREEN); // 모니터의 가로 크기
		int screenHeight = GetSystemMetrics(SM_CYSCREEN); // 모니터의 세로 크기

//  	_screenWidth = screenWidth;
//  	_screenHeight = screenHeight;
		_screenWidth = WIDTH;
		_screenHeight = HEIGHT;
	}

	long EngineProcess::Initialize(void* hInstance, int nCmdShow)
	{
// #ifndef _DEBUG
// 		ChangeDisplayResolution(1920, 1080);
// #endif // _DEBUG
		//ChangeDisplayResolution(1920, 1080);

		/// 윈도우 초기화
		MyRegisterClass((HINSTANCE)hInstance);

		if (!InitInstance((HINSTANCE)hInstance, nCmdShow))
		{
			return S_FALSE;
		}

		RECT rect;
		GetClientRect(_hWnd, &rect);

		_objectSystem.Initialize();
		_graphicsSystem.Initialize(_hWnd, _screenWidth, _screenHeight);
		_sceneSystem.Initialize();
		_timeSystem.Initialize();
		_inputSystem.Initialize(_hWnd, _screenWidth, _screenHeight);
		_tweenSystem.Initialize();
		//_physicsSystem.Initialize();
		_soundSystem.Initialize();
		_debugSystem.Initialize();
		_uiSystem.Initialize();
		//_dataSystem.Initialize();
		_resourceSystem.Initialize();

		isInitialized = true;

		return S_OK;
	}

	long EngineProcess::InitializeForEditor(void* hWnd)
	{
		_objectSystem.Initialize();
		_graphicsSystem.Initialize(static_cast<HWND>(hWnd), _screenWidth, _screenHeight, true);
		_sceneSystem.Initialize();
		_timeSystem.Initialize();
		_inputSystem.Initialize(static_cast<HWND>(hWnd), _screenWidth, _screenHeight);
		_tweenSystem.Initialize();
		//_physicsSystem.Initialize();
		_soundSystem.Initialize();
		_debugSystem.Initialize();
		_uiSystem.Initialize();
		//_dataSystem 은 에디터에서 필요 없음. 자체적으로 뭔가있기때문.


		return S_OK;
	}

	void EngineProcess::RunCycleOnceForEditor()
	{
		_timeSystem.MeasureTime();

// 		_physicsSystem.PhysicsUpdate(_timeSystem.GetDeltaTime());
// 		_physicsSystem.Flush();

		_objectSystem.FlushEnable();
		_inputSystem.InputUpdateForEditor();

		//_uiSystem.CheckFocus();

		_sceneSystem.UpdateLoadScene();
		_objectSystem.StartCurrentScene();
		_tweenSystem.Update();
		_objectSystem.UpdateCurrentScene();
		_soundSystem.UpdateSound();
		_debugSystem.CheckDebugMode();

		_objectSystem.LateUpdateCurrentScene();
		 
/*		_physicsSystem.UpdateToPhysics();*/

		_graphicsSystem.DrawProcess();
		_objectSystem.FlushDisable();

		_inputSystem.FlushForEditor();
	}

	void EngineProcess::Finalize()
	{
		//_dataSystem.Finalize();
		_uiSystem.Finalize();
		_debugSystem.Finalize();
		//_soundSystem.Finalize();
		//_physicsSystem.Finalize();
		_tweenSystem.Finalize();
		_inputSystem.Finalize();
		_timeSystem.Finalize();
		_sceneSystem.Finalize();
		_graphicsSystem.Finalize();
		_objectSystem.Finalize();
	}

	/// <summary>
	/// 엔진의 라이프싸이클
	/// </summary>
	void EngineProcess::RunEngine()
	{
		// 프로세스 내에서 하나의 엔진 인스턴스만 돌고 있을 수 있도록 함
		static EngineProcess* instance;
		if (instance == nullptr)
		{
			instance = this;
		}

		if (instance != this)
		{
			return;
		}

		while (1)
		{
			if (PeekMessage(&_msg, NULL, 0, 0, PM_REMOVE))
			{
				if (_msg.message == WM_QUIT)
				{
					break;
				}

				DispatchMessage(&_msg);
			}
			else
			{
				RunCycleOnce();
			}
		}

		// 엔진의 동작이 종료되면 instance도 nullptr로 초기화
		instance = nullptr;
	}

	void EngineProcess::RunCycleOnce()
	{
		_timeSystem.MeasureTime();

		_objectSystem.FlushEnable();

// 		_physicsSystem.PhysicsUpdate(_timeSystem.GetDeltaTime());
// 		_physicsSystem.Flush();

		_inputSystem.InputUpdate();

		if (_inputSystem.GetKeyDown(VK_F1))
		{
			_isDebugMode = !_isDebugMode;
			_graphicsSystem.SetDebugMode(_isDebugMode);
		}

		_uiSystem.CheckFocusCurrentScene();

		_sceneSystem.UpdateLoadScene();
		_objectSystem.StartCurrentScene();
		_tweenSystem.Update();
		_objectSystem.UpdateCurrentScene();
		_soundSystem.UpdateSound();
		_debugSystem.CheckDebugMode();

		_objectSystem.LateUpdateCurrentScene();

/*		_physicsSystem.UpdateToPhysics();*/

		_graphicsSystem.DrawProcess();
		_objectSystem.FlushDisable();

		_inputSystem.Flush();
	}

	void EngineProcess::Resize(int width, int height)
	{

	}

	ATOM EngineProcess::MyRegisterClass(HINSTANCE hInstance)
	{
		WNDCLASSEXW wcex;

		wcex.cbSize = sizeof(WNDCLASSEX);

		wcex.style = CS_HREDRAW | CS_VREDRAW;
		wcex.lpfnWndProc = WndProc;
		wcex.cbClsExtra = 0;
		wcex.cbWndExtra = 0;
		wcex.hInstance = hInstance;
		wcex.hIcon = NULL;
		wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
		wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
		wcex.lpszMenuName = NULL; // MAKEINTRESOURCEW(IDC_FIRSTD2D);
		wcex.lpszClassName = CLASSNAME;
		wcex.hIconSm = NULL;

		return RegisterClassExW(&wcex);
	}

	BOOL EngineProcess::InitInstance(HINSTANCE hInstance, int nCmdShow)
	{
// 
// 		_hWnd = CreateWindow(CLASSNAME, WINDOWNAME, WS_OVERLAPPED,
// 			CW_USEDEFAULT, CW_USEDEFAULT, _screenWidth, _screenHeight, nullptr, nullptr, hInstance, nullptr);
// #else
// 		_hWnd = CreateWindow(CLASSNAME, WINDOWNAME, WS_POPUP,
// 			CW_USEDEFAULT, CW_USEDEFAULT, _screenWidth, _screenHeight, nullptr, nullptr, hInstance, nullptr);
// #endif

		_hWnd = CreateWindow(CLASSNAME, WINDOWNAME, WS_OVERLAPPED | WS_OVERLAPPEDWINDOW,
			CW_USEDEFAULT, CW_USEDEFAULT, _screenWidth, _screenHeight, nullptr, nullptr, hInstance, nullptr);

		if (!_hWnd)
		{
			return FALSE;
		}

		ShowWindow(_hWnd, nCmdShow);
		UpdateWindow(_hWnd);

		return TRUE;
	}

	void EngineProcess::ChangeDisplayResolution(int width, int height)
	{
		DEVMODE devMode = {};
		devMode.dmSize = sizeof(DEVMODE);

		// 현재 디스플레이 모드 가져오기
		EnumDisplaySettings(NULL, ENUM_CURRENT_SETTINGS, &devMode);

		// 새로운 해상도 설정
		devMode.dmPelsWidth = width; // 원하는 가로 해상도
		devMode.dmPelsHeight = height; // 원하는 세로 해상도
		devMode.dmFields = DM_PELSWIDTH | DM_PELSHEIGHT;

		// 해상도 변경 시도
		LONG result = ChangeDisplaySettings(&devMode, CDS_FULLSCREEN);
	}

	LRESULT CALLBACK EngineProcess::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
	{
		switch (message)
		{
			case WM_SIZE:
// 				if (!isInitialized)
// 				{
// 					break;
// 				}
// 
// 				RECT temp;
// 				GetWindowRect(hWnd, &temp);
// 				RENDERSYSTEM->OnResize(temp.right - temp.left, temp.bottom - temp.top);
				break;
			case WM_DESTROY:
				ChangeDisplaySettings(NULL, 0);
				PostQuitMessage(0);
				break;
			default:
				return DefWindowProc(hWnd, message, wParam, lParam);
		}
		return 0;
	}

	void EngineProcess::RecalcWindowSize()
	{

	}
}
