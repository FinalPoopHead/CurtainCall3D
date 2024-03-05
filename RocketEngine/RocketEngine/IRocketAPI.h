#pragma once


/// <summary>
/// API를 그냥 함수로 랩핑해서 넘기는게 나은가?
/// 아니면 클래스로 만들어서 로드하는 쪽에서 객체를 생성하는게 나은가?
/// 뭐가 나은가..?
/// 
/// 23.07.13 강석원 인재원.
/// </summary>
namespace Rocket
{
	class IRocketAPI abstract
	{

	};

	extern "C" __declspec(dllexport) IRocketAPI* CreateAPI();
	extern "C" __declspec(dllexport) void FreeAPI(IRocketAPI* instance);
}
