#include "SpriteAnimatorComponent.h"
#include "ResourceManager.h"
#include "Renderer.h"
#include "Timer.h"

namespace dae
{
	SpriteAnimatorComponent::SpriteAnimatorComponent(GameObject* owner, const std::string& path, int frameWidth, int frameHeight, int frameCount, float frameTime)
		: Component(owner)
		, m_Texture(ResourceManager::GetInstance().LoadTexture(path))
		, m_FrameWidth(frameWidth)
		, m_FrameHeight(frameHeight)
		, m_FrameCount(frameCount)
		, m_FrameTime(frameTime)
		, m_ElapsedTime(0.0f)
		, m_CurrentFrame(0)
	{
	}

	void SpriteAnimatorComponent::Update()
	{
		// Update the elapsed time
		m_ElapsedTime += Timer::GetInstance().GetDeltaTime();

		// Check if it's time to switch to the next frame
		if (m_ElapsedTime >= m_FrameTime)
		{
			m_ElapsedTime -= m_FrameTime; // Reset elapsed time
			m_CurrentFrame = (m_CurrentFrame + 1) % m_FrameCount; // Cycle through frames
		}
	}

	void SpriteAnimatorComponent::Render() const
	{
		if (m_Texture != nullptr)
		{
			// Calculate the source rectangle for the current frame
			SDL_Rect srcRect{
				m_CurrentFrame * m_FrameWidth, // Move to the next frame horizontally
				0,                            // Assuming all frames are in a single row
				m_FrameWidth,
				m_FrameHeight
			};

			// Calculate the destination rectangle based on the GameObject's position
			const glm::vec3 position = GetOwner()->GetWorldPosition();
			SDL_Rect dstRect{
				static_cast<int>(position.x),
				static_cast<int>(position.y),
				m_FrameWidth, // Render at the original frame size
				m_FrameHeight
			};

			// Render the current frame
			Renderer::GetInstance().RenderTexture(*m_Texture, dstRect, srcRect);
		}
	}

	void SpriteAnimatorComponent::Texture(const std::string& path)
	{
		m_Texture = ResourceManager::GetInstance().LoadTexture(path);
	}
}

