#pragma once
#pragma once
#include "Singleton.h"
#include <vector>

namespace Rocket::Core
{
	class UIRenderer;
}

namespace Rocket::Core
{
	/// <summary>
	/// ���� ��Ŀ�� �� ��ü ������ ���
	/// 
	/// 23.08.11 ������ �����.
	/// </summary>
	class UISystem : public Singleton<UISystem>
	{
		friend Singleton;
	private:
		UISystem();		// �̱����̱� ������ �ܺο��� ������ �� ������.

	public:
		void Initialize();
		void Finalize();

	public:
		void CheckFocusCurrentScene();

	public:
		void AddUICurrentScene(UIRenderer* ui);
	};
}
