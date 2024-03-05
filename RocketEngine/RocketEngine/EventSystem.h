#pragma once
#include "Singleton.h"

namespace Rocket::Core
{
	class EventSystem : public Singleton<EventSystem>
	{
		friend Singleton;
	private:
		EventSystem() = default;		// 싱글턴이기 때문에 외부에서 생성할 수 없도록.
	};
}
