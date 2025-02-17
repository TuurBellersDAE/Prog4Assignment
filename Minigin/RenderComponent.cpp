#include "RenderComponent.h"
#include "ResourceManager.h"
#include "Renderer.h"

dae::RenderComponent::RenderComponent(const std::string& filename, glm::vec3 position)
	: m_needsUpdate(true), m_Texture(nullptr), m_Position(position)
{
	SetTexture(filename);
}

void dae::RenderComponent::Render() const
{
	if (m_Texture != nullptr)
	{
		Renderer::GetInstance().RenderTexture(*m_Texture, m_Position.x, m_Position.y);
	}
}

void dae::RenderComponent::SetTexture(const std::string& filename)
{
	m_Texture = ResourceManager::GetInstance().LoadTexture(filename);
}
