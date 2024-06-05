#pragma once

namespace FMOD
{
	class System;
	class Sound;
	class ChannelControl;
	class Channel;
	class ChannelGroup;
	class SoundGroup;
	class DSP;
	class DSPConnection;
	class Geometry;
	class Reverb3D;
}

namespace flt
{
	class SoundEngine
	{
	public:
		SoundEngine();
		void Initialize();
		void Finalize();

	private:
		FMOD::System* _system;
		FMOD::Sound* _sound;
		FMOD::Channel* _channel;

		const float DISTANCEFACTOR = 1.0f;
		const int   INTERFACE_UPDATETIME = 50; // ms
	};
}


