#pragma once
#include "Component.h"
#include <vector>
#include <string>


namespace flt
{
	class SoundEngine;
	class Sound;

	class SoundComponent : public Component<SoundComponent>
	{
	public:
		SoundComponent();

		int AddSound(std::wstring path);
		void Play(int index, bool isLoop = false);
		bool isPlay(int index);
		void Pause(int index);
		void Stop(int index);


	private:
		SoundEngine* _soundEngine;
		std::vector<Sound*> _sounds;
	};

}

