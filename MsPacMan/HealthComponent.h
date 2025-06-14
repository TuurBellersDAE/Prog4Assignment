#pragma once
#include "Component.h"
#include "Observer.h"

namespace dae
{
	class GameObject;
	class TextComponent;

	class HealthComponent : public Component
	{
	public:
		HealthComponent(GameObject* pOwner);
		~HealthComponent() = default;
		HealthComponent(const HealthComponent& other) = delete;
		HealthComponent(HealthComponent&& other) = delete;
		HealthComponent& operator=(const HealthComponent& other) = delete;
		HealthComponent& operator=(HealthComponent&& other) = delete;

		void BindTextComponent(TextComponent* textComponent);
		TextComponent* GetTextComponent() const;

		int GetHealth() const;
		int GetMaxHealth() const;

		void SetHealth(int health);
		void TakeDamage(int damage);
		void Heal(int health);

		Subject m_Subject;
	private:
		int m_Health{ 5 };
		int m_MaxHealth{ 5 };
		TextComponent* m_TextComponent;

	};
}

