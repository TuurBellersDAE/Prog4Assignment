#pragma once
#include <memory>
#include <string>
#include "Component.h"
#include <glm.hpp>
#include <SDL_ttf.h>

namespace dae
{
	class Font;
	class Texture2D;

	class TextComponent : public Component
	{
	public:
		TextComponent(GameObject* pOwner, const std::string& text, std::shared_ptr<Font> font, glm::vec3 position);
		//TextComponent(const std::string& text, std::shared_ptr<Font> font, glm::vec3 position);
		~TextComponent() = default;
		TextComponent(const TextComponent& other) = delete;
		TextComponent(TextComponent&& other) = delete;
		TextComponent& operator=(const TextComponent& other) = delete;
		TextComponent& operator=(TextComponent&& other) = delete;

		void Update() override;
		void Render() const override;

		void SetText(const std::string& text);
		void SetPosition(glm::vec3 position);
		void SetShow(bool show) { m_IsRendered = show; }

		int GetTextWidth() const;
		int GetTextHeight() const;
		const std::string& GetText() const { return m_Text; }

		void SetColor(const SDL_Color& color);

	private:
		bool m_NeedsUpdate;
		bool m_IsRendered;

		std::string m_Text;
		std::shared_ptr<Font> m_Font;
		std::shared_ptr<Texture2D> m_TextTexture;
		SDL_Color m_Color{ 255, 255, 255, 255 }; // Default to white
	};
}

