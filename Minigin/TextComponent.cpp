#include "TextComponent.h"
#include <stdexcept>
#include <SDL_ttf.h>
#include "Renderer.h"
#include "Font.h"
#include "Texture2D.h"

//dae::TextComponent::TextComponent(const std::string& text, std::shared_ptr<Font> font, glm::vec3 position)
//	: m_NeedsUpdate(true), m_Text(text), m_Font(std::move(font)), m_Position(position), m_TextTexture(nullptr)
//{
//}

dae::TextComponent::TextComponent(GameObject* pOwner, const std::string& text, std::shared_ptr<Font> font, glm::vec3 position)
	: Component(pOwner), m_NeedsUpdate(true), m_IsRendered(true), m_Text(text), m_Font(std::move(font)), m_Position(position), m_TextTexture(nullptr)
{
}

void dae::TextComponent::Update()
{
	if (m_NeedsUpdate)
	{
		const SDL_Color color = { 255,255,255,255 }; // only white text is supported now
		const auto surf = TTF_RenderText_Blended(m_Font->GetFont(), m_Text.c_str(), color);
		if (surf == nullptr)
		{
			throw std::runtime_error(std::string("Render text failed: ") + SDL_GetError());
		}
		auto texture = SDL_CreateTextureFromSurface(Renderer::GetInstance().GetSDLRenderer(), surf);
		if (texture == nullptr)
		{
			throw std::runtime_error(std::string("Create text texture from surface failed: ") + SDL_GetError());
		}
		SDL_FreeSurface(surf);
		m_TextTexture = std::make_shared<Texture2D>(texture);
		m_NeedsUpdate = false;
	}
}

void dae::TextComponent::Render() const
{
	if (m_TextTexture != nullptr && m_IsRendered)
	{
		Renderer::GetInstance().RenderTexture(*m_TextTexture, m_Position.x, m_Position.y);
	}
}

void dae::TextComponent::SetText(const std::string& text)
{
	m_Text = text;
	m_NeedsUpdate = true;
}

void dae::TextComponent::SetPosition(glm::vec3 position)
{
	m_Position = position;
	m_NeedsUpdate = true;
}
