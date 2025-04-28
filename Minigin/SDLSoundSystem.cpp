#include "SDLSoundSystem.h"
#include "..\SDL2_mixer\Include\SDL_mixer.h"
#include <unordered_map>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <thread>
#include <iostream>
#include <stdexcept>

class dae::SDLSoundSystem::Impl
{
public:
	Impl()
	{
		if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
		{
			throw std::runtime_error("Failed to initialize SDL_mixer: " + std::string(Mix_GetError()));
		}

		m_WorkerThread = std::thread(&Impl::ProcessQueue, this);
	}

	~Impl()
	{
		{
			std::lock_guard<std::mutex> lock(m_QueueMutex);
			m_Running = false;
		}
		m_Condition.notify_all();

		if (m_WorkerThread.joinable())
			m_WorkerThread.join();

		CleanupSounds();
		Mix_CloseAudio();
	}

	void LoadSound(const std::string& soundFile)
	{
		std::lock_guard<std::mutex> lock(m_QueueMutex);

		if (m_SoundMap.find(soundFile) != m_SoundMap.end())
		{
			return; // Sound already loaded
		}

		Mix_Chunk* chunk = Mix_LoadWAV(soundFile.c_str());
		if (!chunk)
		{
			throw std::runtime_error("Failed to load sound: " + soundFile + ", Error: " + Mix_GetError());
		}
		m_SoundMap[soundFile] = chunk;
	}

	void Play(const std::string& soundFile, float volume)
	{
		if (volume < 0.0f || volume > 1.0f)
		{
			std::cerr << "Volume out of range (0.0 - 1.0): " << volume << std::endl;
			return;
		}

		std::lock_guard<std::mutex> lock(m_QueueMutex);

		auto it = m_SoundMap.find(soundFile);
		if (it == m_SoundMap.end())
		{
			throw std::runtime_error("Sound not loaded: " + soundFile);
		}

		m_SoundQueue.emplace(SoundJob{ it->second, static_cast<int>(volume * MIX_MAX_VOLUME) });
		m_Condition.notify_one();
	}

	void StopAll()
	{
		Mix_HaltChannel(-1);
	}

private:
	struct SoundJob
	{
		Mix_Chunk* chunk;
		int volume;
	};

	void ProcessQueue()
	{
		while (true)
		{
			SoundJob job;

			{
				std::unique_lock<std::mutex> lock(m_QueueMutex);
				m_Condition.wait(lock, [this]() { return !m_SoundQueue.empty() || !m_Running; });

				if (!m_Running && m_SoundQueue.empty())
					break;

				job = m_SoundQueue.front();
				m_SoundQueue.pop();
			}

			if (job.chunk)
			{
				Mix_VolumeChunk(job.chunk, job.volume);
				Mix_PlayChannel(-1, job.chunk, 0);
			}
		}
	}

	void CleanupSounds()
	{
		for (auto& [file, chunk] : m_SoundMap)
		{
			Mix_FreeChunk(chunk);
		}
		m_SoundMap.clear();
	}

	std::unordered_map<std::string, Mix_Chunk*> m_SoundMap;
	std::queue<SoundJob> m_SoundQueue;
	std::mutex m_QueueMutex;
	std::condition_variable m_Condition;
	std::thread m_WorkerThread;
	bool m_Running{ true };
};

dae::SDLSoundSystem::SDLSoundSystem() : m_Impl(std::make_unique<Impl>()) {}
dae::SDLSoundSystem::~SDLSoundSystem() = default;

void dae::SDLSoundSystem::LoadSound(const std::string& soundFile)
{
	std::cout << "Loading sound: " << soundFile << std::endl;
	m_Impl->LoadSound(soundFile);
	std::cout << "Sound loaded successfully: " << soundFile << std::endl;
}

void dae::SDLSoundSystem::Play(const std::string& soundFile, float volume)
{
	std::cout << "Playing sound: " << soundFile << " at volume: " << volume << std::endl;
	m_Impl->Play(soundFile, volume);
	std::cout << "Sound played successfully: " << soundFile << std::endl;
}
