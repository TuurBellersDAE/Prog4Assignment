#include <string>
#include "GameObject.h"
#include "Component.h"
#include <iostream>
#include "Timer.h"

#include "Observer.h"

dae::GameObject::GameObject(GameObject* pParent)
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
	if (m_pParent)
	{
		auto& siblings = m_pParent->m_Children;
		siblings.erase(std::remove(siblings.begin(), siblings.end(), this), siblings.end());
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

	if (!m_pParent)
	{
		m_WorldTransform.SetPosition(m_LocalTransform.GetPosition());
	}
	else
	{
		m_WorldTransform.SetPosition(m_LocalTransform.GetPosition() + m_pParent->GetWorldPosition());
	}

	UpdateMovement();
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
	if (!m_pParent)
	{
		m_LocalTransform.SetPosition(x, y, z);
	}
	else
	{
		m_LocalTransform.SetPosition(x - m_pParent->GetWorldPosition().x, y - m_pParent->GetWorldPosition().y, z - m_pParent->GetWorldPosition().z);
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
		if (!m_pParent)
		{
			m_WorldTransform.SetPosition(m_LocalTransform.GetPosition());
		}
		else
		{
			m_WorldTransform.SetPosition(m_LocalTransform.GetPosition() + m_pParent->GetWorldPosition());
		}
		m_IsPositionDirty = false;
	}

	return m_WorldTransform.GetPosition();
}

const glm::vec3& dae::GameObject::GetLocalPosition() const
{
	return m_LocalTransform.GetPosition();
}
#pragma endregion

#pragma region ParentChild Functions
void dae::GameObject::SetParent(GameObject* pParent, bool keepWorldPosition)
{
	// Check if the new parent is valid (not itself or one of its children)
	if (pParent == this || (pParent && pParent->IsChild(this)))
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
	if (m_pParent)
	{
		auto& siblings = m_pParent->m_Children;
		siblings.erase(std::remove(siblings.begin(), siblings.end(), this), siblings.end());
	}

	// Set the given parent on itself
	m_pParent = pParent;

	// Add itself as a child to the given parent
	if (pParent)
	{
		pParent->m_Children.push_back(this);
	}
}

void dae::GameObject::AddChild(GameObject* pChild)
{
	if (pChild)
	{
		pChild->SetParent(this, false);
	}
}

void dae::GameObject::RemoveChild(GameObject* pChild)
{
	if (pChild)
	{
		pChild->SetParent(nullptr, false);
	}
}

bool dae::GameObject::IsChild(const GameObject* pChild) const
{
	return std::find(m_Children.begin(), m_Children.end(), pChild) != m_Children.end();
}
#pragma endregion

#pragma region Move Functions
void dae::GameObject::UpdateMovement()
{
	if (m_IsMoving)
	{
		auto deltaTime = Timer::GetInstance().GetDeltaTime();
		auto position = m_LocalTransform.GetPosition();
		position.x += m_CombinedDirection.x * deltaTime * m_Speed;
		position.y += m_CombinedDirection.y * deltaTime * m_Speed;
		position.z += m_CombinedDirection.z * deltaTime * m_Speed;
		SetWorldPosition(position);
	}
}

void dae::GameObject::MoveLeft()
{
}

void dae::GameObject::MoveRight()
{
}

void dae::GameObject::MoveUp()
{
}

void dae::GameObject::MoveDown()
{
}

void dae::GameObject::SetSpeed(float speed)
{
	m_Speed = speed;
}

void dae::GameObject::SetDirection(const glm::vec3& direction)
{
	m_Direction = direction;
	m_IsMoving = (direction != glm::vec3(0.f, 0.f, 0.f));
	if (m_IsMoving)
	{
		m_CombinedDirection = m_Direction; //+= for diagonal but still buggy
		m_CombinedDirection = glm::normalize(m_CombinedDirection);
	}
	else
	{
		m_CombinedDirection = glm::vec3(0.f, 0.f, 0.f);
	}
}

void dae::GameObject::SetIsMoving(bool isMoving)
{
	m_IsMoving = isMoving;
}

glm::vec3 dae::GameObject::GetDirection() const
{
	return m_Direction;
}

float dae::GameObject::GetSpeed() const
{
	return m_Speed;
}

bool dae::GameObject::IsMoving() const
{
	return m_IsMoving;
}
#pragma endregion

#pragma region Observer Functions
dae::Observer* dae::GameObject::AddObserver(std::unique_ptr<Observer> pObserver)
{
	m_pObservers.push_back(std::move(pObserver));
	return m_pObservers.back().get();
}

void dae::GameObject::RemoveObserver(Observer* pObserver)
{
	m_pObservers.erase(std::remove_if(m_pObservers.begin(), m_pObservers.end(), [pObserver](const std::unique_ptr<Observer>& o) { return o.get() == pObserver; }), m_pObservers.end());
}
#pragma endregion
