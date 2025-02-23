#include <string>
#include "GameObject.h"
//#include "ResourceManager.h"
//#include "Renderer.h"
#include "Component.h"

dae::GameObject::GameObject(GameObject* pParent)
{
	SetParent(pParent);
}

dae::GameObject::~GameObject() = default;

void dae::GameObject::Update()
{
	for (const auto& component : m_Components)
	{
		component.second->Update();
	}

	for (const auto& component : m_Components)
	{
		if (component.second->IsDestroyed())
		{
			m_Components.erase(component.first);
		}
	}
}

void dae::GameObject::Render() const
{
	for (const auto& component : m_Components)
	{
		component.second->Render();
	}
}

#pragma region Position Functions
void dae::GameObject::SetWorldPosition(float x, float y, float z)
{
	m_WorldTransform.SetPosition(x, y, z);
}

void dae::GameObject::SetWorldPosition(const glm::vec3& position)
{
	m_WorldTransform.SetPosition(position);
}

void dae::GameObject::SetLocalPosition(float x, float y, float z)
{
	m_LocalTransform.SetPosition(x, y, z);
}

void dae::GameObject::SetLocalPosition(const glm::vec3& position)
{
	m_LocalTransform.SetPosition(position);
}
const glm::vec3& dae::GameObject::GetWorldPosition() const
{
	// TODO: insert return statement here
}
const glm::vec3& dae::GameObject::GetLocalPosition() const
{
	// TODO: insert return statement here
}
#pragma endregion

#pragma region ParentChild Functions
void dae::GameObject::SetParent(GameObject* pParent, bool worldPositionStays)
{
	// Check if the new parent is valid (not itself or one of its children)
	if (pParent == this || (pParent && pParent->IsChild(this)))
	{
		throw std::runtime_error("Invalid parent.");
	}

	// Update position, rotation, and scale
	if (pParent)
	{
		// Calculate the new local position
		const auto& parentWorldPos = pParent->GetWorldPosition();
		const auto& worldPos = GetWorldPosition();
		const auto localPos = worldPos - parentWorldPos;
		// Set the new local position
		SetLocalPosition(localPos);
	}
	else
	{
		// Set the new world position
		SetWorldPosition(GetLocalPosition());
	}

	// Remove itself from the previous parent
	if (m_pParent)
	{
		auto& siblings = m_pParent->m_Children;
		siblings.erase(std::remove_if(siblings.begin(), siblings.end(),
			[this](const std::unique_ptr<GameObject>& child) { return child.get() == this; }), siblings.end());
	}

	// Set the given parent on itself
	m_pParent = pParent;

	// Add itself as a child to the given parent
	if (pParent)
	{
		pParent->m_Children.push_back(std::unique_ptr<GameObject>(this));
	}
}

void dae::GameObject::AddChild(std::shared_ptr<GameObject> pChild)
{

}

void dae::GameObject::RemoveChild(std::shared_ptr<GameObject> pChild)
{
	//m_Children.erase(std::remove(m_Children.begin(), m_Children.end(), pChild), m_Children.end());
}

bool dae::GameObject::IsChild(const GameObject* pChild) const
{
	return std::find(m_Children.begin(), m_Children.end(), pChild) != m_Children.end();
}
#pragma endregion
