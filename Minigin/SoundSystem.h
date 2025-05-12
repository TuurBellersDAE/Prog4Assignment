#pragma once
#include <string>

namespace dae
{
	class SoundSystem
	{
	public:
		SoundSystem() = default;
		virtual ~SoundSystem() = default;

		virtual void Play(const std::string& soundFile, const float volume) = 0;
		virtual void LoadSound(const std::string& soundFile) = 0;
		virtual void StopAllSounds() = 0;
	};
}
