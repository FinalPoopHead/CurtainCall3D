#pragma once
#include <string>

namespace FMOD
{
	class Sound;
	class Channel;
}

namespace flt
{
	class SoundEngine;
	class SoundComponent;

	class Sound
	{
		friend class SoundEngine;
		friend class SoundComponent;

	public:
		enum Category
		{
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
