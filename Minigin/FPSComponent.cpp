#include "FPSComponent.h"
#include <iostream>
#include <iomanip>
#include <sstream>
#include <csignal> // For triggering breakpoints
#include "TextComponent.h"
#include "Timer.h"

dae::FPSComponent::FPSComponent(GameObject* pOwner)
	: Component(pOwner)
	, m_FPS(0.0f)
	, m_FrameCount(0)
	, m_TimeAccumulator(0.f)
	, m_LastTime(std::chrono::high_resolution_clock::now())
	, m_TextComponent(nullptr)
	, m_PreviousFPS(0.0f) // Initialize previous FPS
{
}

void dae::FPSComponent::Update()
{
	m_TimeAccumulator += Timer::GetInstance().GetDeltaTime();
	m_FrameCount++;

	if (m_TimeAccumulator >= 0.1f) // Update every 0.1 seconds
	{
		float newFPS = static_cast<float>(m_FrameCount) / m_TimeAccumulator;

		// Check for a significant FPS drop
		//if (m_PreviousFPS > 0.0f && (m_PreviousFPS - newFPS) >= 50.0f)
		//{
		//	std::cerr << "Significant FPS drop detected: Previous FPS = " << m_PreviousFPS
		//		<< ", Current FPS = " << newFPS << std::endl;
		//
		//	// Trigger a breakpoint
		//	TriggerBreakpoint();
		//}

		// Update FPS only if it changes significantly
		if (std::abs(newFPS - m_FPS) > 0.1f)
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

		// Update the previous FPS
		m_PreviousFPS = newFPS;

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

#include <csignal> // Ensure this is included for signal handling

void dae::FPSComponent::TriggerBreakpoint()
{
#ifdef _DEBUG
   #if defined(_MSC_VER) // Check if using Microsoft Visual Studio
       __debugbreak(); // Use MSVC-specific debug break
   #else
       std::raise(SIGTRAP); // Use standard SIGTRAP for other compilers
   #endif
#endif
}
