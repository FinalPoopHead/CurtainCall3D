#pragma once
#include <string>
#include "../FloaterUtil/include/FloaterType.h"


namespace FMOD
{
	class Sound;
	class Channel;
}

namespace flt
{
	class SoundEngine;
	class SoundComponent;
	class FMODInstance;

	class Sound
	{
		friend class SoundEngine;
		friend class SoundComponent;
		friend class FMODInstance;

	public:
		enum Category
		{
			NONE,
			BGM,
			Effect,
			Voice,
			MASTER,
			COUNT
		};

		inline static std::string s_categoryName[Category::COUNT] =
		{ 
			"BGM", 
			"Effect", 
			"Voice",
			"MASTER"
		};

		Sound();
		~Sound();

		void SetLoop(bool isLoop);
		bool IsLoop() const;
		void SetCategory(Category category);
		Category GetCategory() const;

	private:
		std::wstring _path;
		void* _buffer;
		FMOD::Sound* _fSound;
		FMOD::Channel* _channel;
		Category _category;
		bool _isLoop;
	};
}
