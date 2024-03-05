#pragma once

/// <summary>
/// 엔진의 Process를 담당하는 클래스의 인터페이스.
/// 엔진을 초기화, 엔진의 한 싸이클 내의 순서, 엔진의 마무리를 정의한다.
/// 
/// 23.06.20 강석원 인재원.
/// </summary>
namespace Rocket
{
	class IEngineProcess abstract
	{
		/// 초기화, Loop 관련 함수들.
	public:
		virtual long Initialize(void* hInstance, int nCmdShow) abstract;
		virtual void Finalize() abstract;
		
		/// 엔진 라이프사이클 관련 함수들.
	public:
		// 엔진을 라이프사이클에 맞게 반복시킨다.
		virtual void RunEngine() abstract;
		
		/// Resize 함수.
	public:
		virtual void Resize(int width, int height) abstract;
	};

	extern "C" __declspec(dllexport) IEngineProcess * CreateEngine();
	extern "C" __declspec(dllexport) void ReleaseEngine(IEngineProcess * instance);
}
