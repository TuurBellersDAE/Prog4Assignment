#include "RenderComponent.h"
#include "ResourceManager.h"
#include "Renderer.h"

dae::RenderComponent::RenderComponent(GameObject* pOwner, const std::string& filename, glm::vec3 position)
	: Component(pOwner), m_NeedsUpdate(true), m_Texture(nullptr)//, m_Position(position)
{
	GetOwner()->SetWorldPosition(position);
	SetTexture(filename);
}

void dae::RenderComponent::Render() const
{
	const glm::vec3 position = GetOwner()->GetWorldPosition();
	if (m_Texture != nullptr)
	{
		Renderer::GetInstance().RenderTexture(*m_Texture, position.x, position.y);
	}
}

void dae::RenderComponent::SetTexture(const std::string& filename)
{
	m_Texture = ResourceManager::GetInstance().LoadTexture(filename);
}

void dae::RenderComponent::SetPosition(glm::vec3 position)
{
	GetOwner()->SetWorldPosition(position);
	m_NeedsUpdate = true;
}
