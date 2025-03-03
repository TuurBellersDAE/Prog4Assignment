#include "FPSComponent.h"
#include <iostream>
#include <iomanip>
#include <sstream>
#include "TextComponent.h"
#include "Timer.h"

dae::FPSComponent::FPSComponent(GameObject* pOwner)
	: Component(pOwner)
	, m_FPS(0.0f)
	, m_FrameCount(0)
	, m_TimeAccumulator(0.f)
	, m_LastTime(std::chrono::high_resolution_clock::now())
	, m_TextComponent(nullptr)
{
}

void dae::FPSComponent::Update()
{
	//auto currentTime = std::chrono::high_resolution_clock::now();
	//std::chrono::duration<float> deltaTime = currentTime - m_LastTime;
	//m_LastTime = currentTime;

	m_TimeAccumulator += Timer::GetInstance().GetDeltaTime();
	m_FrameCount++;

	if (m_TimeAccumulator >= 0.1f) // Update every 0.1 seconds
	{
		float newFPS = static_cast<float>(m_FrameCount) / m_TimeAccumulator;
		if (std::abs(newFPS - m_FPS) > 0.1f) // Update only if FPS changes significantly
		{
			m_FPS = newFPS;

			if (!m_TextComponent)
			{
				m_TextComponent = GetOwner()->GetComponentPtr<TextComponent>();
			}

			if (m_TextComponent)
			{
				m_TextComponent->SetText("FPS: " + GetStringFPS());
			}
		}

		m_FrameCount = 0;
		m_TimeAccumulator = 0.f;
	}

	std::cout << "FPS: " << m_FPS << std::endl;
}

std::string dae::FPSComponent::GetStringFPS() const
{
	std::ostringstream stream;
	stream << std::fixed << std::setprecision(1) << m_FPS;
	return stream.str();
}
