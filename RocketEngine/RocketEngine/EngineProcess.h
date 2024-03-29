#pragma once
#include "DLLExporter.h"
#include "IEngineProcess.h"
#include <memory>
#include <windows.h>

namespace SAD
{
	class SadScene;
}

namespace Rocket::Core
{
	class ObjectSystem;
	class GraphicsSystem;
	class SceneSystem;
	class TimeSystem;
	class InputSystem;
	class TweenSystem;
	class PhysicsSystem;
	class SoundSystem;
	class DebugSystem;
	class UISystem;
	class DataSystem;
	class ResourceSystem;
}

namespace Rocket
{
	class ROCKET_API EngineProcess final : public IEngineProcess
	{
		friend class SAD::SadScene;

	private:
		long InitializeForEditor(void* hWnd);
		void RunCycleOnceForEditor();

		/// Initialize, Finalize
	public:
		// 로켓런처 생성자. System들을 각각의 unique_ptr에 생성한다.
		EngineProcess();

		// 엔진 초기화 함수. Window창을 초기화하고 System들을 초기화한다.
		long Initialize(void* hInstance, int nCmdShow) override;

		// 엔진 마무리 함수.
		void Finalize() override;

		/// 엔진 라이프사이클에 관련된 함수들.
	public:
		// 엔진을 라이프사이클에 맞춰 반복시킨다.
		void RunEngine() override;

	private:
		// 엔진을 라이프사이클에 맞게 1회 동작시킨다.
		void RunCycleOnce();


		/// 윈도우 관련 (창사이즈 변경, 마우스 입력)
		/// Resize 함수.
	public:
		void Resize(int width, int height) override;

		/// System들 스마트포인터
		/// 내부적으로만 사용한다. 아마 C4251 경고가 발생할 것.
		/// 경고를 어떻게 해제할지 잘 고민해봐야 할듯.
		/// 23.06.30 강석원 인재원.
	private:
		Rocket::Core::ObjectSystem& _objectSystem;
		Rocket::Core::GraphicsSystem& _graphicsSystem;
		Rocket::Core::SceneSystem& _sceneSystem;
		Rocket::Core::TimeSystem& _timeSystem;
		Rocket::Core::InputSystem& _inputSystem;
		Rocket::Core::TweenSystem& _tweenSystem;
		Rocket::Core::PhysicsSystem& _physicsSystem;
		Rocket::Core::SoundSystem& _soundSystem;
		Rocket::Core::DebugSystem& _debugSystem;
		Rocket::Core::UISystem& _uiSystem;
		Rocket::Core::ResourceSystem& _resourceSystem;
		//Rocket::Core::DataSystem& _dataSystem;


	private:
		bool _isDebugMode;

		/// 윈도우 관련
	private:
		HWND _hWnd;
		MSG _msg;
		int _screenWidth;
		int _screenHeight;
		const WCHAR* CLASSNAME;
		const WCHAR* WINDOWNAME;

	private:
		ATOM MyRegisterClass(HINSTANCE hInstance);
		BOOL InitInstance(HINSTANCE hInstance, int nCmdShow);
		void ChangeDisplayResolution(int width, int height);

		// 메시지 핸들러 (윈도 콜백)
		static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

		// for Initialize
		void RecalcWindowSize();

	};
}
