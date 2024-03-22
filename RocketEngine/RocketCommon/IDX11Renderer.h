#pragma once
/// <summary>
/// 로켓 엔진이 사용할 렌더러의 인터페이스
/// 그래픽스 렌더러는 이 인터페이스를 상속받아 구현해야한다.
/// 로켓 엔진에서는 이 인터페이스를 통해 렌더러를 동작시킨다.
/// </summary>

namespace Rocket::Core
{
	class IDX11Renderer
	{
	public:
		virtual void Initialize(void* hWnd, int screenWidth, int screenHeight) abstract;	//그래픽스 엔진을 초기화한다.

		virtual void SetDebugMode(bool isDebug) abstract;	// 디버그 모드를 설정한다.

		virtual void Update(float deltaTime) abstract;		// 그래픽스 엔진을 업데이트한다.

		virtual void Render() abstract;						// 그래픽스 엔진을 렌더링한다.	

		virtual void OnResize(int _width, int _height) abstract;	// 그래픽스 엔진의 윈도우 크기를 조정한다.

		virtual void Finalize() abstract;					// 그래픽스 엔진을 종료한다.
	};

	/// dllexport 하는 렌더러를 반환하는 팩토리 함수.

	// IRocketGraphics를 상속받는 렌더러를 생성해서 업캐스팅하여 반환한다.
	extern "C" __declspec(dllexport) IDX11Renderer * CreateGraphicsInstance();

	// 렌더러를 로드한쪽에서 렌더러를 해제할때 사용하는 함수.
	// 로드하는쪽에서 delete하지 않고 dll한테 요청하는 식으로 구성했다.
	// 각각의 렌더러에 맞는 Release 동작이 정의되어 있을 것이다.
	extern "C" __declspec(dllexport) void ReleaseGraphicsInstance(IDX11Renderer * instance);
}
