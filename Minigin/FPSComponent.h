#pragma once
#include <string>
#include <chrono>
#include <deque>
#include "Component.h"

namespace dae
{
	class GameObject;
	class TextComponent;

	class FPSComponent : public Component
	{
	public:
		FPSComponent(GameObject* pOwner);
		~FPSComponent() = default;
		FPSComponent(const FPSComponent& other) = delete;
		FPSComponent(FPSComponent&& other) = delete;
		FPSComponent& operator=(const FPSComponent& other) = delete;
		FPSComponent& operator=(FPSComponent&& other) = delete;

		void Update() override;
		void Render() const {};

		std::string GetStringFPS() const;
		float GetFPS() const { return m_FPS; }

	private:
		void TriggerBreakpoint(); // Method to trigger a breakpoint

		float m_FPS;
		float m_PreviousFPS; // Store the previous FPS
		float m_FrameCount;
		float m_TimeAccumulator;
		std::chrono::time_point<std::chrono::steady_clock> m_LastTime;
		TextComponent* m_TextComponent;

		std::deque<float> m_FPSBuffer;
		static constexpr size_t m_BufferSize = 100;
	};
}
