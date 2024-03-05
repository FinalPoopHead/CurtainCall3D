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
	/// 현재 포커스 된 객체 정보도 담고
	/// 
	/// 23.08.11 강석원 인재원.
	/// </summary>
	class UISystem : public Singleton<UISystem>
	{
		friend Singleton;
	private:
		UISystem();		// 싱글턴이기 때문에 외부에서 생성할 수 없도록.

	public:
		void Initialize();
		void Finalize();

	public:
		void CheckFocusCurrentScene();

	public:
		void AddUICurrentScene(UIRenderer* ui);
	};
}
