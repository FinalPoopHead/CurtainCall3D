#pragma once


/// <summary>
/// API�� �׳� �Լ��� �����ؼ� �ѱ�°� ������?
/// �ƴϸ� Ŭ������ ���� �ε��ϴ� �ʿ��� ��ü�� �����ϴ°� ������?
/// ���� ������..?
/// 
/// 23.07.13 ������ �����.
/// </summary>
namespace Rocket
{
	class IRocketAPI abstract
	{

	};

	extern "C" __declspec(dllexport) IRocketAPI* CreateAPI();
	extern "C" __declspec(dllexport) void FreeAPI(IRocketAPI* instance);
}
