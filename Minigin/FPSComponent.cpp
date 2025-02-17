#include "FPSComponent.h"
#include <iostream>
#include "TextComponent.h"
#include "Timer.h"

dae::FPSComponent::FPSComponent()
	: m_FPS(0)
	, m_FrameCount(0)
	, m_LastTime(std::chrono::high_resolution_clock::now())
	, m_TextComponent(nullptr)
{
}

void dae::FPSComponent::Update()
{
	auto& timer = Timer::GetInstance();
	float deltaTime = timer.GetDeltaTime();
	m_FrameCount++;

	if (deltaTime > 0.0f)
	{
		float currentFPS = 1.0f / deltaTime;
		m_FPSBuffer.push_back(currentFPS);

		if (m_FPSBuffer.size() > m_BufferSize)
		{
			m_FPSBuffer.pop_front();
		}

		float sumFPS = 0.0f;
		for (float fps : m_FPSBuffer)
		{
			sumFPS += fps;
		}
		m_FPS = sumFPS / m_FPSBuffer.size();

		if (m_TextComponent)
		{
			m_TextComponent->SetText(GetStringFPS());
		}
	}

	std::cout << "FPS: " << m_FPS << std::endl;
}

std::string dae::FPSComponent::GetStringFPS() const
{
	std::ostringstream stream;
	stream.precision(1);
	stream << std::fixed << m_FPS;
	return stream.str();
}
