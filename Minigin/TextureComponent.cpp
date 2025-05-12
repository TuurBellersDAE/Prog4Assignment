#include "TextureComponent.h"
#include "ResourceManager.h"
#include "Renderer.h"

dae::TextureComponent::TextureComponent(GameObject* Owner) : Component(Owner)
{
}

dae::TextureComponent::TextureComponent(GameObject* Owner, const std::string& Path) : Component(Owner)
{
	SetTexture(Path);
}

dae::TextureComponent::TextureComponent(GameObject* Owner, const std::string& Path, SDL_Rect dstRect, SDL_Rect srcRect) : Component(Owner)
{
	SetTexture(Path);
	SetRenderParams(dstRect, srcRect);
}

void dae::TextureComponent::Render() const
{
	if (m_Texture != nullptr)
	{
	const glm::vec3 position = GetOwner()->GetWorldPosition();
	auto dstRect = m_DstRect;
	dstRect.x = static_cast<int>(position.x);
	dstRect.y = static_cast<int>(position.y);

		if (m_DstRect.w == 0 && m_SrsRect.w == 0)
		{
			Renderer::GetInstance().RenderTexture(*m_Texture, position.x, position.y);
		}
		else
		{
			Renderer::GetInstance().RenderTexture(*m_Texture, dstRect, m_SrsRect);

		}
	}
}

void dae::TextureComponent::SetTexture(const std::string& filename)
{
	m_Texture = ResourceManager::GetInstance().LoadTexture(filename);
}


