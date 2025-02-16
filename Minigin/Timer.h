#pragma once
#include <chrono>

namespace dae
{
	class Timer
	{
	public:
		static Timer& GetInstance()
		{
			static Timer instance;
			return instance;
		}

		void Start()
		{
			m_lastTime = std::chrono::high_resolution_clock::now();
		}

		void Update()
		{
			auto currentTime = std::chrono::high_resolution_clock::now();
			m_deltaTime = std::chrono::duration<float>(currentTime - m_lastTime).count();
			m_lastTime = currentTime;
		}

		float GetDeltaTime() const { return m_deltaTime; }

	private:
		Timer() : m_deltaTime(0.0f) {}
		~Timer() = default;

		Timer(const Timer&) = delete;
		Timer& operator=(const Timer&) = delete;

		std::chrono::high_resolution_clock::time_point m_lastTime;
		float m_deltaTime;
	};
}
