#pragma once
/// <summary>
/// ���� ������ ����� �������� �������̽�
/// �׷��Ƚ� �������� �� �������̽��� ��ӹ޾� �����ؾ��Ѵ�.
/// ���� ���������� �� �������̽��� ���� �������� ���۽�Ų��.
/// </summary>

namespace Rocket::Core
{
	class IDX11Renderer
	{
	public:
		virtual void Initialize(void* hWnd, int screenWidth, int screenHeight) abstract;	//�׷��Ƚ� ������ �ʱ�ȭ�Ѵ�.

		virtual void SetDebugMode(bool isDebug) abstract;	// ����� ��带 �����Ѵ�.

		virtual void Update(float deltaTime) abstract;		// �׷��Ƚ� ������ ������Ʈ�Ѵ�.

		virtual void Render() abstract;						// �׷��Ƚ� ������ �������Ѵ�.	

		virtual void OnResize(int _width, int _height) abstract;	// �׷��Ƚ� ������ ������ ũ�⸦ �����Ѵ�.

		virtual void Finalize() abstract;					// �׷��Ƚ� ������ �����Ѵ�.
	};

	/// dllexport �ϴ� �������� ��ȯ�ϴ� ���丮 �Լ�.

	// IRocketGraphics�� ��ӹ޴� �������� �����ؼ� ��ĳ�����Ͽ� ��ȯ�Ѵ�.
	extern "C" __declspec(dllexport) IDX11Renderer * CreateGraphicsInstance();

	// �������� �ε����ʿ��� �������� �����Ҷ� ����ϴ� �Լ�.
	// �ε��ϴ��ʿ��� delete���� �ʰ� dll���� ��û�ϴ� ������ �����ߴ�.
	// ������ �������� �´� Release ������ ���ǵǾ� ���� ���̴�.
	extern "C" __declspec(dllexport) void ReleaseGraphicsInstance(IDX11Renderer * instance);
}
