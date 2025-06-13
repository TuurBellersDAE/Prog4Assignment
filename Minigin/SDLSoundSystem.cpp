#include "SDLSoundSystem.h"
#include <unordered_map>
#include <unordered_set>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <thread>
#include <iostream>
#include <variant>
#include <chrono>

#include "..\SDL2_mixer\Include\SDL_mixer.h"

class dae::SDLSoundSystem::Impl
{
public:
	Impl() {
		if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096) < 0) {
			throw std::runtime_error("Failed to initialize SDL_mixer: " + std::string(Mix_GetError()));
		}
		m_WorkerThread = std::thread(&Impl::ProcessQueue, this);
	}

	~Impl() {
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

	void LoadSound(const std::string& soundFile) {
		std::lock_guard<std::mutex> lock(m_QueueMutex);
		if (m_SoundMap.contains(soundFile) || m_LoadingSet.contains(soundFile)) return;
		m_LoadingSet.insert(soundFile);
		m_JobQueue.emplace(LoadJob{ soundFile });
		m_Condition.notify_one();
	}

	void Play(const std::string& soundFile, float volume, bool loop) {
		if (volume < 0.0f || volume > 1.0f) {
			std::cerr << "Volume out of range (0.0 - 1.0): " << volume << std::endl;
			return;
		}

		using namespace std::chrono;
		auto now = steady_clock::now();
		{
			std::lock_guard<std::mutex> lock(m_QueueMutex);
			auto it = m_LastPlayTime.find(soundFile);
			if (it != m_LastPlayTime.end()) {
				auto elapsed = duration_cast<milliseconds>(now - it->second);
				if (elapsed.count() < m_MinDelayMs) return;
			}
			m_LastPlayTime[soundFile] = now;

			if (!m_SoundMap.contains(soundFile)) {
				if (!m_LoadingSet.contains(soundFile)) {
					m_LoadingSet.insert(soundFile);
					m_JobQueue.emplace(LoadJob{ soundFile });
					m_Condition.notify_one();
				}
				return; // skip playing until it's loaded
			}

			m_JobQueue.emplace(PlayJob{ soundFile, static_cast<int>(volume * MIX_MAX_VOLUME), loop });
			m_Condition.notify_one();
		}
	}

	void StopLoop(const std::string& soundFile)
	{
		auto it = m_LoopingChannels.find(soundFile);
		if (it != m_LoopingChannels.end()) {
			// Fade out over 500ms (adjust as needed)
			const int fadeOutMs = 100;
			Mix_FadeOutChannel(it->second, fadeOutMs);
			m_LoopingChannels.erase(it);
		}
	}



	void StopAll() {
		Mix_HaltChannel(-1);
	}

private:
	struct LoadJob 
	{
		std::string file;
	};

	struct PlayJob 
	{
		std::string file;
		int volume;
		bool loop;
	};

	using Job = std::variant<LoadJob, PlayJob>;

	void ProcessQueue() {
		while (true) {
			Job job;

			{
				std::unique_lock<std::mutex> lock(m_QueueMutex);
				m_Condition.wait(lock, [this]() { return !m_JobQueue.empty() || !m_Running; });

				if (!m_Running && m_JobQueue.empty())
					break;

				job = std::move(m_JobQueue.front());
				m_JobQueue.pop();
			}

			std::visit([this](auto&& j) { HandleJob(j); }, job);
		}
	}

	void HandleJob(const PlayJob& job) 
	{
		auto it = m_SoundMap.find(job.file);
		if (it == m_SoundMap.end()) return;
		Mix_VolumeChunk(it->second, job.volume);

		if (job.loop) {
			// Check if already looping
			auto chanIt = m_LoopingChannels.find(job.file);
			if (chanIt != m_LoopingChannels.end() && Mix_Playing(chanIt->second)) {
				// Already looping, do nothing
				return;
			}
			// Start new loop
			int channel = Mix_PlayChannel(-1, it->second, -1);
			if (channel != -1) {
				m_LoopingChannels[job.file] = channel;
			}
		}
		else {
			Mix_PlayChannel(-1, it->second, 0);
		}
	}


	void HandleJob(const LoadJob& job) {
		auto it = m_SoundMap.find(job.file);
		if (it != m_SoundMap.end()) return;
		Mix_Chunk* chunk = Mix_LoadWAV(job.file.c_str());
		if (!chunk) 
		{
			std::cerr << "Failed to load sound: " << job.file << ", Error: " << Mix_GetError() << std::endl;
			m_LoadingSet.erase(job.file);
			return;
		}
		m_SoundMap[job.file] = chunk;
		m_LoadingSet.erase(job.file);
	}


	void CleanupSounds() {
		for (auto& [_, chunk] : m_SoundMap) {
			Mix_FreeChunk(chunk);
		}
		m_SoundMap.clear();
		m_LastPlayTime.clear();
		m_LoadingSet.clear();
	}

	std::unordered_map<std::string, Mix_Chunk*> m_SoundMap;
	std::unordered_map<std::string, std::chrono::steady_clock::time_point> m_LastPlayTime;
	std::unordered_set<std::string> m_LoadingSet;
	std::queue<Job> m_JobQueue;
	std::mutex m_QueueMutex;
	std::condition_variable m_Condition;
	std::thread m_WorkerThread;
	bool m_Running{ true };
	const int m_MinDelayMs = 100; // throttle interval in ms
	std::unordered_map<std::string, int> m_LoopingChannels;
};

// Public interface impl

dae::SDLSoundSystem::SDLSoundSystem() : m_Impl(std::make_unique<Impl>()) {}
dae::SDLSoundSystem::~SDLSoundSystem() = default;

void dae::SDLSoundSystem::LoadSound(const std::string& soundFile) 
{
	m_Impl->LoadSound(soundFile);
}

void dae::SDLSoundSystem::Play(const std::string& soundFile, float volume, bool loop = false) 
{
	m_Impl->Play(soundFile, volume, loop);
}

void dae::SDLSoundSystem::StopLoop(const std::string& soundFile)
{
	m_Impl->StopLoop(soundFile);
}

void dae::SDLSoundSystem::StopAllSounds() {
	m_Impl->StopAll();
}
