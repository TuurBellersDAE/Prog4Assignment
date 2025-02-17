#include <string>
#include "GameObject.h"
//#include "ResourceManager.h"
//#include "Renderer.h"
#include "Component.h"

dae::GameObject::~GameObject() = default;

void dae::GameObject::Update()
{
	for (const auto& component : m_Components)
	{
		component.second->Update();
	}
}

void dae::GameObject::Render() const
{
	//const auto& pos = m_transform.GetPosition();

	for (const auto& component : m_Components)
	{
		component.second->Render();
	}

	//Renderer::GetInstance().RenderTexture(*m_texture, pos.x, pos.y);
}

//void dae::GameObject::SetTexture(const std::string& filename)
//{
//	m_texture = ResourceManager::GetInstance().LoadTexture(filename);
//}

void dae::GameObject::SetPosition(float x, float y)
{
	m_transform.SetPosition(x, y, 0.0f);
}
