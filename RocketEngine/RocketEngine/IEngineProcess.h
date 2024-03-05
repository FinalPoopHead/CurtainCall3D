#pragma once

/// <summary>
/// ������ Process�� ����ϴ� Ŭ������ �������̽�.
/// ������ �ʱ�ȭ, ������ �� ����Ŭ ���� ����, ������ �������� �����Ѵ�.
/// 
/// 23.06.20 ������ �����.
/// </summary>
namespace Rocket
{
	class IEngineProcess abstract
	{
		/// �ʱ�ȭ, Loop ���� �Լ���.
	public:
		virtual long Initialize(void* hInstance, int nCmdShow) abstract;
		virtual void Finalize() abstract;
		
		/// ���� ����������Ŭ ���� �Լ���.
	public:
		// ������ ����������Ŭ�� �°� �ݺ���Ų��.
		virtual void RunEngine() abstract;
		
		/// Resize �Լ�.
	public:
		virtual void Resize(int width, int height) abstract;
	};

	extern "C" __declspec(dllexport) IEngineProcess * CreateEngine();
	extern "C" __declspec(dllexport) void ReleaseEngine(IEngineProcess * instance);
}
