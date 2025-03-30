#include "HealthComponent.h"
#include "GameObject.h"
#include "TextComponent.h"

dae::HealthComponent::HealthComponent(GameObject* pOwner)
	: Component(pOwner)
	, m_Health(100)
	, m_TextComponent(nullptr)
{
}

void dae::HealthComponent::BindTextComponent(TextComponent* textComponent)
{
	m_TextComponent = textComponent;
	if (m_TextComponent)
	{
		std::string healthText = "Health: " + std::to_string(m_Health);
		m_TextComponent->SetText(healthText);
	}
}

dae::TextComponent* dae::HealthComponent::GetTextComponent() const
{
	return m_TextComponent;
}

int dae::HealthComponent::GetHealth() const
{
	return m_Health;
}

int dae::HealthComponent::GetMaxHealth() const
{
	return m_MaxHealth;
}

void dae::HealthComponent::SetHealth(int health)
{
	m_Health = health;
	if (m_Health > m_MaxHealth)
	{
		m_Health = m_MaxHealth;
	}
	else if (m_Health < 0)
	{
		m_Health = 0;
		m_Subject.Notify(*GetOwner(), Event::PLAYER_DIED);
	}
	m_Subject.Notify(*GetOwner(), Event::PLAYER_HIT);
}

void dae::HealthComponent::TakeDamage(int damage)
{
	m_Health -= damage;
	if (m_Health < 0)
	{
		m_Health = 0;
		m_Subject.Notify(*GetOwner(), Event::PLAYER_DIED);
	}
	m_Subject.Notify(*GetOwner(), Event::PLAYER_HIT);
}

void dae::HealthComponent::Heal(int healAmount)
{
	m_Health += healAmount;
	if (m_Health > m_MaxHealth)
	{
		m_Health = m_MaxHealth;
	}
	m_Subject.Notify(*GetOwner(), Event::PLAYER_HIT);
}

