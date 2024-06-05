#pragma once
#include "Component.h"
#include <string>


namespace flt
{
	class SoundEngine;

	class SoundComponent : public Component<SoundComponent>
	{
	public:
		SoundComponent();

		void AddSound(std::wstring path);


	private:
		SoundEngine* _soundEngine;
	};

}

