#include <string>
#include "GameObject.h"
//#include "ResourceManager.h"
//#include "Renderer.h"
#include "Component.h"
#include <iostream>

dae::GameObject::GameObject(std::shared_ptr<GameObject> pParent)
{
	SetParent(pParent);
}

dae::GameObject::~GameObject()
{
	// Detach all children
	for (auto& child : m_Children)
	{
		child->SetParent(nullptr, false);
	}

	// If this object has a parent, remove it from the parent's children
	if (auto parent = m_pParent.lock())
	{
		auto& siblings = parent->m_Children;
		siblings.erase(std::remove(siblings.begin(), siblings.end(), shared_from_this()), siblings.end());
	}
}

void dae::GameObject::Update()
{
	// Update all components
	for (const auto& component : m_Components)
	{
		component.second->Update();
	}

	// Remove destroyed components
	for (auto it = m_Components.begin(); it != m_Components.end();)
	{
		if (it->second->IsDestroyed())
		{
			it = m_Components.erase(it);
		}
		else
		{
			++it;
		}
	}

	if (m_pParent.expired())
	{
		m_WorldTransform.SetPosition(m_LocalTransform.GetPosition());
	}
	else
	{
		auto parent = m_pParent.lock();
		m_WorldTransform.SetPosition(m_LocalTransform.GetPosition() + parent->GetWorldPosition());
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
	if (m_pParent.expired())
	{
		m_LocalTransform.SetPosition(x, y, z);
	}
	else
	{
		auto parent = m_pParent.lock();
		m_LocalTransform.SetPosition(x - parent->GetWorldPosition().x, y - parent->GetWorldPosition().y, z - parent->GetWorldPosition().z);
	}
}

void dae::GameObject::SetWorldPosition(const glm::vec3& position)
{
	SetWorldPosition(position.x, position.y, position.z);
}

void dae::GameObject::SetLocalPosition(float x, float y, float z)
{
	m_LocalTransform.SetPosition(x, y, z);
	SetPositionDirty(true);
}

void dae::GameObject::SetLocalPosition(const glm::vec3& position)
{
	SetLocalPosition(position.x, position.y, position.z);
}

const glm::vec3& dae::GameObject::GetWorldPosition()
{
	if (m_IsPositionDirty)
	{
		if (m_pParent.expired())
		{
			m_WorldTransform.SetPosition(m_LocalTransform.GetPosition());
		}
		else
		{
			auto parent = m_pParent.lock();
			m_WorldTransform.SetPosition(m_LocalTransform.GetPosition() + parent->GetWorldPosition());
		}
		m_IsPositionDirty = false;
	}

	return m_WorldTransform.GetPosition();
}

const glm::vec3& dae::GameObject::GetLocalPosition() const
{
	return m_LocalTransform.GetPosition();
}
void dae::GameObject::Move(float dx, float dy, float dz)
{
	m_LocalTransform.Move(dx, dy, dz);
}
#pragma endregion

#pragma region ParentChild Functions
void dae::GameObject::SetParent(std::shared_ptr<GameObject> pParent, bool keepWorldPosition)
{
	// Check if the new parent is valid (not itself or one of its children)
	if (pParent.get() == this || (pParent && pParent->IsChild(this)))
	{
		throw std::runtime_error("Invalid parent.");
	}

	// Update position, rotation, and scale
	if (!pParent)
	{
		SetLocalPosition(GetWorldPosition());
	}
	else
	{
		if (keepWorldPosition)
		{
			SetLocalPosition(GetWorldPosition() - pParent->GetWorldPosition());
		}
	}

	// Remove itself from the previous parent
	if (auto parent = m_pParent.lock())
	{
		auto& siblings = parent->m_Children;
		siblings.erase(std::remove(siblings.begin(), siblings.end(), shared_from_this()), siblings.end());
	}

	// Set the given parent on itself
	m_pParent = pParent;

	// Add itself as a child to the given parent
	if (pParent)
	{
		pParent->m_Children.push_back(shared_from_this());
	}
}

void dae::GameObject::AddChild(std::shared_ptr<GameObject> pChild)
{
	if (pChild)
	{
		pChild->SetParent(shared_from_this(), false);
	}
}

void dae::GameObject::RemoveChild(std::shared_ptr<GameObject> pChild)
{
	if (pChild)
	{
		pChild->SetParent(nullptr, false);
	}
}

bool dae::GameObject::IsChild(const GameObject* pChild) const
{
	return std::find_if(m_Children.begin(), m_Children.end(),
		[pChild](const std::shared_ptr<GameObject>& child) { return child.get() == pChild; }) != m_Children.end();
}
#pragma endregion
