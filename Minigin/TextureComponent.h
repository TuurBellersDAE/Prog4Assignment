#pragma once
#include "Component.h"
#include <memory>
#include "Transform.h"
#include "Texture2D.h"
#include "SDL_rect.h"

namespace dae 
{
	class TextureComponent final : public Component
	{
	public:
		TextureComponent(GameObject* Owner);
		TextureComponent(GameObject* Owner, const std::string& Path);
		TextureComponent(GameObject* Owner, const std::string& Path, SDL_Rect dstRect= { 0, 0, 0, 0 }, SDL_Rect srcRect = { 0, 0, 0, 0 });
		virtual ~TextureComponent() = default;

		TextureComponent(const TextureComponent&) = delete;
		TextureComponent(TextureComponent&&) noexcept = delete;
		TextureComponent& operator=(const TextureComponent&) = delete;
		TextureComponent& operator=(TextureComponent&&) noexcept = delete;

		void Render() const override;

		void SetTexture(const std::string& filename);

		void SetRenderParams(const SDL_Rect& dstRect, const SDL_Rect& srcRect) { m_DstRect = dstRect; m_SrsRect = srcRect; }

		const glm::ivec2 GetTextureResolution() const { return m_Texture->GetSize(); }

	private:
		std::shared_ptr<Texture2D> m_Texture{};

		SDL_Rect m_DstRect{};
		SDL_Rect m_SrsRect{};


	};
}


