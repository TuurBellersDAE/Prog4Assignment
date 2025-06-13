#pragma once
#include "Singleton.h"
#include "SoundSystem.h"
#include <memory>

namespace dae
{
	class NullSoundSystem final : public SoundSystem
	{
	public:
		void Play(const std::string& /*soundFile*/, const float /*volume*/, bool /*loop*/) override {}
		void StopLoop(const std::string& /*soundFile*/) override {}
		void LoadSound(const std::string& /*soundFile*/) override {}
		void StopAllSounds() override {}
	};

	class ServiceLocator final : public Singleton<ServiceLocator>
	{
	public:
		SoundSystem& GetSoundSystem()
		{
			return *m_SoundSystemInstance;
		}

		void RegisterSoundSystem(std::unique_ptr<SoundSystem> soundSystem)
		{
			m_SoundSystemInstance = std::move(soundSystem);
		}

	private:
		std::unique_ptr<SoundSystem> m_SoundSystemInstance = std::make_unique<NullSoundSystem>();
	};
}
