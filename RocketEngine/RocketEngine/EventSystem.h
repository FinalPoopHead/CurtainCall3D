#pragma once
#include "Singleton.h"

namespace Rocket::Core
{
	class EventSystem : public Singleton<EventSystem>
	{
		friend Singleton;
	private:
		EventSystem() = default;		// �̱����̱� ������ �ܺο��� ������ �� ������.
	};
}
