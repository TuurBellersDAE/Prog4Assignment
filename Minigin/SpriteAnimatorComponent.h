#pragma once
#include "Component.h"
#include <string>
#include <SDL.h>
#include <memory>

namespace dae
{
	class Texture2D;

	class SpriteAnimatorComponent : public Component
	{
	public:
		SpriteAnimatorComponent(GameObject* owner, const std::string& path, int frameWidth, int frameHeight, int frameCount, float frameTime);
		~SpriteAnimatorComponent() override = default;

		void Update() override;
		void Render() const override;

		void Texture(const std::string& path);

		void SetCurrentRow(int row) { m_CurrentRow = row; }

	private:
		std::shared_ptr<Texture2D> m_Texture;
		int m_FrameWidth;
		int m_FrameHeight;
		int m_FrameCount;
		float m_FrameTime;
		float m_ElapsedTime;
		int m_CurrentFrame;

		int m_CurrentRow = 0; // For multi-row sprites
	};
}

