#pragma once
#include "SoundSystem.h"
#include <memory>

namespace dae
{
	class SDLSoundSystem final : public SoundSystem
	{
	public:
		SDLSoundSystem();
		~SDLSoundSystem() override;

		void Play(const std::string& soundFile, const float volume) override;
		void LoadSound(const std::string& soundFile) override;
		void StopAllSounds() override;

	private:
		class Impl;
		std::unique_ptr<Impl> m_Impl;
	};
}

